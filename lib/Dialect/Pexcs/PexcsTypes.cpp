//===- PexcsTypes.cpp - Pexcs dialect types ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// clang-format off
#include "circt/Dialect/Pexcs/PexcsTypes.h"
#include "circt/Dialect/Pexcs/PexcsDialect.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/DialectImplementation.h"
// clang-format on

using namespace mlir;
using namespace circt;
using namespace circt::pexcs;

//===----------------------------------------------------------------------===//
// Generated Logic
//===----------------------------------------------------------------------===//

#define GET_TYPEDEF_CLASSES
#include "circt/Dialect/Pexcs/PexcsTypes.cpp.inc" 