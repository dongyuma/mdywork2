//==- CheckSecuritySyntaxOnly.cpp - Basic security checks --------*- C++ -*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//  This file defines a set of flow-insensitive security checks.
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include <string>
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/StmtVisitor.h"
#include "clang/Analysis/AnalysisDeclContext.h"
#include "clang/Basic/TargetInfo.h"
//#include "clang/ARCMigrate/Transforms.h"//
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;
using namespace ento;

static bool isArc4RandomAvailable(const ASTContext &Ctx) {
  const llvm::Triple &T = Ctx.getTargetInfo().getTriple();
  return T.getVendor() == llvm::Triple::Apple ||
         T.getOS() == llvm::Triple::CloudABI ||
         T.isOSFreeBSD() ||
         T.isOSNetBSD() ||
         T.isOSOpenBSD() ||
         T.isOSDragonFly();
}

namespace {
struct ChecksFilter {
  DefaultBool check_bcmp;
  DefaultBool check_bcopy;
  DefaultBool check_bzero;
  DefaultBool check_gets;
  DefaultBool check_getpw;
  DefaultBool check_mktemp;
  DefaultBool check_mkstemp;
  DefaultBool check_strcpy;
  DefaultBool check_DeprecatedOrUnsafeBufferHandling;
  DefaultBool check_rand;
  DefaultBool check_vfork;
  DefaultBool check_FloatLoopCounter;
  DefaultBool check_WhileLoopCounter;  
  DefaultBool check_IfCondition;
  DefaultBool check_SwitchCase;
  DefaultBool check_GotoStmt;
  DefaultBool check_UncheckedReturn;
  DefaultBool check_decodeValueOfObjCType;

  CheckerNameRef checkName_bcmp;
  CheckerNameRef checkName_bcopy;
  CheckerNameRef checkName_bzero;
  CheckerNameRef checkName_gets;
  CheckerNameRef checkName_getpw;
  CheckerNameRef checkName_mktemp;
  CheckerNameRef checkName_mkstemp;
  CheckerNameRef checkName_strcpy;
  CheckerNameRef checkName_DeprecatedOrUnsafeBufferHandling;
  CheckerNameRef checkName_rand;
  CheckerNameRef checkName_vfork;
  CheckerNameRef checkName_FloatLoopCounter;
  CheckerNameRef checkName_WhileLoopCounter;
  CheckerNameRef checkName_IfCondition;
  CheckerNameRef checkName_SwitchCase;
  CheckerNameRef checkName_GotoStmt;
  CheckerNameRef checkName_UncheckedReturn;
  CheckerNameRef checkName_decodeValueOfObjCType;
};

class WalkAST : public StmtVisitor<WalkAST> {
  BugReporter &BR;
  AnalysisDeclContext* AC;
  enum { num_setids = 6 };
  IdentifierInfo *II_setid[num_setids];

  const bool CheckRand;
  const ChecksFilter &filter;

public:
  WalkAST(BugReporter &br, AnalysisDeclContext* ac,
          const ChecksFilter &f)
  : BR(br), AC(ac), II_setid(),
    CheckRand(isArc4RandomAvailable(BR.getContext())),
    filter(f) {}

  // Statement visitor methods.
  void VisitCallExpr(CallExpr *CE);
  void VisitObjCMessageExpr(ObjCMessageExpr *CE);
  void VisitForStmt(ForStmt *S);
  void VisitGotoStmt(GotoStmt *GS);
  void VisitWhileStmt(WhileStmt *S);
  void VisitIfStmt(IfStmt *S);
  void VisitSwitchStmt(SwitchStmt *S);
  // void VisitGotoStmt(GotoStmt *GS);
  void VisitCompoundStmt (CompoundStmt *S);
  void VisitStmt(Stmt *S) { VisitChildren(S); }

  void VisitChildren(Stmt *S);

  // Helpers.
  bool checkCall_strCommon(const CallExpr *CE, const FunctionDecl *FD);

  typedef void (WalkAST::*FnCheck)(const CallExpr *, const FunctionDecl *);
  typedef void (WalkAST::*MsgCheck)(const ObjCMessageExpr *);

  // Checker-specific methods.
  void checkLoopConditionForFloat(const ForStmt *FS);
  void checkWhileLoopCondition(const WhileStmt *WS);
  void checkIfCondition(const IfStmt *IS);
  void checkSwitchCase(const SwitchStmt *SS);
  void checkGotoStmt(const GotoStmt *GS);
  void checkCall_bcmp(const CallExpr *CE, const FunctionDecl *FD);
  void checkCall_bcopy(const CallExpr *CE, const FunctionDecl *FD);
  void checkCall_bzero(const CallExpr *CE, const FunctionDecl *FD);
  void checkCall_gets(const CallExpr *CE, const FunctionDecl *FD);
  void checkCall_getpw(const CallExpr *CE, const FunctionDecl *FD);
  void checkCall_mktemp(const CallExpr *CE, const FunctionDecl *FD);
  void checkCall_mkstemp(const CallExpr *CE, const FunctionDecl *FD);
  void checkCall_strcpy(const CallExpr *CE, const FunctionDecl *FD);
  void checkCall_strcat(const CallExpr *CE, const FunctionDecl *FD);
  void checkDeprecatedOrUnsafeBufferHandling(const CallExpr *CE,
                                             const FunctionDecl *FD);
  void checkCall_rand(const CallExpr *CE, const FunctionDecl *FD);
  void checkCall_random(const CallExpr *CE, const FunctionDecl *FD);
  void checkCall_vfork(const CallExpr *CE, const FunctionDecl *FD);
  void checkMsg_decodeValueOfObjCType(const ObjCMessageExpr *ME);
  void checkUncheckedReturnValue(CallExpr *CE);

  void reportBug(const SwitchStmt *SS, const char *bugType, const char *bugReason);
  void reportBug(const CaseStmt *SC, const char *bugType, const char *bugReason);
  void reportBug(const DefaultStmt *DS, const char *bugType, const char *bugReason);
  void reportBug(const GotoStmt *G, const char *bugType, const char *bugReason);
  void reportBug(const IfStmt *If, const char *bugType, const char *bugReason);

};
} // end anonymous namespace

//===----------------------------------------------------------------------===//
// AST walking.
//===----------------------------------------------------------------------===//

void WalkAST::VisitChildren(Stmt *S) {
  for (Stmt *Child : S->children())
    if (Child)
      Visit(Child);
}

void WalkAST::VisitCallExpr(CallExpr *CE) {
  // Get the callee.
  const FunctionDecl *FD = CE->getDirectCallee();

  if (!FD)
    return;

  // Get the name of the callee. If it's a builtin, strip off the prefix.
  IdentifierInfo *II = FD->getIdentifier();
  if (!II)   // if no identifier, not a simple C function
    return;
  StringRef Name = II->getName();
  if (Name.startswith("__builtin_"))
    Name = Name.substr(10);

  // Set the evaluation function by switching on the callee name.
  FnCheck evalFunction = llvm::StringSwitch<FnCheck>(Name)
    .Case("bcmp", &WalkAST::checkCall_bcmp)
    .Case("bcopy", &WalkAST::checkCall_bcopy)
    .Case("bzero", &WalkAST::checkCall_bzero)
    .Case("gets", &WalkAST::checkCall_gets)
    .Case("getpw", &WalkAST::checkCall_getpw)
    .Case("mktemp", &WalkAST::checkCall_mktemp)
    .Case("mkstemp", &WalkAST::checkCall_mkstemp)
    .Case("mkdtemp", &WalkAST::checkCall_mkstemp)
    .Case("mkstemps", &WalkAST::checkCall_mkstemp)
    .Cases("strcpy", "__strcpy_chk", &WalkAST::checkCall_strcpy)
    .Cases("strcat", "__strcat_chk", &WalkAST::checkCall_strcat)
    .Cases("sprintf", "vsprintf", "scanf", "wscanf", "fscanf", "fwscanf",
           "vscanf", "vwscanf", "vfscanf", "vfwscanf",
           &WalkAST::checkDeprecatedOrUnsafeBufferHandling)
    .Cases("sscanf", "swscanf", "vsscanf", "vswscanf", "swprintf",
           "snprintf", "vswprintf", "vsnprintf", "memcpy", "memmove",
           &WalkAST::checkDeprecatedOrUnsafeBufferHandling)
    .Cases("strncpy", "strncat", "memset",
           &WalkAST::checkDeprecatedOrUnsafeBufferHandling)
    .Case("drand48", &WalkAST::checkCall_rand)
    .Case("erand48", &WalkAST::checkCall_rand)
    .Case("jrand48", &WalkAST::checkCall_rand)
    .Case("lrand48", &WalkAST::checkCall_rand)
    .Case("mrand48", &WalkAST::checkCall_rand)
    .Case("nrand48", &WalkAST::checkCall_rand)
    .Case("lcong48", &WalkAST::checkCall_rand)
    .Case("rand", &WalkAST::checkCall_rand)
    .Case("rand_r", &WalkAST::checkCall_rand)
    .Case("random", &WalkAST::checkCall_random)
    .Case("vfork", &WalkAST::checkCall_vfork)
    .Default(nullptr);

  // If the callee isn't defined, it is not of security concern.
  // Check and evaluate the call.
  if (evalFunction)
    (this->*evalFunction)(CE, FD);

  // Recurse and check children.
  VisitChildren(CE);
}

void WalkAST::VisitObjCMessageExpr(ObjCMessageExpr *ME) {
  MsgCheck evalFunction =
      llvm::StringSwitch<MsgCheck>(ME->getSelector().getAsString())
          .Case("decodeValueOfObjCType:at:",
                &WalkAST::checkMsg_decodeValueOfObjCType)
          .Default(nullptr);

  if (evalFunction)
    (this->*evalFunction)(ME);

  // Recurse and check children.
  VisitChildren(ME);
}

void WalkAST::VisitCompoundStmt(CompoundStmt *S) {
  for (Stmt *Child : S->children())
    if (Child) {
      if (CallExpr *CE = dyn_cast<CallExpr>(Child))
        checkUncheckedReturnValue(CE);
      Visit(Child);
    }
}

void WalkAST::VisitForStmt(ForStmt *FS) {
  checkLoopConditionForFloat(FS);

  // Recurse and check children.
  VisitChildren(FS);
}

void WalkAST::VisitWhileStmt(WhileStmt *WS) {
  checkWhileLoopCondition(WS);

  // Recurse and check children.
  VisitChildren(WS);
}

void WalkAST::VisitIfStmt(IfStmt *IS) {
  checkIfCondition(IS);

  // Recurse and check children.
  VisitChildren(IS);
}

void WalkAST::VisitSwitchStmt(SwitchStmt *SS) {
  checkSwitchCase(SS);

  // Recurse and check children.
  VisitChildren(SS);
}

void WalkAST::VisitGotoStmt(GotoStmt *GS) {
  checkGotoStmt(GS);

  // Recurse and check children.
  VisitChildren(GS);
}

//===----------------------------------------------------------------------===//
// Check: floating point variable used as loop counter.
// Originally: <rdar://problem/6336718>
// Implements: CERT security coding advisory FLP-30.
//===----------------------------------------------------------------------===//

// Returns either 'x' or 'y', depending on which one of them is incremented
// in 'expr', or nullptr if none of them is incremented.
static const DeclRefExpr*
getIncrementedVar(const Expr *expr, const VarDecl *x, const VarDecl *y) {
  expr = expr->IgnoreParenCasts();

  if (const BinaryOperator *B = dyn_cast<BinaryOperator>(expr)) {
    if (!(B->isAssignmentOp() || B->isCompoundAssignmentOp() ||
          B->getOpcode() == BO_Comma))
      return nullptr;

    if (const DeclRefExpr *lhs = getIncrementedVar(B->getLHS(), x, y))
      return lhs;

    if (const DeclRefExpr *rhs = getIncrementedVar(B->getRHS(), x, y))
      return rhs;

    return nullptr;
  }

  if (const DeclRefExpr *DR = dyn_cast<DeclRefExpr>(expr)) {
    const NamedDecl *ND = DR->getDecl();
    return ND == x || ND == y ? DR : nullptr;
  }

  if (const UnaryOperator *U = dyn_cast<UnaryOperator>(expr))
    return U->isIncrementDecrementOp()
      ? getIncrementedVar(U->getSubExpr(), x, y) : nullptr;

  return nullptr;
}

bool TraverseStmtBool(Stmt *rootS) {
  if (!rootS)
    return false;

  Stmt::child_iterator begin, end, temp;

  begin = rootS->child_begin();
  end = rootS->child_end();

  for(; begin!=end; begin++)
  {
    if(const BreakStmt *breakStmt = dyn_cast<BreakStmt>(*begin)){
      return true;
    } else if(const GotoStmt *gotoStmt = dyn_cast<GotoStmt>(*begin)){
      return true;
    } else if(const ReturnStmt *returnStmt = dyn_cast<ReturnStmt>(*begin)){
      return true;
    } else if(clang::IfStmt *D = dyn_cast<clang::IfStmt>(*begin)){
      Stmt *ifRoot = D->getThen();
      bool retIF = TraverseStmtBool(ifRoot);
      if(retIF){
        return true;
      }
      Stmt *elseRoot = D->getElse();
      bool retElse = TraverseStmtBool(elseRoot);
      if(retElse){
        return true;
      }
    } else {}
  }
  return false;
}


bool SearchChildExpr(Stmt *rootS, std::string paraName) {
  if (!rootS)
    return false;

  Stmt::child_iterator begin, end, temp;

  begin = rootS->child_begin();
  end = rootS->child_end();

  for(; begin!=end; begin++)
  {
    if(const BreakStmt *breakStmt = dyn_cast<BreakStmt>(*begin)){
      return true;
    } else if(const GotoStmt *gotoStmt = dyn_cast<GotoStmt>(*begin)){
      return true;
    } else if(const ReturnStmt *returnStmt = dyn_cast<ReturnStmt>(*begin)){
      return true;
    } else if(clang::IfStmt *D = dyn_cast<clang::IfStmt>(*begin)){
      Stmt *ifRoot = D->getThen();
      bool retIF = SearchChildExpr(ifRoot, paraName);
      if(retIF){
        return true;
      }
      Stmt *elseRoot = D->getElse();
      bool retElse = SearchChildExpr(elseRoot, paraName);
      if(retElse){
        return true;
      }
    } else if (const UnaryOperator *D = dyn_cast<UnaryOperator>(*begin)) {
      const Expr *temp = D->getSubExpr();
      if (const DeclRefExpr *DE = dyn_cast<DeclRefExpr>(temp))
      {
        if(DE->getDecl()->getName().str()==paraName)
        {
          return true;
        }
      }
    } else if(const BinaryOperator *D = dyn_cast<BinaryOperator>(*begin)) {
      const Expr *temp = D->getLHS();
      if (const DeclRefExpr *DE = dyn_cast<DeclRefExpr>(temp))
      {
        if(DE->getDecl()->getName().str()==paraName)
        {
          return true;
        }
      }

    }
    else {}
  }
  return false;
}


void WalkAST::reportBug(const SwitchStmt *SS, const char *bugType, const char *bugReason) {
  SmallVector<SourceRange, 1> ranges;
  SmallString<256> sbuf;
  llvm::raw_svector_ostream os(sbuf);
  os << bugReason;
  ranges.push_back(SS->getSourceRange());
  // const char *bugType = "Switch require Default";
  PathDiagnosticLocation FSLoc =
    PathDiagnosticLocation::createBegin(SS, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_SwitchCase,
                    bugType, "Warning", os.str(),
                    FSLoc, ranges);
}


void WalkAST::reportBug(const CaseStmt *CS, const char *bugType, const char *bugReason) {
  SmallVector<SourceRange, 1> ranges;
  SmallString<256> sbuf;
  llvm::raw_svector_ostream os(sbuf);
  os << bugReason;
  ranges.push_back(CS->getSourceRange());
  // const char *bugType = "Case require break";
  PathDiagnosticLocation FSLoc =
    PathDiagnosticLocation::createBegin(CS, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_SwitchCase,
                    bugType, "Warning", os.str(),
                    FSLoc, ranges);
}


void WalkAST::reportBug(const DefaultStmt *DS, const char *bugType, const char *bugReason) {
  SmallVector<SourceRange, 1> ranges;
  SmallString<256> sbuf;
  llvm::raw_svector_ostream os(sbuf);
  os << bugReason;
  ranges.push_back(DS->getSourceRange());
  // const char *bugType = "Default require break";
  PathDiagnosticLocation FSLoc =
    PathDiagnosticLocation::createBegin(DS, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_SwitchCase,
                    bugType, "Warning", os.str(),
                    FSLoc, ranges);
}


void WalkAST::checkSwitchCase(const SwitchStmt *SS) {
  if (!filter.check_SwitchCase)
    return;

  // Does the loop have a condition?
  Expr *condition =  const_cast<clang::Expr*>(SS->getCond());

  if (!condition)
    return;

  condition = condition->IgnoreParenCasts();
  if(condition->getType()->isBooleanType()) {
    reportBug(SS, "Switch条件是布尔类型", "Switch条件是布尔类型，这是不被推荐的");
    return;
  }

  const SwitchCase *CaseList = SS->getSwitchCaseList();

  if(!CaseList){
    reportBug(SS, "Switch结构没有Case语句或者Default语句", "Switch结构要求存在Case语句或者Default语句");
    return;
  }

  unsigned sign_default = 0;
  unsigned count = 0;
  unsigned default_pos = 0;

  while(CaseList)
  {
    if(const CaseStmt *CS = dyn_cast<CaseStmt>(CaseList)){
      count += 1;
      Stmt *caseBody = const_cast<Stmt*>(CS->getSubStmt());
      Stmt::child_iterator begin, end;

      begin = caseBody->child_begin();
      end = caseBody->child_end();
      int sign = 0;
      for(; begin!=end; begin++)
      {
        if (const BreakStmt *BS = dyn_cast_or_null<BreakStmt>(*begin))
        {
          sign = 1;
        }
      }
      if(sign == 0){
        reportBug(CS, "Swicth结构的Case条件没有break语句", "Swicth结构的Case条件必须有break语句");
        break;
      }
    } else if(const DefaultStmt *DS = dyn_cast<DefaultStmt>(CaseList)){
      count += 1;
      
      Stmt *defaultBody = const_cast<Stmt*>(DS->getSubStmt());
      default_pos = count;

      Stmt::child_iterator begin, end;
      begin = defaultBody->child_begin();
      end = defaultBody->child_end();
      int sign = 0;
      for(; begin!=end; begin++)
      {
        if (const BreakStmt *BS = dyn_cast<BreakStmt>(*begin))
        {
          sign = 1;
        }
      }
      if(sign == 0){
        reportBug(DS, "Switch结构的Default条件没有break语句", "Swicth结构的Default条件要求有break语句");
        break;
      }
    } else {
      ;
    }
    CaseList = CaseList->getNextSwitchCase();
  }
  if(default_pos==0)
  {
    reportBug(SS, "Switch结构没有Default标签", "Switch结构没有Default标签");
  }

  if(default_pos!=1 || default_pos!=count)
  {
    reportBug(SS, "Switch结构的Default标签位置是错的", "Switch结构的Default标签必须是第一个标签或者最后一个标签");
  }
  if(count < 2)
  {
    reportBug(SS, "Switch结构分支数量不足", "Switch至少要两个分支语句");
  }

}

void WalkAST::reportBug(const GotoStmt *GS, const char *bugType, const char *bugReason) {
  SmallVector<SourceRange, 1> ranges;
  SmallString<256> sbuf;
  llvm::raw_svector_ostream os(sbuf);
  os << bugReason;
  ranges.push_back(GS->getSourceRange());
  // const char *bugType = "Default require break";
  PathDiagnosticLocation FSLoc =
    PathDiagnosticLocation::createBegin(GS, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_GotoStmt,
                    bugType, "Warning", os.str(),
                    FSLoc, ranges);
}

void WalkAST::checkGotoStmt(const GotoStmt *GS) {
  if (!filter.check_GotoStmt)
    return;

  reportBug(GS, "不建议使用Goto", "Goto应被替换为其他控制流语句");
  
  SourceLocation gotoSL = GS->getGotoLoc();
  SourceLocation labelSL = GS->getLabelLoc();

  // if (!gotoSL || !labelSL)
  //   return;

  const unsigned gotoOffset = gotoSL.getOffsetPub();
  const unsigned labelOffset = labelSL.getOffsetPub();

  if(gotoOffset > labelOffset)
  {
    reportBug(GS, "Goto语句的使用应该先于Goto标签的声明", "Goto语句的使用应该先于Goto标签的声明");
  }
}

void WalkAST::reportBug(const IfStmt *IS, const char *bugType, const char *bugReason) {
  SmallVector<SourceRange, 1> ranges;
  SmallString<256> sbuf;
  llvm::raw_svector_ostream os(sbuf);
  os << bugReason;
  ranges.push_back(IS->getSourceRange());
  // const char *bugType = "Default require break";
  PathDiagnosticLocation FSLoc =
    PathDiagnosticLocation::createBegin(IS, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_IfCondition,
                    bugType, "Warning", os.str(),
                    FSLoc, ranges);
}

void WalkAST::checkIfCondition(const IfStmt *IS) {
  if (!filter.check_IfCondition)
    return;

  // Does the loop have a condition?
  Expr *condition =  const_cast<clang::Expr*>(IS->getCond());

  if (!condition)
    return;

  // Strip away '()' and casts.
  condition = condition->IgnoreParenCasts();

  if(!condition->getType()->isBooleanType()) {
    reportBug(IS, "If语句的条件不是布尔类型", "If语句的条件推荐为布尔类型");
    // return;
  }
  if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(condition)) {
    BinaryOperator::Opcode Op = BO->getOpcode();
    if(Op==BO_Assign) {
        SmallVector<SourceRange, 1> ranges;
        SmallString<256> sbuf;
        llvm::raw_svector_ostream os(sbuf);
        os << "条件语句可能是错误的，是否应该使用‘==’符号?";
        ranges.push_back(condition->getSourceRange());
        const char *bugType = "条件语句可能是错误的";
        PathDiagnosticLocation FSLoc =
          PathDiagnosticLocation::createBegin(IS, BR.getSourceManager(), AC);
        BR.EmitBasicReport(AC->getDecl(), filter.checkName_IfCondition,
                          bugType, "Warning", os.str(),
                          FSLoc, ranges);
    }
    else if(Op==BO_Or || Op==BO_And) {
        SmallVector<SourceRange, 1> ranges;
        SmallString<256> sbuf;
        llvm::raw_svector_ostream os(sbuf);
        if(Op==BO_Or)
          os << "条件语句可能是错误的，是否应该使用‘||’符号?";
        else
          os << "条件语句可能是错误的，是否应该使用‘&&’符号?";
        ranges.push_back(condition->getSourceRange());
        const char *bugType = "条件语句可能是错误的";
        PathDiagnosticLocation FSLoc =
          PathDiagnosticLocation::createBegin(IS, BR.getSourceManager(), AC);
        BR.EmitBasicReport(AC->getDecl(), filter.checkName_IfCondition,
                          bugType, "Warning", os.str(),
                          FSLoc, ranges);      
    }
  }
}


/// checkWhileLoopCondition - This check looks for 'While' statements that
///  use a floating point variable as a loop counter.
///  CERT: FLP30-C, FLP30-CPP.
///
void WalkAST::checkWhileLoopCondition(const WhileStmt *WS) {
  if (!filter.check_WhileLoopCounter)
    return;

  // Does the loop have a condition?
  Expr *condition =  const_cast<clang::Expr*>(WS->getCond());

  if (!condition)
    return;

  // Strip away '()' and casts.
  condition = condition->IgnoreParenCasts();

  Stmt *body =  const_cast<clang::Stmt*>(WS->getBody());
  if(condition->getType()->isBooleanType()) {
    bool Result;
    condition->EvaluateAsBooleanCondition(Result, BR.getContext());
    if(Result)
    {
      bool re = TraverseStmtBool(body);
      if(!re){
        SmallVector<SourceRange, 1> ranges;
        SmallString<256> sbuf;
        llvm::raw_svector_ostream os(sbuf);
        os << "条件语句为true且不能跳出";
        ranges.push_back(condition->getSourceRange());
        const char *bugType = "死循环";
        PathDiagnosticLocation FSLoc =
          PathDiagnosticLocation::createBegin(WS, BR.getSourceManager(), AC);
        BR.EmitBasicReport(AC->getDecl(), filter.checkName_WhileLoopCounter,
                          bugType, "Warning", os.str(),
                          FSLoc, ranges);
      }
    } else {
      if(!condition->isModifiableLvalue(BR.getContext()))
      {
        DeclRefExpr * conditionDecl = dyn_cast<DeclRefExpr>(condition);
        std::string ParaName = conditionDecl->getDecl()->getName().str();
        bool re = SearchChildExpr(body, ParaName);
        if(!re){
          SmallVector<SourceRange, 1> ranges;
          SmallString<256> sbuf;
          llvm::raw_svector_ostream os(sbuf);
          os << "条件语句为true且不能跳出";
          ranges.push_back(condition->getSourceRange());
          const char *bugType = "死循环";
          PathDiagnosticLocation FSLoc =
            PathDiagnosticLocation::createBegin(WS, BR.getSourceManager(), AC);
          BR.EmitBasicReport(AC->getDecl(), filter.checkName_WhileLoopCounter,
                            bugType, "Warning", os.str(),
                            FSLoc, ranges);
        }
      }
    }
  } else if(condition->getType()->isIntegerType()) {
    Expr::EvalResult EVResult;
    condition->EvaluateAsInt(EVResult, BR.getContext(), Expr::SE_NoSideEffects);
    if(!EVResult.Val.isInt()){
      return;
    }
    llvm::APSInt ResultInt = EVResult.Val.getInt();
    if(ResultInt!=0)
    {
      bool re = TraverseStmtBool(body);
      if(!re){
        SmallVector<SourceRange, 1> ranges;
        SmallString<256> sbuf;
        llvm::raw_svector_ostream os(sbuf);
        os << "条件语句为true且不能跳出";
        ranges.push_back(condition->getSourceRange());
        const char *bugType = "死循环";
        PathDiagnosticLocation FSLoc =
          PathDiagnosticLocation::createBegin(WS, BR.getSourceManager(), AC);
        BR.EmitBasicReport(AC->getDecl(), filter.checkName_WhileLoopCounter,
                          bugType, "Warning", os.str(),
                          FSLoc, ranges);
      }
    } else {
      if(!condition->isModifiableLvalue(BR.getContext()))
      {
        DeclRefExpr * conditionDecl = dyn_cast<DeclRefExpr>(condition);
        std::string ParaName = conditionDecl->getDecl()->getName().str();
        bool re = SearchChildExpr(body, ParaName);
        if(!re){
          SmallVector<SourceRange, 1> ranges;
          SmallString<256> sbuf;
          llvm::raw_svector_ostream os(sbuf);
          os << "条件语句为true且不能跳出";
          ranges.push_back(condition->getSourceRange());
          const char *bugType = "死循环";
          PathDiagnosticLocation FSLoc =
            PathDiagnosticLocation::createBegin(WS, BR.getSourceManager(), AC);
          BR.EmitBasicReport(AC->getDecl(), filter.checkName_WhileLoopCounter,
                            bugType, "Warning", os.str(),
                            FSLoc, ranges);
        }
      }
    }
  } else if(condition->getType()->isFloatingType()) {
    // std::cout << "isFloatingType" << std::endl;
    llvm::APFloat ResultFloat(0.0);
    condition->EvaluateAsFloat(ResultFloat, BR.getContext());
    if(!ResultFloat.isZero())
    {
      bool re = TraverseStmtBool(body);
      if(!re){
        SmallVector<SourceRange, 1> ranges;
        SmallString<256> sbuf;
        llvm::raw_svector_ostream os(sbuf);
        os << "条件语句为true且不能跳出";
        ranges.push_back(condition->getSourceRange());
        const char *bugType = "死循环";
        PathDiagnosticLocation FSLoc =
          PathDiagnosticLocation::createBegin(WS, BR.getSourceManager(), AC);
        BR.EmitBasicReport(AC->getDecl(), filter.checkName_WhileLoopCounter,
                          bugType, "Warning", os.str(),
                          FSLoc, ranges);
      }
    } else {
      if(!condition->isModifiableLvalue(BR.getContext()))
      {
        DeclRefExpr * conditionDecl = dyn_cast<DeclRefExpr>(condition);
        std::string ParaName = conditionDecl->getDecl()->getName().str();
        bool re = SearchChildExpr(body, ParaName);
        if(!re){
          SmallVector<SourceRange, 1> ranges;
          SmallString<256> sbuf;
          llvm::raw_svector_ostream os(sbuf);
          os << "条件语句为true且不能跳出";
          ranges.push_back(condition->getSourceRange());
          const char *bugType = "死循环";
          PathDiagnosticLocation FSLoc =
            PathDiagnosticLocation::createBegin(WS, BR.getSourceManager(), AC);
          BR.EmitBasicReport(AC->getDecl(), filter.checkName_WhileLoopCounter,
                            bugType, "Warning", os.str(),
                            FSLoc, ranges);
        }
      }
    }
  } else{}
}
//   else if(const DeclRefExpr *D = dyn_cast<DeclRefExpr>(condition)) {
//     std::cout << "DeclRefExpr" << std::endl;
//   } else if(const UnaryOperator *D = dyn_cast<UnaryOperator>(condition)) {
//     std::cout << "UnaryOperator" << std::endl;
//   } else if(const CallExpr *D = dyn_cast<CallExpr>(condition)) {
//     std::cout << "CallExpr" << std::endl;
//   } else if(const BinaryOperator *D = dyn_cast<BinaryOperator>(condition)) {
//     std::cout << "BinaryOperator" << std::endl;
//   } else if(const StmtExpr *D = dyn_cast<StmtExpr>(condition)) {
//     std::cout << "StmtExpr" << std::endl;
//   } else if(const UnaryExprOrTypeTraitExpr *D = dyn_cast<UnaryExprOrTypeTraitExpr>(condition)) {
//     std::cout << "UnaryExprOrTypeTraitExpr" << std::endl;
//   } else if(const ParenListExpr *D = dyn_cast<ParenListExpr>(condition)) {
//     std::cout << "ParenListExpr" << std::endl;
//   } else if(const BreakStmt *D = dyn_cast<BreakStmt>(condition)) {
//     std::cout << "BreakStmt" << std::endl;
//   } else {
//     std::cout << "Other Expr" << std::endl;
//   }
//   std::cout << "Other Expr11" << std::endl;
// }


/// CheckLoopConditionForFloat - This check looks for 'for' statements that
///  use a floating point variable as a loop counter.
///  CERT: FLP30-C, FLP30-CPP.
///
void WalkAST::checkLoopConditionForFloat(const ForStmt *FS) {
  if (!filter.check_FloatLoopCounter)
    return;

  // Does the loop have a condition?
  const Expr *condition = FS->getCond();

  if (!condition)
    return;

  // Does the loop have an increment?
  const Expr *increment = FS->getInc();

  if (!increment)
    return;

  // Strip away '()' and casts.
  condition = condition->IgnoreParenCasts();
  increment = increment->IgnoreParenCasts();

  // Is the loop condition a comparison?
  const BinaryOperator *B = dyn_cast<BinaryOperator>(condition);

  if (!B)
    return;

  // Is this a comparison?
  if (!(B->isRelationalOp() || B->isEqualityOp()))
    return;

  // Are we comparing variables?
  const DeclRefExpr *drLHS =
    dyn_cast<DeclRefExpr>(B->getLHS()->IgnoreParenLValueCasts());
  const DeclRefExpr *drRHS =
    dyn_cast<DeclRefExpr>(B->getRHS()->IgnoreParenLValueCasts());

  // Does at least one of the variables have a floating point type?
  drLHS = drLHS && drLHS->getType()->isRealFloatingType() ? drLHS : nullptr;
  drRHS = drRHS && drRHS->getType()->isRealFloatingType() ? drRHS : nullptr;

  if (!drLHS && !drRHS)
    return;

  const VarDecl *vdLHS = drLHS ? dyn_cast<VarDecl>(drLHS->getDecl()) : nullptr;
  const VarDecl *vdRHS = drRHS ? dyn_cast<VarDecl>(drRHS->getDecl()) : nullptr;

  if (!vdLHS && !vdRHS)
    return;

  // Does either variable appear in increment?
  const DeclRefExpr *drInc = getIncrementedVar(increment, vdLHS, vdRHS);
  if (!drInc)
    return;

  const VarDecl *vdInc = cast<VarDecl>(drInc->getDecl());
  assert(vdInc && (vdInc == vdLHS || vdInc == vdRHS));

  // Emit the error.  First figure out which DeclRefExpr in the condition
  // referenced the compared variable.
  const DeclRefExpr *drCond = vdLHS == vdInc ? drLHS : drRHS;

  SmallVector<SourceRange, 2> ranges;
  SmallString<256> sbuf;
  llvm::raw_svector_ostream os(sbuf);

  os << "变量'" << drCond->getDecl()->getName()
     << "'的类型为'" << drCond->getType().getAsString()
     << "'不应该用作循环计数器";

  ranges.push_back(drCond->getSourceRange());
  ranges.push_back(drInc->getSourceRange());

  const char *bugType = "浮点类型变量被用作循环计数器";

  PathDiagnosticLocation FSLoc =
    PathDiagnosticLocation::createBegin(FS, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_FloatLoopCounter,
                     bugType, "Security", os.str(),
                     FSLoc, ranges);
}

//===----------------------------------------------------------------------===//
// Check: Any use of bcmp.
// CWE-477: Use of Obsolete Functions
// bcmp was deprecated in POSIX.1-2008
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_bcmp(const CallExpr *CE, const FunctionDecl *FD) {
  if (!filter.check_bcmp)
    return;

  const FunctionProtoType *FPT = FD->getType()->getAs<FunctionProtoType>();
  if (!FPT)
    return;

  // Verify that the function takes three arguments.
  if (FPT->getNumParams() != 3)
    return;

  for (int i = 0; i < 2; i++) {
    // Verify the first and second argument type is void*.
    const PointerType *PT = FPT->getParamType(i)->getAs<PointerType>();
    if (!PT)
      return;

    if (PT->getPointeeType().getUnqualifiedType() != BR.getContext().VoidTy)
      return;
  }

  // Verify the third argument type is integer.
  if (!FPT->getParamType(2)->isIntegralOrUnscopedEnumerationType())
    return;

  // Issue a warning.
  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_bcmp,
                     "bcmp()函数已被弃用",
                     "Security",
                     "bcmp()函数已经过时，应该使用memcmp()函数",
                     CELoc, CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: Any use of bcopy.
// CWE-477: Use of Obsolete Functions
// bcopy was deprecated in POSIX.1-2008
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_bcopy(const CallExpr *CE, const FunctionDecl *FD) {
  if (!filter.check_bcopy)
    return;

  const FunctionProtoType *FPT = FD->getType()->getAs<FunctionProtoType>();
  if (!FPT)
    return;

  // Verify that the function takes three arguments.
  if (FPT->getNumParams() != 3)
    return;

  for (int i = 0; i < 2; i++) {
    // Verify the first and second argument type is void*.
    const PointerType *PT = FPT->getParamType(i)->getAs<PointerType>();
    if (!PT)
      return;

    if (PT->getPointeeType().getUnqualifiedType() != BR.getContext().VoidTy)
      return;
  }

  // Verify the third argument type is integer.
  if (!FPT->getParamType(2)->isIntegralOrUnscopedEnumerationType())
    return;

  // Issue a warning.
  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_bcopy,
                     "bcopy()函数已被弃用",
                     "Security",
                     "bcopy()函数已经过时，应该使用memcpy()函数或者memmove()函数",
                     CELoc, CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: Any use of bzero.
// CWE-477: Use of Obsolete Functions
// bzero was deprecated in POSIX.1-2008
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_bzero(const CallExpr *CE, const FunctionDecl *FD) {
  if (!filter.check_bzero)
    return;

  const FunctionProtoType *FPT = FD->getType()->getAs<FunctionProtoType>();
  if (!FPT)
    return;

  // Verify that the function takes two arguments.
  if (FPT->getNumParams() != 2)
    return;

  // Verify the first argument type is void*.
  const PointerType *PT = FPT->getParamType(0)->getAs<PointerType>();
  if (!PT)
    return;

  if (PT->getPointeeType().getUnqualifiedType() != BR.getContext().VoidTy)
    return;

  // Verify the second argument type is integer.
  if (!FPT->getParamType(1)->isIntegralOrUnscopedEnumerationType())
    return;

  // Issue a warning.
  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_bzero,
                     "bzero()函数已被弃用",
                     "Security",
                     "bzero()函数已经过时，应该使用memset()函数",
                     CELoc, CE->getCallee()->getSourceRange());
}


//===----------------------------------------------------------------------===//
// Check: Any use of 'gets' is insecure.
// Originally: <rdar://problem/6335715>
// Implements (part of): 300-BSI (buildsecurityin.us-cert.gov)
// CWE-242: Use of Inherently Dangerous Function
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_gets(const CallExpr *CE, const FunctionDecl *FD) {
  if (!filter.check_gets)
    return;

  const FunctionProtoType *FPT = FD->getType()->getAs<FunctionProtoType>();
  if (!FPT)
    return;

  // Verify that the function takes a single argument.
  if (FPT->getNumParams() != 1)
    return;

  // Is the argument a 'char*'?
  const PointerType *PT = FPT->getParamType(0)->getAs<PointerType>();
  if (!PT)
    return;

  if (PT->getPointeeType().getUnqualifiedType() != BR.getContext().CharTy)
    return;

  // Issue a warning.
  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_gets,
                     "调用gets函数可能导致缓存区溢出",
                     "Security",
                     "调用gets函数很不安全，因为它可能导致缓存区溢出",
                     CELoc, CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: Any use of 'getpwd' is insecure.
// CWE-477: Use of Obsolete Functions
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_getpw(const CallExpr *CE, const FunctionDecl *FD) {
  if (!filter.check_getpw)
    return;

  const FunctionProtoType *FPT = FD->getType()->getAs<FunctionProtoType>();
  if (!FPT)
    return;

  // Verify that the function takes two arguments.
  if (FPT->getNumParams() != 2)
    return;

  // Verify the first argument type is integer.
  if (!FPT->getParamType(0)->isIntegralOrUnscopedEnumerationType())
    return;

  // Verify the second argument type is char*.
  const PointerType *PT = FPT->getParamType(1)->getAs<PointerType>();
  if (!PT)
    return;

  if (PT->getPointeeType().getUnqualifiedType() != BR.getContext().CharTy)
    return;

  // Issue a warning.
  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_getpw,
                     "调用getpw函数可能导致缓存区溢出",
                     "Security",
                     "调用getpw函数可能导致提供的缓存区溢出，可以使用getpwuid()函数替换",
                     CELoc, CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: Any use of 'mktemp' is insecure.  It is obsoleted by mkstemp().
// CWE-377: Insecure Temporary File
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_mktemp(const CallExpr *CE, const FunctionDecl *FD) {
  if (!filter.check_mktemp) {
    // Fall back to the security check of looking for enough 'X's in the
    // format string, since that is a less severe warning.
    checkCall_mkstemp(CE, FD);
    return;
  }

  const FunctionProtoType *FPT = FD->getType()->getAs<FunctionProtoType>();
  if(!FPT)
    return;

  // Verify that the function takes a single argument.
  if (FPT->getNumParams() != 1)
    return;

  // Verify that the argument is Pointer Type.
  const PointerType *PT = FPT->getParamType(0)->getAs<PointerType>();
  if (!PT)
    return;

  // Verify that the argument is a 'char*'.
  if (PT->getPointeeType().getUnqualifiedType() != BR.getContext().CharTy)
    return;

  // Issue a warning.
  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_mktemp,
                     "调用mktemp()函数可能导致不安全的临时文件",
                     "Security",
                     "调用mktemp()函数是不安全的因为它可能新建或者使用不安全的临时文件"
                     "可以使用mkstemp()函数替换",
                     CELoc, CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: Use of 'mkstemp', 'mktemp', 'mkdtemp' should contain at least 6 X's.
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_mkstemp(const CallExpr *CE, const FunctionDecl *FD) {
  if (!filter.check_mkstemp)
    return;

  StringRef Name = FD->getIdentifier()->getName();
  std::pair<signed, signed> ArgSuffix =
    llvm::StringSwitch<std::pair<signed, signed> >(Name)
      .Case("mktemp", std::make_pair(0,-1))
      .Case("mkstemp", std::make_pair(0,-1))
      .Case("mkdtemp", std::make_pair(0,-1))
      .Case("mkstemps", std::make_pair(0,1))
      .Default(std::make_pair(-1, -1));

  assert(ArgSuffix.first >= 0 && "Unsupported function");

  // Check if the number of arguments is consistent with out expectations.
  unsigned numArgs = CE->getNumArgs();
  if ((signed) numArgs <= ArgSuffix.first)
    return;

  const StringLiteral *strArg =
    dyn_cast<StringLiteral>(CE->getArg((unsigned)ArgSuffix.first)
                              ->IgnoreParenImpCasts());

  // Currently we only handle string literals.  It is possible to do better,
  // either by looking at references to const variables, or by doing real
  // flow analysis.
  if (!strArg || strArg->getCharByteWidth() != 1)
    return;

  // Count the number of X's, taking into account a possible cutoff suffix.
  StringRef str = strArg->getString();
  unsigned numX = 0;
  unsigned n = str.size();

  // Take into account the suffix.
  unsigned suffix = 0;
  if (ArgSuffix.second >= 0) {
    const Expr *suffixEx = CE->getArg((unsigned)ArgSuffix.second);
    Expr::EvalResult EVResult;
    if (!suffixEx->EvaluateAsInt(EVResult, BR.getContext(), Expr::SE_NoSideEffects))
      return;
    llvm::APSInt Result = EVResult.Val.getInt();
    // FIXME: Issue a warning.
    if (Result.isNegative())
      return;
    suffix = (unsigned) Result.getZExtValue();
    n = (n > suffix) ? n - suffix : 0;
  }

  for (unsigned i = 0; i < n; ++i)
    if (str[i] == 'X') ++numX;

  if (numX >= 6)
    return;

  // Issue a warning.
  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  SmallString<512> buf;
  llvm::raw_svector_ostream out(buf);
  out << "Call to '" << Name << "' should have at least 6 'X's in the"
    " format string to be secure (" << numX << " 'X'";
  if (numX != 1)
    out << 's';
  out << " seen";
  if (suffix) {
    out << ", " << suffix << " character";
    if (suffix > 1)
      out << 's';
    out << " used as a suffix";
  }
  out << ')';
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_mkstemp,
                     "不安全的临时文件创建", "Security",
                     out.str(), CELoc, strArg->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: Any use of 'strcpy' is insecure.
//
// CWE-119: Improper Restriction of Operations within
// the Bounds of a Memory Buffer
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_strcpy(const CallExpr *CE, const FunctionDecl *FD) {
  if (!filter.check_strcpy)
    return;

  if (!checkCall_strCommon(CE, FD))
    return;

  const auto *Target = CE->getArg(0)->IgnoreImpCasts(),
             *Source = CE->getArg(1)->IgnoreImpCasts();

  if (const auto *Array = dyn_cast<ConstantArrayType>(Target->getType())) {
    uint64_t ArraySize = BR.getContext().getTypeSize(Array) / 8;
    if (const auto *String = dyn_cast<StringLiteral>(Source)) {
      if (ArraySize >= String->getLength() + 1)
        return;
    }
  }

  // Issue a warning.
  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_strcpy,
                     "潜在的不安全内存缓冲区限制"
                     "调用'strcpy'",
                     "Security",
                     "调用函数'strcpy'是不安全的，因为它不 "
                     "提供内存缓冲区的边界。"
                     "用支持长度参数的类似函数替换无界复制函数 "
                     "例如'strlcpy'. CWE-119.", 
                     CELoc, CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: Any use of 'strcat' is insecure.
//
// CWE-119: Improper Restriction of Operations within
// the Bounds of a Memory Buffer
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_strcat(const CallExpr *CE, const FunctionDecl *FD) {
  if (!filter.check_strcpy)
    return;

  if (!checkCall_strCommon(CE, FD))
    return;

  // Issue a warning.
  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_strcpy,
                     "潜在的不安全的内存缓冲区限制 "
                     "调用'strcat'",
                     "Security",
                     "调用函数'strcat' 是不安全的，因为它不"
                     "提供内存缓冲区的边界。"
                     "用支持长度参数的类似函数替换无界复制函数"
                     "例如'strcat' . CWE-119.",
                     CELoc, CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: Any use of 'sprintf', 'vsprintf', 'scanf', 'wscanf', 'fscanf',
//        'fwscanf', 'vscanf', 'vwscanf', 'vfscanf', 'vfwscanf', 'sscanf',
//        'swscanf', 'vsscanf', 'vswscanf', 'swprintf', 'snprintf', 'vswprintf',
//        'vsnprintf', 'memcpy', 'memmove', 'strncpy', 'strncat', 'memset'
//        is deprecated since C11.
//
//        Use of 'sprintf', 'vsprintf', 'scanf', 'wscanf','fscanf',
//        'fwscanf', 'vscanf', 'vwscanf', 'vfscanf', 'vfwscanf', 'sscanf',
//        'swscanf', 'vsscanf', 'vswscanf' without buffer limitations
//        is insecure.
//
// CWE-119: Improper Restriction of Operations within
// the Bounds of a Memory Buffer
//===----------------------------------------------------------------------===//

void WalkAST::checkDeprecatedOrUnsafeBufferHandling(const CallExpr *CE,
                                                    const FunctionDecl *FD) {
  if (!filter.check_DeprecatedOrUnsafeBufferHandling)
    return;

  if (!BR.getContext().getLangOpts().C11)
    return;

  // Issue a warning. ArgIndex == -1: Deprecated but not unsafe (has size
  // restrictions).
  enum { DEPR_ONLY = -1, UNKNOWN_CALL = -2 };

  StringRef Name = FD->getIdentifier()->getName();
  if (Name.startswith("__builtin_"))
    Name = Name.substr(10);

  int ArgIndex =
      llvm::StringSwitch<int>(Name)
          .Cases("scanf", "wscanf", "vscanf", "vwscanf", 0)
          .Cases("sprintf", "vsprintf", "fscanf", "fwscanf", "vfscanf",
                 "vfwscanf", "sscanf", "swscanf", "vsscanf", "vswscanf", 1)
          .Cases("swprintf", "snprintf", "vswprintf", "vsnprintf", "memcpy",
                 "memmove", "memset", "strncpy", "strncat", DEPR_ONLY)
          .Default(UNKNOWN_CALL);

  assert(ArgIndex != UNKNOWN_CALL && "Unsupported function");
  bool BoundsProvided = ArgIndex == DEPR_ONLY;

  if (!BoundsProvided) {
    // Currently we only handle (not wide) string literals. It is possible to do
    // better, either by looking at references to const variables, or by doing
    // real flow analysis.
    auto FormatString =
        dyn_cast<StringLiteral>(CE->getArg(ArgIndex)->IgnoreParenImpCasts());
    if (FormatString &&
        FormatString->getString().find("%s") == StringRef::npos &&
        FormatString->getString().find("%[") == StringRef::npos)
      BoundsProvided = true;
  }

  SmallString<128> Buf1;
  SmallString<512> Buf2;
  llvm::raw_svector_ostream Out1(Buf1);
  llvm::raw_svector_ostream Out2(Buf2);

  Out1 << "Potential insecure memory buffer bounds restriction in call '"
       << Name << "'";
  Out2 << "Call to function '" << Name
       << "' is insecure as it does not provide ";

  if (!BoundsProvided) {
    Out2 << "bounding of the memory buffer or ";
  }

  Out2 << "security checks introduced "
          "in the C11 standard. Replace with analogous functions that "
          "support length arguments or provides boundary checks such as '"
       << Name << "_s' in case of C11";

  PathDiagnosticLocation CELoc =
      PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(),
                     filter.checkName_DeprecatedOrUnsafeBufferHandling,
                     Out1.str(), "Security", Out2.str(), CELoc,
                     CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Common check for str* functions with no bounds parameters.
//===----------------------------------------------------------------------===//

bool WalkAST::checkCall_strCommon(const CallExpr *CE, const FunctionDecl *FD) {
  const FunctionProtoType *FPT = FD->getType()->getAs<FunctionProtoType>();
  if (!FPT)
    return false;

  // Verify the function takes two arguments, three in the _chk version.
  int numArgs = FPT->getNumParams();
  if (numArgs != 2 && numArgs != 3)
    return false;

  // Verify the type for both arguments.
  for (int i = 0; i < 2; i++) {
    // Verify that the arguments are pointers.
    const PointerType *PT = FPT->getParamType(i)->getAs<PointerType>();
    if (!PT)
      return false;

    // Verify that the argument is a 'char*'.
    if (PT->getPointeeType().getUnqualifiedType() != BR.getContext().CharTy)
      return false;
  }

  return true;
}

//===----------------------------------------------------------------------===//
// Check: Linear congruent random number generators should not be used
// Originally: <rdar://problem/63371000>
// CWE-338: Use of cryptographically weak prng
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_rand(const CallExpr *CE, const FunctionDecl *FD) {
  if (!filter.check_rand || !CheckRand)
    return;

  const FunctionProtoType *FTP = FD->getType()->getAs<FunctionProtoType>();
  if (!FTP)
    return;

  if (FTP->getNumParams() == 1) {
    // Is the argument an 'unsigned short *'?
    // (Actually any integer type is allowed.)
    const PointerType *PT = FTP->getParamType(0)->getAs<PointerType>();
    if (!PT)
      return;

    if (! PT->getPointeeType()->isIntegralOrUnscopedEnumerationType())
      return;
  } else if (FTP->getNumParams() != 0)
    return;

  // Issue a warning.
  SmallString<256> buf1;
  llvm::raw_svector_ostream os1(buf1);
  os1 << '\'' << *FD << "' is a poor random number generator";

  SmallString<256> buf2;
  llvm::raw_svector_ostream os2(buf2);
  os2 << "Function '" << *FD
      << "' is obsolete because it implements a poor random number generator."
      << "  Use 'arc4random' instead";

  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_rand, os1.str(),
                     "Security", os2.str(), CELoc,
                     CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: 'random' should not be used
// Originally: <rdar://problem/63371000>
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_random(const CallExpr *CE, const FunctionDecl *FD) {
  if (!CheckRand || !filter.check_rand)
    return;

  const FunctionProtoType *FTP = FD->getType()->getAs<FunctionProtoType>();
  if (!FTP)
    return;

  // Verify that the function takes no argument.
  if (FTP->getNumParams() != 0)
    return;

  // Issue a warning.
  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_rand,
                     "'random'不是安全随机数生成器",
                     "Security",
                     "'random'函数产生一系列的值"
                     "对手可能能够预测。用'arc4random'"
                     "代替", CELoc, CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: 'vfork' should not be used.
// POS33-C: Do not use vfork().
//===----------------------------------------------------------------------===//

void WalkAST::checkCall_vfork(const CallExpr *CE, const FunctionDecl *FD) {
  if (!filter.check_vfork)
    return;

  // All calls to vfork() are insecure, issue a warning.
  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_vfork,
                     "潜在的不安全的特定于实现的行为"
                     "调用'vfork'",
                     "Security",
                     "对函数'vfork'的调用不安全，因为它可能导致 "
                     "父进程中出现拒绝服务情况。"
                     "将对vfork的调用替换为对更安全的 "
                     "'posix_spawn'函数的调用",
                     CELoc, CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: '-decodeValueOfObjCType:at:' should not be used.
// It is deprecated in favor of '-decodeValueOfObjCType:at:size:' due to
// likelihood of buffer overflows.
//===----------------------------------------------------------------------===//

void WalkAST::checkMsg_decodeValueOfObjCType(const ObjCMessageExpr *ME) {
  if (!filter.check_decodeValueOfObjCType)
    return;

  // Check availability of the secure alternative:
  // iOS 11+, macOS 10.13+, tvOS 11+, and watchOS 4.0+
  // FIXME: We probably shouldn't register the check if it's not available.
  const TargetInfo &TI = AC->getASTContext().getTargetInfo();
  const llvm::Triple &T = TI.getTriple();
  const VersionTuple &VT = TI.getPlatformMinVersion();
  switch (T.getOS()) {
  case llvm::Triple::IOS:
    if (VT < VersionTuple(11, 0))
      return;
    break;
  case llvm::Triple::MacOSX:
    if (VT < VersionTuple(10, 13))
      return;
    break;
  case llvm::Triple::WatchOS:
    if (VT < VersionTuple(4, 0))
      return;
    break;
  case llvm::Triple::TvOS:
    if (VT < VersionTuple(11, 0))
      return;
    break;
  default:
    return;
  }

  PathDiagnosticLocation MELoc =
      PathDiagnosticLocation::createBegin(ME, BR.getSourceManager(), AC);
  BR.EmitBasicReport(
      AC->getDecl(), filter.checkName_decodeValueOfObjCType,
      "'-decodeValueOfObjCType:at:'中的潜在缓冲区溢出", "Security",
      "不建议使用的方法'-decodeValueOfObjCType:at:'是不安全的 "
      "因为它可能导致潜在的缓冲区溢出。使用更安全的 "
      "'-decodeValueOfObjCType:at:size:' 方法。",
      MELoc, ME->getSourceRange());
}

//===----------------------------------------------------------------------===//
// Check: Should check whether privileges are dropped successfully.
// Originally: <rdar://problem/6337132>
//===----------------------------------------------------------------------===//

void WalkAST::checkUncheckedReturnValue(CallExpr *CE) {
  if (!filter.check_UncheckedReturn)
    return;

  const FunctionDecl *FD = CE->getDirectCallee();
  if (!FD)
    return;

  if (II_setid[0] == nullptr) {
    static const char * const identifiers[num_setids] = {
      "setuid", "setgid", "seteuid", "setegid",
      "setreuid", "setregid"
    };

    for (size_t i = 0; i < num_setids; i++)
      II_setid[i] = &BR.getContext().Idents.get(identifiers[i]);
  }

  const IdentifierInfo *id = FD->getIdentifier();
  size_t identifierid;

  for (identifierid = 0; identifierid < num_setids; identifierid++)
    if (id == II_setid[identifierid])
      break;

  if (identifierid >= num_setids)
    return;

  const FunctionProtoType *FTP = FD->getType()->getAs<FunctionProtoType>();
  if (!FTP)
    return;

  // Verify that the function takes one or two arguments (depending on
  //   the function).
  if (FTP->getNumParams() != (identifierid < 4 ? 1 : 2))
    return;

  // The arguments must be integers.
  for (unsigned i = 0; i < FTP->getNumParams(); i++)
    if (!FTP->getParamType(i)->isIntegralOrUnscopedEnumerationType())
      return;

  // Issue a warning.
  SmallString<256> buf1;
  llvm::raw_svector_ostream os1(buf1);
  os1 << "Return value is not checked in call to '" << *FD << '\'';

  SmallString<256> buf2;
  llvm::raw_svector_ostream os2(buf2);
  os2 << "The return value from the call to '" << *FD
      << "' is not checked.  If an error occurs in '" << *FD
      << "', the following code may execute with unexpected privileges";

  PathDiagnosticLocation CELoc =
    PathDiagnosticLocation::createBegin(CE, BR.getSourceManager(), AC);
  BR.EmitBasicReport(AC->getDecl(), filter.checkName_UncheckedReturn, os1.str(),
                     "Security", os2.str(), CELoc,
                     CE->getCallee()->getSourceRange());
}

//===----------------------------------------------------------------------===//
// SecuritySyntaxChecker
//===----------------------------------------------------------------------===//

namespace {
class SecuritySyntaxChecker : public Checker<check::ASTCodeBody> {
public:
  ChecksFilter filter;

  void checkASTCodeBody(const Decl *D, AnalysisManager& mgr,
                        BugReporter &BR) const {
    WalkAST walker(BR, mgr.getAnalysisDeclContext(D), filter);
    walker.Visit(D->getBody());
  }
};
}

void ento::registerSecuritySyntaxChecker(CheckerManager &mgr) {
  mgr.registerChecker<SecuritySyntaxChecker>();
}

bool ento::shouldRegisterSecuritySyntaxChecker(const CheckerManager &mgr) {
  return true;
}

#define REGISTER_CHECKER(name)                                                 \
  void ento::register##name(CheckerManager &mgr) {                             \
    SecuritySyntaxChecker *checker = mgr.getChecker<SecuritySyntaxChecker>();  \
    checker->filter.check_##name = true;                                       \
    checker->filter.checkName_##name = mgr.getCurrentCheckerName();            \
  }                                                                            \
                                                                               \
  bool ento::shouldRegister##name(const CheckerManager &mgr) { return true; }

REGISTER_CHECKER(bcmp)
REGISTER_CHECKER(bcopy)
REGISTER_CHECKER(bzero)
REGISTER_CHECKER(gets)
REGISTER_CHECKER(getpw)
REGISTER_CHECKER(mkstemp)
REGISTER_CHECKER(mktemp)
REGISTER_CHECKER(strcpy)
REGISTER_CHECKER(rand)
REGISTER_CHECKER(vfork)
REGISTER_CHECKER(FloatLoopCounter)
REGISTER_CHECKER(WhileLoopCounter)
REGISTER_CHECKER(IfCondition)
REGISTER_CHECKER(SwitchCase)
REGISTER_CHECKER(GotoStmt)
REGISTER_CHECKER(UncheckedReturn)
REGISTER_CHECKER(DeprecatedOrUnsafeBufferHandling)
REGISTER_CHECKER(decodeValueOfObjCType)
