//===--- LoopControlCheck.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LoopControlCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Analysis/Analyses/ExprMutationAnalyzer.h"
#include "../utils/Aliasing.h"
#include "iostream"
#include <string.h>
using namespace clang::ast_matchers;
using namespace std;

namespace clang {
namespace tidy {
namespace readability {

void LoopControlCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(forStmt().bind("loop-stmt"),this);                
}

void LoopControlCheck::check(const MatchFinder::MatchResult &Result) {
  if(const auto *forstmt = Result.Nodes.getNodeAs<ForStmt>("loop-stmt")){
    if(const auto *IncVar = forstmt->getInc()){
        //IncVar->dump();
        /*判断二元操作符
         *
         *   unsigned int global_int = 0;
         * 	 for(global_int=0; global_int<10; global_int=global_int+1){
	       *   	 j--;
	       *   }
         */
        if(isa<BinaryOperator>(IncVar)){
            if(const auto *DRE = dyn_cast<DeclRefExpr>(dyn_cast<BinaryOperator>(IncVar)->getLHS())){
              if (const auto *Var = dyn_cast<VarDecl>(DRE->getDecl())){
                /*
                  std::cout << "是局部声明的吗？" << Var->hasLocalStorage() << std::endl;
                  std::cout << "是局部声明的吗？" << Var->isLocalVarDecl() << std::endl;
                  std::cout << "是局部声明的吗？" << Var->hasGlobalStorage() << std::endl;*/
                  if(!Var->hasLocalStorage()){
                      diag("循环变量必须是局部声明的");
              }
            }
          }
        }
        /*  判断二元操作符
         *
         *   for(i = 0; j < 100; ++i){}
         */
        else if(isa<UnaryOperator>(IncVar)){
          if(const auto *DRE = dyn_cast<DeclRefExpr>(dyn_cast<UnaryOperator>(IncVar)->getSubExpr())){
            if (const auto *Var = dyn_cast<VarDecl>(DRE->getDecl())){
              /*
                std::cout << "是局部声明的吗？" << Var->hasLocalStorage() << std::endl;
                std::cout << "是局部声明的吗？" << Var->isLocalVarDecl() << std::endl;
                std::cout << "是局部声明的吗？" << Var->hasGlobalStorage() << std::endl;*/
                if(!Var->hasLocalStorage()){
                    diag("循环变量必须是局部声明的");
                }
            }
          }
        }  
    }
  }
}

} // namespace readability
} // namespace tidy
} // namespace clang
