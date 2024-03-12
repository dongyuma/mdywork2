#include "Taint.h"
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h"
#include <iostream>


using namespace clang;
using namespace ento;
using namespace taint;
using namespace std;


namespace {
class UnsignMinusOverflowChecker : public Checker< check::PreStmt<BinaryOperator> > {
  mutable std::unique_ptr<BuiltinBug> BT;
  mutable std::set<const Expr *> OverflowExprs;
  public:
  void checkPreStmt(const BinaryOperator *BO, CheckerContext &Ctx) const;

  //private:
  void reportBug(CheckerContext &Ctx,
                 const ProgramStateRef ErrorState,
                 const char *Msg,
                 const Expr *E) const;


 // llvm::Optional<QualType> getBaseType(Expr *E) const;
 // void checkSubtraction(const BinaryOperator *BO, CheckerContext &Ctx,
//                        QualType &VarType) const;
  void verify(CheckerContext &Ctx, const ProgramStateRef &CurState,
              const SVal &VerifySVal, const Expr *E) const;
};
}// end anonymous namespace



//checkPreStmt get binary operator
void UnsignMinusOverflowChecker::checkPreStmt(const BinaryOperator *BO,
                                  CheckerContext &Ctx) const {
  // cout<<"进入unsignchecker"<<endl;
  if (OverflowExprs.find(BO) != OverflowExprs.end()) {
    return;
  }
  
  // if left and right parameter  are unsigned integer
  if((!(BO->getLHS()->getType()->isUnsignedIntegerType())) && (!(BO->getRHS()->getType()->isUnsignedIntegerType()))){
	 return;
  }
  
  //BO->getLHS()->getType()->dump();
  BinaryOperator::Opcode Op = BO->getOpcode();
  if (!(Op == BO_Sub) && !(Op == BO_SubAssign)) {
    //checkSubtraction(BO, Ctx, *BaseType);
	return;
	}
  //Prepare needed sval
  // Expr *LHS = BO->getLHS();
  // Expr *RHS = BO->getRHS();
  SVal LHSSval = Ctx.getSVal(BO->getLHS()); //binary operator left
  SVal RHSSval = Ctx.getSVal(BO->getRHS());//binary operator right
  SValBuilder &SVB = Ctx.getSValBuilder();
  QualType BoolType = SVB.getConditionType();
  const ProgramStateRef &CurState = Ctx.getState();
  if(!(LHSSval.isConstant()&&RHSSval.isConstant())){
    return;
  }
  
	SVal VerifySVal = SVB.evalBinOp(CurState, BO_LT , LHSSval, RHSSval, BoolType); //l < r
  verify(Ctx, CurState, VerifySVal, BO);
}


void UnsignMinusOverflowChecker::verify(CheckerContext &Ctx,
                                    const ProgramStateRef &CurState,
                                    const SVal &VerifySVal,
                                    const Expr *E) const {
  Optional<DefinedSVal> VerifyCond = VerifySVal.getAs<DefinedSVal>();
  if (!VerifyCond) {
    return;
  }
  ProgramStateRef Overflow = CurState->assume(*VerifyCond, true);
  if (Overflow) {
    std::string MSG = "存在潜在的整数溢出 ";
    reportBug(Ctx, Overflow, MSG.c_str(), E);
    OverflowExprs.insert(E);
  }
  return;
}

void UnsignMinusOverflowChecker::reportBug(CheckerContext &Ctx,
                                       const ProgramStateRef ErrorState,
                                       const char *Msg,
                                       const Expr *E) const {
  if (ExplodedNode *N = Ctx.generateErrorNode(ErrorState)) {
    if (!BT) {
      BT.reset(new BuiltinBug(this, "无符号负溢出", "无符号负溢出"));
    }
    // Generate a report for this warning.
    auto report = std::make_unique<PathSensitiveBugReport>(*BT, Msg, N);
    report->addRange(E->getSourceRange());
    Ctx.emitReport(std::move(report));
  }
}

void ento::registerUnsignMinusOverflowChecker(CheckerManager &mgr) {
  mgr.registerChecker<UnsignMinusOverflowChecker>();
}

bool ento::shouldRegisterUnsignMinusOverflowChecker(const CheckerManager &mgr) {
  return true;
}

