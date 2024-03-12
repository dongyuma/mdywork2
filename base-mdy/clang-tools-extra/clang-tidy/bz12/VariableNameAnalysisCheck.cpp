//===--- VariableNameAnalysisCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "VariableNameAnalysisCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/include/rapidjson/document.h"
#include "llvm/Support/Regex.h"
#include "utils.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bz12 {

void VariableNameAnalysisCheck::registerMatchers(MatchFinder *Finder) 
{
    //尝试将比较运算左右的一致性合并到该checker，Cheer22.6.3
    Finder->addMatcher(binaryOperator(unless(isExpansionInSystemHeader()),anyOf(isAssignmentOperator(), isComparisonOperator())).bind("binaryOperator"), this);
    Finder->addMatcher(varDecl(unless(isExpansionInSystemHeader())).bind("varDecl"), this);
}

//对于嵌套型变量，提供三种检测模式     
//default：只检测最右端     
//section：将变量按结构分段(待实现)     
//whole：将变量作为一个单一长变量处理

void VariableNameAnalysisCheck::check(const MatchFinder::MatchResult &Result) 
{
    //从配置文件读取嵌套型变量检测模式，只读一次
    if(unread)
    {
        nested_variable_check_mode = read_nested_variable_check_mode();
        read_negative_string_vector(negative_string_vector);
        read_default_string_level_map(default_string_level_map);
        read_default_string_set(default_string_set);
        read_custom_string_set (custom_string_set);
        read_custom_string_level_map(custom_string_level_map);
        read_subname_map(sub_name_group_map, sub_name_level_map, group_map);
        read_default_keyword_levelset_map(keyword_levelset_map);
        read_custom_keyword_levelset_map(keyword_levelset_map);
        custom_case_sensitive = read_custom_case_sensitive ();

        unread = false;

        //找一下自定义关键字中最大的数组下标
        for(auto iter=custom_string_set.begin(); iter!=custom_string_set.end(); iter++)
        {
            if(iter->find("arrayindex") != string::npos)
            {
                string indexStr = iter->substr(iter->find("arrayindex")+10, iter->length()-1);
                maxIndexValue = max(maxIndexValue, stoi(indexStr));              
            }
        }
        //cout << "最大下标：" << maxIndexValue << endl;
    }
    //获取BinaryOperator，将BinaryOperator涉及的变量名和等式的对应关系加入相应集合
    if(const auto *MatchedBinaryOperator = Result.Nodes.getNodeAs<BinaryOperator>("binaryOperator"))
    {
        //对于比较表达式，要考虑左侧为非变量的情况
        set<const Expr*> lExprSet, rExprSet;
        travelExpr(MatchedBinaryOperator->getLHS()->IgnoreParenCasts(), lExprSet);
        travelExpr(MatchedBinaryOperator->getRHS()->IgnoreParenCasts(), rExprSet);
        for(auto iter1=lExprSet.begin(); iter1!=lExprSet.end(); iter1++)
        {
            string varName1 = getVarName(*iter1);
            if(varName1 != "")
            {
                variable_name_set.insert(varName1);
                //cout << "varName1:" << varName1 << endl;
            }
            for(auto iter2=rExprSet.begin(); iter2!=rExprSet.end(); iter2++)
            {
                string varName2 = getVarName(*iter2);
                if(varName2 != "")
                {
                    variable_name_set.insert(varName2);
                    //cout << "varName2:" << varName2 << endl;
                }    
                binaryoperator_correspond_map[*iter1].push_back(*iter2);
                expr_binaryOperator_map[*iter2] = MatchedBinaryOperator;
            }
        }
    } 
    else if(const auto *MatchedVarDecl = Result.Nodes.getNodeAs<VarDecl>("varDecl"))
    {  
        if(MatchedVarDecl->hasInit())
        {   
            //不再对数组的初始化语句进行处理，以免引入干扰
            if(!isa<InitListExpr>(MatchedVarDecl->getInit()))
            {
                set<const Expr*> exprSet;
                travelExpr(MatchedVarDecl->getInit(), exprSet);
                string varName = MatchedVarDecl->getDeclName().getAsString();
                variable_name_set.insert(varName);
                for(auto iter=exprSet.begin(); iter!=exprSet.end(); iter++)
                { 
                    string varName = getVarName(*iter);
                    if(varName != "")
                    {
                        variable_name_set.insert(varName);
                    }
                    vardecl_correspond_map[MatchedVarDecl].push_back(*iter);
                    expr_varDecl_map[*iter] = MatchedVarDecl;
                }       
            }
        }
        //如果没有初始化，就将变量名加到set，以免成组时有缺失
        else
        {
            string varName = MatchedVarDecl->getDeclName().getAsString();
            variable_name_set.insert(varName);
        }
    }
}

/*在完成全部单元的处理工作后，根据等价字符串集合equivalent_string_set_initial生成集合equivalent_string_map，并调用analysisVariableName进行变量分析*/
void VariableNameAnalysisCheck::onEndOfTranslationUnit() 
{
    transform_default_string_set_to_list(default_string_list_sorted_by_lenth, default_string_set);
    transform_custom_string_set_to_list(custom_string_list_sorted_by_lenth, custom_string_set);

    analysisVariableName();
} 

void VariableNameAnalysisCheck::analysisVariableName()
{
    //针对每个变量生成其关键字
    for(set<string>::iterator it=variable_name_set.begin(); it!=variable_name_set.end(); it++)
    {
        for(set<string>::iterator it2=variable_name_set.begin(); it2!=variable_name_set.end(); it2++)
        {
            if(*it!=*it2)
            {
                string sub_name1, sub_name2;    
                int start_pos1 = 0, start_pos2 = 0;
                bool analysisResult = analysisNameSimilarity(
                    default_string_list_sorted_by_lenth, custom_string_list_sorted_by_lenth, 
                    *it, *it2, sub_name1, sub_name2, start_pos1, start_pos2);
                if(analysisResult)
                {
                    if(sub_name_group_map[sub_name1] == sub_name_group_map[sub_name2]) 
                    {
                        variable_keyword_map[*it].insert(sub_name1);
                        variable_keyword_map[*it2].insert(sub_name2);
                    }
                }
            }
        }
    }

    //测试代码，用于查看每个变量的所有关键字
    // for(auto iter=variable_keyword_map.begin(); iter!=variable_keyword_map.end(); iter++)
    // {
    //     cout << "*****" << endl << iter->first << ":" << endl;
    //     for(auto it=iter->second.begin(); it!= iter->second.end(); it++)
    //     {
    //         cout << *it << endl;
    //     }
    // }

    //遍历存储vardecl名为vardecl_correspond_map哈希表中的等式对应关系
    for(auto iter=vardecl_correspond_map.begin(); iter!=vardecl_correspond_map.end(); iter++) 
    {
        string varName = iter->first->getDeclName().getAsString();
        checkConsistency(varName, iter->second);
    }
    //遍历存储赋值等式名为binaryoperator_correspond_map哈希表中的等式对应关系
    for (auto iter=binaryoperator_correspond_map.begin(); iter!=binaryoperator_correspond_map.end();iter++)
    {
        string varName = getVarName(iter->first);         
        // if(varName == "")         
        // {        
        //     if(isa<ArraySubscriptExpr>(iter->first))
        //     {
        //         const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(iter->first);             
        //         if(isa<IntegerLiteral>(ASE->getIdx()))  
        //         {
        //             const IntegerLiteral *IL = dyn_cast<IntegerLiteral>(ASE->getIdx());
        //             varName = "arrayindex"+std::to_string(IL->getValue().getSExtValue());
        //         }
        //         else if(isa<BinaryOperator>(ASE->getIdx()))
        //         {
        //             const BinaryOperator *BO = dyn_cast<BinaryOperator>(ASE->getIdx());
        //             if(calcIndex(BO) != -1)
        //             {
        //                 varName = "arrayindex"+std::to_string(calcIndex(BO));
        //             }
        //             else
        //             {
        //                 continue;
        //             }
        //         }                               
        //     }
        // }             
        checkConsistency(varName, iter->second);
    }
}
/*检查左侧名称与右侧expr集合中每一个元素的命名一致性*/
void VariableNameAnalysisCheck::checkConsistency(string expr_name,vector<const Expr*> expr_vector)
{
    size_t index = 0;
    string set_key_word = "";
    //如果变量名字包含"arrayindex"且不包含"++arrayindex"，那么说明不是whole模式
    if((expr_name.find("arrayindex")!=string::npos) && (expr_name.find("++arrayindex")==string::npos))
    {
        if(case_insensitive_find_in_set(expr_name, default_string_set, set_key_word))
        {
            bool found = case_insensitive_find_in_negative_vector(expr_name, negative_string_vector, index);
            if(found) 
            {
                return;
            }
            doCheck(expr_name, expr_name, expr_vector, index);
        }
    }
    else
    {
        if(variable_keyword_map.find(expr_name) != variable_keyword_map.end()) 
        {
            for(auto iter=variable_keyword_map[expr_name].begin(); iter!=variable_keyword_map[expr_name].end(); iter++)
            {
                string keyword = *iter;
                bool found = case_insensitive_find_in_negative_vector(expr_name, negative_string_vector, index);
                if(found) 
                {
                    return;
                }
                doCheck(expr_name, keyword, expr_vector, index);
            }
        }
    } 
}

string VariableNameAnalysisCheck::getVarName(const Expr* MatchedExpr)
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
        if(nested_variable_check_mode == "section")         
        {         
            //todo
        }         
        else if(nested_variable_check_mode == "whole")         
        {             
            varName = getMemberExprString(ME);         
        }         
        else         
        {             
            varName = ME->getMemberDecl()->getDeclName().getAsString();         
        }
    }
    else if(isa<ArraySubscriptExpr>(MatchedExpr))
    {
        const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(MatchedExpr);         
        if(nested_variable_check_mode == "section")         
        {          
            //todo
        }         
        else if(nested_variable_check_mode == "whole")         
        {             
            varName = getArraySubscriptExprString(ASE); 
            if(varName == "")
            {
                return varName;
            }
            string strIndex;

            //处理嵌套变量中存在多维数组及多个数组，只取最右
            if(varName.find("++array-index") != string::npos)
            {
                string temp;
                temp = varName.substr(varName.rfind("++array-index"),varName.length()-1);
                if(temp.find("..") == string::npos)
                {
                    varName.erase(varName.rfind("++array-index")+7, 1);
                    int i = varName.rfind("++arrayindex")+12;
                    while((varName[i] >= 48) && (varName[i] <= 57))
                    {
                        strIndex += varName[i];
                        i++;
                        if(i >= varName.length())
                        {
                            break;
                        }
                    }
                    //cout << varName << ":strIndex:" << strIndex << endl;
                }
            }

            //如果出现了下标大于最大下标，那么该数组不需要处理,加到负向字库
            if(stoi(strIndex) > maxIndexValue)
            {
                string negativeArray = varName;
                while(negativeArray[negativeArray.length()-1] != 'x')
                {
                    negativeArray.pop_back();
                }
                //cout << "negativeArray:" << negativeArray << endl; 
                negative_string_vector.push_back(negativeArray); 
            }
        }         
        else         
        {             
            //doNothing         
        }
    }
    else
    {
        //不做处理
    }

    return varName; 
}

//计算常量表达式形式的数组下标
int VariableNameAnalysisCheck::calcIndex(const BinaryOperator *BO) const
{
    int lValue = 0;
    int rValue = 0;

    if(const IntegerLiteral *L_IL = dyn_cast<IntegerLiteral>(BO->getLHS()->IgnoreParenCasts()))
    {
        lValue = L_IL->getValue().getSExtValue();
    }
    else if(const BinaryOperator *L_BO = dyn_cast<BinaryOperator>(BO->getLHS()->IgnoreParenCasts()))
    {
        lValue = calcIndex(L_BO);
        if(lValue == -1) 
        {
            return -1;
        }
    } 
    else 
    {
        return -1;
    }

    if(const IntegerLiteral *R_IL = dyn_cast<IntegerLiteral>(BO->getRHS()->IgnoreParenCasts())) 
    {
        rValue = R_IL->getValue().getSExtValue();
    } 
    else if(const BinaryOperator *R_BO = dyn_cast<BinaryOperator>(BO->getRHS()->IgnoreParenCasts()))
    {
        rValue = calcIndex(R_BO);
        if(rValue == -1) 
        {
            return -1;
        }
    } 
    else 
    {
        return -1;
    }

    switch(BO->getOpcode())
    {
        case BO_Add:
            return lValue+rValue;
        case BO_Sub:
            return lValue-rValue;
        case BO_Mul:
            return lValue*rValue;
        case BO_Div:
            return lValue/rValue;
        case BO_Rem:
            return lValue%rValue;
        default:
            return -1;
    }
}

std::string VariableNameAnalysisCheck::getInterLiteralExprString(
    const BinaryOperator *BO) const {
  std::string str = "";
  if (BO) {
    const BinaryOperator *temp_bo = BO;
    if (const IntegerLiteral *IL = dyn_cast<IntegerLiteral>(
            temp_bo->getLHS()->IgnoreParenCasts())) {
      str = "++" + IL->getValue().toString(10, true) + str;
    } else if (const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(
                   temp_bo->getLHS()->IgnoreParenCasts())) {
      str = "++" + DRE->getDecl()->getDeclName().getAsString() + str;
    } else {
    }
    if (const IntegerLiteral *IR = dyn_cast<IntegerLiteral>(
            temp_bo->getRHS()->IgnoreParenCasts())) {
      str = str + "++" + IR->getValue().toString(10, true);
    } else if (const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(
                   temp_bo->getRHS()->IgnoreParenCasts())) {
      str = str + "++" + DRE->getDecl()->getDeclName().getAsString();
    } else {
    }
    if (const BinaryOperator *BOL = dyn_cast<BinaryOperator>(
            temp_bo->getLHS()->IgnoreParenCasts())) {
      str = getInterLiteralExprString(BOL) + str;
    }
    if (const BinaryOperator *BOR = dyn_cast<BinaryOperator>(
            temp_bo->getRHS()->IgnoreParenCasts())) {
      str = str + getInterLiteralExprString(BOR);
    }
  }
  return str;
}

std::string VariableNameAnalysisCheck::getArraySubscriptExprString(
    const ArraySubscriptExpr *ASE) const {
  std::string str = "";
  if (ASE) {
    const ArraySubscriptExpr *temp_ase = ASE;
    while (true) {
      if (const IntegerLiteral *IL = dyn_cast<IntegerLiteral>(
              temp_ase->getRHS()->IgnoreParenCasts())) {
        str = "++array-index" + IL->getValue().toString(10, true) + str;
      } else if (const BinaryOperator *BO = dyn_cast<BinaryOperator>(
                     temp_ase->getRHS()->IgnoreParenCasts())) {
        //不再使用getInterLiteralExprString函数，而是用calcIndex计算下标，Cheer22.6.3
        if(calcIndex(BO) != -1)
        {
            str = "++array-index" + to_string(calcIndex(BO)) + str;
        }
        else
        {
           str = "";
           break;
        }
      } else {
        str = "";
        break;
      }
      if (const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(
              temp_ase->getLHS()->IgnoreParenCasts())) {
        str = DRE->getDecl()->getDeclName().getAsString() + str;
        break;
      } else if (const ArraySubscriptExpr *LASE = dyn_cast<ArraySubscriptExpr>(
                     temp_ase->getLHS()->IgnoreParenCasts())) {
        temp_ase = LASE;
      } else if (const MemberExpr *LME = dyn_cast<MemberExpr>(
                     temp_ase->getLHS()->IgnoreParenCasts())) {
        str = getMemberExprString(LME) + str;
        break;
      } else {
        break;
      }
    }
  }
  return str;
}

std::string VariableNameAnalysisCheck::getMemberExprString(
    const MemberExpr *ME) const {
  std::string str = "";
  if (ME) {
    const MemberExpr *temp_me = ME;
    while (true) {
      str = ".." + temp_me->getMemberDecl()->getDeclName().getAsString() + str;
      if (const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(
              temp_me->getBase()->IgnoreParenCasts())) {
        str = DRE->getDecl()->getDeclName().getAsString() + str;
        break;
      } else if (const MemberExpr *BME = dyn_cast<MemberExpr>(
                     temp_me->getBase()->IgnoreParenCasts())) {
        temp_me = BME;
      } else if (const ArraySubscriptExpr *BASE = dyn_cast<ArraySubscriptExpr>(
                     temp_me->getBase()->IgnoreParenCasts())) {
        str = getArraySubscriptExprString(BASE) + str;
        break;
      } else {
        break;
      }
    }
  }
  return str;
}

std::string VariableNameAnalysisCheck::getDeclRefExprString(
    const DeclRefExpr *DRE) const {
  std::string str = "";
  if (DRE) {
    str = DRE->getDecl()->getDeclName().getAsString();
  }
  return str;
}

bool VariableNameAnalysisCheck::shouldCheck(string keyword1, string keyword2)
{
    set<string> temp;
    temp.insert(keyword_levelset_map[keyword1].begin(), keyword_levelset_map[keyword1].end());
    temp.insert(keyword_levelset_map[keyword2].begin(), keyword_levelset_map[keyword2].end());
    
    return ((keyword_levelset_map[keyword1].size()+keyword_levelset_map[keyword2].size()) != temp.size());
}

void VariableNameAnalysisCheck::reportBug(string key_word1, string key_word2, const Expr* MatchedExpr)
{
    string lhs_level = "", rhs_level = "";
    //注意优先处理用户自定义
    if(lhs_level == "") 
    {
        lhs_level = get_custom_string_level(key_word1, custom_string_level_map);
    }
    if(lhs_level == "") 
    {
        lhs_level = get_default_string_level(key_word1, default_string_level_map);
    }

    if(rhs_level == "") 
    {
        rhs_level = get_custom_string_level(key_word2, custom_string_level_map);
    }
    if(rhs_level == "") 
    {
        rhs_level = get_default_string_level(key_word2, default_string_level_map);
    }

    if(lhs_level != "" && rhs_level != "" && shouldCheck(key_word1, key_word2)) 
    {
        if(lhs_level == rhs_level) 
        {
            return;
        } 
        else
        {
            // cout << "************" << endl;
            // cout << "key_word1: " << key_word1 << endl;
            // cout << "lhs_level: " << lhs_level << endl;
            // cout << "key_word2: " << key_word2 << endl;
            // cout << "rhs_level: " << rhs_level << endl;
            if(expr_binaryOperator_map.find(MatchedExpr) != expr_binaryOperator_map.end())
            {
                diag(expr_binaryOperator_map[MatchedExpr]->getBeginLoc(),"左值与右值不匹配");
            }
            else if(expr_varDecl_map.find(MatchedExpr) != expr_varDecl_map.end())
            {
                diag(expr_varDecl_map[MatchedExpr]->getBeginLoc(),"左值与右值不匹配");
            }
            else
            {
                diag(MatchedExpr->getBeginLoc(),"左值与右值不匹配");
            }
        }
    }
}

vector<Expr*> VariableNameAnalysisCheck::handlingBinaryOperator(const BinaryOperator *CircleBinaryOperator)
{
    vector<Expr*>vecExpr;
    queue<const BinaryOperator*> BinaryOperatorQueue;
    BinaryOperatorQueue.push(CircleBinaryOperator);
    while(!BinaryOperatorQueue.empty())
    {
        const BinaryOperator *FrontBinaryOperator=BinaryOperatorQueue.front();
        BinaryOperatorQueue.pop();
        if(isa<BinaryOperator>(FrontBinaryOperator->getLHS()->IgnoreParenCasts()))
        {
            const BinaryOperator *LeftBinaryOperator = dyn_cast<BinaryOperator>(FrontBinaryOperator->getLHS()->IgnoreParenCasts());
            BinaryOperatorQueue.push(LeftBinaryOperator);
        } 
        else 
        {
            vecExpr.push_back(FrontBinaryOperator->getLHS()->IgnoreParenCasts());
        }
        if(isa<BinaryOperator>(FrontBinaryOperator->getRHS()->IgnoreParenCasts()))
        {
            const BinaryOperator *RightBinaryOperator = dyn_cast<BinaryOperator>(FrontBinaryOperator->getRHS()->IgnoreParenCasts());
            BinaryOperatorQueue.push(RightBinaryOperator);
        } 
        else 
        {
            vecExpr.push_back(FrontBinaryOperator->getRHS()->IgnoreParenCasts());
        }
    }
    return vecExpr;
}

void VariableNameAnalysisCheck::doCheck(string expr_name, string keyword, vector<const Expr*> expr_vector, size_t &index)
{
    for(int i=0; i<expr_vector.size(); i++)
    {
        const Expr* expr = expr_vector.at(i);
        if(isa<DeclRefExpr>(expr)) 
        {
            const DeclRefExpr *MatchedDeclRefExpr = dyn_cast<DeclRefExpr>(expr);
            string key_word=MatchedDeclRefExpr->getNameInfo().getName().getAsString();

            if(case_insensitive_find_in_negative_vector(key_word, negative_string_vector, index)) 
            {
                return;
            }

            //如果左右变量是相似的，那么就说明它们是同组变量，不符合极性特征
            string sub_name1, sub_name2;
            int start_pos1 = 0;
            int start_pos2 = 0;
            if(analysisNameSimilarity(default_string_list_sorted_by_lenth, custom_string_list_sorted_by_lenth, 
                    expr_name, key_word, sub_name1, sub_name2, start_pos1, start_pos2) )
            {
                return;
            }

            if(variable_keyword_map.find(key_word)!=variable_keyword_map.end()) 
            {
                for(auto iter=variable_keyword_map[key_word].begin(); iter!=variable_keyword_map[key_word].end(); iter++)
                {
                    string realKeyword;
                    realKeyword = *iter;
                    reportBug(keyword, realKeyword, MatchedDeclRefExpr);
                }
            }
            
        }
        else if(isa<MemberExpr>(expr))
        {
            const MemberExpr *ME = dyn_cast<MemberExpr>(expr);
            bool Check=false;
            string key_word = getVarName(ME);
            
            //对负向字符串进行过滤
            for (int i = 0; i < negative_string_vector.size(); i++) 
            {
                if (key_word.find(negative_string_vector[i]) != key_word.npos) 
                {
                    return;
                }
            }

            //如果左右变量是相似的，那么就说明它们是同组变量，不符合极性特征
            string sub_name1, sub_name2;
            int start_pos1 = 0;
            int start_pos2 = 0;
            if(analysisNameSimilarity(default_string_list_sorted_by_lenth, custom_string_list_sorted_by_lenth, 
                    expr_name, key_word, sub_name1, sub_name2, start_pos1, start_pos2) )
            {
                return;
            }
            
            if(variable_keyword_map.find(key_word)!=variable_keyword_map.end())
            {
                for(auto iter=variable_keyword_map[key_word].begin(); iter!=variable_keyword_map[key_word].end(); iter++)
                {
                    string realKeyword;
                    realKeyword = *iter;
                    reportBug(keyword, realKeyword, ME);
                }
            }
        }
        else if(isa<ArraySubscriptExpr>(expr))
        {
            const ArraySubscriptExpr *ASE = dyn_cast<ArraySubscriptExpr>(expr);
            //cout << "L:" << expr_name << endl;
            //cout << "R:" << getVarName(ASE) << endl;
            if(const IntegerLiteral *IL = dyn_cast<IntegerLiteral>(ASE->getIdx()))
            {
                //检查数组下标是否位于等价集合
                string key_word;
                if(nested_variable_check_mode == "section")                 
                {                  

                }                 
                else if(nested_variable_check_mode == "whole")                 
                {   
                    //过滤负向字库
                    if(case_insensitive_find_in_negative_vector(getVarName(ASE), negative_string_vector, index)) 
                    {
                        return;
                    }
                            
                    //如果左右变量是相似的，那么就说明它们是同组变量，不符合极性特征
                    string sub_name1, sub_name2;
                    int start_pos1 = 0;
                    int start_pos2 = 0;
                    if(analysisNameSimilarity(default_string_list_sorted_by_lenth, custom_string_list_sorted_by_lenth, 
                            expr_name, getVarName(ASE), sub_name1, sub_name2, start_pos1, start_pos2) )
                    {
                        return;
                    }  
             
                    for(auto iter=variable_keyword_map[getVarName(ASE)].begin(); iter!=variable_keyword_map[getVarName(ASE)].end(); iter++)
                    {
                        string realKeyword;
                        realKeyword = *iter;
                        reportBug(keyword, realKeyword, ASE);
                    }              
                }                 
                else                 
                {                     
                    key_word = "arrayindex"+std::to_string(IL->getValue().getSExtValue());
                    reportBug(keyword, key_word, ASE);               
                }
            } 
            else if(const BinaryOperator *BO = dyn_cast<BinaryOperator>(ASE->getIdx()))
            {
                int index = calcIndex(BO);
                string key_word;
                if(nested_variable_check_mode == "section")                                      
                {                                        

                }                                      
                else if(nested_variable_check_mode == "whole")                                      
                {   
                            
                    //如果左右变量是相似的，那么就说明它们是同组变量，不符合极性特征
                    string sub_name1, sub_name2;
                    int start_pos1 = 0;
                    int start_pos2 = 0;
                    if(analysisNameSimilarity(default_string_list_sorted_by_lenth, custom_string_list_sorted_by_lenth, 
                            expr_name, getVarName(ASE), sub_name1, sub_name2, start_pos1, start_pos2) )
                    {
                        return;
                    }     

                    for(auto iter=variable_keyword_map[getVarName(ASE)].begin(); iter!=variable_keyword_map[getVarName(ASE)].end(); iter++)
                    {
                        string realKeyword;
                        realKeyword = *iter;
                        reportBug(keyword, realKeyword, ASE);
                    }                                   
                }                                      
                else                                      
                {                                              
                    //key_word = "arrayindex"+std::to_string(IL->getValue().getSExtValue());
                    if(index != -1)
                    {
                        key_word = "arrayindex"+std::to_string(index);
                        reportBug(keyword, key_word, ASE);
                    }
                }
            }
        }
    }
}

void VariableNameAnalysisCheck::travelExpr(const Expr* matchedExpr, set<const Expr*> &exprSet)
{
    //对二元表达式的支持
    if(isa<BinaryOperator>(matchedExpr))
    {
        auto CircleBinaryOperator = dyn_cast<BinaryOperator>(matchedExpr);
        auto vecExpr = handlingBinaryOperator(CircleBinaryOperator);
        for(auto iter=vecExpr.begin(); iter!=vecExpr.end(); iter++)
        {
            travelExpr(*iter, exprSet);
        }
    }
    //对函数中参数的支持
    else if(isa<CallExpr>(matchedExpr))
    {
        auto matchedCallExpr = dyn_cast<CallExpr>(matchedExpr);
        for(auto iter=matchedCallExpr->arg_begin(); iter!=matchedCallExpr->arg_end(); iter++)
        {
            travelExpr(*iter, exprSet);
        }
    }
    //对一元表达式的支持
    else if(isa<UnaryOperator>(matchedExpr))
    {
        auto matchedUnaryOperator = dyn_cast<UnaryOperator>(matchedExpr);
        travelExpr(matchedUnaryOperator->getSubExpr(), exprSet);
    }
    else
    {
        //这里对数组进行一下处理
        if(isa<ArraySubscriptExpr>(matchedExpr))
        { 
            auto matchedArraySubscriptExpr = dyn_cast<ArraySubscriptExpr>(matchedExpr);
            //matchedArraySubscriptExpr->getLHS()->IgnoreParenCasts()->getType().dump();
            //matchedArraySubscriptExpr->IgnoreParenCasts()->getType().dump();
            if(isa<ConstantArrayType>(matchedArraySubscriptExpr->getLHS()->IgnoreParenCasts()->getType()))
            {
                auto matchedArrayType = dyn_cast<ConstantArrayType>(matchedArraySubscriptExpr->getLHS()->IgnoreParenCasts()->getType());
                int arraySize = *(matchedArrayType->getSize().getRawData());
                //cout << "arraySize:" << arraySize << endl;
                if(arraySize!=(maxIndexValue+1) && arraySize!=3)
                {
                    return;
                }
            }
        }
        exprSet.insert(matchedExpr->IgnoreParenCasts());
    }
}

} // namespace bz12
} // namespace tidy
} // namespace clang
