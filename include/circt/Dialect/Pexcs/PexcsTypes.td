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

def PexcsOperandType : PexcsType<"PXOperand", "operand"> {
  let summary = "A 16-bit structured operand type for Pexcs dialect";
  let description = [{
    A structured operand type used in the Pexcs dialect, consisting of an operand space
    (namespace) and an address or immediate value.
    
    The operand space defines the target memory space:
    - Immediate: 立即数 (immediate value)
    - StateMemory: 设计状态存储器空间 (design state memory space)
    - GlobalInputMemory: 全局输入存储器空间 (global input memory space)
    - MemoryOutputReg: 内存输出寄存器空间 (memory output register space)
    - PipelineBackend: 流水线后端单元空间 (pipeline backend unit space)
    - InterconnectNet: 互联网络空间 (interconnect network space)
    
    The address is a 13-bit value (0-8191) representing either an immediate value or
    a memory address depending on the space.
    
    Both the operand space and address must be compile-time constants.
    
    This type is used for operands in LUT6 and CEV instructions.
  }];
  
  // 定义结构化参数
  let parameters = (ins
    "OperandSpace":$space,  // 寻址目标空间
    "uint16_t":$address     // 地址或立即数值
  );
  
  // 定义汇编格式
  let assemblyFormat = "`<` $space `(` $address `)` `>`";
  
  // 验证器声明
  let genVerifyDecl = 1;
  
  // 额外方法声明
  let extraClassDeclaration = [{
    // 判断是否为立即数
    bool isImmediate() const {
      return getSpace() == OperandSpace::Immediate;
    }
    
    // 获取操作数空间的字符串表示
    StringRef getSpaceName() const;
    
    // 获取16位打包值（如果需要传统表示）
    uint16_t getPackedValue() const {
      return (static_cast<uint16_t>(getSpace()) << 13) | (getAddress() & 0x1FFF);
    }
  }];
}

#endif // CIRCT_DIALECT_PEXCS_PEXCSTYPES_TD 