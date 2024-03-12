//===--- DifferentFunctionDeclarationAndDefinitionCheck.cpp - clang-tidy --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DifferentFunctionDeclarationAndDefinitionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"


using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void DifferentFunctionDeclarationAndDefinitionCheck::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  Finder->addMatcher(functionDecl(unless(isDefinition())).bind("functionDecl"), this);
  Finder->addMatcher(functionDecl(isDefinition()).bind("functionDefinition"), this);
}

void DifferentFunctionDeclarationAndDefinitionCheck::check(const MatchFinder::MatchResult &Result) {
  // FIXME: Add callback implementation.
	if(const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("functionDecl"))
	{
		vDecl.push_back(MatchedDecl);
	}
	if (const auto* MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("functionDefinition"))
	{
		vDef.push_back(MatchedDecl);
	}

}
void DifferentFunctionDeclarationAndDefinitionCheck::onEndOfTranslationUnit()
{
	if (!vDecl.empty()&&!vDef.empty())
	{
		std::vector<const FunctionDecl*>::iterator decl_itr = vDecl.begin();
		for (; decl_itr != vDecl.end(); decl_itr++)
		{
			if (!*decl_itr)
			{
				continue;
			}
			if (!(*decl_itr)->getDefinition())
			{
				const auto decl_II = (*decl_itr)->getIdentifier();
				if (decl_II)
				{
					std::vector<const FunctionDecl*>::iterator def_itr = vDef.begin();
					for (; def_itr != vDef.end(); def_itr++)
					{
						const auto def_II = (*def_itr)->getIdentifier();
						if (def_II)
						{
							if (def_II->getName() == decl_II->getName())
							{
								diag((*def_itr)->getLocation(), "函数的定义和声明不一致");
							}
						}

					}
				}
			}
		}
	}
}


} // namespace bz12
} // namespace tidy
} // namespace clang
