//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_DIALECT_PEXCS_PEXCSOPS_TD
#define CIRCT_DIALECT_PEXCS_PEXCSOPS_TD

include "mlir/IR/OpBase.td"
include "mlir/Interfaces/InferTypeOpInterface.td"
include "mlir/Bytecode/BytecodeOpInterface.td"
include "circt/Dialect/Pexcs/PexcsDialect.td"
include "circt/Dialect/Pexcs/PexcsTypes.td"

// Base class for Pexcs operations
class PexcsOp<string mnemonic, list<Trait> traits = []> : Op<PexcsDialect, mnemonic, traits>;

//===----------------------------------------------------------------------===//
// LUT Operations
//===----------------------------------------------------------------------===//

// Base class for LUT operations
class PexcsLutBase<string mnemonic, list<Trait> traits = []> : PexcsOp<mnemonic, !listconcat(traits, [])> {
  let summary = "Lookup table operation";
  let results = (outs I1:$result);
  let hasVerifier = 1;
  let description = [{
    Lookup table operation with a single output bit. The INIT attribute defines 
    the lookup table contents as a 64-bit integer where each bit represents the output 
    value for the corresponding input pattern.

    Example:
    ```mlir
    // 6-input LUT
    %result = pexcs.lut6(%a, %b, %c, %d, %e, %f) {INIT = 8589934590 : ui64} : (i1, i1, i1, i1, i1, i1) -> i1
    ```
  }];
}

// Pexcs dialect only supports LUT6 operation
def PexcsLut6Op : PexcsLutBase<"lut6"> {
  let arguments = (ins 
    UI64Attr:$INIT, 
    PexcsOperandType:$input0, 
    PexcsOperandType:$input1, 
    PexcsOperandType:$input2, 
    PexcsOperandType:$input3, 
    PexcsOperandType:$input4, 
    PexcsOperandType:$input5
  );
  let assemblyFormat = [{
    `(` $input0 `,` $input1 `,` $input2 `,` $input3 `,` $input4 `,` $input5 `)`
    attr-dict `:`
    `(` type($input0) `,` type($input1) `,` type($input2) `,` type($input3) `,` type($input4) `,` type($input5) `)` `->` type($result)
    }];
  
  let description = [{
    Lookup table operation with a single output bit. The INIT attribute defines 
    the lookup table contents as a 64-bit integer where each bit represents the output 
    value for the corresponding input pattern.

    每个输入操作数是一个16位的结构化操作数，其中：
    - 高3bit表示寻址目标空间
    - 低13bit表示立即数或者寻址的地址

    操作数空间定义：
    - 0: 立即数
    - 1: 设计状态存储器空间
    - 2: 全局输入存储器空间
    - 3: 内存输出寄存器空间
    - 4: 流水线后端单元空间
    - 5: 互联网络空间

    Example:
    ```mlir
    // 6-input LUT
    %result = pexcs.lut6(%a, %b, %c, %d, %e, %f) {INIT = 8589934590 : ui64} : (operand, operand, operand, operand, operand, operand) -> i1
    ```
  }];
}

//===----------------------------------------------------------------------===//
// Clock Event Evaluation Operation
//===----------------------------------------------------------------------===//

def PexcsCevOp : PexcsOp<"cev"> {
  let summary = "Clock event evaluation operation";
  let description = [{
    时钟事件评估指令 (Clock Event Evaluation) 用于评估时钟事件并写回结果。
    
    该指令接受三个结构化操作数：
    1. clockPrev - 时钟前值操作数
    2. clockCurr - 时钟现值操作数
    3. evalResult - 评估写回操作数
    
    每个操作数是一个16位的结构化操作数，其中：
    - 高3bit表示寻址目标空间
    - 低13bit表示立即数或者寻址的地址

    操作数空间定义：
    - 0: 立即数
    - 1: 设计状态存储器空间
    - 2: 全局输入存储器空间
    - 3: 内存输出寄存器空间
    - 4: 流水线后端单元空间
    - 5: 互联网络空间
    
    Example:
    ```mlir
    pexcs.cev %clockPrev, %clockCurr, %evalResult : operand, operand, operand
    ```
  }];
  
  let arguments = (ins
    PexcsOperandType:$clockPrev,  // 时钟前值操作数
    PexcsOperandType:$clockCurr,  // 时钟现值操作数
    PexcsOperandType:$evalResult  // 评估写回操作数
  );
  
  let assemblyFormat = [{
    $clockPrev `,` $clockCurr `,` $evalResult attr-dict `:` type($clockPrev) `,` type($clockCurr) `,` type($evalResult)
  }];
}

#endif // CIRCT_DIALECT_PEXCS_PEXCSOPS_TD 