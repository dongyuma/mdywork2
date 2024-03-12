/*!
 * FileName: CheckScope.h
 *
 * Author:   ZhangChaoZe
 * Date:     2021-8-5
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 一些基础定义
 */
#ifndef _CHECK_SCOPE_H_
#define _CHECK_SCOPE_H_
#include <set>
#include <vector>

#include "clang/StaticAnalyzer/Frontend/CheckerRegistry.h"

#define HIDDEN_CHECKER true
#define NOT_HIDDEN_CHECKER false
#define NOT_CHECKER_DOC ""
#define DEV_STAGE "released"

#ifndef BUILIT_IN_SCOPE_CHECKERS
extern "C"
{
	void clang_registerCheckers(clang::ento::CheckerRegistry &registry);
}
#endif

namespace scope
{
	//-------------------------------------
	// Checker注册工厂类
	// 所有Checker都必须从该类派生出一个注册类用于注册Checker
	// 该类为抽象类，所有子类必须实现registerCheckers函数,并在其中实现注册逻辑
	// 派生类同时还应提供一个全局的对象实例用于触发Checker的注册
	// 派生类命名规范: CheckerCppFileNameRegister    CheckerCppFile为实现该Checker的源文件名称
	// 派生类全局对象实例命名规范: CheckerCppFileName  _CheckerCppFile为实现该Checker的源文件名称
	//------------------------------------
	class CheckerRegisterFactory
	{
	public:
		static std::set<CheckerRegisterFactory *> RegFactorySet;
	public:
		CheckerRegisterFactory();
		virtual ~CheckerRegisterFactory();

		//************************************
		// Method:    registerCheckers
		// FullName:  scope::CheckerRegisterFactory::registerCheckers
		// Access:    virtual public 
		// Returns:   void
		// Qualifier: 纯虚函数请在派生类中实现注册逻辑 
		//            可参照ScopeBadShiftCheckerRegister中的实现(含依赖关系)
		// Parameter: clang::ento::CheckerRegistry & registry
		//************************************
		virtual void registerCheckers(clang::ento::CheckerRegistry &registry) = 0;
		CheckerRegisterFactory(const CheckerRegisterFactory &) = delete;
		CheckerRegisterFactory &operator=(const CheckerRegisterFactory &) = delete;

		llvm::StringRef getCheckScopeNameStr(const std::string &checker_name);
		llvm::StringRef getCheckScopeProbeNameStr(const std::string &checker_name);
		llvm::StringRef getCheckScopeAlphaNameStr(const std::string &checker_name);
		llvm::StringRef getCheckScopeDevNameStr(const std::string &checker_name);

		virtual bool shouldRegister()
		{
			return true;
		}

	private:
		std::vector<std::string *> fullNameCacheV;
	};

	void authCheckScopeSign(clang::ento::CheckerRegistry *CR);

	void registerScopeCheckers(clang::ento::CheckerRegistry &registry);
}

#define SCOPE_REGISTER_CLASS(ClassName)  \
ClassName _##ClassName;					 \
bool shouldRegister##ClassName(){		 \
	return _##ClassName.shouldRegister();\
}

#endif