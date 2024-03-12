//===--- MatrixCalcParametersCheck.h - clang-tidy ---------------*- C++ -*-===//
//
//该checker用于检查使用重用构件进行矩阵计算时，输入参数的正确性。

//输入参数包含以下类型：
//矩阵以一维数组形式传入；
//矩阵行\列数为以常数或宏定义形式传入，暂不考虑其为变量的情况。

//目前检查内容包括：
//(1) 参数数量及类型是否符合重用构件格式；
//(2) 矩阵大小是否与对应的数组长度一致。
//PS:根据以上两点，如果有矩阵行\列数为0，一定能报出来，所以不用再单独考虑
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_MATRIXCALCPARAMETERSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_MATRIXCALCPARAMETERSCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bz12 {

class MatrixCalcParametersCheck : public ClangTidyCheck 
{
public:
	MatrixCalcParametersCheck(StringRef Name, ClangTidyContext *Context) : ClangTidyCheck(Name, Context) {}
  	void registerMatchers(ast_matchers::MatchFinder *Finder) override;
 	void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
	//通过参数获取矩阵行\列数
	void processParameters(const Expr *ParaExpr, llvm::APInt &num);
	//通过参数获取数组长度
	void processParameters(const Expr *ParaExpr, llvm::APInt &num, bool arrayFlag);

	//用于记录矩阵大小及数组长度
	llvm::APInt M, N, P, MP, MN, NP, NM, NN;
	bool checkFlag = true;
};

} // namespace bz12
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_MATRIXCALCPARAMETERSCHECK_H
