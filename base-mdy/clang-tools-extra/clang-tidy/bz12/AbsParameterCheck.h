//===--- AbsParameterCheck.h - clang-tidy -----------------------*- C++ -*-===//
//
//该checker用于检查abs、fabs函数参数类型的正确性。

//目前检查内容包括：
//(1) 参数数量是否为1，如果不为1则报错提醒检查是否为标准库函数，且不进行后续检查；
//(2) abs函数的参数是否为IntegerTypeSet中存在的类型；
//(3) fabs函数的参数是否为FloatTypeSet中存在的类型。
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_ABSPARAMETERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_ABSPARAMETERCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bz12 {

class AbsParameterCheck : public ClangTidyCheck 
{
public:
	AbsParameterCheck(StringRef Name, ClangTidyContext *Context): ClangTidyCheck(Name, Context) {}
	void registerMatchers(ast_matchers::MatchFinder *Finder) override;
	void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
	
	//有符号整型类型集合(在语法树中signed int就是int，short int就是short，所以不用因此扩展该集合)
	std::set<std::string> IntegerTypeSet = {"int", "short", "long", "long long", "char"};
	//无符号整型类型集合
	std::set<std::string> unsignedIntegerTypeSet = {"unsigned int", "unsigned short", "unsigned long", "unsigned long long", "unsigned char"};
	//浮点类型集合
	std::set<std::string> FloatTypeSet = {"float", "double", "long double"};
};

} // namespace bz12
} // namespace tidy
} // namespace clangunsigned

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BZ12_ABSPARAMETERCHECK_H
