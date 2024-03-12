//===--- InitialValueCheck.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "InitialValueCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "utils.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void InitialValueCheck::registerMatchers(MatchFinder *Finder) 
{
	Finder->addMatcher(varDecl().bind("varDecl"), this);
}

void InitialValueCheck::check(const MatchFinder::MatchResult &Result) 
{
	//读取配置文件，记录其中设定的变量名及其相应的预期值，并记录所有需要检查的变量、结构体名
	if(unRead)
	{
		unRead = false;
		read_expected_value_map(expected_value_map);
		for(auto iter=expected_value_map.begin(); iter!=expected_value_map.end(); iter++)
		{
			string varName = iter->first;
			//如果不是结构体相关变量那么就直接加到集合中
			if(varName.find(".") == string::npos)
			{
				targetVarNameSet.insert(varName);
			}
			//如果是结构体相关变量那么只加结构体名称到集合中
			else
			{
				targetVarNameSet.insert(varName.substr(0, varName.find(".")));
			}
		}
	}
	auto MatchedVarDecl = Result.Nodes.getNodeAs<VarDecl>("varDecl");
	string varName = MatchedVarDecl->getDeclName().getAsString();
	//如果变量存在初始化语句并且该变量名称在待查集合中，就对其进行处理
	if(MatchedVarDecl->hasInit() && (targetVarNameSet.find(varName) != targetVarNameSet.end()))
	{	
		//判断是否是结构体类型
		if(MatchedVarDecl->getType().getCanonicalType()->isRecordType())
		{
			//清空vector
			structElementName.clear();
			auto matchedRecordDecl = MatchedVarDecl->getType().getCanonicalType()->getAsRecordDecl();
			//遍历结构体元素，将名称存入vector
			for(auto iter=matchedRecordDecl->field_begin(); iter!=matchedRecordDecl->field_end(); iter++)
			{
				string elementName = varName + "." + iter->getDeclName().getAsString();
				structElementName.push_back(elementName);
			}
			if(const InitListExpr* matchdedILE = dyn_cast<InitListExpr>(MatchedVarDecl->getInit()->IgnoreParenCasts()))
			{	
				processInitListExpr(matchdedILE, structElementName);
			}
		}
		else if(const InitListExpr* matchdedILE = dyn_cast<InitListExpr>(MatchedVarDecl->getInit()->IgnoreParenCasts()))
		{
			processInitListExpr(matchdedILE, varName);
		}
	}
}

void InitialValueCheck::onEndOfTranslationUnit()
{  
	// for(auto iter=arrayDoubleMap.begin(); iter!=arrayDoubleMap.end(); iter++)
	// {
	// 	cout << "++++++++++++++++++++" << endl;
	// 	cout << "name:" << iter->first << endl;
	// 	for(auto subIter=iter->second.second.begin(); subIter!=iter->second.second.end(); subIter++)
	// 	{
	// 		cout << *subIter << endl;
	// 	}
	// }

	// for(auto iter=arrayIntMap.begin(); iter!=arrayIntMap.end(); iter++)
	// {
	// 	cout << "++++++++++++++++++++" << endl;
	// 	cout << "name:" << iter->first << endl;
	// 	for(auto subIter=iter->second.second.begin(); subIter!=iter->second.second.end(); subIter++)
	// 	{
	// 		cout << *subIter << endl;
	// 	}
	// }

	for(auto iter=expected_value_map.begin(); iter!=expected_value_map.end(); iter++)
	{
		if(arrayDoubleMap.find(iter->first) != arrayDoubleMap.end())
		{
			if(arrayDoubleMap[iter->first].second.size() == iter->second.size())
			{
				vector<int> indexVec;
				for(int i = 0; i < iter->second.size(); i++)
				{
					if(fabs(arrayDoubleMap[iter->first].second[i] - iter->second[i]) > (1E-8))
					{
						indexVec.push_back(i+1);
					}
				}
				if(indexVec.size() != 0)
				{
					string errorInfo;
					for(auto subIter=indexVec.begin(); subIter!=indexVec.end(); subIter++)
					{
						errorInfo += (to_string(*subIter) + "、");
					}
					errorInfo.pop_back();
					diag(arrayDoubleMap[iter->first].first->getBeginLoc(), "第%0个初始化值与用户设定预期值不匹配！") << errorInfo;
				}
			}	
			else
			{
				diag(arrayDoubleMap[iter->first].first->getBeginLoc(), "初始化值与用户设定预期值数量不匹配！");
			}
		}
		else if(arrayIntMap.find(iter->first) != arrayIntMap.end())
		{
			if(arrayIntMap[iter->first].second.size() == iter->second.size())
			{
				vector<int> indexVec;
				for(int i = 0; i < iter->second.size(); i++)
				{
					if(arrayIntMap[iter->first].second[i] != (int)iter->second[i])
					{
						indexVec.push_back(i+1);
					}
				}
				if(indexVec.size() != 0)
				{
					string errorInfo;
					for(auto subIter=indexVec.begin(); subIter!=indexVec.end(); subIter++)
					{
						errorInfo += (to_string(*subIter) + "、");
					}
					errorInfo.erase(errorInfo.length()-3, 3);
					//cout << "errorInfo:" << errorInfo << endl;
					diag(arrayIntMap[iter->first].first->getBeginLoc(), "第" + errorInfo + "个初始化值与用户设定预期值不匹配！");
				}
			}
			else
			{
				diag(arrayIntMap[iter->first].first->getBeginLoc(), "初始化值与用户设定预期值数量不匹配！");
			}
		}
		else
		{
			//todo报错(没找到对应名字的数组)
		}
	}
}

void InitialValueCheck::processInitListExpr(const InitListExpr* matchdedILE, vector<string> structElementName) 
{
	int counter = 0;
	for(auto iter=matchdedILE->child_begin(); iter!=matchdedILE->child_end(); iter++)
	{
		string varName = structElementName[counter];
		if(structElementName.size() <= counter)
		{
			return;
		}
		if(auto matchedExpr = dyn_cast<Expr>(*iter))
		{
			matchedExpr = matchedExpr->IgnoreParenCasts();
			if(auto ILE = dyn_cast<InitListExpr>(matchedExpr))
			{
				processInitListExpr(ILE, varName);
			}
			else if(auto IL = dyn_cast<IntegerLiteral>(matchedExpr))
			{
				//std::cout << "int:" << *(IL->getValue().getRawData()) << std::endl;
				arrayIntMap[varName].first = matchedExpr;
				arrayIntMap[varName].second.push_back(*(IL->getValue().getRawData()));
			}
			else if(auto FL = dyn_cast<FloatingLiteral>(matchedExpr))
			{
				//std::cout << "double:" << FL->getValueAsApproximateDouble() << std::endl;
				arrayIntMap[varName].first = matchedExpr;
				arrayIntMap[varName].second.push_back(FL->getValueAsApproximateDouble());
			}
			else
			{
				//doNothing
			}
		}
		counter++;
	}
}

void InitialValueCheck::processInitListExpr(const InitListExpr* matchdedILE, string varName) 
{
	for(auto iter=matchdedILE->child_begin(); iter!=matchdedILE->child_end(); iter++)
	{
		if(auto matchedExpr = dyn_cast<Expr>(*iter))
		{
			matchedExpr = matchedExpr->IgnoreParenCasts();
			if(auto ILE = dyn_cast<InitListExpr>(matchedExpr))
			{
				processInitListExpr(ILE, varName);
			}
			else if(auto IL = dyn_cast<IntegerLiteral>(matchedExpr))
			{
				//std::cout << "int:" << *(IL->getValue().getRawData()) << std::endl;
				arrayIntMap[varName].first = matchedExpr;
				arrayIntMap[varName].second.push_back(*(IL->getValue().getRawData()));
			}
			else if(auto FL = dyn_cast<FloatingLiteral>(matchedExpr))
			{
				//std::cout << "double:" << FL->getValueAsApproximateDouble() << std::endl;
				arrayIntMap[varName].first = matchedExpr;
				arrayIntMap[varName].second.push_back(FL->getValueAsApproximateDouble());
			}
			else
			{
				//doNothing
			}
		}			
	}
}

} // namespace bz12
} // namespace tidy
} // namespace clang
