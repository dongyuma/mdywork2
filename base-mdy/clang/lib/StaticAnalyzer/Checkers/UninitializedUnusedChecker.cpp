//#include <iostream>
#include "Taint.h"
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h" 
#include "clang/StaticAnalyzer/Core/PathSensitive/DynamicExtent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ExprEngine.h"


using namespace clang;
using namespace ento;
using namespace std;
using namespace taint;
namespace
{
	class UninitializedUnusedChecker : public Checker<check::PostStmt<DeclStmt>,
		check::Location,
		check::EndAnalysis,
		check::PostCall>
	{
		mutable std::unique_ptr<BuiltinBug> BT;
		void reportBug(const char* Msg, CheckerContext& C) const;
	public:
		void checkPostStmt(const DeclStmt* DS, CheckerContext& C) const;
		void checkLocation(SVal Loc, bool IsLoad, const Stmt* S, CheckerContext& C) const;
		void checkEndAnalysis(ExplodedGraph& G, BugReporter& BR, ExprEngine& Eng) const;
		void checkPostCall(const CallEvent& Call, CheckerContext& C) const;
		bool checkState(CheckerContext& C, const MemRegion* MR) const;
		void checkUninitedUse(CheckerContext& C, const MemRegion* MR, ProgramStateRef State) const;
	};
} // end anonymous namespace

enum varState
{
	inited,
	unInited,
	used,
	unUsed //目前没用到
};

//用于记录报错需要的节点
//map<const MemRegion *, ExplodedNode *> nodeMap;

REGISTER_MAP_WITH_PROGRAMSTATE(varStateMap, const MemRegion*, int) //此处将类型定义为int，实际使用时用enum，可以保证编译通过

//标记memset、memcpy
CallDescription funcMemset("memset"), funcMemcpy("memcpy");


//对变量声明进行处理
void UninitializedUnusedChecker::checkPostStmt(const DeclStmt* DS, CheckerContext& C) const
{
	if (C.getSourceManager().isInSystemHeader(DS->getBeginLoc()))
	{
		return;
	}
	ProgramStateRef State = C.getState();
	for (auto iter = DS->decl_begin(); iter != DS->decl_end(); iter++)
	{
		if (const VarDecl* VD = dyn_cast<VarDecl>(*iter))
		{
			Loc varLoc = State->getLValue(VD, C.getLocationContext());
			const MemRegion* MR = varLoc.getAsRegion();
			if (!MR)
			{
				if (!VD->hasInit())
				{
					reportBug("未初始化!", C);
				}
				return;
			}
			//如果变量进行了初始化，将其状态置为已初始化
			if (VD->hasInit())
			{
				if(VD->getInitStyle() == VarDecl::CInit)
				{
					State = State->set<varStateMap>(MR, inited);
					C.addTransition(State);
				}
				else
				{
					State = State->set<varStateMap>(MR, unInited);
					C.addTransition(State);					
				}
				
			}
			//如果变量未初始化，将其状态置为未初始化
			else
			{
				State = State->set<varStateMap>(MR, unInited);
				C.addTransition(State);
			}

		}
	}
}

void UninitializedUnusedChecker::checkLocation(SVal Loc, bool IsLoad, const Stmt* S, CheckerContext& C) const
{
	if (S)
	{
		if (C.getSourceManager().isInSystemHeader(S->getBeginLoc()))
		{
			return;
		}
	}
		
	//如果是存储操作,说明对变量进行了初始化或者重新赋值，由于重新赋值后正常情况应该会再次使用，
	//所以两种情况下均将变量状态置为已初始化
	if (!IsLoad)
	{
		ProgramStateRef State = C.getState();
		State = State->set<varStateMap>(Loc.getAsRegion(), inited);
		C.addTransition(State);

	}
	//如果是读取操作，说明对变量进行了使用，此时需要判断该变量是否为已初始化或已使用状态，
	//如果不是这两种状态，则需要判断其上层结构变量是否为已初始化或已使用状态
	else
	{
		const MemRegion* MR = Loc.getAsRegion();
		ProgramStateRef State = C.getState();
		checkUninitedUse(C, MR, State);
	}

}

void UninitializedUnusedChecker::checkPostCall(const CallEvent& Call, CheckerContext& C) const
{

	if (Call.isInSystemHeader())
	{
		return;
	}
	//如果是调用了memset函数，则将第一个参数置为初始化状态，
	//至于是否初始化完全，通过CString检查器来进行检查
	if (Call.isCalled(funcMemset))
	{
		ProgramStateRef State = C.getState();
		string curVar = Call.getArgSVal(0).getAsRegion()->getString();
		State = State->set<varStateMap>(Call.getArgSVal(0).getAsRegion(), inited);
		C.addTransition(State);
		return;
	}
	//如果是调用了memcpy函数，则将第一个参数置为初始化状态，第二个参数置为使用状态，
	//至于是否初始化、使用完全，通过CString检查器来进行检查
	else if (Call.isCalled(funcMemcpy))
	{
		ProgramStateRef State = C.getState();
		const MemRegion* MR = Call.getArgSVal(0).getAsRegion();
		string curVar = MR->getString();
		const int* temp = State->get<varStateMap>(MR);
		if (temp)
		{
			State = State->set<varStateMap>(MR, inited);
			C.addTransition(State);
		}

		MR = Call.getArgSVal(1).getAsRegion();
		checkUninitedUse(C, MR, State);
	}
	//如果是调用了其他函数，待补充
	else
	{
		/*
		ProgramStateRef State = C.getState();
		int Number = Call.getNumArgs();
		for (int i = 0; i < Number; i++)
		{
			SVal sv = Call.getArgSVal(i);
			
			if (isTainted(State, sv))
			{
				const MemRegion*  MR = Call.getArgSVal(i).getAsRegion();
				State = State->set<varStateMap>(MR, used);
				C.addTransition(State);
			}
		}
		*/
		//todo
	}
}

void UninitializedUnusedChecker::checkEndAnalysis(ExplodedGraph& G, BugReporter& BR, ExprEngine& Eng) const
{
	
	for (auto iter = G.eop_begin(); iter != G.eop_end(); iter++)
	{
		ProgramStateRef State = (*iter)->getState();
		varStateMapTy trackedVarState = State->get<varStateMap>();
		for (auto subIter = trackedVarState.begin(); subIter != trackedVarState.end(); subIter++)
		{
			if (subIter->second == inited )
			{
				if (!BT)
				{
					BT.reset(new BuiltinBug(this, "未初始化或未使用"));
				}
				if (!subIter->first->getDescriptiveName().empty())
				{
					string msg = subIter->first->getDescriptiveName();

						msg += "变量未被使用或未被完全使用！" ;
						auto R = make_unique<PathSensitiveBugReport>(*BT, msg, *iter);
						BR.emitReport(move(R));

				}
				
			}
			else if(subIter->second == unInited)
			{
				if (!BT)
				{
					BT.reset(new BuiltinBug(this, "未初始化或未使用"));
				}
				if (!subIter->first->getDescriptiveName().empty())
				{
					string msg = subIter->first->getDescriptiveName();

					{
						msg += "变量未初始化！" ;
						auto R = make_unique<PathSensitiveBugReport>(*BT, msg, *iter);
						BR.emitReport(move(R));
					}
					
				
				}
			}
		}
	}
}

//如果是已初始化状态或者已使用状态就返回true，否则返回false
bool UninitializedUnusedChecker::checkState(CheckerContext& C, const MemRegion* MR) const
{
	ProgramStateRef State = C.getState();
	const int* temp = State->get<varStateMap>(MR);
	if (temp)
	{
		if ((*temp) == inited || (*temp) == used)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

//检查是否未初始化就使用了
void UninitializedUnusedChecker::checkUninitedUse(CheckerContext& C, const MemRegion* MR, ProgramStateRef State) const
{
	do
	{
		//检查是否为正确使用，如果是，则添加状态并返回									
		if (checkState(C, MR))
		{
			State = State->set<varStateMap>(MR, used);
			C.addTransition(State);
			continue;
		}
		//取baseregion
		MR = MR->getBaseRegion();
	} while (MR != MR->getBaseRegion());

	//如果上面的循环没有return，说明存在未初始化的使用，报错
	//reportBug("变量未经初始化就使用了!", C);
	//避免重复报错
	// if(nodeMap.find(MR) != nodeMap.end())
	// {
	// 	nodeMap.erase(MR); 					
	// }
}

//用于报错
void UninitializedUnusedChecker::reportBug(const char* Msg, CheckerContext& C) const
{
	if (!BT)
	{
		BT.reset(new BuiltinBug(this, "未初始化或未使用"));
	}
	ExplodedNode* N = C.generateNonFatalErrorNode();
	auto R = make_unique<PathSensitiveBugReport>(*BT, Msg, N);
	C.emitReport(move(R));
}

void ento::registerUninitializedUnusedChecker(CheckerManager& mgr) {
	mgr.registerChecker<UninitializedUnusedChecker>();
}

bool ento::shouldRegisterUninitializedUnusedChecker(const CheckerManager& mgr) {
	return true;
}
