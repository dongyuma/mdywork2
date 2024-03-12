/*!
 * FileName: ScopeFileDescriptorChecker.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2021-8-9
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 *
 * Describe: 文件描述符Checker
 */
//检测能力
/*
1. 检测unix文件描述符泄漏
2. 检测unix文件描述符多次close

但是目前还有一个致命缺陷，因为Unix文件描述符是个整数类型，和ISO C的FILE *类型(指针)不同
整数类型没有像指针类型那样的checkPointerEscape能力，即当在不同的源文件内实现了文件描述符的close
此时又是按值传递将文件描述符传递给翻译单元外的函数，在外部函数中将其close() 这种场景还比较常见，
对于整数类型的Escape，CAS没有相应的基础能力，不能将传出去的文件描述符设置为未知，所以对于Unix
的整型文件描述符，就不能可靠地对其进行Leak检测了！

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int myclose(int fd)
{
	close(fd);
	return 0;
}
void leakTest()
{
	int fd = open("1111111", O_CREAT | O_RDWR);
	if (fd < 0)
	{
		return;
	}
}//Detect file desc leak
void doubleCloseTest()
{
	int fd = open("1111111", O_CREAT | O_RDWR);
	if (fd < 0)
	{
		return;
	}
	myclose(fd);
	int a = fd;
	close(a);  //Detect double close
}

int main(int argc, char *argv[])
{
	return 0;
}
*/


#include "common/CheckScope.h"
#include "common/ScopeExprCmp.h"

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTNodeTraverser.h"
#include "clang/AST/ParentMap.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporterVisitors.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h"
#include "llvm/ADT/SmallString.h"

using namespace clang;
using namespace ento;

namespace
{

	class ScopeFileDescriptorChecker;
	struct FileDesc;

	typedef std::function<void(const ScopeFileDescriptorChecker *, const FileDesc *, const CallEvent &, CheckerContext &)> FileDescCheckPtr;

	struct FileDesc
	{
		FileDescCheckPtr PreFn;
		FileDescCheckPtr EvalFn;
	};

	class FileDescState
	{
	private:
		enum Kind
		{
			Opened,
			Closed
		} K;

		FileDescState(Kind _K) :K(_K) {}

	public:

		static FileDescState getOpened() 
		{
			return FileDescState(Opened);
		}

		static FileDescState getClosed()
		{
			return FileDescState(Closed);
		}

		bool isOpened() const
		{
			return K == Opened;
		}

		bool isClosed() const
		{
			return K == Closed;
		}

		bool operator==(const FileDescState &X) const
		{
			return X.K == K;
		}

		void Profile(llvm::FoldingSetNodeID &ID) const
		{
			ID.AddInteger(K);
		}
	};


	class ScopeFileDescriptorChecker :public Checker< check::PostCall, check::PreCall, check::DeadSymbols >
	{
	public:
		void checkPostCall(const CallEvent &Call, CheckerContext &C) const;
		void checkPreCall(const CallEvent &Call, CheckerContext &C) const;
		void checkDeadSymbols(SymbolReaper &SR, CheckerContext &C) const;

	private:
		void evalOpen(const FileDesc *FD, const CallEvent &CE, CheckerContext &C) const;
		void preClose(const FileDesc *FD, const CallEvent &CE, CheckerContext &C) const;


		mutable std::unique_ptr<BugType> LeakBugType;
		mutable std::unique_ptr<BugType> DoubleCloseBugType;

		//************************************
		// Method:    ensureFileDescNonNegative
		// FullName:  ScopeFileDescriptorChecker::ensureFileDescNonNegative
		// Access:    private 
		// Returns:   clang::ento::ConstraintManager::ProgramStatePair
		// Qualifier: const 确保文件描述符为非负的
		// Parameter: SymbolRef fdSymRef
		// Parameter: ProgramStateRef PSR
		// Parameter: CheckerContext & C
		//************************************
		ConstraintManager::ProgramStatePair ensureFileDescNonNegative(SymbolRef fdSymRef, ProgramStateRef PSR,CheckerContext &C) const;

		const FileDesc *lookupFileDesc(const CallEvent &Call) const 
		{
			//不是全局的函数可能是由用户重写了 不进行检查
			if (!Call.isGlobalCFunction())
			{
				return nullptr;
			}
			return FileDescMap.lookup(Call);
		}

		CallDescriptionMap<FileDesc> FileDescMap =
		{
			{{"creat"},{nullptr,&ScopeFileDescriptorChecker::evalOpen}},
			{{"open"},{nullptr,&ScopeFileDescriptorChecker::evalOpen}},
			{{"openat"},{nullptr,&ScopeFileDescriptorChecker::evalOpen}},
			{{"fopen"},{nullptr,&ScopeFileDescriptorChecker::evalOpen}},
			//{{"dup"},{nullptr,&ScopeFileDescriptorChecker::evalOpen}},
			//{{"dup2"},{&ScopeFileDescriptorChecker::preClose,&ScopeFileDescriptorChecker::evalOpen}},
			//增加对CSR-146的支持，使用时开启此check即可
			{{"fclose"},{&ScopeFileDescriptorChecker::preClose,nullptr}},
			{{"close"},{&ScopeFileDescriptorChecker::preClose,nullptr}}
		};
	};
}

//创建文件描述符 状态<->符号 映射
REGISTER_MAP_WITH_PROGRAMSTATE(FileDescStateMap, SymbolRef, FileDescState)


clang::ento::ConstraintManager::ProgramStatePair ScopeFileDescriptorChecker::ensureFileDescNonNegative(SymbolRef fdSymRef, ProgramStateRef PSR, CheckerContext &C) const
{
	SValBuilder &SVB = C.getSValBuilder();
	SVal V = SVB.makeSymbolVal(fdSymRef);
	ConstraintManager &CM = PSR->getConstraintManager();
	Optional<DefinedSVal> DV = V.getAs<DefinedSVal>();
	ConstraintManager::ProgramStatePair res;
	if (DV)
	{
		SVal zeroVal = SVB.makeIntVal(0, false);
		//构造符号表达式
		SVal CondSVal = SVB.evalBinOp(PSR, BinaryOperatorKind::BO_GE, *DV, zeroVal, SVB.getConditionType());
		Optional<DefinedSVal> FDV = CondSVal.getAs<DefinedSVal>();
		if (FDV)
		{
			res = CM.assumeDual(PSR, *FDV);
		}
	}
	return res;
}

void ScopeFileDescriptorChecker::checkPostCall(const CallEvent &Call, CheckerContext &C) const
{
	const FileDesc *Desc = lookupFileDesc(Call);
	if (!Desc || !Desc->EvalFn)
	{
		return;
	}
	Desc->EvalFn(this, Desc, Call, C);
}

void ScopeFileDescriptorChecker::checkPreCall(const CallEvent &Call, CheckerContext &C) const
{
	const FileDesc *Desc = lookupFileDesc(Call);
	if (!Desc || !Desc->PreFn)
	{
		return;
	}
	Desc->PreFn(this, Desc, Call, C);
}

void ScopeFileDescriptorChecker::evalOpen(const FileDesc *FD, const CallEvent &CE, CheckerContext &C) const
{
	//打开的文件描述符创建状态
	ProgramStateRef PSR = C.getState();
	SymbolRef SR = CE.getReturnValue().getAsSymbol();
	if (!SR)
	{
		return;
	}

	PSR = PSR->set<FileDescStateMap>(SR, FileDescState::getOpened());
	C.addTransition(PSR);
}

void ScopeFileDescriptorChecker::checkDeadSymbols(SymbolReaper &SR, CheckerContext &C) const
{
	ProgramStateRef PSR = C.getState();
	const FileDescStateMapTy &DeadFileDescSymbol = PSR->get<FileDescStateMap>();
	SmallVector<SymbolRef, 2> LeakSymbolV;
	for (FileDescStateMapTy::iterator I = DeadFileDescSymbol.begin(), E = DeadFileDescSymbol.end(); I != E; ++I)
	{
		SymbolRef symRef = I->first;
		bool isDeadSym = SR.isDead(symRef);

		if (isDeadSym)
		{
			if (I->second.isOpened())
			{
				//Dead时文件描述符是>=0的 则判定为描述符泄漏
				ConstraintManager::ProgramStatePair ret = ensureFileDescNonNegative(symRef, PSR, C);
				if (ret.first)
				{
					LeakSymbolV.push_back(symRef);
				}
			}

			PSR = PSR->remove<FileDescStateMap>(symRef);
		}
	}

	if (!LeakSymbolV.empty())
	{
		std::cerr << "Dead opened !!!!!!!" << LeakSymbolV.size() << "\n";

		for (SymbolRef Leaked : LeakSymbolV) 
		{
			if (!LeakBugType)
			{
				LeakBugType.reset(new BugType(this, "Resource Leak", categories::LogicError, true));
			}
			auto R = std::make_unique<PathSensitiveBugReport>(*LeakBugType, "打开的文件描述符从未关闭,可能存在泄漏", C.generateNonFatalErrorNode(PSR));
			R->markInteresting(Leaked);
			C.emitReport(std::move(R));
		}
		
	}
	C.addTransition(PSR);
}

void ScopeFileDescriptorChecker::preClose(const FileDesc *FD, const CallEvent &CE, CheckerContext &C) const
{
	//CE.dump();
	ProgramStateRef PSR = C.getState();
	SymbolRef SR = CE.getArgSVal(0).getAsSymbol();
	if (!SR)
	{
		return;
	}
	const FileDescState *FDState = PSR->get<FileDescStateMap>(SR);
	if (!FDState)
	{
		return;
	}
	if (FDState->isClosed())
	{
		ConstraintManager::ProgramStatePair ret = ensureFileDescNonNegative(SR, PSR, C);
		if (ret.first)
		{
			//非负的文件描述符多次关闭
			if (!DoubleCloseBugType)
			{
				DoubleCloseBugType.reset(new BugType(this, "Double close", categories::LogicError, true));
			}
			auto R = std::make_unique<PathSensitiveBugReport>(*DoubleCloseBugType, "多次关闭已关闭的文件描述符!", C.generateNonFatalErrorNode(PSR));
			R->addRange(CE.getSourceRange());
			R->markInteresting(SR);
			C.emitReport(std::move(R));
			return;
		}
	}

	PSR = PSR->set<FileDescStateMap>(SR,FileDescState::getClosed());
	C.addTransition(PSR);
}



class ScopeFileDescriptorCheckerRegister :public scope::CheckerRegisterFactory
{
public:
	virtual void registerCheckers(clang::ento::CheckerRegistry &registry)
	{
		//声明要用到的checker名称(这样可以降低内存开销)
		llvm::StringRef CheckBase = getCheckScopeAlphaNameStr("FileDescriptior");

		//添加Checker
		registry.addChecker<ScopeFileDescriptorChecker>(CheckBase,
			"Detect file descriptor leaks", NOT_CHECKER_DOC, NOT_HIDDEN_CHECKER);
	}
};
SCOPE_REGISTER_CLASS(ScopeFileDescriptorCheckerRegister)
