/*!
 * FileName: ScopeSValCmp.h
 *
 * Author:   ZhangChaoZe
 * Date:     2021-8-12
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 符号比较相关
 */

#ifndef _SCOPE_SVAL_CMP_H_
#define _SCOPE_SVAL_CMP_H_

#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

namespace scope
{
	//************************************
	// Method:    isNegativeSval
	// FullName:  scope::isNegativeSval
	// Access:    public 
	// Returns:   clang::ento::ConstraintManager::ProgramStatePair isNegitative,isNotNegative 返回一对值 为负,不为负  
	//            false,false-未知  false,true-不为负 true,false-为负 true,true-可能为负 false,false-未知
	// Qualifier: 本函数会判断V是否为负数(不判断是否为ConcreteInt)
	// Parameter: clang::ento::SVal V
	// Parameter: clang::ento::CheckerContext & C
	//************************************
	clang::ento::ConstraintManager::ProgramStatePair isNegativeSval(clang::ento::SVal V, clang::ento::CheckerContext &C);

	//************************************
	// Method:    isConcreteNegativeSval
	// FullName:  scope::isConcreteNegativeSval
	// Access:    public 
	// Returns:   clang::ento::ConstraintManager::ProgramStatePair isNegitative,isNotNegative 返回一对值 为负,不为负 
	//            false,false-未知  false,true-不为负 true,false-为负 true,true-可能为负 false,false-未知
	// Qualifier: 本函数会判断V是否为ConcreteInt且是否为负，如果不是直接返回false false
	// Parameter: SVal V
	// Parameter: CheckerContext & C
	//************************************
	clang::ento::ConstraintManager::ProgramStatePair isConcreteNegativeSval(clang::ento::SVal V, clang::ento::CheckerContext &C);

	//************************************
	// Method:    getSValCmp
	// FullName:  scope::getSValCmp
	// Access:    public 
	// Returns:   clang::ento::ConstraintManager::ProgramStatePair 返回一对值 为负,不为负  
	//            false,false-未知  false,true-不为负 true,false-为负 true,true-可能为负 false,false-未知
	// Qualifier: 对两个SVal进行比较操作 注: V1 V2的值的类型须一致(如: int-int uint-uint)
	// Parameter: clang::ento::SVal V1
	// Parameter: clang::BinaryOperatorKind operator_kind
	// Parameter: clang::ento::SVal V2
	// Parameter: clang::ento::CheckerContext & C
	//************************************
	clang::ento::ConstraintManager::ProgramStatePair getSValCmp(clang::ento::SVal V1, clang::BinaryOperatorKind operator_kind, clang::ento::SVal V2, clang::ento::CheckerContext &C);


	//************************************
	// Method:    getConcreteIntCmp
	// FullName:  scope::getConcreteIntCmp
	// Access:    public 
	// Returns:   clang::ento::ConstraintManager::ProgramStatePair false,true-不成立 true,false-成立 true,true-可能成立 false,false-未知 
	// Qualifier: 本函数会判断V1 V2是否为ConcreteInt，如果不是直接返回false false
	// Parameter: clang::ento::SVal V1
	// Parameter: clang::BinaryOperatorKind operator_kind
	// Parameter: clang::ento::SVal V2
	// Parameter: clang::ento::CheckerContext & C
	//************************************
	clang::ento::ConstraintManager::ProgramStatePair getConcreteIntCmp(clang::ento::SVal V1, clang::BinaryOperatorKind operator_kind, clang::ento::SVal V2, clang::ento::CheckerContext &C);
}
#endif