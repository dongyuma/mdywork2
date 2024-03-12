// RUN: %check_clang_tidy %s gjb8114cpp-const-member-function-return %t
#include <iostream>
class A
{
public:
	A();
	~A();

	int *badConstRet1() const;
	int &badConstRet2() const;
	int **badConstRet3() const;
	int *badConstRet4() const;

	int *goodConstRet1();
	int &goodConstRet2();

	const int *goodConstRet3() const;
	const int &goodConstRet4() const;
	const int **goodConstRet5() const;

private:
	int *b;
	mutable int c;
	const int *cb = &c;
	int &rb = c;
	const int &rc = c;
	int i;
	int **cc;
	const int **ccc;
};


A::A()
{
	c = 10;
	i = 11;
}

A::~A()
{

}

int *A::badConstRet1() const
{
	return b; //违背1
	// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: const成员函数禁止返回非const的指针或引用 [gjb8114cpp-const-member-function-return]
}

int &A::badConstRet2() const
{
	return c; //违背2
	// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: const成员函数禁止返回非const的指针或引用 [gjb8114cpp-const-member-function-return]
}

int **A::badConstRet3() const
{
	return cc; //违背3
	// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: const成员函数禁止返回非const的指针或引用 [gjb8114cpp-const-member-function-return]
}

int *A::badConstRet4() const
{
	return &c; //违背4
	// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: const成员函数禁止返回非const的指针或引用 [gjb8114cpp-const-member-function-return]
}

int *A::goodConstRet1()
{
	return b; //遵循1
}

int &A::goodConstRet2()
{
	return i; //遵循2
}

const int *A::goodConstRet3() const
{
	return cb; //遵循3
}

const int &A::goodConstRet4() const
{
	return rc; //遵循4
}


const int **A::goodConstRet5() const
{
	return ccc; //遵循5
}

const int *test()
{
	static int a = 10;
	int *p = &a;
	return p; //遵循6
}