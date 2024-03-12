//===--- MatrixCalcParametersCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MatrixCalcParametersCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void MatrixCalcParametersCheck::registerMatchers(MatchFinder *Finder) 
{
	Finder->addMatcher(callExpr(unless(isExpansionInSystemHeader())).bind("callExpr"), this);
}

void MatrixCalcParametersCheck::check(const MatchFinder::MatchResult &Result) 
{
	const auto *MatchedCallExpr = Result.Nodes.getNodeAs<CallExpr>("callExpr");
	std::string funcName;

	if(!MatchedCallExpr->getCalleeDecl())
	{
		return;
	}

	if(isa<FunctionDecl>(MatchedCallExpr->getCalleeDecl()))
	{
		const auto *MatchedFunctionDecl = dyn_cast<FunctionDecl>(MatchedCallExpr->getCalleeDecl());
		funcName = MatchedFunctionDecl->getDeclName().getAsString();

		//Mat_MulDouble(real64* 计算结果(M*P)， const real64* 第一个矩阵(M*N)，const real64* 第二个矩阵(N*P)，byte M,N,P)
		if(funcName == "Mat_MulDouble")
		{
			if(MatchedCallExpr->getNumArgs() != 6)
			{
				diag(MatchedCallExpr->getBeginLoc(), "函数参数数量错误，请确认该函数是否为重用构件！");
				return;
			}

			processParameters(*(MatchedCallExpr->arg_begin()), MP, 1);
			processParameters(*(MatchedCallExpr->arg_begin()+1), MN, 1);
			processParameters(*(MatchedCallExpr->arg_begin()+2), NP, 1);
			processParameters(*(MatchedCallExpr->arg_begin()+3), M);
			processParameters(*(MatchedCallExpr->arg_begin()+4), N);
			processParameters(*(MatchedCallExpr->arg_begin()+5), P);
			
			if(!checkFlag)
			{
				checkFlag = true;
				return;
			}
			
			if((M*N)!=MN || (M*P)!=MP || (N*P)!=NP)
			{
				diag(MatchedCallExpr->getBeginLoc(), "数组长度与矩阵大小参数不匹配！");
			}
		}
		//Mat_RotDouble(real64* 计算结果(N*M), const real64* 原矩阵(M*N)，byte M,N)
		else if(funcName == "Mat_RotDouble")
		{
			if(MatchedCallExpr->getNumArgs() != 4)
			{
				diag(MatchedCallExpr->getBeginLoc(), "函数参数数量错误，请确认该函数是否为重用构件！");
				return;
			}

			processParameters(*(MatchedCallExpr->arg_begin()), NM, 1);
			processParameters(*(MatchedCallExpr->arg_begin()+1), MN, 1);
			processParameters(*(MatchedCallExpr->arg_begin()+2), M);
			processParameters(*(MatchedCallExpr->arg_begin()+3), N);

			if(!checkFlag)
			{
				checkFlag = true;
				return;
			}
			
			if((M*N)!=MN || (M*N)!=NM)
			{
				diag(MatchedCallExpr->getBeginLoc(), "数组长度与矩阵大小参数不匹配！");
			}
		}
		//Mat_InvDouble(real64* 计算结果(N*N), const real64* 原矩阵(N*N)，byte N)
		else if(funcName == "Mat_InvDouble")
		{
			if(MatchedCallExpr->getNumArgs() != 3)
			{
				diag(MatchedCallExpr->getBeginLoc(), "函数参数数量错误，请确认该函数是否为重用构件！");
				return;
			}

			processParameters(*(MatchedCallExpr->arg_begin()), NN, 1);
			processParameters(*(MatchedCallExpr->arg_begin()+1), MN, 1); //此处应为NN，因与上面冲突，所以借用MN
			processParameters(*(MatchedCallExpr->arg_begin()+2), N);

			if(!checkFlag)
			{
				checkFlag = true;
				return;
			}
			
			if((N*N)!=NN || (N*N)!=MN)
			{
				diag(MatchedCallExpr->getBeginLoc(), "数组长度与矩阵大小参数不匹配！");
			}
		}
		//Mat_AddDouble(real64* 计算结果(M*N)， const real64* 第一个矩阵(M*N)，const real64* 第二个矩阵(M*N)，byte M,N)
		//Mat_SubDouble(real64* 计算结果(M*N)， const real64* 第一个矩阵(M*N)，const real64* 第二个矩阵(M*N)，byte M,N)
		else if(funcName == "Mat_AddDouble" || funcName == "Mat_SubDouble")
		{
			if(MatchedCallExpr->getNumArgs() != 5)
			{
				diag(MatchedCallExpr->getBeginLoc(), "函数参数数量错误，请确认该函数是否为重用构件！");
				return;
			}

			processParameters(*(MatchedCallExpr->arg_begin()), MN, 1);
			processParameters(*(MatchedCallExpr->arg_begin()+1), NN, 1); //此处应为MN，因与上面冲突，所以借用NN
			processParameters(*(MatchedCallExpr->arg_begin()+2), MP, 1); //此处应为MN，因与上面冲突，所以借用MP
			processParameters(*(MatchedCallExpr->arg_begin()+3), M);
			processParameters(*(MatchedCallExpr->arg_begin()+4), N);

			if(!checkFlag)
			{
				checkFlag = true;
				return;
			}
			
			if((M*N)!=MN || (M*N)!=NN || (M*N)!=MP)
			{
				diag(MatchedCallExpr->getBeginLoc(), "数组长度与矩阵大小参数不匹配！");
			}
		}
		//MakeMatI(real64* 矩阵（N*N），unsigned short N）
		else if(funcName == "MakeMatI")
		{
			if(MatchedCallExpr->getNumArgs() != 2)
			{
				diag(MatchedCallExpr->getBeginLoc(), "函数参数数量错误，请确认该函数是否为重用构件！");
				return;
			}

			processParameters(*(MatchedCallExpr->arg_begin()), NN, 1);
			processParameters(*(MatchedCallExpr->arg_begin()+1), N);

			if(!checkFlag)
			{
				checkFlag = true;
				return;
			}
			
			if((N*N) != NN)
			{
				diag(MatchedCallExpr->getBeginLoc(), "数组长度与矩阵大小参数不匹配！");
			}
		}
		else
		{
			//doNothing
		}
	}
}

//输入：参数表达式、要计算的值
void MatrixCalcParametersCheck::processParameters(const Expr *ParaExpr, llvm::APInt &num)
{
	if(checkFlag)
	{
		//取矩阵行\列数
		if(isa<IntegerLiteral>(ParaExpr->IgnoreParenImpCasts()))
		{
			const auto *MatchedIntegerLiteral = dyn_cast<IntegerLiteral>(ParaExpr->IgnoreParenImpCasts());
			num = MatchedIntegerLiteral->getValue();
		}
		//如果有其他情况，说明与重用构件格式不匹配，报错，并且针对该次调用不再进行进一步检查与报错
		else
		{
			diag(ParaExpr->getExprLoc(), "参数类型与重用构件格式不匹配，请确认该函数是否为重用构件！");
			checkFlag = false;
		}
	}
}

//输入：参数表达式、要计算的值
void MatrixCalcParametersCheck::processParameters(const Expr *ParaExpr, llvm::APInt &num, bool arrayFlag)
{
	if(checkFlag)
	{
		//取数组长度
		if(isa<ConstantArrayType>(ParaExpr->IgnoreParenImpCasts()->getType()))
		{
			const auto *MatchedConstantArrayType = dyn_cast<ConstantArrayType>(ParaExpr->IgnoreParenImpCasts()->getType());
			num = MatchedConstantArrayType->getSize();
		}
		//如果有其他情况，说明与重用构件格式不匹配，报错，并且针对该次调用不再进行进一步检查与报错
		else
		{
			diag(ParaExpr->getExprLoc(), "参数类型与重用构件格式不匹配，请确认该函数是否为重用构件！");
			checkFlag = false;
		}
	}
}

} // namespace bz12
} // namespace tidy
} // namespace clang
