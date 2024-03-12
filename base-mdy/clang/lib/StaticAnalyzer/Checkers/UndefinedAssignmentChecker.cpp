//===--- UndefinedAssignmentChecker.h ---------------------------*- C++ -*--==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This defines UndefinedAssignmentChecker, a builtin check in ExprEngine that
// checks for assigning undefined values.
//
//===----------------------------------------------------------------------===//

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

using namespace clang;
using namespace ento;

namespace {
class UndefinedAssignmentChecker
 //   : public Checker<check::PostStmt<BinaryOperator>>
  : public Checker<check::Bind,check::PreStmt<ReturnStmt>>
{
  mutable std::unique_ptr<BugType> BT;

public:
  //检查Bind语句
  void checkBind(SVal location, SVal val, const Stmt *S,CheckerContext &C) const;
  void checkPreStmt(const ReturnStmt *RS, CheckerContext &C) const;
 // void checkPostStmt(const BinaryOperator* BO, CheckerContext& C) const;

};
}
//checkBind检查函数
void UndefinedAssignmentChecker::checkBind(SVal location, SVal val,
                                           const Stmt *StoreE,
                                           CheckerContext &C) const {
  //数值未定义
  //if (!val.isUndef())
  //{
    //return;
  //}

  // Do not report assignments of uninitialized values inside swap functions.
  // This should allow to swap partially uninitialized structs
  // (radar://14129997)
  //获取FunctionDecl
  if (const FunctionDecl *EnclosingFunctionDecl =dyn_cast<FunctionDecl>(C.getStackFrame()->getDecl()))
  {
    //交换函数不再检查
    if(C.getCalleeName(EnclosingFunctionDecl) == "swap")
    {
      return;
    }
  }
  //Generate a transition to a node that will be used to report an error.
  //This node will be a sink.That is, it will stop exploration of the given path.
  
  //ExplodedNode* N = C.generateErrorNode();

  //Generate a transition to a node that will be used to report an error.
  //This node will not be a sink.That is, exploration will continue along this path.
  //由于gb8114 R-11-1测试用例多次使用未初始化变量，且要求全部检查出来所以为了R-11-1检测不能使用generateErrorNode
  ExplodedNode *N = C.generateNonFatalErrorNode();
  
  if (!N)
  {
    return;
  }
  //定义报错字符串
  static const char *const DefaultMsg =
      "被赋的值是未定义的或者无效的";
  if (!BT)
  {
    //创建BuiltinBug
    BT.reset(new BuiltinBug(this, DefaultMsg));
  }

  // Generate a report for this bug.
  llvm::SmallString<128> Str;
  llvm::raw_svector_ostream OS(Str);
  //创建ex
  const Expr *ex = nullptr;
  bool HasError=false;
  //转换为UnaryOperator
  if (const UnaryOperator *MatchedUnaryOperator = dyn_cast<UnaryOperator>(StoreE))
  {
    //如果UnaryOperator的子节点未定义
    if(C.getSVal(MatchedUnaryOperator->getSubExpr()).isUndef())
    {
      OS << "一元操作的表达式是一个未初始化的值，一元操作运算是无效的";
      //转换为子节点
      ex = MatchedUnaryOperator->getSubExpr();
      HasError=true;
    }
  }
  //如果是BinaryOperator
  else if(const BinaryOperator *B = dyn_cast<BinaryOperator>(StoreE))
  {
    //是一个赋值符号
    if(B->isAssignmentOp())
    {
      //如果右值未定义
      if(C.getSVal(B->getRHS()).isUndef())
      {
          OS << "赋值运算的右值是一个未初始化的值，赋值语句无效";
          //获取BinaryOperator的左值
          ex = B->getRHS();
          HasError=true;
      }
    }
  }
  //如果是DeclStmt
  else if (const DeclStmt *DS = dyn_cast<DeclStmt>(StoreE))
  {
    //转换为VarDecl
    const VarDecl *VD = cast<VarDecl>(DS->getSingleDecl());
    //如果VarDecl已经被初始化
    if(VD->hasInit())
    {
      //初始化数值未定义
      if(C.getSVal(VD->getInit()).isUndef())
      {
        OS << "变量定义语句的值未初始化，赋值语句无效";
        //获取VarDecl的初始化内容
        ex = VD->getInit();
        HasError=true;
      }
    }
  }
  if (const auto *CD =dyn_cast<CXXConstructorDecl>(C.getStackFrame()->getDecl()))
  {
    if (CD->isImplicit())
    {
      for (auto I : CD->inits())
      {
        if (I->getInit()->IgnoreImpCasts() == StoreE)
        {
          OS<<"在隐式构造中赋值到域'"<<I->getMember()->getName()<<"'的值是未定义的或者无效的";
          break;
        }
      }
    }
  }

  if (HasError==true)
  {
    if (OS.str().empty())
    {
      OS << DefaultMsg;
    }
    auto R = std::make_unique<PathSensitiveBugReport>(*BT, OS.str(), N);
    R->addRange(ex->getSourceRange());
    bugreporter::trackExpressionValue(N, ex, *R);
    C.emitReport(std::move(R));
  }
}
void UndefinedAssignmentChecker::checkPreStmt(const ReturnStmt *RS,
                                          CheckerContext &C) const {
    if(C.getSVal(RS->getRetValue()).isUndef())
    {
      llvm::SmallString<128> Str;
      llvm::raw_svector_ostream OS(Str);
      OS << "返回值是一个未初始化的值，赋值语句无效";
      ExplodedNode *N = C.generateNonFatalErrorNode();
      auto R = std::make_unique<PathSensitiveBugReport>(*BT, OS.str(), N);
      R->addRange(RS->getRetValue()->getSourceRange());
      bugreporter::trackExpressionValue(N, RS->getRetValue(), *R);
      C.emitReport(std::move(R));
    }
}
//此处代码为了检测CSR-49
/*
void UndefinedAssignmentChecker::checkPostStmt(const BinaryOperator* BO, CheckerContext& C) const
{
    std::cout << "start\n";
    AnalysisDeclContext* AC = C.getCurrentAnalysisDeclContext();
    //Generate a transition to a node that will be used to report an error.
    //This node will be a sink.That is, it will stop exploration of the given path.

    //ExplodedNode* N = C.generateErrorNode();

    //Generate a transition to a node that will be used to report an error.
    //This node will not be a sink.That is, exploration will continue along this path.
    //由于gb8114 R-11-1测试用例多次使用未初始化变量，且要求全部检查出来所以为了R-11-1检测不能使用generateErrorNode

    ExplodedNode* N = C.generateNonFatalErrorNode();
    static const char* const DefaultMsg =
        "被赋的值是未定义的或者无效的";
    const Expr* ex = nullptr;
    llvm::SmallString<128> Str;
    llvm::raw_svector_ostream OS(Str);
    if (BO->isAssignmentOp())
    {
        std::cout << "isAssignmentOp\n";
       (C.getSVal(BO->getRHS())).dump();
        if (C.getSVal(BO->getRHS()).isUndef())
        {

            PathDiagnosticLocation ELoc(BO->getOperatorLoc(), C.getSourceManager());
            C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "", categories::SecurityError, "赋值运算的左值是一个未初始化的值，赋值语句无效", ELoc, BO->getRHS()->getSourceRange());

        }
    }
    if (BO->isEqualityOp())
    {
        if (C.getSVal(BO->getLHS()).isUndef())
        {
            PathDiagnosticLocation ELoc(BO->getOperatorLoc(), C.getSourceManager());
            C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "", categories::SecurityError, "比较运算的左值是一个未初始化的值，赋值语句无效", ELoc, BO->getLHS()->getSourceRange());

        }
        else if (C.getSVal(BO->getRHS()).isUndef())
        {
            PathDiagnosticLocation ELoc(BO->getOperatorLoc(), C.getSourceManager());
            C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "", categories::SecurityError, "比较运算的左值是一个未初始化的值，赋值语句无效", ELoc, BO->getRHS()->getSourceRange());

        }

    }
    


}
*/
void ento::registerUndefinedAssignmentChecker(CheckerManager &mgr) {
  mgr.registerChecker<UndefinedAssignmentChecker>();
}

bool ento::shouldRegisterUndefinedAssignmentChecker(const CheckerManager &mgr) {
  return true;
}
