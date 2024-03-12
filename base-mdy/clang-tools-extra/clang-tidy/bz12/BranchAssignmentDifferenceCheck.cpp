//===--- BranchAssignmentDifferenceCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "BranchAssignmentDifferenceCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <queue>
#include "utils.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void BranchAssignmentDifferenceCheck::registerMatchers(MatchFinder *Finder) 
{
	//匹配if语句（只匹配第一个if）
    Finder->addMatcher(ifStmt(unless(anyOf(isExpansionInSystemHeader(), hasParent(ifStmt())))).bind("matchedIfStmt"), this);
    Finder->addMatcher(switchStmt(unless(isExpansionInSystemHeader())).bind("matchedSwitchStmt"), this);
}

void BranchAssignmentDifferenceCheck::check(const MatchFinder::MatchResult &Result) 
{
    if(const auto *matchedIfStmt = Result.Nodes.getNodeAs<IfStmt>("matchedIfStmt")) 
    {
        traverseIfBranch(matchedIfStmt);
    }
    else if(const auto *matchedSwitchStmt = Result.Nodes.getNodeAs<SwitchStmt>("matchedSwitchStmt")) 
    {
        traverseSwitchBody(matchedSwitchStmt->getBody());
    }
}

void BranchAssignmentDifferenceCheck::traverseIfBranch(const IfStmt* matchedIfStmt) 
{
    //当对一组新的if进行检查时，清空变量-赋值map
    var_Rhs_Map.clear();
    analysisCompoundStmt(matchedIfStmt->getThen());
	while(matchedIfStmt->hasElseStorage())
    {
        //如果下一个是else if，那么继续循环
		if(const IfStmt* elseStmt = dyn_cast<IfStmt>(matchedIfStmt->getElse())) 
        {
            matchedIfStmt = elseStmt;
            analysisCompoundStmt(matchedIfStmt->getThen());
        } 
        //如果下一个是else，停止循环
        else 
        {
            analysisCompoundStmt(matchedIfStmt->getElse());
            break;
        }
    }
    analysisAssignmentDifference();
}

void BranchAssignmentDifferenceCheck::traverseSwitchBody(const Stmt *switchBody)
{
    //当对一组新的switch进行检查时，清空变量-赋值map
    var_Rhs_Map.clear();
    if(const CompoundStmt *CPS = dyn_cast<CompoundStmt>(switchBody)) 
    {
        CompoundStmt::const_body_iterator it_begin = CPS->body_begin();
        CompoundStmt::const_body_iterator it_end = CPS->body_end();
        for(; it_begin!= it_end; it_begin++) 
        {
            if(const CaseStmt *CS = dyn_cast<CaseStmt>(*it_begin)) 
            {
                for(auto iter=CS->child_begin(); iter!=CS->child_end(); iter++)
                {
                    if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(*iter)) 
                    {
                        if(BO->isAssignmentOp())
                        {
                            //cout << "************" << endl;
                            //BO->dumpColor();
                            string varName = getVarName(BO->getLHS()->IgnoreParenCasts());
                            if(varName != "")
                            {
                                var_Rhs_Map[varName].push_back(BO->getRHS()->IgnoreParenCasts());
                            }
                        }
                    }
                }
            } 
            else if(const DefaultStmt *DS = dyn_cast<DefaultStmt>(*it_begin)) 
            {
                for(auto iter=DS->child_begin(); iter!=DS->child_end(); iter++)
                {
                    if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(*iter)) 
                    {
                        if(BO->isAssignmentOp())
                        {
                            string varName = getVarName(BO->getLHS()->IgnoreParenCasts());
                            if(varName != "")
                            {
                                var_Rhs_Map[varName].push_back(BO->getRHS()->IgnoreParenCasts());
                            }
                        }
                    }
                }
            }
            //目前clang-tidy存在bug，对于case内的赋值语句，BinaryOperator应该是CaseStmt的子孙，
            //但除了第一个，其他的全变成了CaseStmt的同级，所以增加该分支进行处理
            else if(BinaryOperator *BO = dyn_cast<BinaryOperator>(*it_begin))
            {
                if(BO->isAssignmentOp())
                {
                    string varName = getVarName(BO->getLHS()->IgnoreParenCasts());
                    if(varName != "")
                    {
                        var_Rhs_Map[varName].push_back(BO->getRHS()->IgnoreParenCasts());
                    }
                }
            }
            else
            {
                //doNothing
            }
        }
    }
    analysisAssignmentDifference();
}

void BranchAssignmentDifferenceCheck::analysisCompoundStmt(const Stmt* matchedStmt)
{
    if(const CompoundStmt* compoundStmt = dyn_cast<CompoundStmt>(matchedStmt))
    {
        for(auto subIter=compoundStmt->body_begin(); subIter!=compoundStmt->body_end(); subIter++)
        {
            if(const BinaryOperator* BO = dyn_cast<BinaryOperator>(*subIter))
            {
                //只对赋值表达式进行处理
                if(BO->isAssignmentOp())
                {
                    string varName = getVarName(BO->getLHS()->IgnoreParenCasts());
                    if(varName != "")
                    {
                        var_Rhs_Map[varName].push_back(BO->getRHS()->IgnoreParenCasts());
                    }
                }
            }
        }
    }
    else
    {
        //doNothing
    }
}

//对同一个变量的右侧所有情况进行比较，看是否存在一样的情况
void BranchAssignmentDifferenceCheck::analysisAssignmentDifference()
{
    for(auto iter=var_Rhs_Map.begin(); iter!=var_Rhs_Map.end(); iter++)
    {
        int size = iter->second.size();
        for(int i = 0; i < size; i++)
        {
            for(int j = i+1; j < size; j++)
            {
                analysisResult(iter->second[i], iter->second[j]);
            }
        }
    }
}

//分析并报错
void BranchAssignmentDifferenceCheck::analysisResult(const Expr *lastAssignment, const Expr *currAssignment)
{
    vector<string> lastAssignment_Vec, currAssignment_Vec;
    travelExpr(lastAssignment, lastAssignment_Vec);
    travelExpr(currAssignment, currAssignment_Vec);
    int lastVecSize = lastAssignment_Vec.size();
    int currVecSize = currAssignment_Vec.size();
    //如果表达式右侧元素数量不相同则一定不相同，直接返回
    if(lastVecSize != currVecSize)
    {
        return;
    }
    else
    {
        int i = 0;
        while(i < currVecSize)
        {
            // cout << currVecSize << endl;
            // cout << lastAssignment_Vec[i] << endl;
            //如果不相同就返回
            if(lastAssignment_Vec[i] != currAssignment_Vec[i])
            {
                return;
            }
            i++;
        }
        //如果全部相同就报错
        diag(currAssignment->getBeginLoc(), "本次赋值与之前逻辑块存在重复"); 
    }
}

//获取变量名称
string BranchAssignmentDifferenceCheck::getVarName(const Expr* MatchedExpr)
{
    string varName = "";
	
    if(isa<DeclRefExpr>(MatchedExpr))
    {
        const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(MatchedExpr);         
        varName = (DRE->getDecl()->getDeclName().getAsString());
    }
    else if(isa<MemberExpr>(MatchedExpr))
    {
        const MemberExpr *ME = dyn_cast<MemberExpr>(MatchedExpr);                            
        varName = getMemberExprString(ME);         
    }
    else if(isa<ArraySubscriptExpr>(MatchedExpr))
    {
        const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(MatchedExpr);                     
        varName = getArraySubscriptExprString(ASE); 
    }
    else
    {
        //doNothing
    }

    return varName; 
}

//处理二元表达式
vector<Expr*> BranchAssignmentDifferenceCheck::handlingBinaryOperator(const BinaryOperator *CircleBinaryOperator, vector<string> &exprStrVec)
{
    vector<Expr*> vecExpr;
    queue<const BinaryOperator*> BinaryOperatorQueue;
    BinaryOperatorQueue.push(CircleBinaryOperator);
    exprStrVec.push_back("BinaryOperator" + to_string(CircleBinaryOperator->getOpcode()));
    while(!BinaryOperatorQueue.empty())
    {
        const BinaryOperator *FrontBinaryOperator=BinaryOperatorQueue.front();
        BinaryOperatorQueue.pop();
        if(isa<BinaryOperator>(FrontBinaryOperator->getLHS()->IgnoreParenCasts()))
        {
            const BinaryOperator *LeftBinaryOperator = dyn_cast<BinaryOperator>(FrontBinaryOperator->getLHS()->IgnoreParenCasts());
            exprStrVec.push_back("BinaryOperator" + to_string(LeftBinaryOperator->getOpcode()));
            BinaryOperatorQueue.push(LeftBinaryOperator);
        } 
        else 
        {
            vecExpr.push_back(FrontBinaryOperator->getLHS()->IgnoreParenCasts());
        }
        if(isa<BinaryOperator>(FrontBinaryOperator->getRHS()->IgnoreParenCasts()))
        {
            const BinaryOperator *RightBinaryOperator = dyn_cast<BinaryOperator>(FrontBinaryOperator->getRHS()->IgnoreParenCasts());
            exprStrVec.push_back("BinaryOperator" + to_string(RightBinaryOperator->getOpcode()));
            BinaryOperatorQueue.push(RightBinaryOperator);
        } 
        else 
        {
            vecExpr.push_back(FrontBinaryOperator->getRHS()->IgnoreParenCasts());
        }
    }
    return vecExpr;
}

void BranchAssignmentDifferenceCheck::travelExpr(const Expr* matchedExpr, vector<string> &exprStrVec)
{
    //对二元表达式的支持
    if(isa<BinaryOperator>(matchedExpr))
    {
        auto CircleBinaryOperator = dyn_cast<BinaryOperator>(matchedExpr);
        auto vecExpr = handlingBinaryOperator(CircleBinaryOperator, exprStrVec);
        for(auto iter=vecExpr.begin(); iter!=vecExpr.end(); iter++)
        {
            travelExpr(*iter, exprStrVec);
        }
    }
    //对函数中参数的支持
    else if(isa<CallExpr>(matchedExpr))
    {
        auto matchedCallExpr = dyn_cast<CallExpr>(matchedExpr);
        if(const auto *MatchedFunctionDecl = dyn_cast<FunctionDecl>(matchedCallExpr->getCalleeDecl()))
        {
            string funcName = MatchedFunctionDecl->getDeclName().getAsString();
            exprStrVec.push_back("Function" + funcName);
        }
        for(auto iter=matchedCallExpr->arg_begin(); iter!=matchedCallExpr->arg_end(); iter++)
        {
            travelExpr(*iter, exprStrVec);
        }
    }
    //对一元表达式的支持
    else if(isa<UnaryOperator>(matchedExpr))
    {
        auto matchedUnaryOperator = dyn_cast<UnaryOperator>(matchedExpr);
        exprStrVec.push_back("UnaryOperator" + to_string(matchedUnaryOperator->getOpcode()));
        travelExpr(matchedUnaryOperator->getSubExpr(), exprStrVec);
    }
    else
    {
        if(getVarName(matchedExpr) != "")
        {
            exprStrVec.push_back(getVarName(matchedExpr));
        }   
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang