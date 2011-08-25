//===--- IRGenFunction.cpp - Swift Per-Function IR Generation -------------===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2015 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
//  This file implements basic setup and teardown for the class which
//  performs IR generation for function bodies.
//
//===----------------------------------------------------------------------===//

#include "llvm/Instructions.h"

#include "IRGenFunction.h"
#include "IRGenModule.h"

using namespace swift;
using namespace irgen;

IRGenFunction::IRGenFunction(IRGenModule &IGM, FuncExpr *FE, llvm::Function *Fn)
  : IGM(IGM), Builder(IGM.getLLVMContext()), CurFuncExpr(FE), CurFn(Fn) {
}

/// Create an alloca whose lifetime is the duration of the current
/// full-expression.
llvm::AllocaInst *
IRGenFunction::createFullExprAlloca(llvm::Type *Ty, Alignment Align,
                                    const llvm::Twine &Name) {
  assert(AllocaIP && "alloca insertion point has not been initialized!");
  llvm::AllocaInst *Alloca = new llvm::AllocaInst(Ty, Name, AllocaIP);
  Alloca->setAlignment(Align.getValue());

  // TODO: lifetime intrinsics.
  return Alloca;
}

/// Create an alloca whose lifetime is the duration of the current
/// scope.
llvm::AllocaInst *
IRGenFunction::createScopeAlloca(llvm::Type *Ty, Alignment Align,
                                 const llvm::Twine &Name) {
  assert(AllocaIP && "alloca insertion point has not been initialized!");
  llvm::AllocaInst *Alloca = new llvm::AllocaInst(Ty, Name, AllocaIP);
  Alloca->setAlignment(Align.getValue());

  // TODO: lifetime intrinsics.
  return Alloca;
}

/// Create a new basic block with the given name.  The block is not
/// automatically inserted into the function.
llvm::BasicBlock *
IRGenFunction::createBasicBlock(const llvm::Twine &Name) {
  return llvm::BasicBlock::Create(IGM.getLLVMContext(), Name);
}
