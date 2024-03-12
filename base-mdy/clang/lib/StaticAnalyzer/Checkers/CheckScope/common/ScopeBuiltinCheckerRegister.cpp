/*!
 * FileName: ScopeBuiltinCheckerRegister.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2021-9-23
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: Windows下编译的clang 不能使用common/CheckScope中的代码(只要调用就段错误) 
 *           但使用该文件中的代码注册内置Checker就不崩溃 Linux下使用插件模式不用关心本文件
 */

#include "common/CheckScope.h"
#include "clang/StaticAnalyzer/Frontend/CheckerRegistry.h"

void registCheckScopeBuiltinCheckers(void *arg)
{
	if (!arg)
	{
		return;
	}
	clang::ento::CheckerRegistry *registry = (clang::ento::CheckerRegistry *)arg;
	scope::registerScopeCheckers(*registry);
}