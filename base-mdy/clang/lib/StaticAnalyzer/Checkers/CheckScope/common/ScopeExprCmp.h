/*!
 * FileName: ScopeExprCmp.h
 *
 * Author:   ZhangChaoZe
 * Date:     2021-7-29
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: checkscope通用表达式比较函数
 */

#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

namespace scope 
{
	//************************************
	// Method:    isNegativeExpr
	// FullName:  scope::isNegativeExpr
	// Access:    public 
	// Returns:   clang::ento::ConstraintManager::ProgramStatePair isNegitative,isNotNegative 返回一对值 为负,不为负  
	//            false,false-未知  false,true-不为负 true,false-为负 true,true-可能为负 false,false-未知
	// Qualifier:
	// Parameter: Expr * E
	// Parameter: CheckerContext & C
	//************************************
	clang::ento::ConstraintManager::ProgramStatePair isNegativeExpr(const clang::Expr *E, clang::ento::CheckerContext &C);


	//************************************
	// Method:    getExprCmpInt
	// FullName:  scope::getExprCmpInt
	// Access:    public 
	// Returns:   clang::ento::ConstraintManager::ProgramStatePair false,true-不成立 true,false-成立 true,true-可能成立 false,false-未知
	// Qualifier:
	// Parameter: Expr * E
	// Parameter: BinaryOperatorKind operator_kind
	// Parameter: uint64_t integer
	// Parameter: CheckerContext & C
	//************************************
	clang::ento::ConstraintManager::ProgramStatePair getExprCmpInt(const clang::Expr *E, clang::BinaryOperatorKind operator_kind, uint64_t integer, clang::ento::CheckerContext &C);
}