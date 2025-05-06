//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

include "mlir/IR/CommonAttrConstraints.td"
include "mlir/IR/EnumAttr.td"
include "mlir/IR/OpBase.td"

def UI2 : UI<2>;
def UI4 : UI<4>;

def UI2Attr : TypedUnsignedIntegerAttrBase<
    UI2, "uint8_t", "2-bit unsigned integer attribute">;
def UI4Attr : TypedUnsignedIntegerAttrBase<
    UI4, "uint8_t", "4-bit unsigned integer attribute">;

def AsynchronousControl : NativeOpTrait<"AsynchronousControl"> {
  let cppNamespace = "::circt::xlnx";
}

def SynchronousControl : NativeOpTrait<"SynchronousControl"> {
  let cppNamespace = "::circt::xlnx";
}

def GateType : I32EnumAttr<"GateType", "Gate Type Enum", [
  I32EnumAttrCase<"FDCE", 0>,
  I32EnumAttrCase<"FDPE", 1>,
  I32EnumAttrCase<"FDRE", 2>,
  I32EnumAttrCase<"FDSE", 3>,
  I32EnumAttrCase<"LUT1", 4>,
  I32EnumAttrCase<"LUT2", 5>,
  I32EnumAttrCase<"LUT3", 6>,
  I32EnumAttrCase<"LUT4", 7>,
  I32EnumAttrCase<"LUT5", 8>,
  I32EnumAttrCase<"LUT6", 9>,
  I32EnumAttrCase<"MUXF7", 10>,
  I32EnumAttrCase<"MUXF8", 11>,
  I32EnumAttrCase<"MUXF9", 12>
]> {
  let cppNamespace = "::circt::xlnx";
}
