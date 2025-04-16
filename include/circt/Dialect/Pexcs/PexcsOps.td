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
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/Bytecode/BytecodeOpInterface.td"
include "circt/Dialect/Pexcs/PexcsDialect.td"
include "circt/Dialect/Pexcs/PexcsTypes.td"
include "circt/Dialect/Pexcs/PexcsTraits.td"

// Base class for Pexcs operations
class PexcsOp<string mnemonic, list<Trait> traits = []> : Op<PexcsDialect, mnemonic, traits>;

// Include operation definitions separated by level
include "PexcsAbstractOps.td"
include "PexcsPhysicalOps.td"

#endif // CIRCT_DIALECT_PEXCS_PEXCSOPS_TD 