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
#include "mlir/IR/BuiltinAttributes.h.inc"
// clang-format on

namespace circt {
namespace pexcs {

using namespace ::mlir;

// 操作数空间的枚举类型
enum OperandSpace : uint8_t {
  Immediate = 0,          // 立即数
  StateMemory = 1,        // 设计状态存储器空间
  GlobalInputMemory = 2,  // 全局输入存储器空间
  MemoryOutputReg = 3,    // 内存输出寄存器空间
  PipelineBackend = 4,    // 流水线后端单元空间
  InterconnectNet = 5     // 互联网络空间
};

// 获取操作数空间的字符串表示
StringRef getOperandSpaceName(OperandSpace space);

} // namespace pexcs
} // namespace circt

// 包含由TableGen生成的类型
#define GET_TYPEDEF_CLASSES
#include "circt/Dialect/Pexcs/PexcsTypes.h.inc"

#endif // CIRCT_DIALECT_PEXCS_PEXCSTYPES_H 