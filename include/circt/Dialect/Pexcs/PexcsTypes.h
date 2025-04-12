//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_DIALECT_PEXCS_PEXCSTYPES_H
#define CIRCT_DIALECT_PEXCS_PEXCSTYPES_H

// clang-format off
#include "circt/Dialect/Pexcs/PexcsDialect.h"
#include "mlir/IR/Types.h"
#include "circt/Dialect/Pexcs/PexcsTypes.h.inc"
#include "mlir/IR/BuiltinAttributes.h.inc"
// clang-format on

namespace circt {
namespace pexcs {

using namespace ::mlir;

using UI16Attr = IntegerAttr;

// 操作数空间的枚举类型
enum OperandSpace : uint8_t {
  Immediate = 0,          // 立即数
  StateMemory = 1,        // 设计状态存储器空间
  GlobalInputMemory = 2,  // 全局输入存储器空间
  MemoryOutputReg = 3,    // 内存输出寄存器空间
  PipelineBackend = 4,    // 流水线后端单元空间
  InterconnectNet = 5     // 互联网络空间
};

// 操作数地址掩码
constexpr uint16_t OPERAND_SPACE_MASK = 0xE000;  // 高3位掩码
constexpr uint16_t OPERAND_ADDR_MASK = 0x1FFF;   // 低13位掩码
constexpr uint8_t OPERAND_SPACE_SHIFT = 13;      // 空间位移量

// OperandType类特化的接口实现将由tablegen生成
class OperandType : public Type::TypeBase<OperandType, Type, TypeStorage> {
public:
  using Base::Base;

  // 添加必要的静态成员变量name
  static constexpr StringLiteral name = "operand";

  // 获取OperandType的实例
  static OperandType get(MLIRContext *context);
  
  // 获取类型助记符
  static StringLiteral getMnemonic() { return "operand"; }

  // 创建立即数操作数
  static uint16_t createImmediate(uint16_t value);

  // 创建内存访问操作数
  static uint16_t createMemoryAccess(OperandSpace space, uint16_t addr);

  // 获取操作数空间
  static OperandSpace getSpace(uint16_t value);

  // 获取操作数地址或立即数值
  static uint16_t getAddress(uint16_t value);

  // 判断操作数是否为立即数
  static bool isImmediate(uint16_t value);

};

} // namespace pexcs
} // namespace circt

// 声明OperandType的TypeID
MLIR_DECLARE_EXPLICIT_TYPE_ID(::circt::pexcs::OperandType)

#endif // CIRCT_DIALECT_PEXCS_PEXCSTYPES_H 