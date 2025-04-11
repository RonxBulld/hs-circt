//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CIRCT_DIALECT_PEXCS_PEXCSTYPES_H
#define CIRCT_DIALECT_PEXCS_PEXCSTYPES_H

// clang-format off
#include "circt/Dialect/Pexcs/PexcsDialect.h"
#include "mlir/IR/Types.h"
#include "circt/Dialect/Pexcs/PexcsTypes.h.inc"
// clang-format on

namespace circt {
namespace pexcs {

// UI2 and UI4 type definition (actual implementation uses 8 bits)
using UI2Attr = IntegerAttr;
using UI4Attr = IntegerAttr;

} // namespace pexcs
} // namespace circt

#endif // CIRCT_DIALECT_PEXCS_PEXCSTYPES_H 