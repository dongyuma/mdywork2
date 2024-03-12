/*!
 * FileName: FunctionProcedureChecker.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2021-2-4
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 函数过程检测
 */

#include <iostream>
#include <set>

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTNodeTraverser.h"
#include "clang/AST/ParentMap.h"
#include "clang/AST/PrettyPrinter.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/Analysis/Analyses/ReachableCode.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/Sema/Sema.h"


using namespace clang;
using namespace ento;


enum ControlFlowKind {
	UnknownFallThrough,
	NeverFallThrough,
	MaybeFallThrough,
	AlwaysFallThrough,
	NeverFallThroughOrReturn
};

namespace
{
	class FunctionProcedureChecker :public Checker<check::PreStmt<ReturnStmt>, check::PreStmt<DeclStmt>, check::PreStmt<BinaryOperator>, check::ASTCodeBody, check::PreCall >
	{
	public:
		void checkPreStmt(const ReturnStmt *RS, CheckerContext &C) const;
		void checkPreStmt(const DeclStmt *DS, CheckerContext &C) const;
		void checkPreStmt(const BinaryOperator *BO, CheckerContext &C) const;
		void checkASTCodeBody(const Decl *D, AnalysisManager &Mgr, BugReporter &BR) const;
		void checkPreCall(const CallEvent &Call, CheckerContext &C) const;
	public:
		//************************************
		// Method:    isEquivalentQualType
		// FullName:  FunctionProcedureChecker::isEquivalentQualType
		// Access:    private 
		// Returns:   bool
		// Qualifier: const 比较两个类型是否等效 
		// Parameter: const QualType & a
		// Parameter: const QualType & b
		//************************************
		static bool isEquivalentQualType(const QualType &a, const QualType &b);

		static bool isNullToPointerType(const QualType &T, const Expr *E, ASTContext &C);
	};
}


namespace
{
	//统计表达式中函数调用数量
	class CountFunctionCalls : public RecursiveASTVisitor<CountFunctionCalls>
	{
	public:
		explicit CountFunctionCalls(BugReporter &B, const CheckerBase *C, AnalysisDeclContext *A) :BR(B), Checker(C), AC(A)
		{
			fun_calls_count = 0;
		}

		unsigned int getFunCallsCount()
		{
			return fun_calls_count;
		}

		bool VisitCallExpr(const CallExpr *CE);

	private:
		BugReporter &BR;
		const CheckerBase *Checker;
		AnalysisDeclContext *AC;
		unsigned int fun_calls_count;
	};
}

bool CountFunctionCalls::VisitCallExpr(const CallExpr *CE)
{
	fun_calls_count++;
	return true;
}

static std::string getFunctionDeclString(const FunctionDecl *FD)
{
	std::string declStr;
	declStr = FD->getReturnType().getAsString();
	declStr.append(" ");
	declStr.append(FD->getNameAsString());
	declStr.append("(");
	for (unsigned int i = 0; i < FD->getNumParams(); i++)
	{
		if (i > 0)
		{
			declStr.append(", ");
		}
		declStr.append(FD->parameters()[i]->getType().getAsString());
	}
	declStr.append(")");

	return declStr;
}

#if 0
struct Test
{
	int a;
	int b;
};
typedef struct Test TTT;
TTT test2()
{
	TTT t1;
	struct Test &t = t1;
	scanf("%d", &t.a);
	t.b = 1;
	if (t.a == 1)
	{
		return t1; //no warn
	}
	return t; //no warn
}
int test(const unsigned a, unsigned b)
{
	const int res = 0;
	if (a == 1)
	{
		return a; //warn
	}

	if (a == 2)
	{
		return a + 2.0; //warn
	}

Out:
	return res; //no warn
}
const void *test2(const char *a, const int *b)
{
	if (a != NULL)
	{
		return a; //warn
	}
	if (*b != NULL)
	{
		return b; //warn
	}
	return (const void *)b; //no warn
}
#endif

void FunctionProcedureChecker::checkPreStmt(const ReturnStmt *RS, CheckerContext &C) const
{
	if (!RS || !C.getSourceManager().isInMainFile(RS->getBeginLoc()))
	{
		return;
	}
	AnalysisDeclContext *AC = C.getCurrentAnalysisDeclContext();

	const Expr *R_expr = RS->getRetValue();
	if (!R_expr)
	{
		return;
	}

	QualType R_type = R_expr->IgnoreParenImpCasts()->getType();
	const FunctionDecl *FD=dyn_cast_or_null<FunctionDecl>(C.getCurrentAnalysisDeclContext()->getDecl());

	if (FD != NULL)
	{
		QualType FR_type = FD->getReturnType();
		if (!isNullToPointerType(FR_type, R_expr, C.getASTContext()))
		{
			if (!isEquivalentQualType(FR_type, R_type))
			{
				std::string fun_ret_type_name = FR_type.getAsString();
				std::string ret_type_name = R_type.getAsString();

				std::string warn;
				warn = "错误的返回值类型，函数在";
				warn.append(FD->getBeginLoc().printToString(C.getSourceManager()));
				warn.append("处声明为：");
				warn.append(getFunctionDeclString(FD));
				warn.append(",但是在这儿的返回类型是");
				warn.append(R_type.getUnqualifiedType().getAsString());

				PathDiagnosticLocation ELoc(RS->getBeginLoc(), C.getSourceManager());
				C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "返回类型错误", categories::SecurityError, warn, ELoc, RS->getSourceRange());
			}
		}
	}
}

#if 0 
//禁止同一表达式中调用多个相关函数
int fun1(int a)
{
	return a + 1;
}
int fun2(int a)
{
	return a + 2;
}
int main(int argc, char *argv[])
{
	int a = 10 + fun1(1); //no warn
	printf("%d\n", a);
	a = fun1(a) + fun2(a); //warn
	printf("%d\n", a);
	int b = fun1(a) + fun2(a);  //warn
	printf("%d\n", b);
	b = fun2(b); //no warn
	printf("%d\n", b);
	return 0;
}
#endif

void FunctionProcedureChecker::checkPreStmt(const DeclStmt *DS, CheckerContext &C) const
{
	AnalysisDeclContext *AC = C.getCurrentAnalysisDeclContext();
	//检测声明语句中函数调用数量
	CountFunctionCalls CFC(C.getBugReporter(), this, C.getCurrentAnalysisDeclContext());
	CFC.TraverseDeclStmt(const_cast<DeclStmt *>(DS));
	if (CFC.getFunCallsCount() > 1)
	{
		PathDiagnosticLocation ELoc(DS->getBeginLoc(), C.getSourceManager());
		C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "多次调用", categories::SecurityError, "建议不要在同一表达式中调用多个函数", ELoc, DS->getSourceRange());
	}
}

void FunctionProcedureChecker::checkPreStmt(const BinaryOperator *BO, CheckerContext &C) const
{
	AnalysisDeclContext *AC = C.getCurrentAnalysisDeclContext();

	//检测赋值语句中函数调用数量
	if (BO->isAssignmentOp())
	{
		CountFunctionCalls CFC(C.getBugReporter(), this, C.getCurrentAnalysisDeclContext());
		CFC.TraverseBinaryOperator(const_cast<BinaryOperator *>(BO));
		if (CFC.getFunCallsCount() > 1)
		{
			PathDiagnosticLocation ELoc(BO->getBeginLoc(), C.getSourceManager());
			C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "多次调用", categories::SecurityError, "建议不要在同一表达式中调用多个函数", ELoc, BO->getSourceRange());
		}
	}
}

static ControlFlowKind CheckFallThrough(AnalysisDeclContext &AC) {
	CFG *cfg = AC.getCFG();
	if (!cfg) return UnknownFallThrough;

	// The CFG leaves in dead things, and we don't want the dead code paths to
	// confuse us, so we mark all live things first.
	llvm::BitVector live(cfg->getNumBlockIDs());
	unsigned count = reachable_code::ScanReachableFromBlock(&cfg->getEntry(),
		live);

	bool AddEHEdges = AC.getAddEHEdges();
	if (!AddEHEdges && count != cfg->getNumBlockIDs())
		// When there are things remaining dead, and we didn't add EH edges
		// from CallExprs to the catch clauses, we have to go back and
		// mark them as live.
		for (const auto *B : *cfg) {
			if (!live[B->getBlockID()]) {
				if (B->pred_begin() == B->pred_end()) {
					const Stmt *Term = B->getTerminatorStmt();
					if (Term && isa<CXXTryStmt>(Term))
						// When not adding EH edges from calls, catch clauses
						// can otherwise seem dead.  Avoid noting them as dead.
						count += reachable_code::ScanReachableFromBlock(B, live);
					continue;
				}
			}
		}

	// Now we know what is live, we check the live precessors of the exit block
	// and look for fall through paths, being careful to ignore normal returns,
	// and exceptional paths.
	bool HasLiveReturn = false;
	bool HasFakeEdge = false;
	bool HasPlainEdge = false;
	bool HasAbnormalEdge = false;

	// Ignore default cases that aren't likely to be reachable because all
	// enums in a switch(X) have explicit case statements.
	CFGBlock::FilterOptions FO;
	FO.IgnoreDefaultsWithCoveredEnums = 1;

	for (CFGBlock::filtered_pred_iterator I =
		cfg->getExit().filtered_pred_start_end(FO);
		I.hasMore(); ++I) {
		const CFGBlock &B = **I;
		if (!live[B.getBlockID()])
			continue;

		// Skip blocks which contain an element marked as no-return. They don't
		// represent actually viable edges into the exit block, so mark them as
		// abnormal.
		if (B.hasNoReturnElement()) {
			HasAbnormalEdge = true;
			continue;
		}

		// Destructors can appear after the 'return' in the CFG.  This is
		// normal.  We need to look pass the destructors for the return
		// statement (if it exists).
		CFGBlock::const_reverse_iterator ri = B.rbegin(), re = B.rend();

		for (; ri != re; ++ri)
			if (ri->getAs<CFGStmt>())
				break;

		// No more CFGElements in the block?
		if (ri == re) {
			const Stmt *Term = B.getTerminatorStmt();
			if (Term && isa<CXXTryStmt>(Term)) {
				HasAbnormalEdge = true;
				continue;
			}
			// A labeled empty statement, or the entry block...
			HasPlainEdge = true;
			continue;
		}

		CFGStmt CS = ri->castAs<CFGStmt>();
		const Stmt *S = CS.getStmt();
		if (isa<ReturnStmt>(S) || isa<CoreturnStmt>(S)) {
			HasLiveReturn = true;
			continue;
		}
		if (isa<ObjCAtThrowStmt>(S)) {
			HasFakeEdge = true;
			continue;
		}
		if (isa<CXXThrowExpr>(S)) {
			HasFakeEdge = true;
			continue;
		}
		if (isa<MSAsmStmt>(S)) {
			// TODO: Verify this is correct.
			HasFakeEdge = true;
			HasLiveReturn = true;
			continue;
		}
		if (isa<CXXTryStmt>(S)) {
			HasAbnormalEdge = true;
			continue;
		}
		if (std::find(B.succ_begin(), B.succ_end(), &cfg->getExit())
			== B.succ_end()) {
			HasAbnormalEdge = true;
			continue;
		}

		HasPlainEdge = true;
	}
	if (!HasPlainEdge) {
		if (HasLiveReturn)
			return NeverFallThrough;
		return NeverFallThroughOrReturn;
	}
	if (HasAbnormalEdge || HasFakeEdge || HasLiveReturn)
		return MaybeFallThrough;
	// This says AlwaysFallThrough for calls to functions that are not marked
	// noreturn, that don't return.  If people would like this warning to be more
	// accurate, such functions should be marked as noreturn.
	return AlwaysFallThrough;
}


namespace
{
	//在指定上下文中查找 在F文件中的函数定义 并检测该函数是否在指定上下文中被调用
	class FindUnusedFunctionInAST : public RecursiveASTVisitor<FindUnusedFunctionInAST>
	{
	public:
		explicit FindUnusedFunctionInAST(BugReporter &B, const CheckerBase *C, FileID F, bool inMainFunFile = false) :BR(B), Checker(C), FID(F), isMainFunFile(inMainFunFile) {}

		bool VisitFunctionDecl(const FunctionDecl *FD);

	private:
		BugReporter &BR;
		const CheckerBase *Checker;
		FileID FID;
		bool isMainFunFile;
	};
}

bool FindUnusedFunctionInAST::VisitFunctionDecl(const FunctionDecl *FD)
{
	const SourceManager &SM = BR.getSourceManager();
	if (SM.isInFileID(FD->getLocation(), FID))
	{
		if (FD->isInlineSpecified() || FD->isStatic() || FD->getBody() == NULL || FD->isDeleted() || FD->isMain())
		{
			//跳过 内联函数 静态函数 析构函数 函数声明 main函数
			return true;
		}

		//if (FD->getDescribedFunctionTemplate() == NULL) //clang\lib\Sema\Sema.cpp 用的是这种方式
		if (!FD->isUsed())
		{
			//该函数定义从未该上下文中使用过
			PathDiagnosticLocation ELoc(FD->getBeginLoc(), BR.getSourceManager());

			std::string warn = "函数";
			warn.append(FD->getNameAsString());
			warn.append("没有在以下的函数中被调用：");

			const FileEntry *FE = SM.getFileEntryForID(SM.getMainFileID());
			if (FE != NULL)
			{
				warn.append(FE->getName().str());
				if (isMainFunFile)
				{
					warn.append(" (主函数)");
				}
				BR.EmitBasicReport(FD, Checker, "未使用的函数", categories::LogicError, warn, ELoc);
			}
		}
	}
	return true;
}

#if 0
void test4() //no warn
{
	printf("-----\n");
}

int test3(int a) //warn
{
	if (a == 3)
	{
		return a;
	}
	else
	{
		printf("----------\n");
	}
}


int main() //no warn
{
	return 0;
}
#endif
void FunctionProcedureChecker::checkASTCodeBody(const Decl *D, AnalysisManager &Mgr, BugReporter &BR) const
{
	if (!isa<FunctionDecl>(D))
	{
		return;
	}
	const FunctionDecl *FD = dyn_cast_or_null<FunctionDecl>(D);
	if (FD == NULL)
	{
		return;
	}

	bool ReturnsVoid = false;
	bool HasNoReturn = false;


	const auto *CBody = dyn_cast<CoroutineBodyStmt>(FD->getBody());
	if (CBody != NULL)
	{
		ReturnsVoid = CBody->getFallthroughHandler() != nullptr;
	}
	else
	{
		ReturnsVoid = FD->getReturnType()->isVoidType();
	}	
	HasNoReturn = FD->isNoReturn();

	//检查是否存在应该从返回一个值的地方退出了函数
	ControlFlowKind kind = CheckFallThrough(*(Mgr.getAnalysisDeclContext(D)));

	if (kind == MaybeFallThrough || kind == AlwaysFallThrough)
	{
		if (HasNoReturn)
		{
			//永不返回的函数(像exec)在函数的某个地方退出了
			PathDiagnosticLocation ELoc(FD->getBeginLoc(), BR.getSourceManager());
			std::string warn = "函数";
			warn.append(FD->getNameAsString());
			warn.append("声明为'no return' 不应该返回值");
			BR.EmitBasicReport(FD, this, "退出函数", categories::LogicError, warn, ELoc);
		}
		else if (!ReturnsVoid)
		{
			//有返回值的函数在函数的某个位置直接退出了而不是以return 返回值的方式结束
			PathDiagnosticLocation ELoc(FD->getBeginLoc(), BR.getSourceManager());
			std::string warn = "非空函数";
			warn.append(FD->getNameAsString());
			warn.append("在所有的路径中都没有返回值");
			BR.EmitBasicReport(FD, this, "退出函数", categories::LogicError, warn, ELoc);
		}
	}

	SourceManager &SM = Mgr.getSourceManager();
	if (FD->isStatic() && !FD->isDeleted() && !FD->isReferenced())
	{
		if (SM.isInMainFile(FD->getLocation()) && !FD->isUsed())
		{
			//静态函数从未被使用
			PathDiagnosticLocation ELoc(FD->getBeginLoc(), BR.getSourceManager());
			std::string warn = "静态函数";
			warn.append(FD->getNameAsString());
			warn.append("没有被调用");
			BR.EmitBasicReport(FD, this, "未使用的函数", categories::LogicError, warn, ELoc);
		}
	}
	else if (!FD->isStatic() && FD->isMain())
	{
		//检测main函数所在文件中是否有未被该文件中子过程调用的函数定义并报告bug
		FileID FID = SM.getMainFileID();

		//非静态函数如果在主函数所在文件中则必须在该文件中存在对该函数的调用
		FindUnusedFunctionInAST FUF(BR, this, FID, true);
		FUF.TraverseAST(Mgr.getASTContext()); //遍历主过程所在文件的上下文
	}
	
}

void FunctionProcedureChecker::checkPreCall(const CallEvent &Call, CheckerContext &C) const
{
	const Expr *originExpr = Call.getOriginExpr();
	if (!originExpr || !C.getSourceManager().isInMainFile(originExpr->getBeginLoc()))
	{
		return;
	}

	const FunctionDecl *FD = dyn_cast_or_null<FunctionDecl>(Call.getDecl());
	if (!FD)
	{
		return;
	}

	AnalysisDeclContext *AC = C.getCurrentAnalysisDeclContext();

	//形参列表
	auto formals= FD->parameters();
	std::string fun_name = FD->getNameAsString();

	for (unsigned i = 0, ei = std::min(static_cast<unsigned>(formals.size()), Call.getNumArgs()); i != ei; ++i) 
	{
		//实参 表达式
		const Expr *actual_expr = Call.getArgExpr(i);
		if (actual_expr == NULL)
		{
			continue;
		}

		QualType actual_type = actual_expr->IgnoreParenImpCasts()->getType();
		QualType formal_type = formals[i]->getType();

		if (!isNullToPointerType(formal_type, actual_expr, C.getASTContext()))
		{
			//比较实参形参的类型是否一致
			if (!isEquivalentQualType(formal_type, actual_type))
			{
				PathDiagnosticLocation ELoc(actual_expr->getBeginLoc(), C.getSourceManager());
				std::string warn = "函数";
				warn.append(FD->getNameAsString());
				warn.append("的第");
				warn.append(std::to_string(i + 1));
				warn.append("个参数：");
				warn.append("实参的类型是");
				warn.append(actual_type.getAsString());
				warn.append(",与形参的类型");
				warn.append(formal_type.getAsString());
				warn.append("不一致.");
				C.getBugReporter().EmitBasicReport(FD, this, "类型错误", categories::LogicError, warn, ELoc, actual_expr->getSourceRange());
			}
			else
			{
				//检查实参是否为void类型
				if (actual_type->isVoidPointerType() || actual_type->isVoidType())
				{
					PathDiagnosticLocation ELoc(actual_expr->getBeginLoc(), C.getSourceManager());
					C.getBugReporter().EmitBasicReport(FD, this, "类型错误", categories::LogicError, "不建议将void类型参数用于函数调用", ELoc, actual_expr->getSourceRange());
				}
			}
		}
	}

	if ((fun_name == "longjmp" || fun_name == "siglongjmp") && formals.size() == 2)
	{
		PathDiagnosticLocation ELoc = PathDiagnosticLocation::createBegin(Call.getOriginExpr(), C.getSourceManager(), AC);
		C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "Long jump", categories::SecurityError, "避免在函数过程中使用longjmp", ELoc, Call.getOriginExpr()->getSourceRange());
	}
}

bool FunctionProcedureChecker::isEquivalentQualType(const QualType &a, const QualType &b)
{
	bool res = false;
	QualType T1 = a->isReferenceType() ? a.getNonReferenceType() : a;
	QualType T2 = b->isReferenceType() ? b.getNonReferenceType() : b;

	T1 = QualType(T1->getUnqualifiedDesugaredType(), 0);
	T2 = QualType(T2->getUnqualifiedDesugaredType(), 0);

	//后期优化更多数据类型的比较
	if (T1->isBuiltinType() || T1->isEnumeralType() || T1->isStructureType())
	{
		res = T1 == T2;
	}
	else if (T1->isPointerType() && !T1->isFunctionPointerType())
	{
		if (T2->isPointerType() || T2->isArrayType())
		{
			//如果源也是指针或数组则继续比较子元素类型
			res = isEquivalentQualType(QualType(T1->getPointeeOrArrayElementType(), 0), QualType(T2->getPointeeOrArrayElementType(), 0));
		}
		else
		{
			res = false;
		}
	}
	else
	{
		//其余类型(函数指针 类什么的) 暂时不进行检测
		res = true;
	}
	return res;
}

bool FunctionProcedureChecker::isNullToPointerType(const QualType &T, const Expr *E, ASTContext &C)
{
	bool res = false;
	QualType Tmp = T->isReferenceType() ? T.getNonReferenceType() : T;
	Tmp = QualType(T->getUnqualifiedDesugaredType(), 0);

	QualType Expr_type = E->getType();
	Expr_type = Expr_type->isReferenceType() ? Expr_type.getNonReferenceType() : Expr_type;
	Expr_type = QualType(Expr_type->getUnqualifiedDesugaredType(), 0);

	if (Tmp->isPointerType() && (E->isNullPointerConstant(C, Expr::NPC_ValueDependentIsNull) || Expr_type->isNullPtrType()))
	{
		res = true;
	}
	return res;
}

void ento::registerFunctionProcedureChecker(CheckerManager &Mgr)
{
	Mgr.registerChecker<FunctionProcedureChecker>();
}

bool ento::shouldRegisterFunctionProcedureChecker(const CheckerManager &Mgr)
{
	return true;
}