/*!
 * FileName: AssignNegativeToUnsignedNumberChecker.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2021-2-1
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 * 
 * 
 * Modified: Yuxinglin
 * Date: 2021-11-19
 * Describe: 检测GJB8114 R-1-13-15 禁止给无符号变量赋负值
 * 修改误报太多的bug
 * 
 * Modified: LuMingYin
 * Date: 2022-8-19
 * Describe: 检测GJB8114 R-1-13-15 禁止给无符号变量赋负值
 * 精简checker
 */

#include <iostream>
#include <set>

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTNodeTraverser.h"
#include "clang/AST/ParentMap.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

using namespace clang;
using namespace ento;
namespace
{
	class AssignNegativeToUnsignedNumberChecker :public Checker<check::PreStmt<ImplicitCastExpr>>
	{
	public:
		void checkPreStmt(const ImplicitCastExpr *Cast, CheckerContext &C) const;
	};
}

void AssignNegativeToUnsignedNumberChecker::checkPreStmt(const ImplicitCastExpr *Cast, CheckerContext &C) const
{
	//获取AnalysisDeclContext
	AnalysisDeclContext *AC = C.getCurrentAnalysisDeclContext();
	//获取ASTContext
	ASTContext &ASC = C.getASTContext();
	//获取ParentMap
	const ParentMap &PM = C.getLocationContext()->getParentMap();
	//获取Cast的父节点
	const Stmt *Parent = PM.getParent(Cast);
	//如果不在主文件中
	if (!C.getSourceManager().isInMainFile(Cast->getBeginLoc()))
	{
		return;
	}
	//如果父节点不存在
	if (!Parent)
	{
		return;
	}
	//获取Cast转换前类型
	QualType IgnoImpType = ASC.getCanonicalType(Cast->IgnoreImpCasts()->getType());
	//获取Cast转换后类型
	QualType CastType = ASC.getCanonicalType(Cast->getType());
	//如果转换前类型是有符号数或枚举类型
	if (IgnoImpType->isSignedIntegerOrEnumerationType())
	{
		//定义复数检查标志
		bool checkNegative = false;
		//尝试将父节点转换为BinaryOperator
		const BinaryOperator *BO = dyn_cast_or_null<BinaryOperator>(Parent);
		//尝试将父节点转换为DeclStmt
		const DeclStmt *DS = dyn_cast_or_null<DeclStmt>(Parent);
		//如果父节点转换BinaryOperator成功且为赋值符号
		if (BO != NULL && BO->isAssignmentOp())
		{
			//如果BinaryOperator是无符号类型(也就是等式左边是无符号类型)
			if (ASC.getCanonicalType(BO->getType())->isUnsignedIntegerType())
			{
				//检查标志置为true
				checkNegative = true;
			}
		}
		//父节点是DeclStmt
		else if (DS != NULL)
		{
			//寻找VarDecl类型
			for (const auto *I : DS->decls())
			{
				if (const VarDecl *VD = dyn_cast<VarDecl>(I))
				{
					const Expr *Init = VD->getInit();
					if (Init != NULL)
					{
						if (CastType->isUnsignedIntegerType())
						{
							//声明一个无符号整型变量但使用了有符号整型进行初始化
							checkNegative = true;
						}
					}
				}
			}
		}
		//如果需要检查
		if (checkNegative)
		{
			//约束求解器求解
			if (C.isNegative(Cast->IgnoreImpCasts()))
			{
				PathDiagnosticLocation ELoc(Cast->getBeginLoc(), C.getSourceManager());
				//报错
				C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "溢出标志", categories::SecurityError, "为无符号变量赋负值", ELoc, Parent->getSourceRange());
				return;
			}
		}
	}
}

void ento::registerAssignNegativeToUnsignedNumberChecker(CheckerManager &Mgr)
{
	Mgr.registerChecker<AssignNegativeToUnsignedNumberChecker>();
}

bool ento::shouldRegisterAssignNegativeToUnsignedNumberChecker(const CheckerManager &Mgr)
{
	return true;
}
