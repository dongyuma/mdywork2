//===--- ArgumentsAndStructMembersSameNameCheck.cpp - clang-tidy ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ArgumentsAndStructMembersSameNameCheck.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/Preprocessor.h"
#include <unordered_map>

#include<vector>

std::vector<std::unordered_map<std::string, const clang::Token*>>  NameVec;
using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

namespace {
    class MacroArgumentsAndStructMembersSameNamePPCallbacks : public PPCallbacks {
    public:
        MacroArgumentsAndStructMembersSameNamePPCallbacks(Preprocessor* PP, ArgumentsAndStructMembersSameNameCheck* Check)
            : PP(PP), Check(Check) {}
        void MacroExpands(const Token& MacroNameTok,
            const MacroDefinition& MD,
            SourceRange Range,
            const MacroArgs* Args) override;
    private:
        
        Preprocessor* PP;
        ArgumentsAndStructMembersSameNameCheck* Check;
    };
} // namespace

void MacroArgumentsAndStructMembersSameNamePPCallbacks::MacroExpands(const Token& MacroNameTok,
    const MacroDefinition& MD, 
    SourceRange Range,
    const MacroArgs* Args)
{
    if (!MD.getLocalDirective())
    {
        return;
    }
    const MacroInfo* MI = MD.getLocalDirective()->getInfo();
    if (!MI)
    {
        return;
    }
    if (!MI->isFunctionLike())
    {
        return;
    }
    
    unsigned int ArgNumber = Args->getNumMacroArguments();
    std::unordered_map<std::string, const clang::Token*> TmpMap;
    for (unsigned int index = 0; index < ArgNumber; index++)
    {
        //const std::vector<clang::Token>& ArgT =
        const Token* ArgToken = Args->getUnexpArgument(index);
        if (ArgToken)
        {
            if (ArgToken->is(tok::identifier))
            {
                clang::IdentifierInfo* II = ArgToken->getIdentifierInfo();
                if (II)
                {
                    TmpMap.insert(std::make_pair(II->getName().str(), ArgToken));
                }
               
            }
        }
    }
    if (!TmpMap.empty())
    {
        NameVec.push_back(TmpMap);
    }
   
}

void ArgumentsAndStructMembersSameNameCheck::registerMatchers(ast_matchers::MatchFinder* Finder) 
{
    Finder->addMatcher(declRefExpr(to(varDecl().bind("varDecl")),
        unless(isExpansionInSystemHeader())), this);
}
//todo：由于宏的语句在语法树建立之前，因此在check里面匹配不到宏函数的调用，看到的只是宏函数展开后的样子
//但是代码里有宏函数的调用，因此报错位置产生了奇怪的显现，目前多个错误只能报出一个错误，暂时没有更好的方案
void ArgumentsAndStructMembersSameNameCheck::check(const ast_matchers::MatchFinder::MatchResult& Result) 
{
    if (const auto* MatchVarDecl = Result.Nodes.getNodeAs<VarDecl>("varDecl"))
    {
        if (MatchVarDecl->getType()->isStructureType())
        {
            IdentifierInfo* stu_II = MatchVarDecl->getIdentifier();
            if (!stu_II)
            {
                return;
            }
            std::vector<std::unordered_map<std::string, const clang::Token*>>::iterator  iter;
            for (iter = NameVec.begin(); iter != NameVec.end(); iter++)
            {
                if (iter->find(stu_II->getName().str())!= iter->end())
                {
                    break;
                }
                    
            }
            if (iter == NameVec.end())
            {
                return;
            }
            std::unordered_map<std::string, const clang::Token*> TmpMap = *iter;

            if(const auto* MatchRecordDecl = MatchVarDecl->getType()->getAsRecordDecl())
            {
               
                if (MatchRecordDecl->getDefinition())
                {
                    for (RecordDecl::field_iterator i = MatchRecordDecl->getDefinition()->field_begin(),
                        e = MatchRecordDecl->getDefinition()->field_end();
                        i != e; ++i)
                    {
                        IdentifierInfo* arg_II = (*i)->getIdentifier();
                        if (!arg_II)
                        {
                            return;
                        }
                        std::unordered_map<std::string, const clang::Token*>::iterator  map_iter = TmpMap.find(arg_II->getName().str());
                        if (map_iter != TmpMap.end())
                        {
                            diag((map_iter->second)->getLocation(), "实参与宏定义中结构体变量同名");
                            return;
                        }

                    }
                }
            }

                
        }
    }
       
    
}
void ArgumentsAndStructMembersSameNameCheck::registerPPCallbacks(
    const SourceManager& SM, Preprocessor* PP, Preprocessor* ModuleExpanderPP)
{
    PP->addPPCallbacks(std::make_unique<MacroArgumentsAndStructMembersSameNamePPCallbacks>(PP, this));
}


} // namespace bz12
} // namespace tidy
} // namespace clang
