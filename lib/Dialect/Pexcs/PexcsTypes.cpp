//===- PexcsTypes.cpp - Pexcs dialect types ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// clang-format off
#include "circt/Dialect/Pexcs/PexcsTypes.h"
#include "circt/Dialect/Pexcs/PexcsDialect.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/DialectImplementation.h"
#include "llvm/ADT/TypeSwitch.h"
// clang-format on

using namespace mlir;
using namespace circt;
using namespace circt::pexcs;

//===----------------------------------------------------------------------===//
// 获取操作数空间的字符串表示
//===----------------------------------------------------------------------===//
StringRef circt::pexcs::getOperandSpaceName(OperandSpace space) {
  switch (space) {
  case OperandSpace::Immediate:
    return "imm";
  case OperandSpace::StateMemory:
    return "state";
  case OperandSpace::GlobalInputMemory:
    return "global";
  case OperandSpace::MemoryOutputReg:
    return "memout";
  case OperandSpace::PipelineBackend:
    return "pipeline";
  case OperandSpace::InterconnectNet:
    return "net";
  default:
    return "unknown";
  }
}

//===----------------------------------------------------------------------===//
// PXOperandType 字符串表示
//===----------------------------------------------------------------------===//
StringRef PXOperandType::getSpaceName() const {
  return getOperandSpaceName(getSpace());
}

//===----------------------------------------------------------------------===//
// 验证PXOperandType
//===----------------------------------------------------------------------===//
LogicalResult PXOperandType::verify(function_ref<InFlightDiagnostic()> emitError,
                                  OperandSpace space, uint16_t address) {
  // 验证操作数空间是否合法
  if (space > OperandSpace::InterconnectNet) {
    return emitError() << "invalid operand space: " << static_cast<int>(space) 
                       << " (must be <= " << static_cast<int>(OperandSpace::InterconnectNet) << ")";
  }
  
  // 验证地址是否在13位范围内
  if (address > 0x1FFF) {
    return emitError() << "operand address out of range (must be <= 8191): " << address;
  }
  
  return success();
}

// 在命名空间 mlir 下为 OperandSpace 添加 FieldParser 特化
namespace mlir {
template <>
struct FieldParser<circt::pexcs::OperandSpace> {
  static FailureOr<circt::pexcs::OperandSpace> parse(AsmParser &p) {
    // 解析标识符
    StringRef name;
    if (p.parseKeyword(&name))
      return failure();
    
    // 将标识符转换为枚举值
    if (name == "imm")
      return circt::pexcs::OperandSpace::Immediate;
    if (name == "state")
      return circt::pexcs::OperandSpace::StateMemory;
    if (name == "global")
      return circt::pexcs::OperandSpace::GlobalInputMemory;
    if (name == "memout")
      return circt::pexcs::OperandSpace::MemoryOutputReg;
    if (name == "pipeline")
      return circt::pexcs::OperandSpace::PipelineBackend;
    if (name == "net")
      return circt::pexcs::OperandSpace::InterconnectNet;
    
    // 如果没有匹配项，报告错误
    p.emitError(p.getCurrentLocation()) << "unknown operand space name: " << name;
    return failure();
  }
};
} // namespace mlir

//===----------------------------------------------------------------------===//
// Generated Logic
//===----------------------------------------------------------------------===//

#define GET_TYPEDEF_CLASSES
#include "circt/Dialect/Pexcs/PexcsTypes.cpp.inc" 

//===----------------------------------------------------------------------===//
// 注册PexcsDialect的类型
//===----------------------------------------------------------------------===//
void PexcsDialect::registerTypes() {
  addTypes<
#define GET_TYPEDEF_LIST
#include "circt/Dialect/Pexcs/PexcsTypes.cpp.inc"
      >();
}
