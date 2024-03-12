/*!
 * FileName: CheckScope.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2021-8-5
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 一些基本定义
 */
#include "CheckScope.h"

#ifndef BUILIT_IN_SCOPE_CHECKERS
extern "C" const char clang_analyzerAPIVersionString[] = CLANG_ANALYZER_API_VERSION_STRING;
#endif

namespace scope
{
	std::set<CheckerRegisterFactory *> CheckerRegisterFactory::RegFactorySet;

	CheckerRegisterFactory::CheckerRegisterFactory()
	{
		RegFactorySet.insert(this);
	}

	CheckerRegisterFactory::~CheckerRegisterFactory()
	{
		for (int i = 0; i < (int)fullNameCacheV.size(); i++)
		{
			delete fullNameCacheV[i];
		}
	}

	llvm::StringRef CheckerRegisterFactory::getCheckScopeNameStr(const std::string &checker_name)
	{
		llvm::StringRef res;
		std::string *full_name = new std::string("checkscope.core."); 
		if (full_name != NULL)
		{
			fullNameCacheV.push_back(full_name);
			full_name->append(checker_name);
			res = full_name->c_str(); //llvm::StringRef 只能引用一个字符串常量 所有只能用这样的办法了
		}
		return res;
	}


	llvm::StringRef CheckerRegisterFactory::getCheckScopeProbeNameStr(const std::string &checker_name)
	{
		llvm::StringRef res;
		std::string *full_name = new std::string("checkscope.probe.");
		if (full_name != NULL)
		{
			fullNameCacheV.push_back(full_name);
			full_name->append(checker_name);
			res = full_name->c_str(); //llvm::StringRef 只能引用一个字符串常量 所有只能用这样的办法了
		}
		return res;
	}


	llvm::StringRef CheckerRegisterFactory::getCheckScopeAlphaNameStr(const std::string &checker_name)
	{
		llvm::StringRef res;
		std::string *full_name = new std::string("checkscopealpha.alpha.");
		if (full_name != NULL)
		{
			fullNameCacheV.push_back(full_name);
			full_name->append(checker_name);
			res = full_name->c_str(); //llvm::StringRef 只能引用一个字符串常量 所有只能用这样的办法了
		}
		return res;
	}


	llvm::StringRef CheckerRegisterFactory::getCheckScopeDevNameStr(const std::string &checker_name)
	{
		llvm::StringRef res;
		std::string *full_name = new std::string("checkscopedev.dev.");
		if (full_name != NULL)
		{
			fullNameCacheV.push_back(full_name);
			full_name->append(checker_name);
			res = full_name->c_str(); //llvm::StringRef 只能引用一个字符串常量 所有只能用这样的办法了
		}
		return res;
	}

	void authCheckScopeSign(clang::ento::CheckerRegistry *CR)
	{
		if (CR != NULL && CR->getCheckScopeSignStr() == CHECK_SCOPE_SIGN_STR)
		{
			return;
		}
		exit(1);
	}

	static void _registerScopeCheckers(clang::ento::CheckerRegistry &registry)
	{
		std::set<scope::CheckerRegisterFactory *>::iterator iter = scope::CheckerRegisterFactory::RegFactorySet.begin();
		for (; iter != scope::CheckerRegisterFactory::RegFactorySet.end(); ++iter)
		{
			if ((*iter) != NULL)
			{
				(*iter)->registerCheckers(registry);
			}
		}
	}

	void registerScopeCheckers(clang::ento::CheckerRegistry &registry)
	{
#ifdef BUILIT_IN_SCOPE_CHECKERS
		scope::_registerScopeCheckers(registry);
#endif
	}
}

#ifndef BUILIT_IN_SCOPE_CHECKERS
void clang_registerCheckers(clang::ento::CheckerRegistry &registry)
{
	//对调用的clang进行身份认证 防止越权使用checker
	scope::authCheckScopeSign(&registry);
	//注册Checkers
	scope::_registerScopeCheckers(registry);
}
#endif
