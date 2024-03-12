//=== TypeCastChecker.cpp -------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Check that there is no loss of sign/precision in assignments, comparisons
// and multiplications.
//
// TypeCastChecker uses path sensitive analysis to determine possible values
// of expressions. A warning is reported when:
// * a negative value is implicitly converted to an unsigned value in an
//   assignment, comparison or multiplication.
// * assignment / initialization when the source value is greater than the max
//   value of the target integer type
// * assignment / initialization when the source integer is above the range
//   where the target floating point type can represent all integers
//
// Many compilers and tools have similar checks that are based on semantic
// analysis. Those checks are sound but have poor precision. TypeCastChecker
// is an alternative to those checks.
//
//===----------------------------------------------------------------------===//
#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/ParentMap.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h" 
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "llvm/ADT/APFloat.h"

#include <climits>

using namespace clang;
using namespace ento;

namespace {
class TypeCastChecker : public Checker<check::PreStmt<BinaryOperator>,
                                       check::PreStmt<ExplicitCastExpr>,
                                       check::PreStmt<DeclStmt>,
                                       check::PreStmt<ReturnStmt>> {
public:
    void checkPreStmt(const BinaryOperator *BO, CheckerContext &C) const;
    void checkPreStmt(const ExplicitCastExpr *ECE, CheckerContext &C) const;
    void checkPreStmt(const DeclStmt *DS, CheckerContext &C) const;
    void checkPreStmt(const ReturnStmt *RS, CheckerContext &C) const;

private:
    mutable std::unique_ptr<BuiltinBug> BT;

    bool isLossOfPrecision(const BinaryOperator *BO, CheckerContext &C) const;

    bool isLossOfPrecision(const Stmt *S, CheckerContext &C) const;

    bool isLossOfSign(const ImplicitCastExpr *Cast, CheckerContext &C) const;

    void reportBug(ExplodedNode *N, const Expr *E, CheckerContext &C,
                    const char Msg[]) const;
    
    mutable std::map<BuiltinType::Kind, std::string> builtinTypeStringMap = {
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
    mutable std::map<std::string, unsigned> typeLevelMap = {
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
};
}

void TypeCastChecker::checkPreStmt(const DeclStmt *DS, CheckerContext &C) const {
    if(DS->isSingleDecl()) {
        if(const VarDecl *varDecl = dyn_cast<VarDecl>(DS->getSingleDecl())) {
            bool lossOfPrecision = isLossOfPrecision(DS, C);
            if(lossOfPrecision) {
                ExplodedNode *N = C.generateNonFatalErrorNode(C.getState());
                if (!N)
                    return;
                reportBug(N, varDecl->getInit(), C, "隐式转换中的精度损失");
            }

            if(varDecl->hasInit()) {
                if(const ImplicitCastExpr *RICE = dyn_cast<ImplicitCastExpr>(varDecl->getInit())) {
                    bool LossOfSign = isLossOfSign(RICE, C);
                    if(LossOfSign) {
                        ExplodedNode *N = C.generateNonFatalErrorNode(C.getState());
                        if (!N)
                            return;
                        reportBug(N, varDecl->getInit(), C, "隐式转换中的符号丢失");
                    }
                }
            }
        }
    }
}

void TypeCastChecker::checkPreStmt(const ReturnStmt *RS, CheckerContext &C) const {
    bool lossOfPrecision = isLossOfPrecision(RS, C);
    if(lossOfPrecision) {
        ExplodedNode *N = C.generateNonFatalErrorNode(C.getState());
        if (!N)
            return;
        reportBug(N, RS->getRetValue(), C, "隐式转换中的精度损失");
    }

    if(RS->getRetValue()) {
        if(const ImplicitCastExpr *RICE = dyn_cast<ImplicitCastExpr>(RS->getRetValue())) {
            bool LossOfSign = isLossOfSign(RICE, C);
            if(LossOfSign) {
                ExplodedNode *N = C.generateNonFatalErrorNode(C.getState());
                if (!N)
                    return;
                reportBug(N, RS->getRetValue(), C, "隐式转换中的符号丢失");
            }
        }
    }
}

void TypeCastChecker::checkPreStmt(const ExplicitCastExpr *ECE,
                                     CheckerContext &C) const {
    if(ECE->getType()==ECE->IgnoreCasts()->getType()) {
        ExplodedNode *N = C.generateNonFatalErrorNode(C.getState());
        if (!N)
            return;
        reportBug(N, ECE, C, "强制类型转换前后的类型一致，没有必要使用强制类型转换");
    }
    
}

void TypeCastChecker::checkPreStmt(const BinaryOperator *BO,
                                     CheckerContext &C) const {
    if(BO->isCompoundAssignmentOp()) {
        bool ret = isLossOfPrecision(BO, C);
        if(ret) {
            ExplodedNode *N = C.generateNonFatalErrorNode(C.getState());
            if (!N)
                return;
            reportBug(N, BO, C, "隐式转换中的精度损失");
        }
        if(const ImplicitCastExpr *RICE = dyn_cast<ImplicitCastExpr>(BO->getRHS())) {
            bool LossOfSign = isLossOfSign(RICE, C);
            if(LossOfSign) {
                ExplodedNode *N = C.generateNonFatalErrorNode(C.getState());
                if (!N)
                    return;
                reportBug(N, BO, C, "隐式转换中的符号丢失");
            }
        }
    } else if (BO->isAssignmentOp()) {
        bool ret = isLossOfPrecision(BO, C);
        if(ret) {
            ExplodedNode *N = C.generateNonFatalErrorNode(C.getState());
            if (!N)
                return;
            reportBug(N, BO, C, "隐式转换中的精度损失");
        }
        if(const ImplicitCastExpr *RICE = dyn_cast<ImplicitCastExpr>(BO->getRHS())) {
            bool LossOfSign = isLossOfSign(RICE, C);
            if(LossOfSign) {
                ExplodedNode *N = C.generateNonFatalErrorNode(C.getState());
                if (!N)
                    return;
                reportBug(N, BO, C, "隐式转换中的符号丢失");
            }
        }
    } else if(BO->isMultiplicativeOp() || BO->isAdditiveOp() || BO->isRelationalOp()) {
        if(const ImplicitCastExpr *LICE = dyn_cast<ImplicitCastExpr>(BO->getLHS())) {
            bool LossOfSign = isLossOfSign(LICE, C);
            if(LossOfSign) {
                ExplodedNode *N = C.generateNonFatalErrorNode(C.getState());
                if (!N)
                    return;
                reportBug(N, BO, C, "隐式转换中的符号丢失");
            }
        }
        if(const ImplicitCastExpr *RICE = dyn_cast<ImplicitCastExpr>(BO->getRHS())) {
            bool LossOfSign = isLossOfSign(RICE, C);
            if(LossOfSign) {
                ExplodedNode *N = C.generateNonFatalErrorNode(C.getState());
                if (!N)
                    return;
                reportBug(N, BO, C, "隐式转换中的符号丢失");
            }
        }
    } else {
    }
}

void TypeCastChecker::reportBug(ExplodedNode *N, const Expr *E,
                                  CheckerContext &C, const char Msg[]) const {
    if (!BT)
        BT.reset(
            new BuiltinBug(this, "Conversion", "Possible loss of sign/precision."));

    // Generate a report for this bug.
    auto R = std::make_unique<PathSensitiveBugReport>(*BT, Msg, N);
    bugreporter::trackExpressionValue(N, E, *R);
    C.emitReport(std::move(R));
}

bool TypeCastChecker::isLossOfPrecision(const Stmt *S,
                                          CheckerContext &C) const {
    if(const DeclStmt* DS = dyn_cast<DeclStmt>(S)) {
        if(const VarDecl *varDecl = dyn_cast<VarDecl>(DS->getSingleDecl())) {
            if(varDecl->hasInit()) {
                const Expr *E = varDecl->getInit();
                if(const ImplicitCastExpr *IMP = dyn_cast<ImplicitCastExpr>(varDecl->getInit())) {
                    E = IMP->getSubExpr();
                }

                QualType LT = varDecl->getType();
                QualType RT = varDecl->getInit()->getType();

                if(const TypedefType *TT = dyn_cast<TypedefType>(LT)) {
                    LT = TT->desugar();
                }

                if(const TypedefType *TT = dyn_cast<TypedefType>(RT)) {
                    RT = TT->desugar();
                }
                if(const BuiltinType *LBT = dyn_cast<BuiltinType>(LT)) {
                    if(const BuiltinType *RBT = dyn_cast<BuiltinType>(RT)) {
                        std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_lhs = builtinTypeStringMap.begin();
                        std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_lhs = builtinTypeStringMap.end();
                        for(; iter_begin_lhs!= iter_end_lhs; iter_begin_lhs++) {
                            if(iter_begin_lhs->first == LBT->getKind()) {
                                break;
                            }
                        }
                        if(iter_begin_lhs == iter_end_lhs) {
                            return false;
                        }

                        std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_rhs = builtinTypeStringMap.begin();
                        std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_rhs = builtinTypeStringMap.end();
                        for(; iter_begin_rhs!= iter_end_rhs; iter_begin_rhs++) {
                            if(iter_begin_rhs->first == RBT->getKind()) {
                                break;
                            }
                        }
                        if(iter_begin_rhs == iter_end_rhs) {
                            return false;
                        }

                        if(E->isEvaluatable(C.getASTContext()) && iter_begin_lhs->first == BuiltinType::Short && iter_begin_rhs->first == BuiltinType::Int) {
                            return false;
                        }

                        if(E->isEvaluatable(C.getASTContext()) && iter_begin_lhs->first == BuiltinType::Float && iter_begin_rhs->first == BuiltinType::Double) {
                            return false;
                        }

                        if(typeLevelMap[builtinTypeStringMap[iter_begin_lhs->first]] < typeLevelMap[builtinTypeStringMap[iter_begin_rhs->first]]) {
                            return true;
                        }
                    }
                }
            }
                
        }
    } else if(const ReturnStmt* RS = dyn_cast<ReturnStmt>(S)) {
        if(RS->getRetValue()) {
            const Expr *E = RS->getRetValue();

            if(const ImplicitCastExpr *IMP = dyn_cast<ImplicitCastExpr>(RS->getRetValue())) {
                const Expr *RE = IMP->getSubExpr();

                QualType LT = E->getType();
                QualType RT = RE->getType();

                if(const TypedefType *TT = dyn_cast<TypedefType>(LT)) {
                    LT = TT->desugar();
                }

                if(const TypedefType *TT = dyn_cast<TypedefType>(RT)) {
                    RT = TT->desugar();
                }

                if(const BuiltinType *LBT = dyn_cast<BuiltinType>(LT)) {
                    if(const BuiltinType *RBT = dyn_cast<BuiltinType>(RT)) {
                        std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_lhs = builtinTypeStringMap.begin();
                        std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_lhs = builtinTypeStringMap.end();
                        for(; iter_begin_lhs!= iter_end_lhs; iter_begin_lhs++) {
                            if(iter_begin_lhs->first == LBT->getKind()) {
                                break;
                            }
                        }
                        if(iter_begin_lhs == iter_end_lhs) {
                            return false;
                        }

                        std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_rhs = builtinTypeStringMap.begin();
                        std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_rhs = builtinTypeStringMap.end();
                        for(; iter_begin_rhs!= iter_end_rhs; iter_begin_rhs++) {
                            if(iter_begin_rhs->first == RBT->getKind()) {
                                break;
                            }
                        }
                        if(iter_begin_rhs == iter_end_rhs) {
                            return false;
                        }

                        if(typeLevelMap[builtinTypeStringMap[iter_begin_lhs->first]] < typeLevelMap[builtinTypeStringMap[iter_begin_rhs->first]]) {
                            return true;
                        }
                    }
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    return false;
}

bool TypeCastChecker::isLossOfPrecision(const BinaryOperator *BO,
                                          CheckerContext &C) const {
    if (!BO->getRHS() || !BO->getLHS())
    {
        return false;
    }
    const Expr *E = BO->getRHS();

    if(const ImplicitCastExpr *IMP = dyn_cast<ImplicitCastExpr>(BO->getRHS())) {
        E = IMP->getSubExpr();
    }

    QualType LT = BO->getLHS()->getType();
    QualType RT = E->getType();

    if(const TypedefType *TT = dyn_cast<TypedefType>(LT)) {
        LT = TT->desugar();
    }

    if(const TypedefType *TT = dyn_cast<TypedefType>(RT)) {
        RT = TT->desugar();
    }

    if(const BuiltinType *LBT = dyn_cast<BuiltinType>(LT)) {
        if(const BuiltinType *RBT = dyn_cast<BuiltinType>(RT)) {
            std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_lhs = builtinTypeStringMap.begin();
            std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_lhs = builtinTypeStringMap.end();
            for(; iter_begin_lhs!= iter_end_lhs; iter_begin_lhs++) {
                if(iter_begin_lhs->first == LBT->getKind()) {
                    break;
                }
            }
            if(iter_begin_lhs == iter_end_lhs) {
                return false;
            }

            std::map<BuiltinType::Kind, std::string>::const_iterator iter_begin_rhs = builtinTypeStringMap.begin();
            std::map<BuiltinType::Kind, std::string>::const_iterator iter_end_rhs = builtinTypeStringMap.end();
            for(; iter_begin_rhs!= iter_end_rhs; iter_begin_rhs++) {
                if(iter_begin_rhs->first == RBT->getKind()) {
                    break;
                }
            }
            if(iter_begin_rhs == iter_end_rhs) {
                return false;
            }

            if(E->isEvaluatable(C.getASTContext()) && iter_begin_lhs->first == BuiltinType::Short && iter_begin_rhs->first == BuiltinType::Int) {
                return false;
            }

            if(E->isEvaluatable(C.getASTContext()) && iter_begin_lhs->first == BuiltinType::UShort && iter_begin_rhs->first == BuiltinType::Int) {
                return false;
            }

            if(E->isEvaluatable(C.getASTContext()) && iter_begin_lhs->first == BuiltinType::Float && iter_begin_rhs->first == BuiltinType::Double) {
                return false;
            }

            if(typeLevelMap[builtinTypeStringMap[iter_begin_lhs->first]] < typeLevelMap[builtinTypeStringMap[iter_begin_rhs->first]]) {
                return true;
            }
        }
    }

    return false;
}

bool TypeCastChecker::isLossOfSign(const ImplicitCastExpr *Cast,
                                     CheckerContext &C) const {
    QualType CastType = Cast->getType();
    QualType SubType = Cast->IgnoreParenImpCasts()->getType();

    if (!CastType->isUnsignedIntegerType() || !SubType->isSignedIntegerType())
        return false;

    return C.isNegative(Cast->getSubExpr()->IgnoreParenImpCasts());
}

void ento::registerTypeCastChecker(CheckerManager &mgr) {
    mgr.registerChecker<TypeCastChecker>();
}

bool ento::shouldRegisterTypeCastChecker(const CheckerManager &mgr) {
    return true;
}
