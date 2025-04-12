//===- DataFlowTest.cpp - Tests for Pexcs dialect data flow ops -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "circt/Dialect/Pexcs/PexcsDialect.h"
#include "circt/Dialect/Pexcs/PexcsOps.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/OwningOpRef.h"
#include "mlir/Parser/Parser.h"
#include "mlir/Pass/PassManager.h"
#include "gtest/gtest.h"

using namespace mlir;
using namespace circt;

namespace {

class PexcsDataFlowTest : public testing::Test {
protected:
  void SetUp() override {
    context.getOrLoadDialect<pexcs::PexcsDialect>();
  }

  MLIRContext context;
};

} // namespace 