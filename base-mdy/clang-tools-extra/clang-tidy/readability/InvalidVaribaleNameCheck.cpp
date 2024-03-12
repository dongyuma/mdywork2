//===--- InvalidVaribaleNameCheck.cpp - clang-tidy -------------------------===//
//
//
//   FileName: FunctionParametersCheck.cpp 
//   Author:   YuXinglin
//   Date:     2021-2-23
//   Code:     UTF-8 without BOM
//   Describe: 语句使用的检测
//   此checker完成关于
//    4.8.1.1 禁止单独使用小写字母“l”和大写字母“O”作为变量名
//    4.8.1.3 使用八进制数必须加以注释
//  的检测
//
//===----------------------------------------------------------------------===//
#include "InvalidVaribaleNameCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/SmallString.h"
#include <iostream>
#include <string> 

using namespace clang::ast_matchers;
using namespace std;

namespace clang {
namespace tidy {
namespace readability {

void InvalidVaribaleNameCheck::registerMatchers(MatchFinder *Finder) {
  // FIXME: Add matchers.
  //std::cout << "InvalidVaribaleNameCheck的registerMatchers方法，开始注册matcher" << std::endl;
  Finder->addMatcher(varDecl().bind("vardecl"), this);
  Finder->addMatcher(integerLiteral().bind("literal"), this);
}


void InvalidVaribaleNameCheck::check(const MatchFinder::MatchResult &Result) {
  /*
   * check for GJB-5369 4.8.1.1 禁止单独使用小写字母“l”和大写字母“O”作为变量名
   */
  if(const auto *MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("vardecl")){
	if(MatchedDecl->getName().equals("l")|| MatchedDecl->getName().equals("O")){
		diag(MatchedDecl->getLocation(), "变量名称不能声明小写字母“l”或大写字母“O”")<< MatchedDecl;
	}
  }
  /*
   * check for GJB-5369 4.8.1.3 使用八进制数必须加以注释
   */
  else if(const auto *MatchedintLiteral = Result.Nodes.getNodeAs<IntegerLiteral>("literal")){
	const Expr &Literal = *MatchedintLiteral;
	const auto &L = cast<IntegerLiteral>(Literal);
  	llvm::Optional<SourceRange> Range = L.getSourceRange();
	// Get the whole literal from the source buffer.
	const SourceManager &SM = *Result.SourceManager;
	const LangOptions &LO = getLangOpts();
	bool Invalid;
	const StringRef LiteralSourceText = Lexer::getSourceText(
		CharSourceRange::getTokenRange(*Range), SM, LO, &Invalid);
	// std::cout << LiteralSourceText.str() << std::endl;
	string str = LiteralSourceText.str();
// 	  for (unsigned i=0; i<str.length(); ++i)
//   {
//     std::cout << str.at(i);
//   }
  	// std::cout << std::endl;
  	SourceRange sg = L.getSourceRange();
	if(str.size() > 1 && str.at(0) == '0'){
		diag(sg.getBegin(), "使用八进制数必须加以注释");
	}
  }
  //std::cout << MatchedDecl->getName().str()<<" " <<MatchedDecl->getName().size() << std::endl;	
	/*if(MatchedDecl->getName().equals("??=") ||
	   MatchedDecl->getName().equals("??(") ||
	   MatchedDecl->getName().equals("??)") ||
	   MatchedDecl->getName().equals("??<") ||
	   MatchedDecl->getName().equals("??>") ||
	   MatchedDecl->getName().equals("??/") ||
	   MatchedDecl->getName().equals("??!") ||
	   MatchedDecl->getName().equals("??'") ||
	   MatchedDecl->getName().equals("??-") ){
		diag(MatchedDecl->getLocation(), "禁止三字母词的使用")
			<< MatchedDecl;
	}*/
  
}

} // namespace readability
} // namespace tidy
} // namespace clang
