//===--- FunctionParametersCheck.cpp - clang-tidy -------------------------===//
//
//
//   FileName: FunctionParametersCheck.cpp 
//   Author:   YuXinglin
//   Date:     2021-2-21
//   Code:     UTF-8 without BOM
//   Describe: 名称、符号与变量的使用中关于函数参数名称的检测
//   此checker完成关于
//    4.15.1.1 禁止枚举类型中的元素名与已有变量名同名
//    4.15.1.3 禁止形参名与全局变量同名
//    4.15.1.4 禁止形参名与类型或标识符同名
//  的检测
//
//===----------------------------------------------------------------------===//

#include "FunctionParametersCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "VariablenameDuplicationCheck.h"
#include <iostream>
#include <string> 

using namespace clang::ast_matchers;
using namespace std;

namespace clang {
namespace tidy {
namespace readability {

void FunctionParametersCheck::registerMatchers(MatchFinder *Finder) {
  //GJB是针对C语言的，不用匹配using语法，只匹配语法typedef即可
  Finder->addMatcher(typedefDecl().bind("typedefDecl"), this);
  Finder->addMatcher(varDecl(unless(parmVarDecl())).bind("vardecl"), this);
  Finder->addMatcher(parmVarDecl().bind("parmVarDecl"), this);
  Finder->addMatcher(enumConstantDecl().bind("enumConstantDecl"), this);
}

void FunctionParametersCheck::check(const MatchFinder::MatchResult &Result) {
  //匹配到非函数参数的变量，如果是全局变量就保存
  if(const auto *MatchedVarDecl = Result.Nodes.getNodeAs<VarDecl>("vardecl")){
    if(MatchedVarDecl->hasGlobalStorage()){
      std::string VarName = MatchedVarDecl->getName().str();
      globalVariblenameSet.insert(VarName);
    }
  }
  else if(const auto *MatchedparmVarDecl = Result.Nodes.getNodeAs<ParmVarDecl>("parmVarDecl")){
    std::string currparmVarname = MatchedparmVarDecl->getName().str();
    //std::cout << currparmVarname<< std::endl;
    /*
     * check for GJB-5369 4.15.1.3 禁止形参名与全局变量同名
     */
    if(globalVariblenameSet.count(currparmVarname) != 0){
        diag(MatchedparmVarDecl->getLocation(), "禁止形参名与全局变量同名") << MatchedparmVarDecl;   
        return;
    }
    /*
     * check for GJB-5369 4.15.1.4 禁止形参名与类型或标识符同名
     */
    if(identifiernameSet.count(currparmVarname) != 0){
        diag(MatchedparmVarDecl->getLocation(), "禁止形参名与类型或标识符同名") << MatchedparmVarDecl;   
        return;
    }

  }else if(const auto *MatchedtypedefDecl = Result.Nodes.getNodeAs<TypedefDecl>("typedefDecl")){
    //std::cout << MatchedtypedefDecl->getName().str() << std::endl;
    identifiernameSet.insert(MatchedtypedefDecl->getName().str());
  }else if(const auto *MatchedenumConstantDecl = Result.Nodes.getNodeAs<EnumConstantDecl>("enumConstantDecl")){
    std::string currenumVarname = MatchedenumConstantDecl->getName().str();
    // std::cout << currenumVarname<< std::endl;
    /*
     * check for GJB-5369 4.15.1.1 禁止枚举类型中的元素名与已有变量名同名
     */
    if(globalVariblenameSet.count(currenumVarname) != 0){
        diag(MatchedenumConstantDecl->getLocation(), "禁止枚举型中的元素名与已有变量名同名") << MatchedenumConstantDecl;   
        return;
    }
  }
}
} // namespace readability
} // namespace tidy
} // namespace clang
#if 0
typedef int INT;
unsigned int global_int = 0;
void static_p(unsigned int *global_int){}
void static_func(unsigned int INT){}
void func_17(){
	//int p;
	enum Name_type{e1, global_int} EnumVar;
	EnumVar=e1;
	int k;
	int O;
	{int O;
	int j;}
}
#endif