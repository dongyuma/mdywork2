//===--- VariablenameDuplicationCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "VariablenameDuplicationCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <iostream>
#include <string> 

using namespace clang::ast_matchers;
using namespace std;

namespace clang {
namespace tidy {
namespace readability {

void VariablenameDuplicationCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl().bind("vardecl"), this);
}

void VariablenameDuplicationCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("vardecl");
  
  if (!MatchedDecl || !MatchedDecl->getIdentifier())
   	return;
  string VarName = MatchedDecl->getName().str();
  //std::cout << currvarname<< std::endl;	

  /*
   *  4.15.1.2 禁止局部变量与全局变量同名
   */
  if(MatchedDecl->hasGlobalStorage()){
    if(localvariablenameSet.count(VarName) != 0){
      diag(MatchedDecl->getLocation(), "禁止局部变量与全局变量同名") << MatchedDecl; 
        return; 
    }
  }else if(MatchedDecl->hasLocalStorage()){
    if(globalVariblenameSet.count(VarName) != 0){
      diag(MatchedDecl->getLocation(), "禁止局部变量与全局变量同名") << MatchedDecl; 
        return; 
    }
  }
  /*
   *  4.15.1.5 禁止在内部块中重新定义已有的变量名
   */
  if(MatchedDecl->hasLocalStorage()){
    if(localvariablenameSet.count(VarName) != 0){
      diag(MatchedDecl->getLocation(), "禁止在内部块中重新定义已有的变量名") << MatchedDecl;   
      return;
    }
  }
  if(MatchedDecl->hasLocalStorage()) localvariablenameSet.insert(VarName);
  if(MatchedDecl->hasGlobalStorage()) globalVariblenameSet.insert(VarName);
  // std::cout << "------------------------------------" << std::endl;
  // std::cout << "当前全局变量" << std::endl;
  // for(int i = 0; i < currglobalvariablenum; i++){
  //   std::cout << globalvariablename[i] << std::endl;
  // }
  // std::cout << "------------------------------------" << std::endl;
  // std::cout << "当前局部变量" << std::endl;
  // for(int i = 0; i < currlocalvariablenum; i++){
  //   std::cout << localvariablename[i] << std::endl;
  // }
  // std::cout << "------------------------------------" << std::endl;
  
  // std::cout << currvarname<< std::endl;	
  //std::cout << "有局部变量存储:"<< MatchedDecl->hasLocalStorage() << std::endl;	
  //std::cout << "有全部变量存储:"<< MatchedDecl->hasGlobalStorage() << std::endl;	//Returns true for all variables that do not have local storage.
  //std::cout << "是静态变量:"<< MatchedDecl->isStaticLocal() << std::endl;	
  
}
} // namespace readability
} // namespace tidy
} // namespace clang
