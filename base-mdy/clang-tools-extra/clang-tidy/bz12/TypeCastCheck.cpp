//===--- TypeCastCheck.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TypeCastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;
using namespace std;

namespace clang {
namespace tidy {
namespace bz12 {

void TypeCastCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(binaryOperator().bind("binaryOperator"), this);
  Finder->addMatcher(explicitCastExpr().bind("explicitCastExpr"), this);
  Finder->addMatcher(declStmt().bind("declStmt"), this);
  Finder->addMatcher(returnStmt().bind("returnStmt"), this);
}

  bool isLossOfPrecision(const BinaryOperator *BO) ;

  bool isLossOfPrecision(const Stmt *S) ;

  std::map<BuiltinType::Kind, std::string> builtinTypeStringMap = {
    {BuiltinType::SChar, "char"},
    {BuiltinType::Short, "short"},
    {BuiltinType::Int, "int"},
    {BuiltinType::Long, "long"},
    {BuiltinType::LongLong, "long long"},
    {BuiltinType::Float, "float"},
    {BuiltinType::Double, "double"},
    {BuiltinType::LongDouble, "long double"},
    {BuiltinType::UChar, "unsigned char"},
    {BuiltinType::UShort, "unsigned short"},
    {BuiltinType::UInt, "unsigned int"},
    {BuiltinType::ULong, "unsigned long"},
    {BuiltinType::ULongLong, "unsigned long long"}
};
  std::map<std::string, unsigned> typeLevelMap = {
    {"char", 1},
    {"unsigned char", 2},
    {"short", 3},
    {"unsigned short", 4},
    {"int", 5},
    {"unsigned int", 6},
    {"long", 7},
    {"unsigned long", 8},
    {"long long", 9},
    {"unsigned long long", 10},
    {"float", 11},
    {"double", 12},
    {"long double", 13}
};

void TypeCastCheck::check(const MatchFinder::MatchResult &Result) 
{
  if(const auto *MatcheddeclStmt = Result.Nodes.getNodeAs<DeclStmt>("declStmt"))
  {
    if(MatcheddeclStmt->isSingleDecl()) 
    {
      if(const VarDecl *varDecl = dyn_cast<VarDecl>(MatcheddeclStmt->getSingleDecl())) 
      {
        bool lossOfPrecision = isLossOfPrecision(MatcheddeclStmt);
        if(lossOfPrecision) 
        {
          diag(varDecl->getInit()->getBeginLoc(), "隐式转换中的精度损失");
        }
        if(varDecl->hasInit()) 
        {
          if(const ImplicitCastExpr *RICE = dyn_cast<ImplicitCastExpr>(varDecl->getInit())) 
          {
            if(const auto *a =dyn_cast<UnaryOperator>(RICE->IgnoreParenImpCasts()))
            {
              if(a->getOpcode()==UO_Minus)
              {
                if(RICE->getType()->isUnsignedChar()||
                    RICE->getType()->isUnsignedShortType()||
                    RICE->getType()->isUnsignedLongType()||
                    RICE->getType()->isUnsignedIntType()||
                    RICE->getType()->isUnsignedIntegerType()||
                    RICE->getType()->isUnsignedIntegerOrEnumerationType())
                {
                  if(RICE->IgnoreParenImpCasts()->getType()->isSignedChar()||
                  RICE->IgnoreParenImpCasts()->getType()->isSignedIntegerOrEnumerationType())
                  {
                    diag(MatcheddeclStmt->getBeginLoc(), "隐式转换中的符号丢失");
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  if(const auto *MatchedreturnStmt = Result.Nodes.getNodeAs<ReturnStmt>("returnStmt"))
  {
    bool lossOfPrecision = isLossOfPrecision(MatchedreturnStmt);
    if(lossOfPrecision) 
    {
      diag(MatchedreturnStmt->getRetValue()->getBeginLoc(),"隐式转换中的精度损失");
    }
    if(MatchedreturnStmt->getRetValue()) 
    {
      if(const ImplicitCastExpr *RICE = dyn_cast<ImplicitCastExpr>(MatchedreturnStmt->getRetValue())) 
      {
        if(const auto *a =dyn_cast<UnaryOperator>(RICE->IgnoreParenImpCasts()))
        {
          if(a->getOpcode()==UO_Minus)
          {
            if(RICE->getType()->isUnsignedChar()||
                RICE->getType()->isUnsignedShortType()||
                RICE->getType()->isUnsignedLongType()||
                RICE->getType()->isUnsignedIntType()||
                RICE->getType()->isUnsignedIntegerType()||
                RICE->getType()->isUnsignedIntegerOrEnumerationType())
                {
                  if(RICE->IgnoreParenImpCasts()->getType()->isSignedChar()||
                  RICE->IgnoreParenImpCasts()->getType()->isSignedIntegerOrEnumerationType())
                  {
                    diag(MatchedreturnStmt->getRetValue()->getBeginLoc(), "隐式转换中的符号丢失");
                  }
                }
          }
        }
      }
    }
  }
  if(const auto *MatchedexplicitCastExpr = Result.Nodes.getNodeAs<ExplicitCastExpr>("explicitCastExpr"))
  {
    if(MatchedexplicitCastExpr->getType()==MatchedexplicitCastExpr->IgnoreCasts()->getType()) 
    {
      diag(MatchedexplicitCastExpr->getBeginLoc(),"强制类型转换前后的类型一致，没有必要使用强制类型转换");
    }
  }
  if(const auto *MatchedbinaryOperator = Result.Nodes.getNodeAs<BinaryOperator>("binaryOperator"))
  {
    if(MatchedbinaryOperator->isCompoundAssignmentOp()) 
    {
      bool ret = isLossOfPrecision(MatchedbinaryOperator);
      if(ret) 
      {
        diag(MatchedbinaryOperator->getBeginLoc(),"隐式转换中的精度损失");
      }
      if(const ImplicitCastExpr *RICE = dyn_cast<ImplicitCastExpr>(MatchedbinaryOperator->getRHS())) 
      {
        if(const auto *a =dyn_cast<UnaryOperator>(RICE->IgnoreParenImpCasts()))
        {
          if(a->getOpcode()==UO_Minus)
          {
            if(RICE->getType()->isUnsignedChar()||
            RICE->getType()->isUnsignedShortType()||
            RICE->getType()->isUnsignedLongType()||
            RICE->getType()->isUnsignedIntType()||
            RICE->getType()->isUnsignedIntegerType()||
            RICE->getType()->isUnsignedIntegerOrEnumerationType())
            {
              if(RICE->IgnoreParenImpCasts()->getType()->isSignedChar()||
                  RICE->IgnoreParenImpCasts()->getType()->isSignedIntegerOrEnumerationType())
                {
                  diag(MatchedbinaryOperator->getBeginLoc(), "隐式转换中的符号丢失");
                }
            }
          }
        }
      } 
    } 
    else if (MatchedbinaryOperator->isAssignmentOp()) 
    {
      bool ret = isLossOfPrecision(MatchedbinaryOperator);
      if(ret) 
      {
        diag(MatchedbinaryOperator->getBeginLoc(), "隐式转换中的精度损失");
      }
      if(const ImplicitCastExpr *RICE = dyn_cast<ImplicitCastExpr>(MatchedbinaryOperator->getRHS())) 
      {
        if(const auto *a =dyn_cast<UnaryOperator>(RICE->IgnoreParenImpCasts()))
        {
          if(a->getOpcode()==UO_Minus)
          {
            if(MatchedbinaryOperator->getRHS()->getType()->isUnsignedChar()||
            MatchedbinaryOperator->getRHS()->getType()->isUnsignedShortType()||
            MatchedbinaryOperator->getRHS()->getType()->isUnsignedLongType()||
            MatchedbinaryOperator->getRHS()->getType()->isUnsignedIntType()||
            MatchedbinaryOperator->getRHS()->getType()->isUnsignedIntegerType()||
            MatchedbinaryOperator->getRHS()->getType()->isUnsignedIntegerOrEnumerationType())
            {
              if(MatchedbinaryOperator->getRHS()->IgnoreParenImpCasts()->getType()->isSignedChar()||
              MatchedbinaryOperator->getRHS()->IgnoreParenImpCasts()->getType()->isSignedIntegerOrEnumerationType())
              {
                diag(MatchedbinaryOperator->getBeginLoc(), "隐式转换中的符号丢失");
              }
            }
          }
        }
      }
    }
    else if(MatchedbinaryOperator->isMultiplicativeOp() || MatchedbinaryOperator->isAdditiveOp() || MatchedbinaryOperator->isRelationalOp()) 
    {
      if(const ImplicitCastExpr *RICE = dyn_cast<ImplicitCastExpr>(MatchedbinaryOperator->getLHS())) 
      {
        if(const auto *a =dyn_cast<UnaryOperator>(RICE->IgnoreParenImpCasts()))
        {
          if(a->getOpcode()==UO_Minus)
          {
            if(RICE->getType()->isUnsignedChar()||
                RICE->getType()->isUnsignedShortType()||
                RICE->getType()->isUnsignedLongType()||
                RICE->getType()->isUnsignedIntType()||
                RICE->getType()->isUnsignedIntegerType()||
                RICE->getType()->isUnsignedIntegerOrEnumerationType())
            {
              if(RICE->IgnoreParenImpCasts()->getType()->isSignedChar()||
              RICE->IgnoreParenImpCasts()->getType()->isSignedIntegerOrEnumerationType())
              {
                diag(MatchedbinaryOperator->getBeginLoc(), "隐式转换中的符号丢失");
              }
            }
          }
        }
      }
      if(const ImplicitCastExpr *RICE = dyn_cast<ImplicitCastExpr>(MatchedbinaryOperator->getRHS())) 
      {
        if(const auto *a =dyn_cast<UnaryOperator>(RICE->IgnoreParenImpCasts()))
        {
          if(a->getOpcode()==UO_Minus)
          {
            if(RICE->getType()->isUnsignedChar()||
                RICE->getType()->isUnsignedShortType()||
                RICE->getType()->isUnsignedLongType()||
                RICE->getType()->isUnsignedIntType()||
                RICE->getType()->isUnsignedIntegerType()||
                RICE->getType()->isUnsignedIntegerOrEnumerationType())
            {
              if(RICE->IgnoreParenImpCasts()->getType()->isSignedChar()||
                  RICE->IgnoreParenImpCasts()->getType()->isSignedIntegerOrEnumerationType())
              {
                diag(MatchedbinaryOperator->getBeginLoc(), "隐式转换中的符号丢失");
              }
            }
          }
        }
      }
    } 
    else 
    {
    }
  }
}

bool isLossOfPrecision(const Stmt *S)  
{
  if(const DeclStmt* DS = dyn_cast<DeclStmt>(S)) 
  {
    if(const VarDecl *varDecl = dyn_cast<VarDecl>(DS->getSingleDecl())) 
    {
      if(varDecl->hasInit()) 
      {
        const Expr *E = varDecl->getInit();
        if(const ImplicitCastExpr *IMP = dyn_cast<ImplicitCastExpr>(varDecl->getInit())) 
        {
          E = IMP->getSubExpr();
        }
        QualType LT = varDecl->getType();
        QualType RT = varDecl->getInit()->getType();
        if(const TypedefType *TT = dyn_cast<TypedefType>(LT)) 
        {
          LT = TT->desugar();
        }
        if(const TypedefType *TT = dyn_cast<TypedefType>(RT)) 
        {
          RT = TT->desugar();
        }
        if(const BuiltinType *LBT = dyn_cast<BuiltinType>(LT)) 
        {
          if(const BuiltinType *RBT = dyn_cast<BuiltinType>(RT)) 
          {
            std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_lhs = builtinTypeStringMap.begin();
            std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_lhs = builtinTypeStringMap.end();
            for(; iter_begin_lhs!= iter_end_lhs; iter_begin_lhs++) 
            {
              if(iter_begin_lhs->first == LBT->getKind()) 
              {
                break;
              }
            }
            if(iter_begin_lhs == iter_end_lhs) 
            {
              return false;
            }
            std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_rhs = builtinTypeStringMap.begin();
            std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_rhs = builtinTypeStringMap.end();
            for(; iter_begin_rhs!= iter_end_rhs; iter_begin_rhs++) 
            {
              if(iter_begin_rhs->first == RBT->getKind()) 
              {
                break;
              }
            }
            if(iter_begin_rhs == iter_end_rhs) 
            {
              return false;
            }
            if(iter_begin_lhs->first == BuiltinType::Short && iter_begin_rhs->first == BuiltinType::Int) 
            {
              return false;
            }
            if(iter_begin_lhs->first == BuiltinType::Float && iter_begin_rhs->first == BuiltinType::Double) 
            {
              return false;
            }
            if(typeLevelMap[builtinTypeStringMap[iter_begin_lhs->first]] < typeLevelMap[builtinTypeStringMap[iter_begin_rhs->first]]) 
            {
              return true;
            }
          }
        }
      }
    }
  } 
  else if(const ReturnStmt* RS = dyn_cast<ReturnStmt>(S)) 
  {
    if(RS->getRetValue()) 
    {
      const Expr *E = RS->getRetValue();
      if(const ImplicitCastExpr *IMP = dyn_cast<ImplicitCastExpr>(RS->getRetValue())) 
      {
        const Expr *RE = IMP->getSubExpr();
        QualType LT = E->getType();
        QualType RT = RE->getType();
        if(const TypedefType *TT = dyn_cast<TypedefType>(LT)) 
        {
          LT = TT->desugar();
        }

        if(const TypedefType *TT = dyn_cast<TypedefType>(RT)) 
        {
          RT = TT->desugar();
        }
        if(const BuiltinType *LBT = dyn_cast<BuiltinType>(LT)) 
        {
          if(const BuiltinType *RBT = dyn_cast<BuiltinType>(RT)) 
          {
            std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_lhs = builtinTypeStringMap.begin();
            std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_lhs = builtinTypeStringMap.end();
            for(; iter_begin_lhs!= iter_end_lhs; iter_begin_lhs++) 
            {
              if(iter_begin_lhs->first == LBT->getKind()) 
              {
                break;
              }
            }
            if(iter_begin_lhs == iter_end_lhs) 
            {
              return false;
            }
            std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_rhs = builtinTypeStringMap.begin();
            std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_rhs = builtinTypeStringMap.end();
            for(; iter_begin_rhs!= iter_end_rhs; iter_begin_rhs++) 
            {
              if(iter_begin_rhs->first == RBT->getKind()) 
              {
                break;
              }
            }
            if(iter_begin_rhs == iter_end_rhs) 
            {
              return false;
            }
            if(typeLevelMap[builtinTypeStringMap[iter_begin_lhs->first]] < typeLevelMap[builtinTypeStringMap[iter_begin_rhs->first]]) 
            {
              return true;
            }
          }
        }
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
  return false;
}

bool isLossOfPrecision(const BinaryOperator *BO)  
{
  if (!BO->getRHS() || !BO->getLHS())
  {
    return false;
  }
  const Expr *E = BO->getRHS();
  if(const ImplicitCastExpr *IMP = dyn_cast<ImplicitCastExpr>(BO->getRHS())) 
  {
    E = IMP->getSubExpr();
  }

  QualType LT = BO->getLHS()->getType();
  QualType RT = E->getType();
  if(const TypedefType *TT = dyn_cast<TypedefType>(LT)) 
  {
    LT = TT->desugar();
  }

  if(const TypedefType *TT = dyn_cast<TypedefType>(RT)) 
  {
    RT = TT->desugar();
  }
  if(const auto *E2=dyn_cast<IntegerLiteral>(E->IgnoreParenImpCasts()))
  {
    if(const BuiltinType *LBT = dyn_cast<BuiltinType>(LT)) 
    {
      if(const BuiltinType *RBT = dyn_cast<BuiltinType>(RT)) 
      {
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_lhs = builtinTypeStringMap.begin();
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_lhs = builtinTypeStringMap.end();
        for(; iter_begin_lhs!= iter_end_lhs; iter_begin_lhs++) 
        {
          if(iter_begin_lhs->first == LBT->getKind()) 
          {
            break;
          }
        }
        if(iter_begin_lhs == iter_end_lhs) 
        {
          return false;
        }
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_rhs = builtinTypeStringMap.begin();
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_rhs = builtinTypeStringMap.end();
        for(; iter_begin_rhs!= iter_end_rhs; iter_begin_rhs++) 
        {
          if(iter_begin_rhs->first == RBT->getKind()) 
          {
            break;
          }
        }
        if(iter_begin_rhs == iter_end_rhs) 
        {
          return false;
        }

        if(iter_begin_lhs->first == BuiltinType::Short && iter_begin_rhs->first == BuiltinType::Int) 
        {
          return false;
        }

        if(iter_begin_lhs->first == BuiltinType::UShort && iter_begin_rhs->first == BuiltinType::Int) 
        {
          return false;
        }

        if(iter_begin_lhs->first == BuiltinType::Float && iter_begin_rhs->first == BuiltinType::Double) 
        {
          return false;
        }

        if(typeLevelMap[builtinTypeStringMap[iter_begin_lhs->first]] < typeLevelMap[builtinTypeStringMap[iter_begin_rhs->first]]) 
        {
          return true;
        }
      }
    }
  }
  if(const auto *E2=dyn_cast<FloatingLiteral>(E->IgnoreParenImpCasts()))
  {
    if(const BuiltinType *LBT = dyn_cast<BuiltinType>(LT)) 
    {
      if(const BuiltinType *RBT = dyn_cast<BuiltinType>(RT)) 
      {
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_lhs = builtinTypeStringMap.begin();
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_lhs = builtinTypeStringMap.end();
        for(; iter_begin_lhs!= iter_end_lhs; iter_begin_lhs++) 
        {
          if(iter_begin_lhs->first == LBT->getKind()) 
          {
            break;
          }
        }
        if(iter_begin_lhs == iter_end_lhs) 
        {
          return false;
        }
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_rhs = builtinTypeStringMap.begin();
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_rhs = builtinTypeStringMap.end();
        for(; iter_begin_rhs!= iter_end_rhs; iter_begin_rhs++) 
        {
          if(iter_begin_rhs->first == RBT->getKind()) 
          {
            break;
          }
        }
        if(iter_begin_rhs == iter_end_rhs) 
        {
          return false;
        }
        if(iter_begin_lhs->first == BuiltinType::Short && iter_begin_rhs->first == BuiltinType::Int) 
        {
          return false;
        }
        if(iter_begin_lhs->first == BuiltinType::UShort && iter_begin_rhs->first == BuiltinType::Int) 
        {
          return false;
        }
        if(iter_begin_lhs->first == BuiltinType::Float && iter_begin_rhs->first == BuiltinType::Double) 
        {
          return false;
        }
        if(typeLevelMap[builtinTypeStringMap[iter_begin_lhs->first]] < typeLevelMap[builtinTypeStringMap[iter_begin_rhs->first]]) 
        {
          return true;
        }
      }
    }
  }
  else
  {
    if(const BuiltinType *LBT = dyn_cast<BuiltinType>(LT)) 
    {
      if(const BuiltinType *RBT = dyn_cast<BuiltinType>(RT)) 
      {
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_lhs = builtinTypeStringMap.begin();
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_lhs = builtinTypeStringMap.end();
        for(; iter_begin_lhs!= iter_end_lhs; iter_begin_lhs++) 
        {
          if(iter_begin_lhs->first == LBT->getKind()) 
          {
            break;
          }
        }
        if(iter_begin_lhs == iter_end_lhs) 
        {
          return false;
        }
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_rhs = builtinTypeStringMap.begin();
        std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_rhs = builtinTypeStringMap.end();
        for(; iter_begin_rhs!= iter_end_rhs; iter_begin_rhs++) 
        {
          if(iter_begin_rhs->first == RBT->getKind()) 
          {
            break;
          }
        }
        if(iter_begin_rhs == iter_end_rhs) 
        {
          return false;
        }
        if(iter_begin_lhs->first == BuiltinType::Short && iter_begin_rhs->first == BuiltinType::Int) 
        {
          return false;
        }
        if(iter_begin_lhs->first == BuiltinType::UShort && iter_begin_rhs->first == BuiltinType::Int) 
        {
          return true;
        }
        if(iter_begin_lhs->first == BuiltinType::Float && iter_begin_rhs->first == BuiltinType::Double) 
        {
          return true;
        }
        if(typeLevelMap[builtinTypeStringMap[iter_begin_lhs->first]] < typeLevelMap[builtinTypeStringMap[iter_begin_rhs->first]]) 
        {
          return true;
        }
      }
    }
  }
  return false;
}



} // namespace bz12
} // namespace tidy
} // namespace clang
