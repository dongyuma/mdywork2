/*!
 * FileName: ConditionalBranchChecker.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2021-1-28
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 条件分支检测
 */

#include <iostream>
#include <set> 

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTNodeTraverser.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

using namespace clang;
using namespace ento;


static bool isEmptyStmt(const Stmt *stmt)
{
	bool res = true;
	if (!stmt)
	{
		res = true;
	}
	else if (stmt->getStmtClass() == Stmt::NullStmtClass)
	{
		res = true;
	}
	else if (stmt->getStmtClass() == Stmt::CompoundStmtClass)
	{
		const CompoundStmt *CS = dyn_cast_or_null<CompoundStmt>(stmt);
		for (const Stmt *CSstmt : CS->body())
		{
			if (!isEmptyStmt(CSstmt))
			{
				res = false;
				break;
			}
		}
	}
	else
	{
		res = false;
	}
	return res;
}

namespace
{
	//查找二进制运算符可查找单独的一个运算符或者查找set集合中第一个出现的运算符
	class FindBinaryOperator :public RecursiveASTVisitor<FindBinaryOperator>
	{
	public:
		explicit FindBinaryOperator(BinaryOperatorKind O)
		{
			opcodes.insert(O);
		}

		explicit FindBinaryOperator(const std::set<BinaryOperatorKind> &OSET)
		{
			opcodes = OSET;
		}

		bool VisitBinaryOperator(const BinaryOperator *BO);

		void reset(BinaryOperatorKind O)
		{
			loc = SourceLocation();
			opcodes.clear();
			opcodes.insert(O);
		}

		void reset(const std::set<BinaryOperatorKind> &OSET)
		{
			loc = SourceLocation();
			opcodes = OSET;
		}

		SourceLocation getBinaryOperatorLoc()
		{
			return loc;
		}

	private:
		std::set<BinaryOperatorKind> opcodes;
		SourceLocation loc;
	};

	bool FindBinaryOperator::VisitBinaryOperator(const BinaryOperator *BO)
	{
		BinaryOperatorKind opcode = BO->getOpcode();
		std::set<BinaryOperatorKind>::iterator iter = opcodes.find(opcode);
		if (iter != opcodes.end())
		{
			loc = BO->getOperatorLoc();
			return false;
		}
		return true;
	}

}


static bool conditionExprHasAssignment(const Expr *EXP, SourceLocation &loc)
{
	if (!EXP)
	{
		return false;
	}
	const Stmt *stmt = EXP->getExprStmt();
	
	//在条件表达式中查找有赋值运算的运算符
	std::set<BinaryOperatorKind> ops;
	ops.insert(BinaryOperatorKind::BO_Assign);
	ops.insert(BinaryOperatorKind::BO_MulAssign);
	ops.insert(BinaryOperatorKind::BO_DivAssign);
	ops.insert(BinaryOperatorKind::BO_RemAssign);
	ops.insert(BinaryOperatorKind::BO_AddAssign);
	ops.insert(BinaryOperatorKind::BO_SubAssign);
	ops.insert(BinaryOperatorKind::BO_ShlAssign);
	ops.insert(BinaryOperatorKind::BO_ShrAssign);
	ops.insert(BinaryOperatorKind::BO_AndAssign);
	ops.insert(BinaryOperatorKind::BO_XorAssign);
	ops.insert(BinaryOperatorKind::BO_OrAssign);

	FindBinaryOperator fbo(ops);
	fbo.TraverseStmt(const_cast<Stmt *>(stmt));
	SourceLocation floc = fbo.getBinaryOperatorLoc();
	
	if (floc.isValid())
	{
		//在条件表达式中发现了赋值运算
		loc = floc;
		return true;
	}
	return false;
}

namespace 
{
	class ConditionalBranchVisitor :public RecursiveASTVisitor<ConditionalBranchVisitor>
	{
	public:
		explicit ConditionalBranchVisitor(BugReporter &B, const CheckerBase *C, AnalysisDeclContext *A);

		bool VisitSwitchStmt(const SwitchStmt *SS);
		bool VisitIfStmt(const IfStmt *IFS);
		bool VisitConditionalOperator(const ConditionalOperator *E);

	private:
		BugReporter &BR;
		const CheckerBase *Checker;
		AnalysisDeclContext *AC;
	};	
}


namespace 
{
	class ConditionalBranchChecker :public Checker<check::ASTCodeBody>
	{
	public:
		void checkASTCodeBody(const Decl *D, AnalysisManager &Mgr, BugReporter &BR) const
		{
			ConditionalBranchVisitor Visitor(BR, this, Mgr.getAnalysisDeclContext(D));
			Visitor.TraverseDecl(const_cast<Decl *>(D));
		}
	};
}


ConditionalBranchVisitor::ConditionalBranchVisitor(BugReporter &B, const CheckerBase *C, AnalysisDeclContext *A) :BR(B), Checker(C), AC(A)
{

}

#if 0 
///检测switch case分支逻辑 n<50的case都是应该被检测出来的 但是目前还不能做到case 3:中的break缺省
int main(int argc, char *argv[])
{
	int p;
	int i = 0;
	int x = 0;
	scanf("%d", &p);

	switch (p)
	{
	case 0: //case no break 单条语句
		printf("0\n");
	case 1: //case no break 多语句
		i = 110;
		printf("1\n");
		x = i * 10;
	case 2: //case no break 复合语句
	{
		printf("2\n");
		printf("2 no break\n");
	}
	case 96: //normal case
		break;
	case 97: //normal case
		switch (x) //missing case
		{
		default:
			break;
		}
		break;
	case 98: //normal case
	{
		printf("2\n");
		printf("2 no break\n");
	}
	break;
	case 3: //case no break 嵌套switch  //FIXME 暂时还检测不出来这样的
		switch (x) //missing default
		{
		case 10:
			break;
		}
	case 100: //normal case
		switch (i) //empty switch
		{
		}
		break;
	case 4: //empty case
	case 5: //empty case ;
		;
	case 6: //empty case {}
	{}
	case 7: //empty case {;}
	{; }
	case 101: //normal case
		switch (p * i) //normal switch
		{
		case 10:
			printf("normal\n");
			break;
		default:
			break;
		}
		break;
	case 102: //normal case
		break;
	default:
		printf("default");
	}

	printf("%d %d\n", i, x);
}
#endif

bool ConditionalBranchVisitor::VisitSwitchStmt(const SwitchStmt *SS)
{
	int case_stmt_count = 0;
	bool have_default_stmt = false;

	CFG::BuildOptions cfgBuildOptions;
	std::unique_ptr<CFG> Cfg = CFG::buildCFG(AC->getDecl(), const_cast<Stmt *>((const Stmt *)SS), &(AC->getDecl()->getASTContext()), cfgBuildOptions);
	if (!Cfg)
	{
		return true;
	}
	for (const CFGBlock *B : llvm::reverse(*Cfg)) 
	{	
		const Stmt *Label = B->getLabel();
		if (!Label)
		{
			continue;
		}
		else
		{
			if (!isa<CaseStmt>(Label))
			{
				if (isa<DefaultStmt>(Label))
				{
					have_default_stmt = true;
				}
				continue;
			}
		}

		case_stmt_count++;

		SourceLocation L = Label->getBeginLoc();
		bool bug_reported = false;

		//L.dump(BR.getSourceManager());

		if (L.isMacroID())
		{
			continue;
		}

		const Stmt *Term = B->getTerminatorStmt();
		//Term->dump();
		// Skip empty cases.
		while (B->empty() && !Term && B->succ_size() == 1)
		{
			//case中没有可执行语句
			if (!bug_reported)
			{
				PathDiagnosticLocation ELoc(L, BR.getSourceManager());
				BR.EmitBasicReport(AC->getDecl(), Checker, "空case", categories::LogicError, "case语句为空", ELoc);
				bug_reported = true;
			}

			B = *B->succ_begin();
			Term = B->getTerminatorStmt();
		}
  		if (!bug_reported)
		{
			//检测case不是以break 结尾
			bool found_breakstmt = false;

			//从当前label的终止语句开始找BreakStmt如果当前label的终止语句不存在则这个label也就不存在break
			while (Term != NULL)
			{	
				if (Term->getStmtClass() == Stmt::BreakStmtClass)
				{
					found_breakstmt = true;
					break;
				}
				if (Term->getStmtClass() == Stmt::ReturnStmtClass)
				{
					found_breakstmt = true;
					break;
				}
				if (Term->getStmtClass() == Stmt::GotoStmtClass)
				{
					found_breakstmt = true;
					break;
				}
				Term = NULL;
				if (!B->empty())
				{
					if(B->succ_begin())
					{	
						B = *B->succ_begin();
						if (B)
						{	
							Term = B->getTerminatorStmt();
						}
						else {
							break;
						}
					}
					else {
						break;
					}
				}
			}
			if (!found_breakstmt)
			{
				PathDiagnosticLocation ELoc(L, BR.getSourceManager());
				BR.EmitBasicReport(AC->getDecl(), Checker, "没有break", categories::LogicError, "case语句不是以break结尾", ELoc);
			}
		}
	}
	if (case_stmt_count == 0 && !have_default_stmt)
	{
		//空Switch
		PathDiagnosticLocation ELoc(SS, BR.getSourceManager(), AC);
		BR.EmitBasicReport(AC->getDecl(), Checker, "空Switch", categories::LogicError, "switch语句为空", ELoc);
		return true;
	}
	if (!have_default_stmt)
	{
		//没有default
		PathDiagnosticLocation ELoc(SS, BR.getSourceManager(), AC);
		BR.EmitBasicReport(AC->getDecl(), Checker, "没有default", categories::LogicError, "没有default语句", ELoc);
		return true;
	}
	else if (case_stmt_count == 0)
	{
		//仅有default
		PathDiagnosticLocation ELoc(SS, BR.getSourceManager(), AC);
		BR.EmitBasicReport(AC->getDecl(), Checker, "没有case", categories::LogicError, "switch中没有case，只有default语句", ELoc);
		return true;
	}

	return true;
}


#if 0
int main(int argc, char *argv[])
{
	int a;
	scanf("%d", &a);

	if (a == 0)
	{
		printf("0\n");
	}
	else if (a < 0) //empty if
	{

	}
	else if (a >= 0 && a <= 1) //empty if
		;
	else if (a > 1) //missing else
	{
		if (a == 1) //empty if 
		{

		}
		else  //empty else
			;

		if (a == 2) //empty if
		{
			;
		}
		else    //empty else
		{

		}

		if (a == 3)
		{
			printf("3\n");
		}
		else    //empty else
		{
			;
		}

		if (a == 4) //empty if
		{
#if 0
			printf("4\n");
#endif
		}
		else if (a == 5)
		{
			printf("5\n");
		}
		else  //empty else
		{
#if 0
			printf("5\n");
#endif
		}
	}
}
#endif

bool ConditionalBranchVisitor::VisitIfStmt(const IfStmt *IFS)
{
	const Stmt *Then = IFS->getThen();
	const Stmt *Else = IFS->getElse();

	SourceLocation assignment_loc;
	if (conditionExprHasAssignment(IFS->getCond(), assignment_loc))
	{
		PathDiagnosticLocation ELoc(assignment_loc, BR.getSourceManager());
		BR.EmitBasicReport(AC->getDecl(), Checker, "条件赋值", categories::LogicError, "在条件表达式中找到赋值操作", ELoc,IFS->getCond()->getSourceRange());
	}

	if (isEmptyStmt(Then))
	{
		PathDiagnosticLocation ELoc(IFS, BR.getSourceManager(), AC);
		BR.EmitBasicReport(AC->getDecl(), Checker, "空if", categories::LogicError, "if分支为空", ELoc);
	}

	if (Else)
	{
		const IfStmt *ElseIf = dyn_cast_or_null<IfStmt>(Else);
		if (ElseIf)
		{
			//else if
			if (!ElseIf->getElse())
			{
				PathDiagnosticLocation ELoc(IFS->getElseLoc(), BR.getSourceManager());
				BR.EmitBasicReport(AC->getDecl(), Checker, "没有else", categories::LogicError, "else if之后没有else", ELoc);
			}
		}
		else
		{
			//else ; / else {}
			if (isEmptyStmt(Else))
			{
				PathDiagnosticLocation ELoc(IFS->getElseLoc(), BR.getSourceManager());
				BR.EmitBasicReport(AC->getDecl(), Checker, "空else", categories::LogicError, "else分支为空", ELoc);
			}
		}
	}
	return true;
}

bool ConditionalBranchVisitor::VisitConditionalOperator(const ConditionalOperator *CO)
{	
	SourceLocation assignment_loc;
	if (conditionExprHasAssignment(CO->getCond(), assignment_loc))
	{
		PathDiagnosticLocation ELoc(assignment_loc, BR.getSourceManager());
		BR.EmitBasicReport(AC->getDecl(), Checker, "条件赋值", categories::LogicError, "在条件表达式中找到赋值操作", ELoc, CO->getCond()->getSourceRange());
	}
	return true;
}




void ento::registerConditionalBranchChecker(CheckerManager &Mgr)
{
	Mgr.registerChecker<ConditionalBranchChecker>();
}

bool ento::shouldRegisterConditionalBranchChecker(const CheckerManager &Mgr)
{
	return true;
}