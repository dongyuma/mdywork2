/*!
 * FileName: BinaryOperatorChecker.cpp
 *
 * Author:   ZhangChaoZe
 * Date:     2021-2-1
 * Code:     UTF-8 without BOM
 * LineFeed: Unix LF
 * 
 * 
 * Modified: Yuxinglin
 * Date: 2021-11-19
 * Describe: 检测GJB8114 R-1-13-15 禁止给无符号变量赋负值
 * 修改误报太多的bug
 */

#include <iostream>
#include <set>

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTNodeTraverser.h"
#include "clang/AST/ParentMap.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

using namespace clang;
using namespace ento;
namespace
{
	class BinaryOperatorChecker :public Checker< check::PreStmt<BinaryOperator>, check::PreStmt<UnaryOperator>, check::PreStmt<ImplicitCastExpr>, check::PreStmt<ExplicitCastExpr>, check::ASTDecl<FieldDecl>, check::PreStmt<DeclStmt> >
	{
	public:
		void checkPreStmt(const BinaryOperator *B, CheckerContext &C) const;

		void checkPreStmt(const ImplicitCastExpr *Cast, CheckerContext &C) const;

		void checkPreStmt(const ExplicitCastExpr *Cast, CheckerContext &C) const;

		void checkPreStmt(const DeclStmt *DS, CheckerContext &C) const;

		void checkASTDecl(const FieldDecl *FD, AnalysisManager &Mgr, BugReporter &BR) const;

		void checkPreStmt(const UnaryOperator *U, CheckerContext &C) const;

	private:
		//检测a b类型是否一致 是否可用于赋值操作
		void checkAssignmentType(const QualType &a, const QualType &b, const Decl *D, CheckerContext &C, const PathDiagnosticLocation &ELoc, const SourceRange &SR) const;

		void checkPointerTypeLevel(const QualType &t, const int max_level, const Decl *D, CheckerContext &C, const PathDiagnosticLocation &ELoc, const SourceRange &SR) const;

		void checkFunctionPointerType(const QualType &T, const Decl *D, CheckerContext &C, const PathDiagnosticLocation &ELoc, const SourceRange &SR) const;

		//检测强制类型转换Cast是否可以直接赋值给类型T的变量
		void checkPointerExplicitAssignment(const QualType &T, const ExplicitCastExpr *Cast, const Decl *D, CheckerContext &C, const PathDiagnosticLocation &ELoc, const SourceRange &SR) const;
	};
}

static bool isBooleanType(const QualType &Ty) {
	if (Ty->isBooleanType()) // C++ or C99
		return true;

	if (const TypedefType *TT = Ty->getAs<TypedefType>())
		return TT->getDecl()->getName() == "BOOL" || // Objective-C
		TT->getDecl()->getName() == "_Bool" || // stdbool.h < C99
		TT->getDecl()->getName() == "Boolean";  // MacTypes.h

	return false;
}

static bool isNullToPointerType(const QualType &T, const Expr *E, ASTContext &C)
{
	bool res = false;
	QualType Tmp = T->isReferenceType() ? T.getNonReferenceType() : T;
	Tmp = QualType(T->getUnqualifiedDesugaredType(), 0);

	QualType Expr_type = E->getType();
	Expr_type = Expr_type->isReferenceType() ? Expr_type.getNonReferenceType() : Expr_type;
	Expr_type = QualType(Expr_type->getUnqualifiedDesugaredType(), 0);

	if (Tmp->isPointerType() && (E->isNullPointerConstant(C, Expr::NPC_ValueDependentIsNull) || Expr_type->isNullPtrType()))
	{
		res = true;
	}
	return res;
}

static bool isEquivalentQualType(const QualType &a, const QualType &b)
{
	bool res = false;
	QualType T1 = a->isReferenceType() ? a.getNonReferenceType() : a;
	QualType T2 = b->isReferenceType() ? b.getNonReferenceType() : b;

	T1 = QualType(T1->getUnqualifiedDesugaredType(), 0);
	T2 = QualType(T2->getUnqualifiedDesugaredType(), 0);

	//后期优化更多数据类型的比较
	if (T1->isBuiltinType() || T1->isEnumeralType() || T1->isStructureType())
	{
		res = T1 == T2;
	}
	else if (T1->isPointerType() && !T1->isFunctionPointerType())
	{
		if (T2->isPointerType() || T2->isArrayType())
		{
			//如果源也是指针或数组则继续比较子元素类型
			res = isEquivalentQualType(QualType(T1->getPointeeOrArrayElementType(), 0), QualType(T2->getPointeeOrArrayElementType(), 0));
		}
		else
		{
			res = false;
		}
	}
	else
	{
		//其余类型(函数指针 类什么的) 暂时不进行检测
		res = true;
	}
	return res;
}


static bool IsPointerLevelGreaterThan(const QualType T,const int max_level)
{
	bool res = false;
	int level = 0;
	QualType Tmp = T->isReferenceType() ? T.getNonReferenceType() : T;
	Tmp = QualType(Tmp->getUnqualifiedDesugaredType(), 0);
	while (Tmp->isPointerType())
	{
		++level;
		if (level > max_level)
		{
			break;
		}
		Tmp = QualType(Tmp->getPointeeOrArrayElementType(), 0);
		Tmp = Tmp->isReferenceType() ? Tmp.getNonReferenceType() : Tmp;
		Tmp = QualType(Tmp->getUnqualifiedDesugaredType(), 0);
	}
	if (level > max_level)
	{
		res = true;
	}
	return res;
}


void BinaryOperatorChecker::checkPreStmt(const BinaryOperator *B, CheckerContext &C) const
{
	BinaryOperatorKind bo_kind = B->getOpcode();
	AnalysisDeclContext *AC = C.getCurrentAnalysisDeclContext();
	ASTContext &ASC = C.getASTContext();

	if (!C.getSourceManager().isInMainFile(B->getBeginLoc()))
	{
		return;
	}

	//移位运算
	if (bo_kind == BinaryOperatorKind::BO_Shl || bo_kind == BinaryOperatorKind::BO_ShlAssign || bo_kind == BinaryOperatorKind::BO_Shr || bo_kind == BinaryOperatorKind::BO_ShrAssign)
	{
		//<< >> <<= >>= 移位运算检查左值
#if 0
		int b = 1;
		b = b >> 3; //warn
		b = 2 + b >> 3; //warn
		b = (unsigned int)2 + b >> 3; //no warn
#endif
		Expr *L_exper = B->getLHS();
		if (L_exper)
		{
			const Type *L_type = L_exper->getType().getTypePtrOrNull();
			if (L_type && !L_type->hasUnsignedIntegerRepresentation())
			{
				//移位运算的左值必须是无符号类型
				PathDiagnosticLocation ELoc(B->getOperatorLoc(), C.getSourceManager());
				C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "移位错误类型", categories::SecurityError, "移位运算的左值不是无符号类型", ELoc, L_exper->getSourceRange());
			}
		}
	}

	//位运算
	if ((bo_kind >= BinaryOperatorKind::BO_And && bo_kind <= BinaryOperatorKind::BO_Or) || (bo_kind >= BinaryOperatorKind::BO_AndAssign && bo_kind <= BinaryOperatorKind::BO_OrAssign))
	{
		Expr *L_exper = B->getLHS();
		Expr *R_exper = B->getRHS();
		if (L_exper)
		{
			const Type *L_type = L_exper->getType().getTypePtrOrNull();
			const Type *R_type = R_exper != NULL ? R_exper->getType().getTypePtrOrNull() : NULL;

			if (L_type)
			{
				//先判断左值和右值是否为bool类型
				bool left_is_boolean_type = false, right_is_boolean_type = false;
				left_is_boolean_type = L_type->isBooleanType();
				if (!left_is_boolean_type && L_exper->getStmtClass() == Stmt::ImplicitCastExprClass)
				{
					ImplicitCastExpr *imce = dyn_cast_or_null<ImplicitCastExpr>(L_exper);
					if (imce && isBooleanType(imce->IgnoreParenImpCasts()->getType()))
					{
						left_is_boolean_type = true;
					}
				}

				if (R_type)
				{
					right_is_boolean_type = R_type->isBooleanType();
					if (!right_is_boolean_type && R_exper && R_exper->getStmtClass() == Stmt::ImplicitCastExprClass)
					{
						ImplicitCastExpr *imce = dyn_cast_or_null<ImplicitCastExpr>(R_exper);
						if (imce && isBooleanType(imce->IgnoreParenImpCasts()->getType()))
						{
							right_is_boolean_type = true;
						}
					}
				}

				if (left_is_boolean_type || right_is_boolean_type)
				{
					//位运算的左值和右值都不能是bool类型
					SourceRange range;
					if (left_is_boolean_type && right_is_boolean_type)
					{
						range = B->getExprStmt()->getSourceRange();
					}
					else if (left_is_boolean_type)
					{
						range = L_exper->getSourceRange();
					}
					else
					{
						range = R_exper->getSourceRange();
					}
#if 0
					unsigned int y = 2u;
					bool flag = false;
					bool flag2 = true;
					flag = flag & (y == 2u); //warn
					flag = flag & y;         //warn
					flag = y & flag;         //warn
					flag = flag && flag2;    //no warn
					if (flag && flag2)        //no warn
					{
						printf("---------\n");
					}
					if (flag | (y == 2) == false)		//warn
					{
						printf("++++++++++\n");
					}
#endif
					PathDiagnosticLocation ELoc(B->getOperatorLoc(), C.getSourceManager());
					C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "按位错误类型", categories::SecurityError, "对布尔类型进行位操作，检查是否需要转换为逻辑运算符", ELoc, range);
				}
				else if(!L_type->hasUnsignedIntegerRepresentation())
				{
					//位运算的左值必须是无符号类型
#if 0
					int z = 1;
					z = z | 1; //warn
					z &= 10;   //warn
					z = z ^ 5; //warn
					z = (unsigned)z & 10; //no warn
					unsigned int f = 0;
					f &= 8; //no warn
					printf("%d %u\n", z, f);
#endif
					PathDiagnosticLocation ELoc(B->getOperatorLoc(), C.getSourceManager());
					C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "按位错误类型", categories::SecurityError, "按位运算的左值不是无符号类型", ELoc, L_exper->getSourceRange());
				}
			}
		}
	}

	//赋值运算符
	if (bo_kind == BinaryOperatorKind::BO_Assign)
	{
		Expr *L_exper = B->getLHS();
		if (L_exper)
		{
			QualType LT = ASC.getCanonicalType(L_exper->getType());
			if (LT->isPointerType())
			{
				PathDiagnosticLocation ELoc(B->getBeginLoc(), C.getSourceManager());
				const ParentMap& PM = C.getLocationContext()->getParentMap();
				const Stmt* Parent = PM.getParent(B);
				if (Parent != NULL)
				{
					//检测是否声明了一个函数指针变量
					checkFunctionPointerType(LT, C.getCurrentAnalysisDeclContext()->getDecl(), C, ELoc, Parent->getSourceRange());
				}
			}
		}
				
	}
}

void BinaryOperatorChecker::checkPreStmt(const ImplicitCastExpr *Cast, CheckerContext &C) const
{
	AnalysisDeclContext *AC = C.getCurrentAnalysisDeclContext();
	ASTContext &ASC = C.getASTContext();
	const ParentMap &PM = C.getLocationContext()->getParentMap();
	const Stmt *Parent = PM.getParent(Cast);
	if (!C.getSourceManager().isInMainFile(Cast->getBeginLoc()))
	{
		return;
	}

	if (!Parent)
	{
		return;
	}

	QualType IgnoImpType = ASC.getCanonicalType(Cast->IgnoreImpCasts()->getType());
	QualType CastType = ASC.getCanonicalType(Cast->getType());
	if (IgnoImpType->isSignedIntegerOrEnumerationType())
	{
		bool checkNegative = false;
		const BinaryOperator *BO = dyn_cast_or_null<BinaryOperator>(Parent);
		const DeclStmt *DS = dyn_cast_or_null<DeclStmt>(Parent);
		if (BO != NULL && BO->isAssignmentOp())
		{
			if (ASC.getCanonicalType(BO->getType())->isUnsignedIntegerType())
			{
				//将一个有符号整型隐式类型转换赋值给一个无符号整型
				checkNegative = true;
			}
		}
		else if (DS != NULL)
		{
			for (const auto *I : DS->decls())
			{
				if (const VarDecl *VD = dyn_cast<VarDecl>(I))
				{
					const Expr *Init = VD->getInit();
					if (Init != NULL)
					{
						if (CastType->isUnsignedIntegerType())
						{
							//声明一个无符号整型变量但使用了有符号整型进行初始化
							checkNegative = true;
						}
					}
				}
			}
		}
		if (checkNegative)
		{
			if (C.isNegative(Cast->IgnoreImpCasts()))
			{
				PathDiagnosticLocation ELoc(Cast->getBeginLoc(), C.getSourceManager());
				C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "溢出标志", categories::SecurityError, "为无符号变量赋负值", ELoc, Parent->getSourceRange());
				return;
			}
		}
	}
	
	if (Cast->IgnoreParenImpCasts()->getType()->isEnumeralType())
	{
		//枚举类型只能进行比较运算 不可超限使用
		const BinaryOperator *BO = dyn_cast_or_null<BinaryOperator>(Parent);
		if ((BO != NULL && !BO->isComparisonOp()) || isa<DeclStmt>(Parent))
		{
#if 0
			unsigned int ui;
			ui = Enum1;   //warn
			unsigned int ui2 = Enum2; //warn
			if (ui == Enum1)  //no warn
			{
				printf("----%d\n", Enum1); //no warn
			}
			if (Enum2 >= ui2)  //no warn
			{
				printf("----\n");
			}
			if (Enum1 <= Enum2) //no warn
			{
				printf("----\n");
			}
#endif
			PathDiagnosticLocation ELoc(Parent->getBeginLoc(), C.getSourceManager());
			C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "枚举非法操作", categories::SecurityError, "枚举类型只能用于比较操作", ELoc, Parent->getSourceRange());
			return;
		}
	}

	if (const auto *B = dyn_cast<BinaryOperator>(Parent))
	{
		if (B->isAssignmentOp())
		{
			//赋值运算
			QualType dest_type = B->getLHS()->getType();
			QualType src_type = Cast->IgnoreParenImpCasts()->getType();
			PathDiagnosticLocation ELoc(B->getOperatorLoc(), C.getSourceManager());
			//检查等式左右两边的类型是否一致并警告 (NULL -> 指针类型不检查)
			if (!isNullToPointerType(dest_type, Cast, C.getASTContext()))
			{
				checkAssignmentType(dest_type, src_type, AC->getDecl(), C, ELoc, B->getExprStmt()->getSourceRange());
			}
		}
	}
	else if (isa<DeclStmt>(Parent))
	{
		//声明语句
		const VarDecl *VD = NULL;
		const DeclStmt *DS = dyn_cast_or_null<DeclStmt>(Parent);
		if (DS != NULL)
		{
			VD = dyn_cast_or_null<VarDecl>(DS->getSingleDecl());
		}
		if (VD != NULL)
		{
			QualType dest_type = VD->getType();
			QualType src_type = Cast->IgnoreParenImpCasts()->getType();
			PathDiagnosticLocation ELoc(Parent->getBeginLoc(), C.getSourceManager());
			//检查等式左右两边的类型是否一致并警告 (NULL -> 指针类型不检查)
			if (!isNullToPointerType(dest_type, Cast, C.getASTContext()))
			{
				checkAssignmentType(dest_type, src_type, AC->getDecl(), C, ELoc, Parent->getSourceRange());
			}
		}
	}
	else if (isa<UnaryOperator>(Parent))
	{
		const UnaryOperator *UOP = dyn_cast<UnaryOperator>(Parent);
		QualType src_type = Cast->IgnoreParenImpCasts()->getType();
		//当且仅当非运算符作用在内置常量类型上时才进行警告
		if (UOP->getOpcode() == UnaryOperatorKind::UO_LNot &&
			src_type->isBuiltinType() &&
			(src_type->isIntegerType() || src_type->isFloatingType()) &&
			Cast->isConstantInitializer(C.getASTContext(), false))
		{
#if 0
			bool flag = false;
			const int a = 1;
			int b = 1;
			const float f = 1.0f;
			double d = 1.0;
			const char *p = (const char *)&b;
			if (flag == !1) //warn
			{
				printf("-------------\n");
			}
			if (flag == !a) //warn
			{
				printf("-------------\n");
			}
			if (flag == !f) //warn
			{
				printf("-------------\n");
			}
			if (flag == !b) //no warn
			{
				printf("-------------\n");
			}
			if (flag == !d) //no warn
			{
				printf("-------------\n");
			}
			if (flag == !p) //no warn
			{
				printf("-------------\n");
			}
#endif
			PathDiagnosticLocation ELoc(Parent->getBeginLoc(), C.getSourceManager());
			C.getBugReporter().EmitBasicReport(AC->getDecl(), this, "非操作数错误类型", categories::SecurityError, "对常量值执行非运算", ELoc, Cast->getSourceRange());
		}
	}
}

void BinaryOperatorChecker::checkPreStmt(const DeclStmt *DS, CheckerContext &C) const
{
	if (!C.getSourceManager().isInMainFile(DS->getBeginLoc()))
	{
		return;
	}
	const VarDecl *VD = dyn_cast_or_null<VarDecl>(DS->getSingleDecl());
	if (VD == NULL)
	{
		return;
	}

	QualType DT = VD->getType();
	DT = DT->isReferenceType() ? DT.getNonReferenceType() : DT;
	DT = QualType(DT->getUnqualifiedDesugaredType(), 0);

	if (DT->isPointerType())
	{
		PathDiagnosticLocation ELoc(DS->getBeginLoc(), C.getSourceManager());
		//检测指针是否超过两级
		checkPointerTypeLevel(DT, 2, C.getCurrentAnalysisDeclContext()->getDecl(), C, ELoc, DS->getSourceRange());
		if (VD->getInit() != NULL)
		{
			//检测是否声明了一个函数指针变量
			checkFunctionPointerType(DT, C.getCurrentAnalysisDeclContext()->getDecl(), C, ELoc, DS->getSourceRange());
		}
	}
}

void BinaryOperatorChecker::checkPreStmt(const UnaryOperator *U, CheckerContext &C) const
{
	if (!C.getSourceManager().isInMainFile(U->getBeginLoc()))
	{
		return;
	}
	if (U->getOpcode() == UnaryOperatorKind::UO_AddrOf)
	{
		PathDiagnosticLocation ELoc(U->getBeginLoc(), C.getSourceManager());
		//检测指针是否超过两级
		checkPointerTypeLevel(U->getType(), 2, C.getCurrentAnalysisDeclContext()->getDecl(), C, ELoc, U->getSourceRange());
	}
}

void BinaryOperatorChecker::checkPointerExplicitAssignment(const QualType &T, const ExplicitCastExpr *Cast, const Decl *D, CheckerContext &C, const PathDiagnosticLocation &ELoc, const SourceRange &SR) const
{
	QualType Tmp = T->isReferenceType() ? T.getNonReferenceType() : T;
	Tmp = QualType(T->getUnqualifiedDesugaredType(), 0);

	if (Tmp->isPointerType())
	{
		if (!isNullToPointerType(Tmp, Cast->IgnoreCasts(), C.getASTContext()))
		{
			//检查等式左右两边的类型是否一致并警告 (NULL -> 指针类型不检查)
			QualType src_type = Cast->IgnoreCasts()->getType();
			if (!isEquivalentQualType(Tmp, src_type))
			{
				//指针变量赋值时禁止将不同类型的变量强制类型转换为目的指针变量类型
				std::string warn = "Cast assignment of pointer with inconsistent type '";
				warn.append(src_type.getAsString());
				warn.append("' -> '");
				warn.append(Tmp.getAsString());
				warn.append("'");
				C.getBugReporter().EmitBasicReport(D, this, "指针转换", categories::SecurityError, warn, ELoc, SR);
			}
		}
	}
}


void BinaryOperatorChecker::checkPreStmt(const ExplicitCastExpr *Cast, CheckerContext &C) const
{
	AnalysisDeclContext *AC = C.getCurrentAnalysisDeclContext();
	const ParentMap &PM = C.getLocationContext()->getParentMap();
	const Stmt *Parent = PM.getParent(Cast);

	if (!C.getSourceManager().isInMainFile(Cast->getBeginLoc()))
	{
		return;
	}

	//检测赋值和变量定义中的强制类型转换语句
	if (const auto *B = dyn_cast<BinaryOperator>(Parent))
	{
		if (B->isAssignmentOp())
		{
			//赋值运算
			PathDiagnosticLocation ELoc(B->getOperatorLoc(), C.getSourceManager());
			checkPointerExplicitAssignment(B->getLHS()->getType(), Cast, AC->getDecl(), C, ELoc, B->getSourceRange());
		}
	}
	else if (isa<DeclStmt>(Parent))
	{
		//声明语句
		const VarDecl *VD = NULL;
		const DeclStmt *DS = dyn_cast_or_null<DeclStmt>(Parent);
		if (DS != NULL)
		{
			VD = dyn_cast_or_null<VarDecl>(DS->getSingleDecl());
		}
		if (VD != NULL)
		{
			PathDiagnosticLocation ELoc(Parent->getBeginLoc(), C.getSourceManager());
			checkPointerExplicitAssignment(VD->getType(), Cast, AC->getDecl(), C, ELoc, Parent->getSourceRange());
		}
	}
}

void BinaryOperatorChecker::checkASTDecl(const FieldDecl *FD, AnalysisManager &Mgr, BugReporter &BR) const
{

	if (!Mgr.getSourceManager().isInMainFile(FD->getBeginLoc()))
	{
		return;
	}

	if (!FD->isBitField())
	{
		return;
	}

	QualType field_type = FD->getType();

#if 0
	struct Test
	{
		int a;
		float b;
		int x : 1; //warn
		int : 5;   //no warn
		uint64_t z : 1; //no warn
		int64_t s : 1;  //warn
		unsigned char y : 2; //warn
	};
#endif

	if (!field_type->isIntegerType() || field_type->isCharType())
	{
		//位域必须是有符号整数或无符号整数
		PathDiagnosticLocation ELoc(FD->getBeginLoc(), BR.getSourceManager());
		BR.EmitBasicReport(FD, this, "位域错误", categories::SecurityError, "位字段只能是有符号或无符号整数", ELoc, FD->getSourceRange());
		return;
	}

	if (!field_type->hasUnsignedIntegerRepresentation())
	{
		//有符号整型位域的长度至少为2
		unsigned bit_count = FD->getBitWidthValue(Mgr.getASTContext());
		if (bit_count <= 1)
		{
			PathDiagnosticLocation ELoc(FD->getBeginLoc(), BR.getSourceManager());
			BR.EmitBasicReport(FD, this, "位域错误", categories::SecurityError, "有符号整数的位字段长度应至少为2", ELoc, FD->getSourceRange());
		}
	}
}

void BinaryOperatorChecker::checkAssignmentType(const QualType &dest_type, const QualType &src_type, const Decl *D, CheckerContext &C, const PathDiagnosticLocation &ELoc, const SourceRange &SR) const
{
	if (!isEquivalentQualType(dest_type, src_type))
	{
#if 0
		//内置类型赋值时检测类型是否一致
		unsigned int i;
		unsigned int *ip = &i;
		const double d1 = 2.1521541; //no warn
		const float f2 = 2.12324565; //warn double -> float
		const float f3 = 2.12324565f; //no warn
		i = 2.1234565; //warn double -> unsigned int
		i = (unsigned int)d1;   //no warn
		i = d1;                 //warn double ->  unsigned int
		float f = f2;   //no warn
		*ip = 10;    //warn int -> unsigned int
		printf("%d\n", i);
#endif
		std::string src_type_name = src_type.getAsString();
		std::string dst_type_name = dest_type.getAsString();
		std::string warn;
		warn = "赋值操作被用到不同类型的变量上：'";
		warn.append(src_type_name);
		warn.append("' -> '");
		warn.append(dst_type_name);
		warn.append("'");
		C.getBugReporter().EmitBasicReport(D, this, "赋值错误类型", categories::SecurityError, warn, ELoc, SR);
	}
}


#if 0
typedef unsigned int ***TreeLevelUint;
typedef unsigned int **DoubleLevelUint;
int main(int argc, char *argv[])
{
	unsigned int arry[10] = { 0 };
	unsigned int *p1_ptr, **p2_ptr;
	unsigned int ***p3_ptr; //warn 声明了一个三级指针
	TreeLevelUint p3_ty;   //warn 声明了一个三级指针
	void *v;
	p1_ptr = arry;
	p2_ptr = &p1_ptr;
	p3_ptr = &p2_ptr;  //warn将一个三级指针赋值给了一个指针变量
	v = (void *)&p2_ptr; //warn 将一个三级指针赋值给了一个指针变量
	DoubleLevelUint p2_ty = &p1_ptr;
	p3_ty = &p2_ty; //warn 将一个三级指针赋值给了一个指针变量
	printf("%x %x %x\n", p3_ptr, p3_ty, v);
	return 0;
}
#endif
void BinaryOperatorChecker::checkPointerTypeLevel(const QualType &t, const int max_level, const Decl *D, CheckerContext &C, const PathDiagnosticLocation &ELoc, const SourceRange &SR) const
{
	if (IsPointerLevelGreaterThan(t, max_level))
	{
		std::string warn = "It is suggested that the pointer level should not be greater than ";
		warn.append(std::to_string(max_level));
		warn.append("-level");
		C.getBugReporter().EmitBasicReport(D, this, "深指针级别", categories::SecurityError, warn, ELoc, SR);
	}
}

void BinaryOperatorChecker::checkFunctionPointerType(const QualType &T, const Decl *D, CheckerContext &C, const PathDiagnosticLocation &ELoc, const SourceRange &SR) const
{
	QualType Tmp = T->isReferenceType() ? T.getNonReferenceType() : T;
	Tmp = QualType(Tmp->getUnqualifiedDesugaredType(), 0);
	if (Tmp->isPointerType() && Tmp->isFunctionPointerType())
	{
		C.getBugReporter().EmitBasicReport(D, this, "函数指针", categories::SecurityError, "不建议将函数声明为指针类型", ELoc, SR);
	}
}


void ento::registerBinaryOperatorChecker(CheckerManager &Mgr)
{
	Mgr.registerChecker<BinaryOperatorChecker>();
}

bool ento::shouldRegisterBinaryOperatorChecker(const CheckerManager &Mgr)
{
	return true;
}
