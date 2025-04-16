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

void PexcsDialect::registerOperations() {
  // 注册操作
  addOperations<
#define GET_OP_LIST
#include "circt/Dialect/Pexcs/Pexcs.cpp.inc"
      >();
}
