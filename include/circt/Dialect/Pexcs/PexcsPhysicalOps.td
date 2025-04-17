//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_DIALECT_PEXCS_PEXCSPHYSICALOPS_TD
#define CIRCT_DIALECT_PEXCS_PEXCSPHYSICALOPS_TD

include "circt/Dialect/Pexcs/PexcsBaseOps.td"

// include "PexcsOpsBase.td" // Base class is defined in PexcsOps.td which includes this file.
include "mlir/Interfaces/SideEffectInterfaces.td"

//===----------------------------------------------------------------------===//
// Clock Event Evaluation Operation
//===----------------------------------------------------------------------===//

def PexcsCevOp : PexcsOp<"cev", [CalculateLevel]> {
  let summary = "Clock event evaluation operation";
  let description = [{
    时钟事件评估指令 (Clock Event Evaluation)在高层IR中评估时钟事件。

    Inputs:
    1. clockPrev (i1) - 时钟前值
    2. clockCurr (i1) - 时钟现值
    Outputs:
    3. evalResult (i1) - 评估结果

    Example:
    ```mlir
    %result = pexcs.cev(CP : %clockPrev, CC : %clockCurr) : i1, i1 -> i1
    ```
  }];

  let arguments = (ins
    I1:$clockPrev,  // 时钟前值
    I1:$clockCurr   // 时钟现值
  );

  let results = (outs
    I1:$evalResult  // 评估结果
  );

  let assemblyFormat = [{
    `(` `CP` `:` $clockPrev `,` `CC` `:` $clockCurr `)`
    attr-dict `:` type($clockPrev) `,` type($clockCurr) `->` type($evalResult)
  }];
}

//===----------------------------------------------------------------------===//
// Clock Update Operation
//===----------------------------------------------------------------------===//

def PexcsCuvOp : PexcsOp<"cuv", [CalculateLevel, MemoryEffects<[MemWrite]>]> {
  let summary = "Clock update operation (operates on locations)";
  let description = [{
    时钟更新指令 (Clock Update) 用于将时钟现值更新到时钟前值地址。

    该指令接受两个结构化操作数作为输入：
    1. clockCurrAddr - 时钟现值地址操作数 (源)
    2. clockPrevAddr - 时钟前值地址操作数 (目标)

    指令的作用是从 clockCurrAddr 指定的位置读取时钟值，然后将其写入到 clockPrevAddr 指定的位置。
    该操作具有写内存的副作用。

    每个操作数是一个16位的结构化操作数，其中：
    - 高3bit表示寻址目标空间
    - 低13bit表示寻址的地址

    操作数空间定义：
    - 1: 设计状态存储器空间
    - 2: 全局输入存储器空间
    - 3: 内存输出寄存器空间
    - 4: 流水线后端单元空间
    - 5: 互联网络空间

    Example:
    ```mlir
    pexcs.cuv(CA : %currAddr, PA : %prevAddr) : operand, operand
    ```
  }];

  let arguments = (ins
    PexcsOperandType:$clockCurrAddr, // 时钟现值地址操作数
    PexcsOperandType:$clockPrevAddr  // 时钟前值地址操作数 (作为目标地址输入)
  );
  // No explicit results (outs), operation returns void and has side effects.

  let assemblyFormat = [{
    `(` `CA` `:` $clockCurrAddr `,` `PA` `:` $clockPrevAddr `)`
    attr-dict `:` type($clockCurrAddr) `,` type($clockPrevAddr)
  }];
}

#endif // CIRCT_DIALECT_PEXCS_PEXCSPHYSICALOPS_TD 