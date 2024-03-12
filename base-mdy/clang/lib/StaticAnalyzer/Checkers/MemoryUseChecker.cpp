//== MemoryUseChecker.cpp ------------------------------*- C++ -*--==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//禁止使用已被释放了的内存空间
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/DynamicExtent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h"
#include<iostream>
#include "clang/AST/ExprObjC.h"
#include "clang/AST/ExprOpenMP.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerHelpers.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace ento;
using namespace std;

namespace {
class MemoryUseChecker :
    public Checker<check::Location> {
  mutable std::unique_ptr<BuiltinBug> BT;
  enum DerefKind { MemoryError};
  BugType BT_Memory{this, "禁止使用已被释放了的内存空间", categories::LogicError};

  void reportBug(DerefKind K, ProgramStateRef State, const Stmt *S,
                 CheckerContext &C) const;

public:
  void checkLocation(SVal l, bool isLoad, const Stmt* S,
                     CheckerContext &C) const;
};
}
void MemoryUseChecker::reportBug(DerefKind K, ProgramStateRef State,
                                   const Stmt *S, CheckerContext &C) const {
  const BugType *BT = nullptr;
  llvm::StringRef DerefStr1;
  switch (K) {
  case DerefKind::MemoryError:
    BT = &BT_Memory;
    DerefStr1 = " results in a null pointer dereference";
    break;
  };

  // Generate an error node.
  ExplodedNode *N = C.generateErrorNode(State);
  if (!N)
    return;

  SmallString<100> buf;
  llvm::raw_svector_ostream os(buf);

  SmallVector<SourceRange, 2> Ranges;


  auto report = std::make_unique<PathSensitiveBugReport>(
      *BT, buf.empty() ? BT->getDescription() : StringRef(buf), N);

  bugreporter::trackExpressionValue(N, bugreporter::getDerefExpr(S), *report);

  for (SmallVectorImpl<SourceRange>::iterator
       I = Ranges.begin(), E = Ranges.end(); I!=E; ++I)
    report->addRange(*I);

  C.emitReport(std::move(report));
}

std::pair<const clang::VarDecl *, const clang::Expr *>
parseAssignment(const Stmt *S) {
  const VarDecl *VD = nullptr;
  const Expr *RHS = nullptr;

  if (auto Assign = dyn_cast_or_null<BinaryOperator>(S)) {
    if (Assign->isAssignmentOp()) {
      // Ordinary assignment
      RHS = Assign->getRHS();
      if (auto DE = dyn_cast_or_null<DeclRefExpr>(Assign->getLHS()))
        VD = dyn_cast_or_null<VarDecl>(DE->getDecl());
    }
  } else if (auto PD = dyn_cast_or_null<DeclStmt>(S)) {
    // Initialization
    assert(PD->isSingleDecl() && "We process decls one by one");
    VD = cast<VarDecl>(PD->getSingleDecl());
    RHS = VD->getAnyInitializer();
  }

  return std::make_pair(VD, RHS);
}

// static const Expr *getmemoryExpr(const Stmt *S, bool IsBind=false){
//   const Expr *E = nullptr;

//   // Walk through lvalue casts to get the original expression
//   // that syntactically caused the load.
//   if (const Expr *expr = dyn_cast<Expr>(S))
//     E = expr->IgnoreParenLValueCasts();

//   if (IsBind) {
//     const VarDecl *VD;
//     const Expr *Init;
//     //std::tie(VD, Init) = parseAssignment(S);
//     if (VD && Init)
//       E = Init;
//   }
//   return E;
// }

static bool suppressReport(const Expr *E) {
  // Do not report dereferences on memory in non-default address spaces.
  return E->getType().hasAddressSpace();
}

void MemoryUseChecker::checkLocation(SVal l, bool isLoad, const Stmt* LoadS,
                                      CheckerContext &C) const {
  // cout<<"进入stringchecker"<<endl;
  const MemRegion *R = l.getAsRegion();
  if (!R)
    return;

  const ElementRegion *ER = dyn_cast<ElementRegion>(R);
  if (!ER)
    return;

  if(!(R->hasStackStorage())){
    // cout<<"进入到判断是否有内存空间里面"<<endl;
    // R->dump();
    ProgramStateRef state = C.getState();
    DefinedOrUnknownSVal location = l.castAs<DefinedOrUnknownSVal>();
    ProgramStateRef notNullState, nullState;
    std::tie(notNullState, nullState) = state->assume(location);

  //   if (nullState) {
  //   if (!notNullState) {
  //     // We know that 'location' can only be null.  This is what
  //     // we call an "explicit" null dereference.
  //     const Expr *expr = getmemoryExpr(LoadS);
  //     if (!suppressReport(expr)) {
  //       reportBug(DerefKind::MemoryError, nullState, expr, C);
  //       return;
  //     }
  //   }

  //   // Otherwise, we have the case where the location could either be
  //   // null or not-null.  Record the error node as an "implicit" null
  //   // dereference.
  //   // if (ExplodedNode *N = C.generateSink(nullState, C.getPredecessor())) {
  //   //   ImplicitNullDerefEvent event = {l, isLoad, N, &C.getBugReporter(),
  //   //                                   /*IsDirectDereference=*/true};
  //   //   dispatchEvent(event);
  //   // }
  // }


  // Get the size of the array.
  DefinedOrUnknownSVal ElementCount = getDynamicElementCount(
      state, ER->getSuperRegion(), C.getSValBuilder(), ER->getValueType());

  // ProgramStateRef StInBound = state->getSValAsScalarOrLoc(R);
  // ExplodedNode *N = C.generateErrorNode(StInBound);
  // if (!N)
  //   return;

  //   if (!BT)
  //     BT.reset(new BuiltinBug(
  //         this, "越界数组访问 ，访问越界数组元素（缓冲区溢出）"));

  //   // FIXME: It would be nice to eventually make this diagnostic more clear,
  //   // e.g., by referencing the original declaration or by saying *why* this
  //   // reference is outside the range.

  //   // Generate a report for this bug.
  //   auto report =
  //       std::make_unique<PathSensitiveBugReport>(*BT, BT->getDescription(), N);

  //   report->addRange(LoadS->getSourceRange());
  //   C.emitReport(std::move(report));
  //   return;
  }
  }
  // Array bound check succeeded.  From this point forward the array bound
  // should always succeed.


void ento::registerMemoryUseChecker(CheckerManager &mgr) {
  mgr.registerChecker<MemoryUseChecker>();
}

bool ento::shouldRegisterMemoryUseChecker(const CheckerManager &mgr) {
  return true;
}


#if 0
#include<stdlib.h>
#include<malloc.h>
int main(void){
    int *x = (int *)malloc(sizeof(int));
    int y;
    if(NULL!=x){
        *x=1;
        free(x);
        x=NULL;
    }
    else{
        return(-1);
    }
    y=(*x);
    return(0);
}


#endif