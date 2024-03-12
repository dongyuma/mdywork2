//===--- UninitializedStructElementCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UninitializedStructElementCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void UninitializedStructElementCheck::registerMatchers(MatchFinder *Finder) 
{
	//匹配变量定义
	Finder->addMatcher(varDecl(unless(isExpansionInSystemHeader())).bind("varDecl"), this);
	//匹配赋值操作
	Finder->addMatcher(binaryOperator(unless(isExpansionInSystemHeader()), isAssignmentOperator()).bind("binaryOperator"), this);
	//匹配Cpp下的结构体赋值
	Finder->addMatcher(cxxOperatorCallExpr(unless(isExpansionInSystemHeader()), isAssignmentOperator()).bind("cxxOperatorCallExpr"), this);
	//匹配memcpy函数
	Finder->addMatcher(callExpr(unless(isExpansionInSystemHeader()), callee(functionDecl(hasName("memcpy")))).bind("callExpr"), this);
}

void UninitializedStructElementCheck::check(const MatchFinder::MatchResult &Result) 
{
	//如果匹配到变量定义
	if(const auto *MatchedVarDecl = Result.Nodes.getNodeAs<VarDecl>("varDecl"))
	{
        //如果没有初始化(注意CallInit在cpp中也算没有初始化)，并且不是ParmVarDecl，则记录该变量节点，否则不用处理
		if(( (!MatchedVarDecl->hasInit()) || (MatchedVarDecl->getInitStyle() == VarDecl::CallInit) ) && (!isa<ParmVarDecl>(MatchedVarDecl)) )
		{
			varDeclSet.insert(MatchedVarDecl);
        }
    }

	//如果匹配到了赋值语句，则记录左值
	else if(const auto *MatchedBinaryOperator = Result.Nodes.getNodeAs<BinaryOperator>("binaryOperator"))
	{
		if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(MatchedBinaryOperator->getLHS()->IgnoreParenImpCasts()))
		{
			declRefNameSet.insert(DRE->getDecl()->getNameAsString());
		}
    }

	//如果匹配到了Cpp赋值，则记录左值(第二个子节点为左值)
	else if(const auto *MatchedCxxOperatorCallExpr = Result.Nodes.getNodeAs<CXXOperatorCallExpr>("cxxOperatorCallExpr"))
	{
		if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(*(++MatchedCxxOperatorCallExpr->child_begin())))
		{
			declRefNameSet.insert(DRE->getDecl()->getNameAsString());
		}
    }

	//如果匹配到memcpy函数，则记录第一个参数
	else if(const auto *MatchedCallExpr = Result.Nodes.getNodeAs<CallExpr>("callExpr"))
	{
		//对于第一个参数，由于可能存在强制转换或者'&'一元操作符，所以要处理一下
		auto MatchedNode = (*MatchedCallExpr->arg_begin())->IgnoreParenImpCasts();
		if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(MatchedNode))
		{
			declRefNameSet.insert(DRE->getDecl()->getNameAsString());
		}
		else if(const UnaryOperator *UO = dyn_cast<UnaryOperator>(MatchedNode))
		{	
			if(const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(UO->getSubExpr()))
			{
				declRefNameSet.insert(DRE->getDecl()->getNameAsString());
			}
		}
	}
}

void UninitializedStructElementCheck::onEndOfTranslationUnit() 
{
	for(auto iter=varDeclSet.begin(); iter!=varDeclSet.end(); iter++)
	{
		string varDeclName = (*iter)->getNameAsString();
		if(declRefNameSet.find(varDeclName) == declRefNameSet.end())
		{
			diag((*iter)->getBeginLoc(),"该结构体/变量未被初始化");
		}
	}
}

} // namespace bz12
} // namespace tidy
} // namespace clang