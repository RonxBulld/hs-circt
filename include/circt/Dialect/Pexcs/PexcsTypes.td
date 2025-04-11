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

def PexcsUI2Type : PexcsType<"UI2", "ui2"> {
  let summary = "A 2-bit unsigned integer type";
  let description = [{
    A 2-bit unsigned integer type used in the Pexcs dialect.
  }];
}

def PexcsUI4Type : PexcsType<"UI4", "ui4"> {
  let summary = "A 4-bit unsigned integer type";
  let description = [{
    A 4-bit unsigned integer type used in the Pexcs dialect.
  }];
}

#endif // CIRCT_DIALECT_PEXCS_PEXCSTYPES_TD 