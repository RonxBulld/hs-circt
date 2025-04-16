//===- PexcsTraits.h - Pexcs dialect traits ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the traits for the Pexcs dialect.
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_DIALECT_PEXCS_PEXCSTRAITS_H
#define CIRCT_DIALECT_PEXCS_PEXCSTRAITS_H

#include "mlir/IR/OpDefinition.h"

namespace mlir {
namespace OpTrait {

/// Trait for operations at the abstract level in the Pexcs dialect.
/// Corresponds to the AbstractLevel ODS trait.
template <typename ConcreteType>
class AbstractLevel
    : public mlir::OpTrait::TraitBase<ConcreteType, AbstractLevel> {};

/// Trait for operations at the calculate level in the Pexcs dialect.
/// Corresponds to the CalculateLevel ODS trait.
template <typename ConcreteType>
class CalculateLevel
    : public mlir::OpTrait::TraitBase<ConcreteType, CalculateLevel> {};

} // namespace OpTrait
} // namespace mlir

#endif // CIRCT_DIALECT_PEXCS_PEXCSTRAITS_H 