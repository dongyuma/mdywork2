//===--- InitialValueCheck.h - clang-tidy -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_INITIALVALUECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_INITIALVALUECHECK_H

#include "../ClangTidyCheck.h"


using namespace std;

namespace clang {
namespace tidy {
namespace bz12 {

class InitialValueCheck : public ClangTidyCheck 
{
public:
    InitialValueCheck(StringRef Name, ClangTidyContext *Context) : ClangTidyCheck(Name, Context) {}
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
    void onEndOfTranslationUnit() override;

    void processInitListExpr(const InitListExpr* matchdedILE, vector<string> structElementName);
    void processInitListExpr(const InitListExpr* matchdedILE, string varName);

    map<string, pair<const Expr*, vector<int> > > arrayIntMap;
    map<string, pair<const Expr*, vector<double> > > arrayDoubleMap;

    map<string, vector<double>> expected_value_map;

    //map<string, int> varName_counter_map;
    vector<string> structElementName;

    set<string> targetVarNameSet;

    bool unRead = true;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_INITIALVALUECHECK_H
