//===--- NestedCommentCheck.cpp - clang-tidy ------------------------------===//
//
//
//   FileName: NestedCommentCheck.cpp 
//   Author:   YuXinglin
//   Date:     2021-3-4
//   Code:     UTF-8 without BOM
//   Describe: 禁止使用嵌套注释
//   此checker完成关于  4.10.1.1 禁止使用嵌套注释的检测
//  
//
//===----------------------------------------------------------------------===//

#include "NestedCommentCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "iostream"
#include <string>

using namespace clang::ast_matchers;
using namespace std;

namespace clang {
namespace tidy {
namespace readability {

class NestedCommentCheck::NestedCommentHandler : public CommentHandler {
public:
  NestedCommentHandler(NestedCommentCheck &Check)
      : Check(Check){}

  bool HandleComment(Preprocessor &PP, SourceRange Range) override {
    StringRef Text =
        Lexer::getSourceText(CharSourceRange::getCharRange(Range),
                             PP.getSourceManager(), PP.getLangOpts());
    // std::cout << Text.str() << std::endl;
    string text = Text.str();
    int flag = 0, length = text.length();
    // std::cout << length << std::endl;
    // for(auto x : text){
    //   std::cout << x;
    // }
    // std::cout << std::endl;
    for(int i = 1; i < length; i++){
      if(text.at(i-1) == '/' && text.at(i) == '*')  flag++;
      if(flag>1){
        Check.diag(Range.getBegin(), "禁止使用嵌套注释");
      }
      if(text.at(i-1) == '*' && text.at(i) == '/')  flag--;
    }
    return false;
  }

private:
  NestedCommentCheck &Check;
};

NestedCommentCheck::NestedCommentCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      Handler(std::make_unique<NestedCommentHandler>(*this)) {}

NestedCommentCheck::~NestedCommentCheck() = default;

void NestedCommentCheck::registerPPCallbacks(const SourceManager &SM,
                                           Preprocessor *PP,
                                           Preprocessor *ModuleExpanderPP) {
  PP->addCommentHandler(Handler.get());
}

} // namespace readability
} // namespace tidy
} // namespace clang
