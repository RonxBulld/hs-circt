//===- PexcsPasses.td - Pexcs passes -----------------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_DIALECT_PEXCS_PASSES_TD
#define CIRCT_DIALECT_PEXCS_PASSES_TD

include "mlir/Pass/PassBase.td"

def HWToPEXCS : Pass<"hw-to-pexcs", "mlir::ModuleOp"> {
  let summary = "Convert HW dialect to PEXCS dialect";
  let constructor = "circt::createHWToPexcsPass()";
}

#endif // CIRCT_DIALECT_PEXCS_PASSES_TD 