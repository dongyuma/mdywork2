//===--- BranchAssignmentDifferenceCheck.h - clang-tidy ---------------*- C++ -*-===//
//
//该checker用于检查使用if-else以及switch-case的不同分支中是否存在对同一个变量赋同样值的情况。
//
//目前检查内容包括：
//将各个块中所有赋值表达式的左侧变量名以及右侧元素使用map进行记录，检查是否存在相同的情况。
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BRANCHASSIGNMENTDIFFERENCECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BRANCHASSIGNMENTDIFFERENCECHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bz12 {

class BranchAssignmentDifferenceCheck : public ClangTidyCheck 
{
public:
    BranchAssignmentDifferenceCheck(StringRef Name, ClangTidyContext *Context)
        : ClangTidyCheck(Name, Context) {}
    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

    void traverseIfBranch(const IfStmt* matchedIfStmt);
    void traverseSwitchBody(const Stmt *switchBody);
    void analysisCompoundStmt(const Stmt* matchedStmt);
    void analysisAssignmentDifference();
    void analysisResult(const Expr *lastAssignment, const Expr *currAssignment);
    void travelExpr(const Expr* matchedExpr, std::vector<std::string> &exprStrVec);
    std::vector<Expr*> handlingBinaryOperator(const BinaryOperator *CircleBinaryOperator, std::vector<std::string> &exprStrVec);
    std::string getVarName(const Expr* MatchedExpr);

    std::map<std::string, std::vector<const Expr *>> var_Rhs_Map;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_BLOCKCONSISTENCYCHECK_H
