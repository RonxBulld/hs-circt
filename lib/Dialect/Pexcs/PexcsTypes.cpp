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
// PexcsOperandType 方法实现
//===----------------------------------------------------------------------===//

// 从空间ID和地址构造16位操作数值
static uint16_t packOperand(OperandSpace space, uint16_t addr) {
  // 确保地址在13位范围内
  addr &= OPERAND_ADDR_MASK;
  // 将空间ID左移13位并与地址合并
  return (static_cast<uint16_t>(space) << OPERAND_SPACE_SHIFT) | addr;
}

// 从16位操作数值中提取空间ID
static OperandSpace unpackSpace(uint16_t value) {
  return static_cast<OperandSpace>((value & OPERAND_SPACE_MASK) >> OPERAND_SPACE_SHIFT);
}

// 从16位操作数值中提取地址
static uint16_t unpackAddr(uint16_t value) {
  return value & OPERAND_ADDR_MASK;
}

OperandType OperandType::get(MLIRContext *context) {
  return Base::get(context);
}

// 创建立即数操作数
uint16_t OperandType::createImmediate(uint16_t value) {
  return packOperand(OperandSpace::Immediate, value);
}

// 创建内存访问操作数
uint16_t OperandType::createMemoryAccess(OperandSpace space, uint16_t addr) {
  if (space == OperandSpace::Immediate)
    return createImmediate(addr);
  return packOperand(space, addr);
}

// 获取操作数空间
OperandSpace OperandType::getSpace(uint16_t value) {
  return unpackSpace(value);
}

// 获取操作数地址或立即数值
uint16_t OperandType::getAddress(uint16_t value) {
  return unpackAddr(value);
}

// 判断操作数是否为立即数
bool OperandType::isImmediate(uint16_t value) {
  return getSpace(value) == OperandSpace::Immediate;
}

//===----------------------------------------------------------------------===//
// Generated Logic
//===----------------------------------------------------------------------===//

#include "llvm/ADT/TypeSwitch.h"
#define GET_TYPEDEF_CLASSES
#include "circt/Dialect/Pexcs/PexcsTypes.cpp.inc" 