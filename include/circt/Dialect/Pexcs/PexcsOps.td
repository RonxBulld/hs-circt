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
// DataFlow Operation
//===----------------------------------------------------------------------===//

// Base class for DataFlow operations
class PexcsDataFlowBase<string mnemonic, list<Trait> traits = []> : PexcsOp<mnemonic, !listconcat(traits, [])> {
  let summary = "Data flow operation";
  let results = (outs AnyType:$result);
  let hasVerifier = 1;
  let description = [{
    Data flow operation for computational structures. This is a base class for operations
    that represent various computational patterns in a dataflow manner.
  }];
}

def PexcsMapOp : PexcsDataFlowBase<"map"> {
  let arguments = (ins AnyType:$input, FlatSymbolRefAttr:$mapper);
  let assemblyFormat = "`(` $input `)` attr-dict `:` functional-type($input, $result)";
  let description = [{
    A map operation applies a function to each element of the input.

    Example:
    ```mlir
    %result = pexcs.map(%input) {mapper = @function_symbol} : tensor<10xi32> -> tensor<10xi32>
    ```
  }];
}

def PexcsReduceOp : PexcsDataFlowBase<"reduce"> {
  let arguments = (ins AnyType:$input, AnyType:$initialValue, FlatSymbolRefAttr:$reducer);
  let assemblyFormat = "`(` $input `,` $initialValue `)` attr-dict `:` functional-type([$input, $initialValue], $result)";
  let description = [{
    A reduce operation combines all elements in the input into a single result using a reducer function.

    Example:
    ```mlir
    %result = pexcs.reduce(%input, %init_val) {reducer = @function_symbol} : (tensor<10xi32>, i32) -> i32
    ```
  }];
}

def PexcsFilterOp : PexcsDataFlowBase<"filter"> {
  let arguments = (ins AnyType:$input, FlatSymbolRefAttr:$predicate);
  let assemblyFormat = "`(` $input `)` attr-dict `:` functional-type($input, $result)";
  let description = [{
    A filter operation selects elements from the input based on a predicate function.

    Example:
    ```mlir
    %result = pexcs.filter(%input) {predicate = @function_symbol} : tensor<10xi32> -> tensor<?xi32>
    ```
  }];
}

#endif // CIRCT_DIALECT_PEXCS_PEXCSOPS_TD 