//===-- PexcsTraits.td - Pexcs traits ----------------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines traits for Pexcs dialect operators.
//
//===----------------------------------------------------------------------===//

#ifndef PEXCS_TRAITS
#define PEXCS_TRAITS

include "mlir/IR/OpBase.td"

// Trait for operators belonging to the Abstract Level.
// These operators might be lowered to the Calculate Level.
def AbstractLevel : NativeOpTrait<"AbstractLevel">;

// Trait for operators belonging to the Calculate Level.
// These operators represent concrete computations.
def CalculateLevel : NativeOpTrait<"CalculateLevel">;

#endif // PEXCS_TRAITS 