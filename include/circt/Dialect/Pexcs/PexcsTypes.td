//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_DIALECT_PEXCS_PEXCSTYPES_TD
#define CIRCT_DIALECT_PEXCS_PEXCSTYPES_TD

include "circt/Dialect/Pexcs/PexcsDialect.td"
include "mlir/IR/AttrTypeBase.td"

class PexcsType<string name, string typeMnemonic, list<Trait> traits = []>
    : TypeDef<PexcsDialect, name, traits> {
  let mnemonic = typeMnemonic;
}

def PexcsOperandType : PexcsType<"Operand", "operand"> {
  let summary = "A 16-bit structured operand type for Pexcs dialect";
  let description = [{
    A 16-bit structured operand type used in the Pexcs dialect.
    
    高3bit表示寻址目标空间：
    - 0: 立即数
    - 1: 设计状态存储器空间
    - 2: 全局输入存储器空间
    - 3: 内存输出寄存器空间
    - 4: 流水线后端单元空间
    - 5: 互联网络空间
    
    低13bit表示立即数值或者寻址的地址。
    
    This type is used for operands in LUT6 and CEV instructions.
  }];
}

#endif // CIRCT_DIALECT_PEXCS_PEXCSTYPES_TD 