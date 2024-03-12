/*!
 * FileName: ScopeSampleChecker.cpp
 *
 * Author:   AuthorName
 * Date:     xxxx-x-x
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 检测对main函数的调用
 */

#include "common/CheckScope.h"

#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Frontend/CheckerRegistry.h"

using namespace clang;
using namespace ento;

namespace {
	class MainCallChecker : public Checker<check::PreStmt<CallExpr>> {
		mutable std::unique_ptr<BugType> BT;

	public:
		void checkPreStmt(const CallExpr *CE, CheckerContext &C) const;
	};
} // end anonymous namespace

void MainCallChecker::checkPreStmt(const CallExpr *CE,
	CheckerContext &C) const {
	const Expr *Callee = CE->getCallee();
	const FunctionDecl *FD = C.getSVal(Callee).getAsFunctionDecl();

	if (!FD)
		return;

	// Get the name of the callee.
	IdentifierInfo *II = FD->getIdentifier();
	if (!II) // if no identifier, not a simple C function
		return;

	if (II->isStr("main")) {
		ExplodedNode *N = C.generateErrorNode();
		if (!N)
			return;

		if (!BT)
			BT.reset(new BugType(this, "call to main", "example analyzer plugin"));

		auto report =
			std::make_unique<PathSensitiveBugReport>(*BT, BT->getDescription(), N);
		report->addRange(Callee->getSourceRange());
		C.emitReport(std::move(report));
	}
}

//编写注册逻辑
class ScopeSampleCheckerRegister :public scope::CheckerRegisterFactory
{
public:
	virtual void registerCheckers(clang::ento::CheckerRegistry &registry)
	{
		//声明要用到的checker名称(这样可以降低内存开销)
		llvm::StringRef CheckDSample= getCheckScopeAlphaNameStr("ScopeSample");

		//添加Checker
		registry.addChecker<MainCallChecker>(CheckDSample,
			"Check the explicit call to the main function", NOT_CHECKER_DOC, NOT_HIDDEN_CHECKER);
	}
};
//显式声明全局对象实例以激活注册
SCOPE_REGISTER_CLASS(ScopeSampleCheckerRegister)
