//===--- UnusedStructElementCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UnusedStructElementCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void UnusedStructElementCheck::registerMatchers(MatchFinder *Finder) 
{	
	//匹配结构体定义(因为CxxRecordDecl继承自RecordDecl,所以只匹配RecordDecl即可，但是RecordDecl只有一种，而CxxRecordDecl有两种，所以要加限制条件：有fieldDecl()子孙)
	Finder->addMatcher(recordDecl(unless(isExpansionInSystemHeader()), hasDescendant(fieldDecl())).bind("recordDecl"), this);
	//匹配结构体根节点
	Finder->addMatcher(memberExpr(unless(anyOf(isExpansionInSystemHeader(), hasAncestor(memberExpr())))).bind("rootMemberExpr"), this);
	//匹配结构体(非根节点）
	Finder->addMatcher(memberExpr(unless(isExpansionInSystemHeader()), hasAncestor(memberExpr())).bind("memberExpr"), this);
	//匹配结构体中的DeclRef
	Finder->addMatcher(declRefExpr(unless(isExpansionInSystemHeader()), hasAncestor(memberExpr())).bind("declRefExpr"), this);
	//匹配Typedef结构体类型
	Finder->addMatcher(typedefDecl(unless(isExpansionInSystemHeader()), hasDescendant(recordType())).bind("typedefDecl"), this);
}

void UnusedStructElementCheck::check(const MatchFinder::MatchResult &Result) 
{
	//如果匹配到的是Typedef结构体类型，则将两种类型存入map
	if(const auto *MatchedTypedefDecl = Result.Nodes.getNodeAs<TypedefDecl>("typedefDecl"))
	{
		if(const ElaboratedType *ET = dyn_cast<ElaboratedType>(MatchedTypedefDecl->getUnderlyingType()))
		{
			typedefCorrespondMap[removeUselessSubstr(MatchedTypedefDecl->getDeclName().getAsString())] = removeUselessSubstr(ET->getOwnedTagDecl()->getNameAsString());
		}
	}

  	//如果匹配到的是结构体定义，则将整个结构体节点存入集合
	else if(const auto *MatchedRecordDecl = Result.Nodes.getNodeAs<RecordDecl>("recordDecl"))
	{
		defStructSet.insert(MatchedRecordDecl);
	}

	//如果匹配到根结构体,则更新当前节点
	else if(const MemberExpr *MatchedRootMemberExpr = Result.Nodes.getNodeAs<MemberExpr>("rootMemberExpr"))
	{
		currnetNode = MatchedRootMemberExpr;
	}

	//如果匹配到非根结构体，取结构体类型作为map的key，上一mem节点的结构体元素名作为value
	else if(const auto *MatchedMemberExpr = Result.Nodes.getNodeAs<MemberExpr>("memberExpr"))
	{
		usedStructElementNameMap[exchangStructType(MatchedMemberExpr->getMemberDecl()->getType().getAsString())].insert(currnetNode->getMemberDecl()->getDeclName().getAsString());
		currnetNode = MatchedMemberExpr;
	}

	//如果匹配到DeclRefExpr，取类型作为map的key，上一mem节点的结构体元素名作为value
	else if(const auto *MatchedDeclRefExpr = Result.Nodes.getNodeAs<DeclRefExpr>("declRefExpr"))
	{
		usedStructElementNameMap[exchangStructType(MatchedDeclRefExpr->getType().getAsString())].insert(currnetNode->getMemberDecl()->getDeclName().getAsString());
	}
}

void UnusedStructElementCheck::onEndOfTranslationUnit() 
{
	for(auto iter=defStructSet.begin(); iter!=defStructSet.end(); iter++)
	{
    	string currentStructName = (*iter)->getDeclName().getAsString();
		//如果整个结构体被使用了，就继续找有没有元素未被使用，否则直接报错
		if(usedStructElementNameMap.find(currentStructName) != usedStructElementNameMap.end())
		{
			for(auto iterField=(*iter)->field_begin(); iterField!=(*iter)->field_end(); iterField++)
			{
				string currentStructElementName = (*iterField)->getDeclName().getAsString();
				if(usedStructElementNameMap[currentStructName].find(currentStructElementName) == usedStructElementNameMap[currentStructName].end())
				{
					diag((*iterField)->getBeginLoc(),"该结构体元素未被使用");
				}
			}
		}
		else
		{
			diag((*iter)->getBeginLoc(),"该结构体未被使用");
		}
  	}
}

//删去字符串中的无用信息
string UnusedStructElementCheck::removeUselessSubstr(string targetStr)
{
	for(vector<string>::iterator iter=uselessSubstr.begin(); iter!=uselessSubstr.end(); iter++)
	{
		int pos = 0;
		while (pos < (int)targetStr.length()) 
		{
			if(*iter == targetStr.substr(pos, iter->length())) 
			{
				targetStr = targetStr.erase(pos, iter->length());
			}
			else
			{
				pos++;
			}
		}
	}
	//对数组型,如[3]进行单独处理
	int startPos = 0;
	int endPos = 0;
	bool flag = true;
	for(int i=0; i<(int)targetStr.length(); i++)
	{
		if(targetStr[i] == '[' && flag)
		{
			startPos = i;
			flag = false;
		}
		else if(targetStr[i] == ']')
		{
			endPos = i;
		}
	}
	if(!flag)
	{
		targetStr = targetStr.erase(startPos, (endPos-startPos+1));
	}
	return targetStr;
}

//将typedef的结构体类型换为原类型
string UnusedStructElementCheck::exchangStructType(string targetStructType)
{
	targetStructType = removeUselessSubstr(targetStructType);
	//如果是typedef的则替换，否则返回原值
	if(typedefCorrespondMap.find(targetStructType) != typedefCorrespondMap.end())
	{
		return typedefCorrespondMap[targetStructType];
	}
	else
	{
		return targetStructType;
	}
}

} // namespace bz12
} // namespace tidy
} // namespace clang