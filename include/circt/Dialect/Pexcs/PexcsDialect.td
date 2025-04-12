//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_DIALECT_PEXCS_PEXCSDIALECT_TD
#define CIRCT_DIALECT_PEXCS_PEXCSDIALECT_TD

include "mlir/IR/OpBase.td"

//===----------------------------------------------------------------------===//
// PEXCS dialect definition
//===----------------------------------------------------------------------===//

def PexcsDialect : Dialect {
  let name = "pexcs";
  let summary = "Extensions for expressing and optimizing computational structures";
  let description = [{
    The PEXCS dialect provides operations for expressing and optimizing computational 
    structures that are commonly used in hardware design and optimization.
  }];

  let cppNamespace = "::circt::pexcs";
}

#endif // CIRCT_DIALECT_PEXCS_PEXCSDIALECT_TD 