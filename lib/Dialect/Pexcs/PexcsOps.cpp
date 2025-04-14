//===- PexcsOps.cpp - Pexcs dialect operations -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// clang-format off
#include "circt/Dialect/Pexcs/PexcsOps.h"
#include "circt/Dialect/Pexcs/PexcsDialect.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/OpImplementation.h"
// clang-format on

using namespace mlir;
using namespace circt;
using namespace circt::pexcs;

//===----------------------------------------------------------------------===//
// Tablegen Generated Logic
//===----------------------------------------------------------------------===//

#define GET_OP_CLASSES
#include "circt/Dialect/Pexcs/Pexcs.cpp.inc"

//===----------------------------------------------------------------------===//
// LUT Operations
//===----------------------------------------------------------------------===//

LogicalResult PexcsLut6Op::verify() {
  // Verify 6-input LUT has valid INIT value
  // For LUT6, INIT must be a 64-bit value
  uint64_t initValue = getINIT();
  // No specific validation needed for now since any 64-bit value is valid
  return success();
}

void PexcsDialect::registerOperations() {
  // 注册操作
  addOperations<
#define GET_OP_LIST
#include "circt/Dialect/Pexcs/Pexcs.cpp.inc"
      >();
}
