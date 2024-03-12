// This defines FloatEqualChecker
// checks for float equal.

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/StmtVisitor.h"
#include "clang/Analysis/AnalysisDeclContext.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>

using namespace clang;
using namespace ento;
using namespace std;

namespace {
class WalkAST : public StmtVisitor<WalkAST> {
  BugReporter &BR;
  AnalysisDeclContext* AC;
  const CheckerBase *Checker;

public:
  WalkAST(BugReporter &br, AnalysisDeclContext *ac,const CheckerBase *c)
      : BR(br), AC(ac),Checker(c) {}
	  
  // Statement visitor methods.
  void VisitIfStmt(IfStmt *IS);
  void VisitStmt(Stmt *S) { VisitChildren(S); }
 
  void VisitChildren(Stmt *S);
  
  // Checker-specific methods.
  void checkFloatEqual(const IfStmt *IS);
};
}

//===----------------------------------------------------------------------===//
// AST walking.
//===----------------------------------------------------------------------===//
void WalkAST::VisitChildren(Stmt *S) {
  for (Stmt *Child : S->children())
    if (Child)
      Visit(Child);
}

void WalkAST::VisitIfStmt(IfStmt *FS) {
	
  // std::cout << "进入VisitIfStmt函数中，检测到存在if语句"  << std::endl;
  
  //检查if条件，将语句FS传入这个函数
  checkFloatEqual(FS);

  //递归的查看子语句
  VisitChildren(FS);
}

void WalkAST::checkFloatEqual(const IfStmt *IS) {
	// 语句中有条件吗？
	const Expr *condition = IS->getCond();
	
	// std::cout << "进入checkFloatEqual函数中"  << std::endl;
	
	//如果条件为空就返回
	if (!condition)
		return;
	
	// Is the loop condition a comparison?
	const BinaryOperator *B = dyn_cast<BinaryOperator>(condition);

	//如果没有拿到二元操作符，跳出
	if (!B)
		return;

	// 如果不是判等，跳出
	if (!B->isEqualityOp())
		return;
	
	// Are we comparing variables?
	const DeclRefExpr *drLHS = dyn_cast<DeclRefExpr>(B->getLHS()->IgnoreParenLValueCasts());
	const DeclRefExpr *drRHS = dyn_cast<DeclRefExpr>(B->getRHS()->IgnoreParenLValueCasts());

	// 判断操作数类型是否是float,如果是float则不为空，否则为空指针类型
	drLHS = drLHS && drLHS->getType()->isRealFloatingType() ? drLHS : nullptr;
	drRHS = drRHS && drRHS->getType()->isRealFloatingType() ? drRHS : nullptr;

	//除非两个数都是float类型，否则就跳出
	if (!(drLHS && drRHS))
		return;
	
	// std::cout << "warning：两个float类型的操作数进行比较"  << std::endl;
	
	SmallVector<SourceRange, 2> ranges;
	
	//如果checker走到这里，说明程序中有if语句两个float类型的操作数进行比较，报出警告
	CheckerNameRef checkName_FloatEqual;
	
	const char *bugType = "出现两个float类型的操作数相等 ";
	PathDiagnosticLocation FSLoc = PathDiagnosticLocation::createBegin(IS, BR.getSourceManager(), AC);
	BR.EmitBasicReport(AC->getDecl(), Checker, bugType, "warning", "两个float类型的操作数进行相等比较", FSLoc, ranges);
	
}

//===----------------------------------------------------------------------===//
// FloatEqualChecker
//===----------------------------------------------------------------------===//
namespace {
class FloatEqualChecker : public Checker<check::ASTCodeBody> {
public:
	void checkASTCodeBody(const Decl *D, AnalysisManager& mgr,
						  BugReporter &BR) const {
	  WalkAST walker(BR, mgr.getAnalysisDeclContext(D),this);
	  walker.Visit(D->getBody());
	}
  };
}

void ento::registerFloatEqualChecker(CheckerManager &mgr) {
  mgr.registerChecker<FloatEqualChecker>();
}

bool ento::shouldRegisterFloatEqualChecker(const CheckerManager &mgr) {
  return true;
}
