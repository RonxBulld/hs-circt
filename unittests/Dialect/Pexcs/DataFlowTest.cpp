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

TEST_F(PexcsDataFlowTest, ParseMapOp) {
  StringRef moduleStr = R"(
    module {
      func.func @test_map(%arg0: tensor<10xi32>) -> tensor<10xi32> {
        %0 = pexcs.map(%arg0) {mapper = @square_fn} : tensor<10xi32> -> tensor<10xi32>
        return %0 : tensor<10xi32>
      }

      func.func private @square_fn(i32) -> i32
    }
  )";

  OwningOpRef<ModuleOp> module = parseSourceString<ModuleOp>(moduleStr, &context);
  ASSERT_TRUE(module);
  
  Operation *funcOp = module->lookupSymbol("test_map");
  ASSERT_TRUE(funcOp);
  
  auto mapOps = funcOp->getRegion(0).front().getOps<pexcs::PexcsMapOp>();
  ASSERT_TRUE(llvm::any_of(mapOps, [](pexcs::PexcsMapOp) { return true; }));
}

TEST_F(PexcsDataFlowTest, ParseReduceOp) {
  StringRef moduleStr = R"(
    module {
      func.func @test_reduce(%arg0: tensor<10xi32>, %arg1: i32) -> i32 {
        %0 = pexcs.reduce(%arg0, %arg1) {reducer = @add_fn} : (tensor<10xi32>, i32) -> i32
        return %0 : i32
      }

      func.func private @add_fn(i32, i32) -> i32
    }
  )";

  OwningOpRef<ModuleOp> module = parseSourceString<ModuleOp>(moduleStr, &context);
  ASSERT_TRUE(module);
  
  Operation *funcOp = module->lookupSymbol("test_reduce");
  ASSERT_TRUE(funcOp);
  
  auto reduceOps = funcOp->getRegion(0).front().getOps<pexcs::PexcsReduceOp>();
  ASSERT_TRUE(llvm::any_of(reduceOps, [](pexcs::PexcsReduceOp) { return true; }));
}

TEST_F(PexcsDataFlowTest, ParseFilterOp) {
  StringRef moduleStr = R"(
    module {
      func.func @test_filter(%arg0: tensor<10xi32>) -> tensor<?xi32> {
        %0 = pexcs.filter(%arg0) {predicate = @is_positive} : tensor<10xi32> -> tensor<?xi32>
        return %0 : tensor<?xi32>
      }

      func.func private @is_positive(i32) -> i1
    }
  )";

  OwningOpRef<ModuleOp> module = parseSourceString<ModuleOp>(moduleStr, &context);
  ASSERT_TRUE(module);
  
  Operation *funcOp = module->lookupSymbol("test_filter");
  ASSERT_TRUE(funcOp);
  
  auto filterOps = funcOp->getRegion(0).front().getOps<pexcs::PexcsFilterOp>();
  ASSERT_TRUE(llvm::any_of(filterOps, [](pexcs::PexcsFilterOp) { return true; }));
}

} // namespace 