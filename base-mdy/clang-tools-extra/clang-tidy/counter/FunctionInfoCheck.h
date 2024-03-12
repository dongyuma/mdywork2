//===--- FunctionInfoCheck.h - clang-tidy -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_COUNTER_FUNCTIONINFOCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_COUNTER_FUNCTIONINFOCHECK_H

#include "CounterTidyModule.h"
#include "../ClangTidyCheck.h"
#include "llvm/Support/jsoncpp.h"
#include "llvm/Support/FileSystem.h"

namespace clang {
namespace tidy {
namespace counter {

/// 1. 统计源文件中函数最大行数、最小行数 
/// 2. 源文件中函数数量
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/counter-function-info.html
class FunctionInfoCheck : public ClangTidyCheck {
public:
    FunctionInfoCheck(StringRef Name, ClangTidyContext *Context);
  ~FunctionInfoCheck();
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;
  void checkFunctionInfo(const FunctionDecl *FD, const ast_matchers::MatchFinder::MatchResult &Result);
  void checkFunctionCyclomaticComplexity(const std::string funSginature, const FunctionDecl *FD, const ast_matchers::MatchFinder::MatchResult &Result);
  void checkFunctionFanOut(const std::string funSginature, const FunctionDecl *FD, const ast_matchers::MatchFinder::MatchResult &Result);

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) override;

private:
    bool skipCheck;   //true-跳过该check

    std::string countInfoFilePath;          //源码信息统计文件

	unsigned int functionsNumber;   //函数数量
    std::string maxLinesFunName;    //最大行数函数名称
	unsigned int maxLines;          //最大函数行数
    std::string minLinesFunName;    //最小行数函数名称
	unsigned int minLines;          //最小函数行数
    unsigned int commentLines;      //注释行数(含块注释中的空行)
    unsigned int commentBlanks;     //块注释中的空行数
    unsigned int blanks;            //空行数(不含块注释中的空行)

    //************************************
    // Method:    getFunctionSginatureString
    // FullName:  clang::tidy::counter::FunctionInfoCheck::getFunctionSginatureString
    // Access:    private 
    // Returns:   std::string
    // Qualifier: 获取函数签名字符串(返回类型 函数名称(参数列表))
    // Parameter: const FunctionDecl * FD
    //************************************
    std::string getFunctionSginatureString(const FunctionDecl *FD);


    class FunctionCountInfo
    {
    public:
        FunctionCountInfo()
        {
            cyclomatic_complexity = 0;
            fan_out = 0;
        }

        unsigned int cyclomatic_complexity; //圈复杂度
        unsigned int fan_out;               //扇出


    };

	std::map<std::string, FunctionCountInfo> FunCountMap;

    class CommentsLinesCountHandler;
	friend class CommentsLinesCountHandler;
	std::unique_ptr<CommentsLinesCountHandler> Handler;
};

} // namespace counter
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_COUNTER_FUNCTIONINFOCHECK_H
