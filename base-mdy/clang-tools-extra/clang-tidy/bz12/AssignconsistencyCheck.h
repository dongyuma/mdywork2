//===--- AssignconsistencyCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_ASSIGNCONSISTENCYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_ASSIGNCONSISTENCYCHECK_H

#include "../ClangTidyCheck.h"
#include <fstream>
#include <vector>
#include <string>
namespace clang {
namespace tidy {
namespace bz12 {

/// FIXME: Write a short description.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/bz12-AssignConsistency.html
/// 用于检测语句块内部赋值一致性错误
class AssignconsistencyCheck : public ClangTidyCheck {
public:
    AssignconsistencyCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {}
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
    std::string getInterLiteralExprString(const BinaryOperator *BO) const;
    std::string getArraySubscriptExprString(const ArraySubscriptExpr * ASE) const;
    std::string getMemberExprString(const MemberExpr * ME) const;
    std::string getDeclRefExprString(const DeclRefExpr * DRE) const;
    void checkAssignconsistency(const CompoundStmt *CPS);
    void rcheckAssignconsistency(const CompoundStmt *CPS);
    void TraverseStmt(const Stmt *S, SourceLocation SL) ;
    void rTraverseStmt(const Stmt *S, SourceLocation SL) ;
    void rSwitchCheck(const BinaryOperator * BO)const;
    void read_negative_string_vector(std::vector<std::string> & negative_string_vector);
    bool case_insensitive_find_in_negative_vector(std::string key_word, std::vector<std::string> vec_str, std::size_t &index);
    std::string to_lower_string(std::string str);
    
    std::string getRHSName(const BinaryOperator *BO) ;
    std::multiset<std::string> KeyArrayName;
    std::multiset<std::string> ArrayName;
    std::map<std::string, SourceLocation> KeyLocation;

    std::multiset<std::string> rKeyArrayName;
    std::multiset<std::string> rArrayName;
    std::map<std::string, SourceLocation> rKeyLocation;

    //Used to store reverse filtering strings
    std::vector<std::string> negative_string_vector;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_ASSIGNCONSISTENCYCHECK_H
