//===- HWToPexcs.cpp - Translate HW into Pexcs ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This is the main HW to Pexcs Conversion Pass Implementation.
//
//===----------------------------------------------------------------------===//

#include "circt/Dialect/Pexcs/PexcsPasses.h"
#include "circt/Dialect/HW/HWOps.h"
#include "circt/Dialect/Pexcs/PexcsDialect.h"
#include "circt/Dialect/Pexcs/PexcsOps.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/DialectConversion.h"
#include "llvm/Support/FormatVariadic.h"
#include "mlir/IR/SymbolTable.h"

namespace circt {
#define GEN_PASS_DEF_HWTOPEXCS
#include "circt/Dialect/Pexcs/PexcsPasses.h.inc"
} // namespace circt

using namespace mlir;
using namespace circt;
using namespace pexcs;

namespace {

struct CanonicalPortInfo {
  SmallVector<std::pair<StringRef, unsigned>> inputs;
  SmallVector<std::pair<StringRef, unsigned>> outputs;
};

/// 创建端口名称到索引的映射，提高查找效率
/// 避免重复遍历，从 O(n*m) 优化到 O(n+m)
static llvm::StringMap<size_t> createPortNameToIndexMap(ArrayAttr portNames) {
  llvm::StringMap<size_t> portMap;
  
  for (size_t i = 0; i < portNames.size(); ++i) {
    StringAttr portName = cast<StringAttr>(portNames[i]);
    portMap[portName.getValue()] = i;
  }
  
  return portMap;
}

/// 检查端口不匹配并返回详细的错误信息
/// 支持检查多余端口和缺少端口两种情况
static LogicalResult validatePortsMatch(
    hw::InstanceOp op,
    const llvm::StringMap<size_t> &actualPorts,
    ArrayRef<StringRef> expectedPorts,
    const llvm::StringSet<> &expectedPortSet,
    StringRef portType,  // "input" 或 "output"
    ConversionPatternRewriter &rewriter) {
  
  // 检查多余端口
  SmallVector<StringRef> unexpectedPorts;
  for (const auto &entry : actualPorts) {
    if (expectedPortSet.find(entry.getKey()) == expectedPortSet.end()) {
      unexpectedPorts.push_back(entry.getKey());
    }
  }
  
  if (!unexpectedPorts.empty()) {
    std::string unexpectedList;
    for (size_t i = 0; i < unexpectedPorts.size(); ++i) {
      if (i > 0) unexpectedList += ", ";
      unexpectedList += unexpectedPorts[i].str();
    }
    return rewriter.notifyMatchFailure(
        op, llvm::formatv("unexpected {0} ports: {1}", portType, unexpectedList));
  }
  
  // 检查缺少端口
  if (actualPorts.size() != expectedPorts.size()) {
    SmallVector<StringRef> missingPorts;
    for (StringRef expectedPort : expectedPorts) {
      if (actualPorts.find(expectedPort) == actualPorts.end()) {
        missingPorts.push_back(expectedPort);
      }
    }
    
    std::string missingList;
    for (size_t i = 0; i < missingPorts.size(); ++i) {
      if (i > 0) missingList += ", ";
      missingList += missingPorts[i].str();
    }
    
    return rewriter.notifyMatchFailure(
        op, llvm::formatv("missing {0} ports: {1}", portType, missingList));
  }
  
  return success();
}

/// 统一的端口重新组织函数
/// 根据期望的端口顺序重新组织输入和输出
/// 检查是否缺少端口或存在多余端口
static LogicalResult organizePortsByNames(
    hw::InstanceOp op, 
    ArrayRef<StringRef> expectedInputPorts,
    ArrayRef<StringRef> expectedOutputPorts,
    SmallVectorImpl<Value> &organizedInputs,
    SmallVectorImpl<Type> &organizedOutputTypes,
    ConversionPatternRewriter &rewriter) {
  
  // 一次性建立端口名称到索引的映射
  auto inputPortMap = createPortNameToIndexMap(op.getArgNames());
  auto outputPortMap = createPortNameToIndexMap(op.getResultNames());
  
  // 创建期望端口名称的集合，用于快速查找
  llvm::StringSet<> expectedInputSet;
  for (StringRef port : expectedInputPorts) {
    expectedInputSet.insert(port);
  }
  
  llvm::StringSet<> expectedOutputSet;
  for (StringRef port : expectedOutputPorts) {
    expectedOutputSet.insert(port);
  }
  
  // 验证输入端口匹配
  if (failed(validatePortsMatch(op, inputPortMap, expectedInputPorts, 
                                expectedInputSet, "input", rewriter)))
    return failure();
  
  // 验证输出端口匹配
  if (failed(validatePortsMatch(op, outputPortMap, expectedOutputPorts, 
                                expectedOutputSet, "output", rewriter)))
    return failure();
  
  // 组织输入端口
  organizedInputs.reserve(expectedInputPorts.size());
  for (StringRef expectedPort : expectedInputPorts) {
    auto it = inputPortMap.find(expectedPort);
    if (it == inputPortMap.end()) {
      return rewriter.notifyMatchFailure(
          op, llvm::formatv("missing expected input port: {0}", expectedPort));
    }
    organizedInputs.push_back(op.getOperand(it->second));
  }
  
  // 组织输出端口
  organizedOutputTypes.reserve(expectedOutputPorts.size());
  for (StringRef expectedPort : expectedOutputPorts) {
    auto it = outputPortMap.find(expectedPort);
    if (it == outputPortMap.end()) {
      return rewriter.notifyMatchFailure(
          op, llvm::formatv("missing expected output port: {0}", expectedPort));
    }
    organizedOutputTypes.push_back(op.getResults()[it->second].getType());
  }
  
  return success();
}

/// 从InstanceOp中提取指定名称的参数
/// 返回成功时的参数值，失败时返回空指针
static Attribute extractParameter(hw::InstanceOp op, StringRef paramName) {
  if (auto params = op.getParameters()) {
    for (Attribute param : params) {
      auto paramAttr = cast<hw::ParamDeclAttr>(param);
      if (paramAttr.getName().getValue() == paramName) {
        return paramAttr.getValue();
      }
    }
  }
  return {};
}

// In this namespace, we will define the conversion patterns.
struct InstanceOpConversion : public OpConversionPattern<hw::InstanceOp> {
  using OpConversionPattern<hw::InstanceOp>::OpConversionPattern;

  LogicalResult
  matchAndRewrite(hw::InstanceOp op, OpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
    StringRef moduleName = op.getModuleName();

    // Find the extern module declaration.
    auto externModule =
        SymbolTable::lookupNearestSymbolFrom<hw::HWModuleExternOp>(
            op, op.getModuleNameAttr());

    if (!externModule)
      return rewriter.notifyMatchFailure(
          op, "unable to find extern module for instance");

    if (moduleName == "lut5") {
      // LUT5 预期的端口顺序：输入 I0-I4，输出 O
      static const StringRef expectedInputPorts[] = {"I0", "I1", "I2", "I3", "I4"};
      static const StringRef expectedOutputPorts[] = {"O"};
      
      // 提取 INIT 参数，LUT5 查找表的初始化值
      Attribute initAttr = extractParameter(op, "INIT");
      if (!initAttr)
        return rewriter.notifyMatchFailure(op, "missing INIT parameter for lut5");

      // 按端口名称重新组织输入和输出
      SmallVector<Value> organizedInputs;
      SmallVector<Type> organizedOutputTypes;
      
      if (failed(organizePortsByNames(op, expectedInputPorts, expectedOutputPorts,
                                     organizedInputs, organizedOutputTypes, rewriter)))
        return failure();

      // 创建 pexcs.lut5 操作，保持端口连接的语义正确性
      rewriter.replaceOpWithNewOp<pexcs::PexcsLut5Op>(
          op, organizedOutputTypes[0], cast<IntegerAttr>(initAttr),
          organizedInputs[0], organizedInputs[1], organizedInputs[2], organizedInputs[3], organizedInputs[4]);
      
      return success();
    }
    
    // RAML512X8D - 512x8 双端口 RAM
    if (moduleName == "raml512x8d") {
      static const StringRef expectedInputPorts[] = {"WD", "WA", "WE", "C", "RA"};
      static const StringRef expectedOutputPorts[] = {"RD", "WDO"};
      
      SmallVector<Value> organizedInputs;
      SmallVector<Type> organizedOutputTypes;
      
      if (failed(organizePortsByNames(op, expectedInputPorts, expectedOutputPorts,
                                     organizedInputs, organizedOutputTypes, rewriter)))
        return failure();

      rewriter.replaceOpWithNewOp<pexcs::PexcsRAML512X8DOp>(
          op, organizedOutputTypes, organizedInputs[0], organizedInputs[1], organizedInputs[2], organizedInputs[3], organizedInputs[4]);
      
      return success();
    }
    
    // RAML512X8S - 512x8 单端口 RAM
    if (moduleName == "raml512x8s") {
      static const StringRef expectedInputPorts[] = {"D", "A", "WE", "C"};
      static const StringRef expectedOutputPorts[] = {"O"};
      
      SmallVector<Value> organizedInputs;
      SmallVector<Type> organizedOutputTypes;
      
      if (failed(organizePortsByNames(op, expectedInputPorts, expectedOutputPorts,
                                     organizedInputs, organizedOutputTypes, rewriter)))
        return failure();

      rewriter.replaceOpWithNewOp<pexcs::PexcsRAML512X8SOp>(
          op, organizedOutputTypes[0], organizedInputs[0], organizedInputs[1], organizedInputs[2], organizedInputs[3]);
      
      return success();
    }
    
    // SDFFSE - 同步设置D触发器带时钟使能
    if (moduleName == "sdffse") {
      static const StringRef expectedInputPorts[] = {"D", "C", "CE", "PRE"};
      static const StringRef expectedOutputPorts[] = {"Q"};
      
             // 提取 INIT 参数
       Attribute initAttr = extractParameter(op, "INIT");
       if (!initAttr)
         return rewriter.notifyMatchFailure(op, "missing INIT parameter for sdffse");
      
      SmallVector<Value> organizedInputs;
      SmallVector<Type> organizedOutputTypes;
      
      if (failed(organizePortsByNames(op, expectedInputPorts, expectedOutputPorts,
                                     organizedInputs, organizedOutputTypes, rewriter)))
        return failure();

      rewriter.replaceOpWithNewOp<pexcs::PexcsSDFFSEOp>(
          op, organizedOutputTypes[0], cast<IntegerAttr>(initAttr),
          organizedInputs[0], organizedInputs[1], organizedInputs[2], organizedInputs[3]);
      
      return success();
    }
    
    // SDFFRE - 同步复位D触发器带时钟使能
    if (moduleName == "sdffre") {
      static const StringRef expectedInputPorts[] = {"D", "C", "CE", "CLR"};
      static const StringRef expectedOutputPorts[] = {"Q"};
      
             // 提取 INIT 参数
       Attribute initAttr = extractParameter(op, "INIT");
       if (!initAttr)
         return rewriter.notifyMatchFailure(op, "missing INIT parameter for sdffre");
      
      SmallVector<Value> organizedInputs;
      SmallVector<Type> organizedOutputTypes;
      
      if (failed(organizePortsByNames(op, expectedInputPorts, expectedOutputPorts,
                                     organizedInputs, organizedOutputTypes, rewriter)))
        return failure();

      rewriter.replaceOpWithNewOp<pexcs::PexcsSDFFREOp>(
          op, organizedOutputTypes[0], cast<IntegerAttr>(initAttr),
          organizedInputs[0], organizedInputs[1], organizedInputs[2], organizedInputs[3]);
      
      return success();
    }
    
    // ADFFSE - 异步设置D触发器带时钟使能
    if (moduleName == "adffse") {
      static const StringRef expectedInputPorts[] = {"D", "C", "CE", "PRE"};
      static const StringRef expectedOutputPorts[] = {"Q"};
      
             // 提取 INIT 参数
       Attribute initAttr = extractParameter(op, "INIT");
       if (!initAttr)
         return rewriter.notifyMatchFailure(op, "missing INIT parameter for adffse");
      
      SmallVector<Value> organizedInputs;
      SmallVector<Type> organizedOutputTypes;
      
      if (failed(organizePortsByNames(op, expectedInputPorts, expectedOutputPorts,
                                     organizedInputs, organizedOutputTypes, rewriter)))
        return failure();

      rewriter.replaceOpWithNewOp<pexcs::PexcsADFFSEOp>(
          op, organizedOutputTypes[0], cast<IntegerAttr>(initAttr),
          organizedInputs[0], organizedInputs[1], organizedInputs[2], organizedInputs[3]);
      
      return success();
    }
    
    // ADFFRE - 异步复位D触发器带时钟使能
    if (moduleName == "adffre") {
      static const StringRef expectedInputPorts[] = {"D", "C", "CE", "CLR"};
      static const StringRef expectedOutputPorts[] = {"Q"};
      
             // 提取 INIT 参数
       Attribute initAttr = extractParameter(op, "INIT");
       if (!initAttr)
         return rewriter.notifyMatchFailure(op, "missing INIT parameter for adffre");
      
      SmallVector<Value> organizedInputs;
      SmallVector<Type> organizedOutputTypes;
      
      if (failed(organizePortsByNames(op, expectedInputPorts, expectedOutputPorts,
                                     organizedInputs, organizedOutputTypes, rewriter)))
        return failure();

      rewriter.replaceOpWithNewOp<pexcs::PexcsADFFREOp>(
          op, organizedOutputTypes[0], cast<IntegerAttr>(initAttr),
          organizedInputs[0], organizedInputs[1], organizedInputs[2], organizedInputs[3]);
      
      return success();
    }
    
    // INV - 逆变器操作
    if (moduleName == "inv") {
      static const StringRef expectedInputPorts[] = {"IN"};
      static const StringRef expectedOutputPorts[] = {"O"};
      
      SmallVector<Value> organizedInputs;
      SmallVector<Type> organizedOutputTypes;
      
      if (failed(organizePortsByNames(op, expectedInputPorts, expectedOutputPorts,
                                     organizedInputs, organizedOutputTypes, rewriter)))
        return failure();

      rewriter.replaceOpWithNewOp<pexcs::PexcsINVOp>(
          op, organizedOutputTypes[0], organizedInputs[0]);
      
      return success();
    }
    
    // ZBUF - 零缓冲器操作
    if (moduleName == "zbuf") {
      static const StringRef expectedInputPorts[] = {"IN"};
      static const StringRef expectedOutputPorts[] = {"O"};
      
      SmallVector<Value> organizedInputs;
      SmallVector<Type> organizedOutputTypes;
      
      if (failed(organizePortsByNames(op, expectedInputPorts, expectedOutputPorts,
                                     organizedInputs, organizedOutputTypes, rewriter)))
        return failure();

      rewriter.replaceOpWithNewOp<pexcs::PexcsZBUFOp>(
          op, organizedOutputTypes[0], organizedInputs[0]);
      
      return success();
    }

    // 未识别的模块
    return failure();
  }
};

struct HWToPexcsPass : public circt::impl::HWToPEXCSBase<HWToPexcsPass> {
  void runOnOperation() override;
};

void HWToPexcsPass::runOnOperation() {
  ConversionTarget target(getContext());

  // Mark Pexcs dialect operations as legal.
  target.addLegalDialect<PexcsDialect>();
  // Mark HW dialect operations as illegal.
  target.addIllegalDialect<hw::HWDialect>();

  // We will need a TypeConverter if HW types and Pexcs types are different.
  // TypeConverter typeConverter;

  RewritePatternSet patterns(&getContext());
  // Add conversion patterns here.
  patterns.add<InstanceOpConversion>(&getContext());
  
  if (failed(applyPartialConversion(getOperation(), target, std::move(patterns))))
    signalPassFailure();
}

} // namespace

std::unique_ptr<Pass> circt::createHWToPexcsPass() {
  return std::make_unique<HWToPexcsPass>();
} 