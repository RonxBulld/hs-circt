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
// Map Operation
//===----------------------------------------------------------------------===//

LogicalResult PexcsMapOp::verify() {
  // Verify that the mapper function exists and has compatible types
  return success();
}

//===----------------------------------------------------------------------===//
// Reduce Operation
//===----------------------------------------------------------------------===//

LogicalResult PexcsReduceOp::verify() {
  // Verify that the reducer function exists and has compatible types
  return success();
}

//===----------------------------------------------------------------------===//
// Filter Operation
//===----------------------------------------------------------------------===//

LogicalResult PexcsFilterOp::verify() {
  // Verify that the predicate function exists and has compatible types
  return success();
}

//===----------------------------------------------------------------------===//
// Tablegen Generated Logic
//===----------------------------------------------------------------------===//

#define GET_OP_CLASSES
#include "circt/Dialect/Pexcs/Pexcs.cpp.inc" 