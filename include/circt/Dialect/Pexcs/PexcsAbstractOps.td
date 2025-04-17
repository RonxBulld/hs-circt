//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_DIALECT_PEXCS_PEXCSABSTRACTOPS_TD
#define CIRCT_DIALECT_PEXCS_PEXCSABSTRACTOPS_TD

include "circt/Dialect/Pexcs/PexcsBaseOps.td"
include "mlir/Interfaces/SideEffectInterfaces.td"

//===----------------------------------------------------------------------===//
// LUT Operations
//===----------------------------------------------------------------------===//

// Base class for LUT operations
class PexcsLutBase<string mnemonic, list<Trait> traits = []> : PexcsOp<mnemonic, !listconcat(traits, [])> {
  let summary = "Lookup table operation";
  let results = (outs I1:$result);
}

// Pexcs dialect currently supports LUT5 operation
def PexcsLut5Op : PexcsLutBase<"lut5", [AbstractLevel, CalculateLevel]> {
  let arguments = (ins
    UI32Attr:$INIT,
    I1:$input0,
    I1:$input1,
    I1:$input2,
    I1:$input3,
    I1:$input4
  );
  let results = (outs I1:$result);
  let assemblyFormat = [{
    `(` `I0` `:` $input0 `,` `I1` `:` $input1 `,` `I2` `:` $input2 `,` `I3` `:` $input3 `,` `I4` `:` $input4 `)`
    attr-dict `:`
    `(` type($input0) `,` type($input1) `,` type($input2) `,` type($input3) `,` type($input4) `)` `->` type($result)
    }];

  let description = [{
    5-input lookup table operation with a single output bit (i1). The INIT attribute defines
    the lookup table contents as a 32-bit integer where each bit represents the output
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
    // 5-input LUT returning i1
    %result = pexcs.lut5(I0 : %a, I1 : %b, I2 : %c, I3 : %d, I4 : %e) {INIT = 4294967295 : ui32} : (i1, i1, i1, i1, i1) -> i1
    ```
  }];
}


//===----------------------------------------------------------------------===//
// Memory Evaluation Operation
//===----------------------------------------------------------------------===//

def PexcsRAML512X8DOp : PexcsOp<"raml512x8d", [AbstractLevel, MemoryEffects<[MemRead, MemWrite]>]> {
  let summary = "512x8 双端口 RAM 操作";
  let description = [{
    表示一个 512x8 的双端口 RAM。它有独立的读写端口。

    Inputs:
    1. writeData (i8) - 写入数据
    2. writeAddr (i16) - 写入地址 (使用 i16 表示 9 位地址)
    3. writeEnable (i1) - 写使能信号
    4. clockEvent (i1) - 时钟事件信号 (用于同步写操作)
    5. readAddr (i16) - 读取地址 (使用 i16 表示 9 位地址)

    Outputs:
    1. readData (i8) - 从读地址读取的数据
    2. writeDataOut (i8) - 写入端口输出的数据 (通常是输入 writeData 的透传或锁存值)

    该操作具有读写内存的副作用。

    Example:
    ```mlir
    %rdData, %wdOut = pexcs.raml512x8d(WD : %wrData, WA : %wrAddr, WE : %wrEn, C : %clk, RA : %rdAddr) : i8, i16, i1, i1, i16 -> i8, i8
    ```
  }];

  let arguments = (ins
    I8:$writeData,    // 8位写数据
    I16:$writeAddr,   // 9位写地址 (用 I16)
    I1:$writeEnable,  // 1位写使能
    I1:$clockEvent,   // 1位时钟事件
    I16:$readAddr     // 9位读地址 (用 I16)
  );

  let results = (outs
    I8:$readData,     // 8位读数据
    I8:$writeDataOut  // 8位写数据输出
  );

  let assemblyFormat = [{
    `(` `WD` `:` $writeData `,` `WA` `:` $writeAddr `,` `WE` `:` $writeEnable `,` `C` `:` $clockEvent `,` `RA` `:` $readAddr `)`
    attr-dict `:`
    type($writeData) `,` type($writeAddr) `,` type($writeEnable) `,` type($clockEvent) `,` type($readAddr)
    `->` type($readData) `,` type($writeDataOut)
  }];
}

//===----------------------------------------------------------------------===//
// Flip-Flop Operations
//===----------------------------------------------------------------------===//

// Synchronous Set D Flip-Flop with Clock Enable
def PexcsSDFFSEOp : PexcsOp<"sdffse", [AbstractLevel]> {
  let arguments = (ins
    UI1Attr:$INIT, // Initial value
    I1:$dataInput,    // Data input
    I1:$clock,    // Clock
    I1:$clockEnable,   // Clock enable
    I1:$preSet   // Synchronous preset
  );
  let results = (outs I1:$Q);

  let assemblyFormat = [{
    `(` `D` `:` $dataInput `,` `C` `:` $clock `,` `CE` `:` $clockEnable `,` `PRE` `:` $preSet `)`
    attr-dict `:` type($dataInput) `,` type($Q) `,` type($clock) `,` type($clockEnable) `,` type($preSet)
  }];

  let description = [{
    Synchronous Set D Flip-Flop with Clock Enable.

    Ports:
    - D: Data input
    - Q: Data output
    - C: Clock input
    - CE: Clock enable
    - PRE: Synchronous preset
    - INIT: Initial value attribute
  }];
}

// Synchronous Reset D Flip-Flop with Clock Enable
def PexcsSDFFREOp : PexcsOp<"sdffre", [AbstractLevel]> {
  let arguments = (ins
    UI1Attr:$INIT, // Initial value
    I1:$dataInput,    // Data input
    I1:$clock,    // Clock
    I1:$clockEnable,   // Clock enable
    I1:$clear   // Synchronous clear
  );
  let results = (outs I1:$Q);

  let assemblyFormat = [{
    `(` `D` `:` $dataInput `,` `C` `:` $clock `,` `CE` `:` $clockEnable `,` `CLR` `:` $clear `)`
    attr-dict `:` type($dataInput) `,` type($Q) `,` type($clock) `,` type($clockEnable) `,` type($clear)
  }];

  let description = [{
    Synchronous Reset D Flip-Flop with Clock Enable.

    Ports:
    - D: Data input
    - Q: Data output
    - C: Clock input
    - CE: Clock enable
    - CLR: Synchronous clear
    - INIT: Initial value attribute
  }];
}

// Asynchronous Set D Flip-Flop with Clock Enable
def PexcsADFFSEOp : PexcsOp<"adffse", [AbstractLevel]> {
  let arguments = (ins
    UI1Attr:$INIT, // Initial value
    I1:$dataInput,    // Data input
    I1:$clock,    // Clock
    I1:$clockEnable,   // Clock enable
    I1:$preSet   // Asynchronous preset
  );
  let results = (outs I1:$Q);

  let assemblyFormat = [{
    `(` `D` `:` $dataInput `,` `C` `:` $clock `,` `CE` `:` $clockEnable `,` `PRE` `:` $preSet `)`
    attr-dict `:` type($dataInput) `,` type($Q) `,` type($clock) `,` type($clockEnable) `,` type($preSet)
  }];

  let description = [{
    Asynchronous Set D Flip-Flop with Clock Enable.

    Ports:
    - D: Data input
    - Q: Data output
    - C: Clock input
    - CE: Clock enable
    - PRE: Asynchronous preset
    - INIT: Initial value attribute
  }];
}

// Asynchronous Reset D Flip-Flop with Clock Enable
def PexcsADFFREOp : PexcsOp<"adffre", [AbstractLevel]> {
  let arguments = (ins
    UI1Attr:$INIT, // Initial value
    I1:$dataInput,    // Data input
    I1:$clock,    // Clock
    I1:$clockEnable,   // Clock enable
    I1:$clear   // Asynchronous clear
  );
  let results = (outs I1:$Q);

  let assemblyFormat = [{
    `(` `D` `:` $dataInput `,` `C` `:` $clock `,` `CE` `:` $clockEnable `,` `CLR` `:` $clear `)`
    attr-dict `:` type($dataInput) `,` type($Q) `,` type($clock) `,` type($clockEnable) `,` type($clear)
  }];

  let description = [{
    Asynchronous Reset D Flip-Flop with Clock Enable.

    Ports:
    - D: Data input
    - Q: Data output
    - C: Clock input
    - CE: Clock enable
    - CLR: Asynchronous clear
    - INIT: Initial value attribute
  }];
}

#endif // CIRCT_DIALECT_PEXCS_PEXCSABSTRACTOPS_TD
