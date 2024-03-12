//===--- AbsParameterCheck.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AbsParameterCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void AbsParameterCheck::registerMatchers(MatchFinder *Finder) 
{
	Finder->addMatcher(callExpr(callee(functionDecl(anyOf(hasName("abs"), hasName("fabs")) ) ) ).bind("callExpr"), this);
}

void AbsParameterCheck::check(const MatchFinder::MatchResult &Result) 
{
	const auto *MatchedCallExpr = Result.Nodes.getNodeAs<CallExpr>("callExpr");
	const auto *MatchedFunctionDecl = dyn_cast<FunctionDecl>(MatchedCallExpr->getCalleeDecl());
	std::string funcName = MatchedFunctionDecl->getDeclName().getAsString();

	//如果参数数量不为1，就报错
	if(MatchedCallExpr->getNumArgs() != 1)
	{
		diag(MatchedCallExpr->getBeginLoc(), "abs、fabs函数参数数量错误，请确认是否为标准库函数！");
		{
			return;
		}			
	}

	//忽略掉隐式转换
	const Expr *MatchedArg = (*(MatchedCallExpr->arg_begin()))->IgnoreParenImpCasts();
	//取真实类型，将typedef还原
	std::string typeStr = MatchedArg->getType().getCanonicalType().getAsString();

	//对abs函数进行处理
	if(funcName == "abs")
	{
		if(IntegerTypeSet.find(typeStr) == IntegerTypeSet.end())
		{
			if(unsignedIntegerTypeSet.find(typeStr) != unsignedIntegerTypeSet.end())
			{
				diag(MatchedArg->getBeginLoc(), "abs函数参数为无符号整型，请检查是否使用正确！");
			}
			else
			{
				diag(MatchedArg->getBeginLoc(), "abs函数参数不允许为非整型！");
			}
		}
	}
	//对fabs函数进行处理
	else
	{
		if(FloatTypeSet.find(typeStr) == FloatTypeSet.end())
		{
			diag(MatchedArg->getBeginLoc(), "fabs函数参数不允许为非浮点型！");
		}
	}
}

} // namespace bz12
} // namespace tidy
} // namespace clang

