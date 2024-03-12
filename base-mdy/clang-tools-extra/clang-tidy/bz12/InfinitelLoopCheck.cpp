//===--- InfinitelLoopCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "InfinitelLoopCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Analysis/Analyses/ExprMutationAnalyzer.h"
#include "../utils/Aliasing.h"

using namespace clang::ast_matchers;
using clang::tidy::utils::hasPtrOrReferenceInFunc;

namespace clang {
namespace tidy {
namespace bz12 {

///传入一个循环和要检测的变量，判断变量是否在循环中被改变
static bool isChanged(const Stmt *LoopStmt, const VarDecl *Var,
                      ASTContext *Context) {
  //如果是ForStmt
  if (const auto *ForLoop = dyn_cast<ForStmt>(LoopStmt))
    //判断是否改变
    return (ForLoop->getInc() &&
            ExprMutationAnalyzer(*ForLoop->getInc(), *Context)
                .isMutated(Var)) ||
           (ForLoop->getBody() &&
            ExprMutationAnalyzer(*ForLoop->getBody(), *Context)
                .isMutated(Var)) ||
           (ForLoop->getCond() &&
            ExprMutationAnalyzer(*ForLoop->getCond(), *Context).isMutated(Var));
  //判断是否改变
  return ExprMutationAnalyzer(*LoopStmt, *Context).isMutated(Var);
}

/// Return whether `Cond` is a variable that is possibly changed in `LoopStmt`.
static bool isVarThatIsPossiblyChanged(const FunctionDecl *Func,
                                       const Stmt *LoopStmt, const Stmt *Cond,
                                       ASTContext *Context) {
  if (const auto *DRE = dyn_cast<DeclRefExpr>(Cond)) {
    if (const auto *Var = dyn_cast<VarDecl>(DRE->getDecl())) {
      //全局变量认为可以修改
      if (!Var->isLocalVarDeclOrParm())
        return true;
      //volatile类型变量认为可以修改
      if (Var->getType().isVolatileQualified())
        return true;
      //整数指针
      if (!Var->getType().getTypePtr()->isIntegerType())
        return true;
      //存在指针
      return hasPtrOrReferenceInFunc(Func, Var) ||
             isChanged(LoopStmt, Var, Context);
    }
  }
  //都算修改过
  else if (isa<MemberExpr>(Cond) || isa<CallExpr>(Cond)) {
    return true;
  }

  return false;
}

// / Return whether at least one variable of `Cond` changed in `LoopStmt`.
static bool isAtLeastOneCondVarChanged(const FunctionDecl *Func,
                                       const Stmt *LoopStmt, const Stmt *Cond,
                                       ASTContext *Context) {
  if (isVarThatIsPossiblyChanged(Func, LoopStmt, Cond, Context))
    return true;

  for (const Stmt *Child : Cond->children()) {
    if (!Child)
      continue;
    //对每个子节点检查
    if (isAtLeastOneCondVarChanged(Func, LoopStmt, Child, Context))
      return true;
  }
  //没有修改过
  return false;
}

/// Return the variable names in `Cond`.
static std::string getCondVarNames(const Stmt *Cond) {
  if (const auto *DRE = dyn_cast<DeclRefExpr>(Cond)) {
    if (const auto *Var = dyn_cast<VarDecl>(DRE->getDecl()))
      //返回名称
      return std::string(Var->getName());
  }

  std::string Result;
  for (const Stmt *Child : Cond->children()) {
    if (!Child)
      continue;

    std::string NewNames = getCondVarNames(Child);
    if (!Result.empty() && !NewNames.empty())
      Result += ", ";
    Result += NewNames;
  }
  return Result;
}

static bool isKnownFalse(const Expr &Cond, const ASTContext &Ctx) {
  if (Cond.isValueDependent())
    return false;
  bool Result = false;
  if (Cond.EvaluateAsBooleanCondition(Result, Ctx))
    return !Result;
  return false;
}
void InfinitelLoopCheck::registerMatchers(MatchFinder *Finder) {
  //定义LoopCondition为条件并绑定相应函数，并排除后代中存在return，goto等语句的条件(不排除break)
  const auto LoopCondition = allOf(
      hasCondition
      (
        expr(forFunction(functionDecl().bind("func"))).bind("condition")),
        unless(hasBody(hasDescendant(returnStmt(forFunction(equalsBoundNode("func")))))),
        unless(hasBody(hasDescendant(gotoStmt(forFunction(equalsBoundNode("func")))))),
        unless(hasBody(hasDescendant(cxxThrowExpr(forFunction(equalsBoundNode("func")))))),
        unless(hasBody(hasDescendant(callExpr(forFunction(equalsBoundNode("func")), callee(functionDecl(isNoReturn()))))))
      );
  //匹配全部具有上述条件的while、do-while和for语句并绑定到字符串loop-stmt上
  Finder->addMatcher(stmt(anyOf(whileStmt(LoopCondition), doStmt(LoopCondition),
                                forStmt(LoopCondition)))
                         .bind("loop-stmt"),
                     this);
  //检测while、do-while等语句及其子break节点
  Finder->addMatcher(whileStmt(forEachDescendant(breakStmt().bind("breakStmtInWhile"))).bind("whileStmt"),this);
  Finder->addMatcher(doStmt(forEachDescendant(breakStmt().bind("breakStmtInDo"))).bind("doStmt"),this);
  Finder->addMatcher(forStmt(forEachDescendant(breakStmt().bind("breakStmtInFor"))).bind("forStmt"),this);
  Finder->addMatcher(switchStmt(forEachDescendant(breakStmt().bind("breakStmtInSwitch"))).bind("switchStmt"),this);
  const auto LoopConditionRepair = allOf(
      hasCondition
      (
        expr(forFunction(functionDecl().bind("func-repair"))).bind("condition-repair")),
        hasBody(forEachDescendant(breakStmt(forFunction(equalsBoundNode("func-repair"))).bind("breakStmtRepair")))
      );
  //匹配while、do-while、for及其全部的子break节点
  Finder->addMatcher(stmt(anyOf(whileStmt(LoopConditionRepair), doStmt(LoopConditionRepair),
                                forStmt(LoopConditionRepair)))
                         .bind("loop-stmt-repair"),
                     this);
}
//将检测过程抽象为函数
void InfinitelLoopCheck::checkFunction(const clang::Expr *Cond,const clang::Stmt *LoopStmt,const clang::FunctionDecl *Func,const clang::ast_matchers::MatchFinder::MatchResult &Result)
{
  //条件恒为false
  if (isKnownFalse(*Cond, *Result.Context))
  {
    //不存在无限循环问题
    return;
  }
  //定义应该有条件变量判断标志
  bool ShouldHaveConditionVariables = true;
  //如果是WhileStmt
  if (const auto *While = dyn_cast<WhileStmt>(LoopStmt))
  {
    //常量直接报错
    if(const IntegerLiteral *IL = dyn_cast<IntegerLiteral>(Cond))
    {
      //无限循环
      ResultSet.insert(LoopStmt);
      return;
    }
    //定义语句
    else if (const VarDecl *LoopVarDecl = While->getConditionVariable())
    {
      //获取初始化内容
      if (const Expr *Init = LoopVarDecl->getInit())
      {
        ShouldHaveConditionVariables = false;
        Cond = Init;
      }
    }
  }
  //循环变量是否在循环中被改变
  if(isAtLeastOneCondVarChanged(Func, LoopStmt, Cond, Result.Context))
  {
    return;
  }
//获取条件变量的名称
  std::string CondVarNames = getCondVarNames(Cond);
  if (ShouldHaveConditionVariables && CondVarNames.empty())
  {
    return;
  }
  //循环根本没有变量
  if (CondVarNames.empty()) {
    ResultSet.insert(LoopStmt);
  }
  else
  {
    ResultSet.insert(LoopStmt);
  }
}
void InfinitelLoopCheck::check(const MatchFinder::MatchResult &Result) {
  //获取条件
  if(const auto *Cond = Result.Nodes.getNodeAs<Expr>("condition"))
  {
    //获取循环语句
    if(const auto *LoopStmt = Result.Nodes.getNodeAs<Stmt>("loop-stmt"))
    {
      //匹配循环所在函数
      if(const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func"))
      {
        checkFunction(Cond,LoopStmt,Func,Result);
      }
    }
  }
  //匹配while及其子break，并执行break匹配算法(让break找到与之对应的节点，而不是子节点有就算他的)
  else if(const auto *MatchedBreakStmtInWhile= Result.Nodes.getNodeAs<BreakStmt>("breakStmtInWhile"))
  {
    if(const auto *MatchedWhileStmt= Result.Nodes.getNodeAs<WhileStmt>("whileStmt"))
    {
      if(LocationBeginMap.find(MatchedBreakStmtInWhile)==LocationBeginMap.end() && LocationEndMap.find(MatchedBreakStmtInWhile)==LocationEndMap.end())
      {
        LocationBeginMap[MatchedBreakStmtInWhile]=MatchedWhileStmt->getBody()->getBeginLoc().getHashValue();
        LocationEndMap[MatchedBreakStmtInWhile]=MatchedWhileStmt->getBody()->getEndLoc().getHashValue();
      }
      else if(LocationBeginMap[MatchedBreakStmtInWhile]<MatchedWhileStmt->getBody()->getBeginLoc().getHashValue() && LocationEndMap[MatchedBreakStmtInWhile]>MatchedWhileStmt->getBody()->getEndLoc().getHashValue())
      {
        LocationBeginMap[MatchedBreakStmtInWhile]=MatchedWhileStmt->getBody()->getBeginLoc().getHashValue();
        LocationEndMap[MatchedBreakStmtInWhile]=MatchedWhileStmt->getBody()->getEndLoc().getHashValue();
      }
    }
  }
  //匹配do-while，同样有break匹配算法
  else if(const auto *MatchedBreakStmtInDo= Result.Nodes.getNodeAs<BreakStmt>("breakStmtInDo"))
  {
    if(const auto *MatchedDoStmt= Result.Nodes.getNodeAs<DoStmt>("doStmt"))
    {
      if(LocationBeginMap.find(MatchedBreakStmtInDo)==LocationBeginMap.end() && LocationEndMap.find(MatchedBreakStmtInDo)==LocationEndMap.end())
      {
        LocationBeginMap[MatchedBreakStmtInDo]=MatchedDoStmt->getBody()->getBeginLoc().getHashValue();
        LocationEndMap[MatchedBreakStmtInDo]=MatchedDoStmt->getBody()->getEndLoc().getHashValue();
      }
      else if(LocationBeginMap[MatchedBreakStmtInDo]<MatchedDoStmt->getBody()->getBeginLoc().getHashValue() && LocationEndMap[MatchedBreakStmtInDo]>MatchedDoStmt->getBody()->getEndLoc().getHashValue())
      {
        LocationBeginMap[MatchedBreakStmtInDo]=MatchedDoStmt->getBody()->getBeginLoc().getHashValue();
        LocationEndMap[MatchedBreakStmtInDo]=MatchedDoStmt->getBody()->getEndLoc().getHashValue();
      }
    }
  }
  else if(const auto *MatchedBreakStmtInFor= Result.Nodes.getNodeAs<BreakStmt>("breakStmtInFor"))
  {
    if(const auto *MatchedForStmt= Result.Nodes.getNodeAs<ForStmt>("forStmt"))
    {
      if(LocationBeginMap.find(MatchedBreakStmtInFor)==LocationBeginMap.end() && LocationEndMap.find(MatchedBreakStmtInFor)==LocationEndMap.end())
      {
        LocationBeginMap[MatchedBreakStmtInFor]=MatchedForStmt->getBody()->getBeginLoc().getHashValue();
        LocationEndMap[MatchedBreakStmtInFor]=MatchedForStmt->getBody()->getEndLoc().getHashValue();
      }
      else if(LocationBeginMap[MatchedBreakStmtInFor]<MatchedForStmt->getBody()->getBeginLoc().getHashValue() && LocationEndMap[MatchedBreakStmtInFor]>MatchedForStmt->getBody()->getEndLoc().getHashValue())
      {
        LocationBeginMap[MatchedBreakStmtInFor]=MatchedForStmt->getBody()->getBeginLoc().getHashValue();
        LocationEndMap[MatchedBreakStmtInFor]=MatchedForStmt->getBody()->getEndLoc().getHashValue();
      }
    }
  }
  else if(const auto *MatchedBreakStmtInSwitch= Result.Nodes.getNodeAs<BreakStmt>("breakStmtInSwitch"))
  {
    if(const auto *MatchedSwitchStmt= Result.Nodes.getNodeAs<SwitchStmt>("switchStmt"))
    {
      if(LocationBeginMap.find(MatchedBreakStmtInSwitch)==LocationBeginMap.end() && LocationEndMap.find(MatchedBreakStmtInSwitch)==LocationEndMap.end())
      {
        LocationBeginMap[MatchedBreakStmtInSwitch]=MatchedSwitchStmt->getBody()->getBeginLoc().getHashValue();
        LocationEndMap[MatchedBreakStmtInSwitch]=MatchedSwitchStmt->getBody()->getEndLoc().getHashValue();
      }
      else if(LocationBeginMap[MatchedBreakStmtInSwitch]<MatchedSwitchStmt->getBody()->getBeginLoc().getHashValue() && LocationEndMap[MatchedBreakStmtInSwitch]>MatchedSwitchStmt->getBody()->getEndLoc().getHashValue())
      {
        LocationBeginMap[MatchedBreakStmtInSwitch]=MatchedSwitchStmt->getBody()->getBeginLoc().getHashValue();
        LocationEndMap[MatchedBreakStmtInSwitch]=MatchedSwitchStmt->getBody()->getEndLoc().getHashValue();
      }
    }
  }
  //将Loop与全部的break建立关联
  else if(const auto *MatchedBreakStmt = Result.Nodes.getNodeAs<BreakStmt>("breakStmtRepair"))
  {
    //获取循环语句
    if(const auto *LoopStmt = Result.Nodes.getNodeAs<Stmt>("loop-stmt-repair"))
    {
      //匹配循环所在函数
      if(const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func-repair"))
      {
        if(const auto *Cond = Result.Nodes.getNodeAs<Expr>("condition-repair"))
        {
          BreakStmtMap[LoopStmt].insert(MatchedBreakStmt);
        }
      }
    }
  }
}
void InfinitelLoopCheck::onEndOfTranslationUnit() {
  //剔除BreakStmtMap中实际上不是用来终止该循环的break(依据LocationBeginMap和LocationEndMap，这两个哈希表实质上已经指向了完全正确的范围)
  for(auto it=BreakStmtMap.begin();it!=BreakStmtMap.end();it++)
  {
    const Stmt * LoopStmt=it->first;
    unsigned BeginLocation,EndLocation;
    //还原LoopStmt
    if(isa<DoStmt>(LoopStmt))
    {
      const DoStmt *MatchedDoStmt=dyn_cast<DoStmt>(LoopStmt);
      BeginLocation=MatchedDoStmt->getBody()->getBeginLoc().getHashValue();
      EndLocation=MatchedDoStmt->getBody()->getEndLoc().getHashValue();
    }
    else if(isa<WhileStmt>(LoopStmt))
    {
      const WhileStmt *MatchedWhileStmt=dyn_cast<WhileStmt>(LoopStmt);
      BeginLocation=MatchedWhileStmt->getBody()->getBeginLoc().getHashValue();
      EndLocation=MatchedWhileStmt->getBody()->getEndLoc().getHashValue();
    }
    else if(isa<ForStmt>(LoopStmt))
    {
      const ForStmt *MatchedForStmt=dyn_cast<ForStmt>(LoopStmt);
      BeginLocation=MatchedForStmt->getBody()->getBeginLoc().getHashValue();
      EndLocation=MatchedForStmt->getBody()->getEndLoc().getHashValue();
    }
    else if(isa<SwitchStmt>(LoopStmt))
    {
      const SwitchStmt *MatchedSwitchStmt=dyn_cast<SwitchStmt>(LoopStmt);
      BeginLocation=MatchedSwitchStmt->getBody()->getBeginLoc().getHashValue();
      EndLocation=MatchedSwitchStmt->getBody()->getEndLoc().getHashValue();
    }
    std::set<const BreakStmt *>::iterator BreakStmtIter=it->second.begin();
		while (BreakStmtIter!=it->second.end())
		{
      if(LocationEndMap[*BreakStmtIter]!=EndLocation && LocationBeginMap[*BreakStmtIter]!=BeginLocation)
      {
        it->second.erase(BreakStmtIter++);
      }
      else
      {
        BreakStmtIter++;
      }
		}
  }
  //依据BreakStmtMap的内容剔除报错信息
  for(auto it=BreakStmtMap.begin();it!=BreakStmtMap.end();it++)
  {
    if(it->second.size()!=0)
    {
      if(ResultSet.find(it->first)!=ResultSet.end())
      {
        ResultSet.erase(it->first);
      }
    }
  }
  //将剩余全部报错
  for(auto it=ResultSet.begin();it!=ResultSet.end();it++)
  {
    //报错内容为无限循环
    diag((*it)->getBeginLoc(),"该循环为无限循环");
  }
}
} // namespace bz12
} // namespace tidy
} // namespace clang
