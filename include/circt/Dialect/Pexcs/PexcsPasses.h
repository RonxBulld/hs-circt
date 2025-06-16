//===- PexcsPasses.h - Pexcs passes -----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#ifndef CIRCT_DIALECT_PEXCS_PEXCSPASSES_H
#define CIRCT_DIALECT_PEXCS_PEXCSPASSES_H

#include "circt/Dialect/Pexcs/PexcsDialect.h"
#include "mlir/Pass/Pass.h"

namespace circt {

std::unique_ptr<mlir::Pass> createHWToPexcsPass();

#define GEN_PASS_REGISTRATION
#include "circt/Dialect/Pexcs/PexcsPasses.h.inc"

} // namespace circt

#endif // CIRCT_DIALECT_PEXCS_PEXCSPASSES_H 