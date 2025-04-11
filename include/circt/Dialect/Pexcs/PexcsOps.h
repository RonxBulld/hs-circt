//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_DIALECT_PEXCS_PEXCSOPS_H
#define CIRCT_DIALECT_PEXCS_PEXCSOPS_H

// clang-format off
#include "circt/Dialect/Pexcs/PexcsDialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "mlir/Bytecode/BytecodeOpInterface.h"
#include "mlir/IR/ValueRange.h"
#include "circt/Dialect/Pexcs/PexcsTypes.h"

#define GET_OP_CLASSES
#include "circt/Dialect/Pexcs/Pexcs.h.inc"
// clang-format on

namespace circt {
namespace pexcs {
namespace pexcs_helper {
namespace application {
template <class Action>
struct Applicator {
  template <class Opt>
  static void apply(const Action &action, Opt &O) {
    action.apply(O);
  }
};

template <typename Obj>
void ApplyAction(Obj &O) {
  (void)O;
}

template <typename Obj, typename Action>
void ApplyAction(Obj &O, const Action &action) {
  Applicator<Action>::apply(action, O);
}

template <typename Obj, typename Action, typename... Actions>
void ApplyAction(Obj &O, const Action &action, const Actions &...actions) {
  ApplyAction(O, action);
  ApplyAction(O, actions...);
}
} // namespace application
} // namespace pexcs_helper
} // namespace pexcs
} // namespace circt

#endif // CIRCT_DIALECT_PEXCS_PEXCSOPS_H 