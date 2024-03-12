//=== StackAddrEscapeChecker.cpp ----------------------------------*- C++ -*--//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// 
//===----------------------------------------------------------------------===//
//
// This file defines stack address leak checker, which checks if an invalid
// stack address is stored into a global or heap location. See CERT DCL30-C.
//
//===----------------------------------------------------------------------===//

#include "common/CheckScope.h"

#include "clang/AST/ExprCXX.h"
#include "clang/Basic/SourceManager.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ProgramState.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/raw_ostream.h"


using namespace clang;
using namespace ento;
using namespace std;
namespace {
	class ScopeStackAddrEscapeAwayBaseChecker
		: public Checker<check::PreCall, check::PreStmt<ReturnStmt>,
		check::EndFunction > {
		mutable IdentifierInfo *dispatch_semaphore_tII;
		mutable std::unique_ptr<BuiltinBug> BT_stackleak;
		mutable std::unique_ptr<BuiltinBug> BT_returnstack;
		mutable std::unique_ptr<BuiltinBug> BT_capturedstackasync;
		mutable std::unique_ptr<BuiltinBug> BT_capturedstackret;

	public:
		enum CheckKind {
			CK_ScopeStackAddrEscapeAwayChecker,
			CK_ScopeStackAddrAsyncEscapeAwayChecker,
			CK_NumCheckKinds
		};

		DefaultBool ChecksEnabled[CK_NumCheckKinds];
		CheckerNameRef CheckNames[CK_NumCheckKinds];

		void checkPreCall(const CallEvent &Call, CheckerContext &C) const;
		void checkPreStmt(const ReturnStmt *RS, CheckerContext &C) const;
		void checkEndFunction(const ReturnStmt *RS, CheckerContext &Ctx) const;

	private:
		void checkReturnedBlockCaptures(const BlockDataRegion &B,
			CheckerContext &C) const;
		void checkAsyncExecutedBlockCaptures(const BlockDataRegion &B,
			CheckerContext &C) const;
		void EmitStackError(CheckerContext &C, const MemRegion *R,
			const Expr *RetE) const;
		bool isSemaphoreCaptured(const BlockDecl &B) const;
		static SourceRange genName(raw_ostream &os, const MemRegion *R,
			ASTContext &Ctx);
		static SmallVector<const MemRegion *, 4>
			getCapturedStackRegions(const BlockDataRegion &B, CheckerContext &C);
		static bool isArcManagedBlock(const MemRegion *R, CheckerContext &C);
		static bool isNotInCurrentFrame(const MemRegion *R, CheckerContext &C);
	};
} // namespace


SourceRange ScopeStackAddrEscapeAwayBaseChecker::genName(raw_ostream &os, const MemRegion *R,
	ASTContext &Ctx) {
	// Get the base region, stripping away fields and elements.
	R = R->getBaseRegion();
	SourceManager &SM = Ctx.getSourceManager();
	SourceRange range;
	os << "Address of ";
	// Check if the region is a compound literal.
	if (const auto *CR = dyn_cast<CompoundLiteralRegion>(R)) {
		const CompoundLiteralExpr *CL = CR->getLiteralExpr();
		os << "stack memory associated with a compound literal "
			"declared on line "
			<< SM.getExpansionLineNumber(CL->getBeginLoc()) << " returned to caller";
		range = CL->getSourceRange();
	}
	else if (const auto *AR = dyn_cast<AllocaRegion>(R)) {
		const Expr *ARE = AR->getExpr();
		SourceLocation L = ARE->getBeginLoc();
		range = ARE->getSourceRange();
		os << "stack memory allocated by call to alloca() on line "
			<< SM.getExpansionLineNumber(L);
	}
	else if (const auto *BR = dyn_cast<BlockDataRegion>(R)) {
		const BlockDecl *BD = BR->getCodeRegion()->getDecl();
		SourceLocation L = BD->getBeginLoc();
		range = BD->getSourceRange();
		os << "stack-allocated block declared on line "
			<< SM.getExpansionLineNumber(L);
	}
	else if (const auto *VR = dyn_cast<VarRegion>(R)) {
		os << "stack memory associated with local variable '" << VR->getString()
			<< '\'';
    range=VR->getDecl()->getSourceRange();
	}
	else if (const auto *TOR = dyn_cast<CXXTempObjectRegion>(R)) {
		QualType Ty = TOR->getValueType().getLocalUnqualifiedType();
		os << "stack memory associated with temporary object of type '";
		Ty.print(os, Ctx.getPrintingPolicy());
		os << "'";
		range = TOR->getExpr()->getSourceRange();
	}
	else {
		llvm_unreachable("ReturnStackAddressChecker中的无效区域。");
	}
	return range;
}

bool ScopeStackAddrEscapeAwayBaseChecker::isArcManagedBlock(const MemRegion *R,
	CheckerContext &C) {
	assert(R && "MemRegion should not be null");
	return C.getASTContext().getLangOpts().ObjCAutoRefCount 
  && isa<BlockDataRegion>(R); 
}

bool ScopeStackAddrEscapeAwayBaseChecker::isNotInCurrentFrame(const MemRegion *R,
	CheckerContext &C) {
	const StackSpaceRegion *S = cast<StackSpaceRegion>(R->getMemorySpace());
  if(C.getStackFrame()->inTopFrame()&&(S->getStackFrame() != C.getStackFrame()))
  {
    return false;
  }
  if(!C.getStackFrame()->inTopFrame()&&(S->getStackFrame() == C.getStackFrame())
    &&S->hasStackParametersStorage())
  {
    return false;
  }
  else 
  {
	  return  true;
  }
}

bool ScopeStackAddrEscapeAwayBaseChecker::isSemaphoreCaptured(const BlockDecl &B) const {
	if (!dispatch_semaphore_tII)
		dispatch_semaphore_tII = &B.getASTContext().Idents.get("dispatch_semaphore_t");
	for (const auto &C : B.captures()) 
  {
		const auto *T = C.getVariable()->getType()->getAs<TypedefType>();
		if (T && T->getDecl()->getIdentifier() == dispatch_semaphore_tII)
			return true;
	}
	return false;
}

SmallVector<const MemRegion *, 4>
ScopeStackAddrEscapeAwayBaseChecker::getCapturedStackRegions(const BlockDataRegion &B,
	CheckerContext &C) {
	SmallVector<const MemRegion *, 4> Regions;
	BlockDataRegion::referenced_vars_iterator I = B.referenced_vars_begin();
	BlockDataRegion::referenced_vars_iterator E = B.referenced_vars_end();
	for (; I != E; ++I) {
		SVal Val = C.getState()->getSVal(I.getCapturedRegion());
		const MemRegion *Region = Val.getAsRegion();
		if (Region && isa<StackSpaceRegion>(Region->getMemorySpace()))
			Regions.push_back(Region);
	}
	return Regions;
}

void ScopeStackAddrEscapeAwayBaseChecker::EmitStackError(CheckerContext &C,
	const MemRegion *R,
	const Expr *RetE) const {
    if(!RetE)
    {
      return;
    }
	ExplodedNode *N = C.generateNonFatalErrorNode();
	if (!N)
  {
		return;
  }
	if (!BT_returnstack)
  {
		BT_returnstack = std::make_unique<BuiltinBug>(
			CheckNames[CK_ScopeStackAddrEscapeAwayChecker],
			"Return of address to stack-allocated memory");
  }
	// Generate a report for this bug.
	SmallString<128> buf;
	llvm::raw_svector_ostream os(buf);
	SourceRange range = genName(os, R, C.getASTContext());
	os << " returned to caller";
	auto report =
		std::make_unique<PathSensitiveBugReport>(*BT_returnstack, os.str(), N);
	report->addRange(RetE->getSourceRange());
	if (range.isValid())
		report->addRange(range);
	C.emitReport(std::move(report));
}

void ScopeStackAddrEscapeAwayBaseChecker::checkAsyncExecutedBlockCaptures(
	const BlockDataRegion &B, CheckerContext &C) const {
	// There is a not-too-uncommon idiom
	// where a block passed to dispatch_async captures a semaphore
	// and then the thread (which called dispatch_async) is blocked on waiting
	// for the completion of the execution of the block
	// via dispatch_semaphore_wait. To avoid false-positives (for now)
	// we ignore all the blocks which have captured
	// a variable of the type "dispatch_semaphore_t".
	if (isSemaphoreCaptured(*B.getDecl()))
		return;
	for (const MemRegion *Region : getCapturedStackRegions(B, C)) {
		// The block passed to dispatch_async may capture another block
		// created on the stack. However, there is no leak in this situaton,
		// no matter if ARC or no ARC is enabled:
		// dispatch_async copies the passed "outer" block (via Block_copy)
		// and if the block has captured another "inner" block,
		// the "inner" block will be copied as well.
		if (isa<BlockDataRegion>(Region))
			continue;
		ExplodedNode *N = C.generateNonFatalErrorNode();
		if (!N)
			continue;
		if (!BT_capturedstackasync)
			BT_capturedstackasync = std::make_unique<BuiltinBug>(
				CheckNames[CK_ScopeStackAddrAsyncEscapeAwayChecker],
				"Address of stack-allocated memory is captured");
		SmallString<128> Buf;
		llvm::raw_svector_ostream Out(Buf);
		SourceRange Range = genName(Out, Region, C.getASTContext());
		Out << " is captured by an asynchronously-executed block";
		auto Report = std::make_unique<PathSensitiveBugReport>(
			*BT_capturedstackasync, Out.str(), N);
		if (Range.isValid())
			Report->addRange(Range);
		C.emitReport(std::move(Report));
	}
}

void ScopeStackAddrEscapeAwayBaseChecker::checkReturnedBlockCaptures(
	const BlockDataRegion &B, CheckerContext &C) const {
	for (const MemRegion *Region : getCapturedStackRegions(B, C)) {
		if (isArcManagedBlock(Region, C) || isNotInCurrentFrame(Region, C))
			continue;
		ExplodedNode *N = C.generateNonFatalErrorNode();
		if (!N)
			continue;
		if (!BT_capturedstackret)
			BT_capturedstackret = std::make_unique<BuiltinBug>(
				CheckNames[CK_ScopeStackAddrEscapeAwayChecker],
				"Address of stack-allocated memory is captured");
		SmallString<128> Buf;
		llvm::raw_svector_ostream Out(Buf);
		SourceRange Range = genName(Out, Region, C.getASTContext());
		Out << " is captured by a returned block";
		auto Report = std::make_unique<PathSensitiveBugReport>(*BT_capturedstackret,
			Out.str(), N);
		if (Range.isValid())
			Report->addRange(Range);
		C.emitReport(std::move(Report));
	}
}

void ScopeStackAddrEscapeAwayBaseChecker::checkPreCall(const CallEvent &Call,
	CheckerContext &C) const {
  if (!ChecksEnabled[CK_ScopeStackAddrAsyncEscapeAwayChecker])//不启用-check array as pointer assign
  {
		return;
  }
	if (!Call.isGlobalCFunction("dispatch_after") &&
		!Call.isGlobalCFunction("dispatch_async"))
  {
    return;
  }
	for (unsigned Idx = 0, NumArgs = Call.getNumArgs(); Idx < NumArgs; ++Idx) 
  {
		if (const BlockDataRegion *B = dyn_cast_or_null<BlockDataRegion>(
    Call.getArgSVal(Idx).getAsRegion()))
    {
      checkAsyncExecutedBlockCaptures(*B, C);
    }
	}
}

void ScopeStackAddrEscapeAwayBaseChecker::checkPreStmt(const ReturnStmt *RS,
	CheckerContext &C) const {
	if (!ChecksEnabled[CK_ScopeStackAddrEscapeAwayChecker])  
		return;
	const Expr *RetE = RS->getRetValue();
	if (!RetE)
		return;
	RetE = RetE->IgnoreParens();
	SVal V = C.getSVal(RetE);
	const MemRegion *R = V.getAsRegion();//local_auto
	if (!R)
		return;
	if (const BlockDataRegion *B = dyn_cast<BlockDataRegion>(R))
  		checkReturnedBlockCaptures(*B, C);

	if (!isa<StackSpaceRegion>(R->getMemorySpace()) ||
		!isNotInCurrentFrame(R, C) || isArcManagedBlock(R, C))
  {
    return;
  }
	// Returning a record by value is fine. (In this case, the returned
	// expression will be a copy-constructor, possibly wrapped in an
	// ExprWithCleanups node.)
	if (const ExprWithCleanups *Cleanup = dyn_cast<ExprWithCleanups>(RetE))
  {  
		RetE = Cleanup->getSubExpr();
  }
	if (isa<CXXConstructExpr>(RetE) && RetE->getType()->isRecordType())
  {
		return;
  }
	// The CK_CopyAndAutoreleaseBlockObject cast causes the block to be copied
	// so the stack address is not escaping here.
	if (const auto *ICE = dyn_cast<ImplicitCastExpr>(RetE)) 
  {
		if (isa<BlockDataRegion>(R) &&
    ICE->getCastKind() == CK_CopyAndAutoreleaseBlockObject) 
    {
      return;
		}
	}
	EmitStackError(C, R, RetE);
}

//  SourceRange range;
void ScopeStackAddrEscapeAwayBaseChecker::checkEndFunction(const ReturnStmt *RS,
	CheckerContext &Ctx) const {
  if (!ChecksEnabled[CK_ScopeStackAddrEscapeAwayChecker])
  {
		return;
  }
	ProgramStateRef State = Ctx.getState(); 
 
	// Iterate over all bindings to global variables and see //if it contains
	// a memory region in the stack space.//x=0 q=&x
	class CallBack : public StoreManager::BindingsHandler {
	private:
		CheckerContext &Ctx;
		const StackFrameContext *CurSFC;

	public:
		SmallVector<std::pair<const MemRegion *, const MemRegion *>, 10> V;

		CallBack(CheckerContext &CC) : Ctx(CC), CurSFC(CC.getStackFrame()) {}

		bool HandleBinding(StoreManager &SMgr, Store S, const MemRegion *Region,
			SVal Val) override {
			if (!isa<GlobalsSpaceRegion>(Region->getMemorySpace()))
      { 
				return true;
      }
			const MemRegion *VR = Val.getAsRegion(); 
			if (VR && isa<StackSpaceRegion>(VR->getMemorySpace())
        &&!isArcManagedBlock(VR, Ctx)&& isNotInCurrentFrame(VR, Ctx))
      {
        V.emplace_back(Region, VR);
      }
			return true; 
    }
	};

	CallBack Cb(Ctx);
  State->getStateManager().getStoreManager().iterBindings(State->getStore(),
		Cb); 
    
	if (Cb.V.empty())
  {
		return;
  }
	// Generate an error node.
  ExplodedNode *N = Ctx.generateNonFatalErrorNode(State);
	if (!N)
  {
		return;
  }
	if (!BT_stackleak){
		BT_stackleak = std::make_unique<BuiltinBug>(
			CheckNames[CK_ScopeStackAddrEscapeAwayChecker],
			"堆栈地址存储在全局变量中",
			"堆栈地址被保存到全局变量中. "
			"函数返回后，地址将变得无效 "
			"导致危险");
  }
	for (const auto &P : Cb.V) 
  {     
		// Generate a report for this bug.
		SmallString<128> Buf;
		llvm::raw_svector_ostream Out(Buf);
		SourceRange Range = genName(Out, P.second, Ctx.getASTContext());//内存对中的第二个 &x
		Out << " is still referred to by the ";
    
		if (isa<StaticGlobalSpaceRegion>(P.first->getMemorySpace()))
			Out << "static";
		else
			Out << "global";
		Out << " variable '";
		const VarRegion *VR = cast<VarRegion>(P.first->getBaseRegion());
		Out << *VR->getDecl()
			<< "' upon returning to the caller.  This will be a dangling reference";
		
    auto Report =
			std::make_unique<PathSensitiveBugReport>(*BT_stackleak, Out.str(), N);
	
    if (Range.isValid())
			Report->addRange(Range);
    Ctx.emitReport(std::move(Report));
	}
}

#define REGISTER_CHECKER(name)                                                 \
void register##name(CheckerManager &Mgr) {                                                 \
    ScopeStackAddrEscapeAwayBaseChecker *Chk = Mgr.getChecker<ScopeStackAddrEscapeAwayBaseChecker>();    \
    Chk->ChecksEnabled[ScopeStackAddrEscapeAwayBaseChecker::CK_##name] = true;              \
    Chk->CheckNames[ScopeStackAddrEscapeAwayBaseChecker::CK_##name] =                       \
        Mgr.getCurrentCheckerName();                                                         \
}																							\
																				             \
bool shouldRegister##name(const CheckerManager &mgr) { return true; }                        \
static void add##name(clang::ento::CheckerRegistry &registry,                                \
StringRef Name,StringRef Desc, StringRef DocsUri,bool IsHidden)                              \
{                                                                                            \
	registry.addChecker(register##name, shouldRegister##name,Name,Desc,DocsUri,IsHidden);    \
}

REGISTER_CHECKER(ScopeStackAddrEscapeAwayChecker)
REGISTER_CHECKER(ScopeStackAddrAsyncEscapeAwayChecker)


//编写注册逻辑
class ScopeStackAddrEscapeBaseAwayCheckerRegister :public scope::CheckerRegisterFactory
{
public:
	virtual void registerCheckers(clang::ento::CheckerRegistry &registry)
	{
		//声明要用到的checker名称(这样可以降低内存开销)
		llvm::StringRef CheckStackAddrEscapeAwayBase = getCheckScopeDevNameStr("ScopeStackAddrEscapeAwayBase");
		llvm::StringRef CheckStackAddrEscapeAway = getCheckScopeDevNameStr("StackAddrEscapeAway");
		llvm::StringRef CheckStackAddrAsyncEscapeAway = getCheckScopeDevNameStr("StackAddrAsyncEscapeAway");

		//添加Checker
		registry.addChecker<ScopeStackAddrEscapeAwayBaseChecker>(CheckStackAddrEscapeAwayBase,
			"Check where the address of a local variable is returned from a function", NOT_CHECKER_DOC, HIDDEN_CHECKER);

		//添加子检查项
		addScopeStackAddrEscapeAwayChecker(registry,
			CheckStackAddrEscapeAway,
			"check address as pointer compare", NOT_CHECKER_DOC, NOT_HIDDEN_CHECKER);

		addScopeStackAddrAsyncEscapeAwayChecker(registry,
			CheckStackAddrAsyncEscapeAway,
			"check array as pointer assign", NOT_CHECKER_DOC, NOT_HIDDEN_CHECKER);


		//添加依赖关系
		registry.addDependency(CheckStackAddrEscapeAway, CheckStackAddrEscapeAwayBase);
		registry.addDependency(CheckStackAddrAsyncEscapeAway, CheckStackAddrEscapeAwayBase);
	}
};
//显式声明全局对象实例以激活注册
SCOPE_REGISTER_CLASS(ScopeStackAddrEscapeBaseAwayCheckerRegister)
