/*!
 * FileName: ScopeMemoryAllocationChecker.cpp
 *
 * Author:   yuxinglin
 * Date:     2021-10-x
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: CWE-789: Memory Allocation with Excessive Size Value
 *          检测不正确的malloc内存分配，防止栈溢出
 */

#include "common/CheckScope.h"
#include "common/ScopeSValCmp.h"
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/EvaluatedExprVisitor.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "../Taint.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/SValBuilder.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/APInt.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Frontend/CheckerRegistry.h"

#include <utility>

using namespace clang;
using namespace ento;
using namespace taint;
using llvm::APSInt;

namespace {
struct MallocOverflowCheck {
  const BinaryOperator *mulop;
  const Expr *variable;
  APSInt maxVal;

  MallocOverflowCheck(const BinaryOperator *m, const Expr *v, APSInt val)
      : mulop(m), variable(v), maxVal(std::move(val)) {}
};


class ScopeMemoryAllocationChecker : public Checker<check::ASTCodeBody, check::PreStmt<CallExpr>>{
public:                                 
  void checkASTCodeBody(const Decl *D, AnalysisManager &mgr,
                        BugReporter &BR) const;
  
  void checkPreStmt(const CallExpr *CE, CheckerContext &C) const;

  void CheckMallocArgument(
    SmallVectorImpl<MallocOverflowCheck> &PossibleMallocOverflows,
    const Expr *TheArgument, ASTContext &Context) const;

  void OutputPossibleOverflows(
    SmallVectorImpl<MallocOverflowCheck> &PossibleMallocOverflows,
    const Decl *D, BugReporter &BR, AnalysisManager &mgr) const;
private:
  	void checkunderflow(SVal e_SVal, QualType eType, const Expr *e, CheckerContext &C) const;
		void checkTaintflow(SVal e_SVal, QualType eType, const Expr *e, CheckerContext &C) const;
};
} // end anonymous namespace
void ScopeMemoryAllocationChecker::checkunderflow(SVal e_SVal, QualType eType, const Expr *e, CheckerContext &C) const{
  	ProgramStateRef State = C.getState();
    if (!State)
    {
      return;
    }

    SValBuilder &SVB = C.getSValBuilder();
    BasicValueFactory &BVF = SVB.getBasicValueFactory();
    QualType boolTy = SVB.getConditionType();

    SVal zeroSVal = SVB.makeZeroVal(eType);
    SVal maxIntSVal = SVB.makeIntVal(BVF.getMaxValue(eType));
    
    //underflow cond : e_SVal是无符号整数，传入负数会下溢，因此cond := e_SVal>maxIntSVal  成立 返回1； 不成立返回0
    SVal sval_gt = SVB.evalBinOp(State, BinaryOperatorKind::BO_GT, e_SVal, maxIntSVal, boolTy);

    Optional<DefinedSVal> cond_gt = sval_gt.getAs<DefinedSVal>();
    if (!cond_gt)
    {
      return;
    }

    auto cond_res = State->assume(*cond_gt);
    if (cond_res.first && !cond_res.second)
    {
        PathDiagnosticLocation ELoc(e->getBeginLoc(), C.getSourceManager());
        C.getBugReporter().EmitBasicReport(C.getCurrentAnalysisDeclContext()->getDecl(), this,
         "溢出标志", categories::SecurityError, "malloc的参数小于0，溢出", ELoc, e->getSourceRange());
        return;
    }

    SVal sval_eq = SVB.evalBinOp(State, BinaryOperatorKind::BO_EQ, e_SVal, zeroSVal, boolTy);

    Optional<DefinedSVal> cond_eq = sval_eq.getAs<DefinedSVal>();
    if (!cond_eq)
    {
      return;
    }

    cond_res = State->assume(*cond_eq);
    if (cond_res.first && !cond_res.second)
    {
        PathDiagnosticLocation ELoc(e->getBeginLoc(), C.getSourceManager());
        C.getBugReporter().EmitBasicReport(C.getCurrentAnalysisDeclContext()->getDecl(), this,
         "溢出标志", categories::SecurityError, "malloc的参数为0，可能是未定义行为", ELoc, e->getSourceRange());
        return;
    }
}

void ScopeMemoryAllocationChecker::checkTaintflow(SVal e_SVal, QualType eType, const Expr *e, CheckerContext &C) const{
  bool TaintedD = isTainted(C.getState(), e_SVal);
  // std::cout << TaintedD <<std::endl;
  if (TaintedD) {
      PathDiagnosticLocation ELoc(e->getBeginLoc(), C.getSourceManager());
      C.getBugReporter().EmitBasicReport(C.getCurrentAnalysisDeclContext()->getDecl(), this,
        "溢出标志", categories::SecurityError, "malloc的参数为污染值，可能为0", ELoc, e->getSourceRange());
      return;
  }
}

void ScopeMemoryAllocationChecker::checkPreStmt(const CallExpr *CE,CheckerContext &C) const{
  //const Expr *Callee = CE->getCallee();
  const FunctionDecl *FD =  CE->getDirectCallee(); 
  if (!FD)
    return;

  // Get the name of the callee.
  IdentifierInfo *II = FD->getIdentifier();
  if (!II) // if no identifier, not a simple C function
    return;

  if (II->isStr("malloc")) {
    if(CE->getNumArgs() == 1){
      const Expr *e = CE->getArg(0);  //得到第一个参数    malloc函数的原型 void* malloc (size_t size);
      e = e->IgnoreParenImpCasts();
      QualType eType = e->getType();
      SVal sval = C.getSVal(CE->getArg(0));
      Optional<DefinedSVal> tt = sval.getAs<DefinedSVal>();

      checkunderflow(*tt, eType, e, C);
      checkTaintflow(*tt, eType, e, C);

      // std::cout << "啊嘞嘞" << std::endl;
    }
  }
}

// Return true for computations which evaluate to zero: e.g., mult by 0.
static inline bool EvaluatesToZero(APSInt &Val, BinaryOperatorKind op) {
  return (op == BO_Mul) && (Val == 0);
}

void ScopeMemoryAllocationChecker::CheckMallocArgument(
  SmallVectorImpl<MallocOverflowCheck> &PossibleMallocOverflows,
  const Expr *TheArgument,
  ASTContext &Context) const {

  /* Look for a linear combination with a single variable, and at least
   one multiplication.
   Reject anything that applies to the variable: an explicit cast,
   conditional expression, an operation that could reduce the range
   of the result, or anything too complicated :-).  */
  const Expr *e = TheArgument;
  const BinaryOperator * mulop = nullptr;
  APSInt maxVal;
  
  for (;;) {
    maxVal = 0;
    // e->dumpColor();
    // std::cout << std::endl;
    e = e->IgnoreParenImpCasts();
    // e->dump();
    // std::cout << std::endl;
    if (const BinaryOperator *binop = dyn_cast<BinaryOperator>(e)) {
      BinaryOperatorKind opc = binop->getOpcode();
      // TODO: ignore multiplications by 1, reject if multiplied by 0.
      if (mulop == nullptr && opc == BO_Mul)
        mulop = binop;
      if (opc != BO_Mul && opc != BO_Add && opc != BO_Sub && opc != BO_Shl)
        return;

      const Expr *lhs = binop->getLHS();
      const Expr *rhs = binop->getRHS();
      if (rhs->isEvaluatable(Context)) {
        e = lhs;
        maxVal = rhs->EvaluateKnownConstInt(Context);
        if (EvaluatesToZero(maxVal, opc))
          return;
      } else if ((opc == BO_Add || opc == BO_Mul) &&
                 lhs->isEvaluatable(Context)) {
        maxVal = lhs->EvaluateKnownConstInt(Context);
        if (EvaluatesToZero(maxVal, opc))
          return;
        e = rhs;
      } else
        return;
    }
    else if (isa<DeclRefExpr>(e) || isa<MemberExpr>(e))
      break;
    else
      return;
  }
  
  if (mulop == nullptr)
    return;

  //  We've found the right structure of malloc argument, now save
  // the data so when the body of the function is completely available
  // we can check for comparisons.

  // TODO: Could push this into the innermost scope where 'e' is
  // defined, rather than the whole function.
  PossibleMallocOverflows.push_back(MallocOverflowCheck(mulop, e, maxVal));
}

namespace {
// A worker class for OutputPossibleOverflows.
class CheckOverflowOps :
  public EvaluatedExprVisitor<CheckOverflowOps> {
public:
  typedef SmallVectorImpl<MallocOverflowCheck> theVecType;

private:
    theVecType &toScanFor;
    ASTContext &Context;

    bool isIntZeroExpr(const Expr *E) const {
      if (!E->getType()->isIntegralOrEnumerationType())
        return false;
      Expr::EvalResult Result;
      if (E->EvaluateAsInt(Result, Context))
        return Result.Val.getInt() == 0;
      return false;
    }

    static const Decl *getDecl(const DeclRefExpr *DR) { return DR->getDecl(); }
    static const Decl *getDecl(const MemberExpr *ME) {
      return ME->getMemberDecl();
    }

    template <typename T1>
    void Erase(const T1 *DR,
               llvm::function_ref<bool(const MallocOverflowCheck &)> Pred) {
      auto P = [DR, Pred](const MallocOverflowCheck &Check) {
        if (const auto *CheckDR = dyn_cast<T1>(Check.variable))
          return getDecl(CheckDR) == getDecl(DR) && Pred(Check);
        return false;
      };
      toScanFor.erase(std::remove_if(toScanFor.begin(), toScanFor.end(), P),
                      toScanFor.end());
    }

    void CheckExpr(const Expr *E_p) {
      auto PredTrue = [](const MallocOverflowCheck &) { return true; };
      const Expr *E = E_p->IgnoreParenImpCasts();
      if (const DeclRefExpr *DR = dyn_cast<DeclRefExpr>(E))
        Erase<DeclRefExpr>(DR, PredTrue);
      else if (const auto *ME = dyn_cast<MemberExpr>(E)) {
        Erase<MemberExpr>(ME, PredTrue);
      }
    }

    // Check if the argument to malloc is assigned a value
    // which cannot cause an overflow.
    // e.g., malloc (mul * x) and,
    // case 1: mul = <constant value>
    // case 2: mul = a/b, where b > x
    void CheckAssignmentExpr(BinaryOperator *AssignEx) {
      bool assignKnown = false;
      bool numeratorKnown = false, denomKnown = false;
      APSInt denomVal;
      denomVal = 0;

      // Erase if the multiplicand was assigned a constant value.
      const Expr *rhs = AssignEx->getRHS();
      if (rhs->isEvaluatable(Context))
        assignKnown = true;

      // Discard the report if the multiplicand was assigned a value,
      // that can never overflow after multiplication. e.g., the assignment
      // is a division operator and the denominator is > other multiplicand.
      const Expr *rhse = rhs->IgnoreParenImpCasts();
      if (const BinaryOperator *BOp = dyn_cast<BinaryOperator>(rhse)) {
        if (BOp->getOpcode() == BO_Div) {
          const Expr *denom = BOp->getRHS()->IgnoreParenImpCasts();
          Expr::EvalResult Result;
          if (denom->EvaluateAsInt(Result, Context)) {
            denomVal = Result.Val.getInt();
            denomKnown = true;
          }
          const Expr *numerator = BOp->getLHS()->IgnoreParenImpCasts();
          if (numerator->isEvaluatable(Context))
            numeratorKnown = true;
        }
      }
      if (!assignKnown && !denomKnown)
        return;
      auto denomExtVal = denomVal.getExtValue();

      // Ignore negative denominator.
      if (denomExtVal < 0)
        return;

      const Expr *lhs = AssignEx->getLHS();
      const Expr *E = lhs->IgnoreParenImpCasts();

      auto pred = [assignKnown, numeratorKnown,
                   denomExtVal](const MallocOverflowCheck &Check) {
        return assignKnown ||
               (numeratorKnown && (denomExtVal >= Check.maxVal.getExtValue()));
      };

      if (const DeclRefExpr *DR = dyn_cast<DeclRefExpr>(E))
        Erase<DeclRefExpr>(DR, pred);
      else if (const auto *ME = dyn_cast<MemberExpr>(E))
        Erase<MemberExpr>(ME, pred);
    }

  public:
    void VisitBinaryOperator(BinaryOperator *E) {
      if (E->isComparisonOp()) {
        const Expr * lhs = E->getLHS();
        const Expr * rhs = E->getRHS();
        // Ignore comparisons against zero, since they generally don't
        // protect against an overflow.
        if (!isIntZeroExpr(lhs) && !isIntZeroExpr(rhs)) {
          CheckExpr(lhs);
          CheckExpr(rhs);
        }
      }
      if (E->isAssignmentOp())
        CheckAssignmentExpr(E);
      EvaluatedExprVisitor<CheckOverflowOps>::VisitBinaryOperator(E);
    }

    /* We specifically ignore loop conditions, because they're typically
     not error checks.  */
    void VisitWhileStmt(WhileStmt *S) {
      return this->Visit(S->getBody());
    }
    void VisitForStmt(ForStmt *S) {
      return this->Visit(S->getBody());
    }
    void VisitDoStmt(DoStmt *S) {
      return this->Visit(S->getBody());
    }

    CheckOverflowOps(theVecType &v, ASTContext &ctx)
    : EvaluatedExprVisitor<CheckOverflowOps>(ctx),
      toScanFor(v), Context(ctx)
    { }
  };
}

// OutputPossibleOverflows - We've found a possible overflow earlier,
// now check whether Body might contain a comparison which might be
// preventing the overflow.
// This doesn't do flow analysis, range analysis, or points-to analysis; it's
// just a dumb "is there a comparison" scan.  The aim here is to
// detect the most blatent cases of overflow and educate the
// programmer.
void ScopeMemoryAllocationChecker::OutputPossibleOverflows(
  SmallVectorImpl<MallocOverflowCheck> &PossibleMallocOverflows,
  const Decl *D, BugReporter &BR, AnalysisManager &mgr) const {
  // By far the most common case: nothing to check.
  if (PossibleMallocOverflows.empty())
    return;
	
  // Delete any possible overflows which have a comparison.
  CheckOverflowOps c(PossibleMallocOverflows, BR.getContext());
  c.Visit(mgr.getAnalysisDeclContext(D)->getBody());

  // Output warnings for all overflows that are left.
  for (CheckOverflowOps::theVecType::iterator
       i = PossibleMallocOverflows.begin(),
       e = PossibleMallocOverflows.end();
       i != e;
       ++i) {
    // BR.EmitBasicReport(
    //     D, this, "malloc() size overflow", categories::UnixAPI,
    //     "分配的内存可能会导致溢出",   //the computation of the size of the memory allocation may overflow
    //     PathDiagnosticLocation::createOperatorLoc(i->mulop,
    //                                               BR.getSourceManager()),
    //     i->mulop->getSourceRange());
  }
}

void ScopeMemoryAllocationChecker::checkASTCodeBody(const Decl *D,
                                             AnalysisManager &mgr,
                                             BugReporter &BR) const {
                                               
  CFG *cfg = mgr.getCFG(D);
  if (!cfg)
    return;
  // A list of variables referenced in possibly overflowing malloc operands.
  SmallVector<MallocOverflowCheck, 2> PossibleMallocOverflows;

  for (CFG::iterator it = cfg->begin(), ei = cfg->end(); it != ei; ++it) {
    CFGBlock *block = *it;
    for (CFGBlock::iterator bi = block->begin(), be = block->end();
         bi != be; ++bi) {
      if (Optional<CFGStmt> CS = bi->getAs<CFGStmt>()) {
        if (const CallExpr *TheCall = dyn_cast<CallExpr>(CS->getStmt())) {
          // Get the callee.
          const FunctionDecl *FD = TheCall->getDirectCallee();

          if (!FD)
            continue;

          // Get the name of the callee. If it's a builtin, strip off the prefix.
          IdentifierInfo *FnInfo = FD->getIdentifier();
          if (!FnInfo)
            continue;

          if (FnInfo->isStr ("malloc") || FnInfo->isStr ("_MALLOC")) {
            if (TheCall->getNumArgs() == 1){
              CheckMallocArgument(PossibleMallocOverflows, TheCall->getArg(0),
                                  mgr.getASTContext());
            }
          }
        }
      }
    }
  }

  OutputPossibleOverflows(PossibleMallocOverflows, D, BR, mgr);
}

//编写注册逻辑
class ScopeMemoryAllocationCheckerRegister :public scope::CheckerRegisterFactory
{
public:
	virtual void registerCheckers(clang::ento::CheckerRegistry &registry)
	{
		//声明要用到的checker名称(这样可以降低内存开销)
		llvm::StringRef CheckMemoryAllocation = getCheckScopeAlphaNameStr("ScopeMemoryAllocation");

		//添加Checker
		registry.addChecker<ScopeMemoryAllocationChecker>(CheckMemoryAllocation,
			"Check Memory Allocation with Excessive Size Value", NOT_CHECKER_DOC, NOT_HIDDEN_CHECKER);
	}
};
//显式声明全局对象实例以激活注册
SCOPE_REGISTER_CLASS(ScopeMemoryAllocationCheckerRegister)
