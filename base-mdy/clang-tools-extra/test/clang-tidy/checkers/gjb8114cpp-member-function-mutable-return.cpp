// RUN: %check_clang_tidy %s gjb8114cpp-member-function-mutable-return %t

#include <iostream>

class B
{
public:
	B();
	~B();
	int &badMutableRet1();
	int *badMutableRet2();
	int *badMutableRet3();

	int goodMutableRet1();
	const int *goodMutableRet2();
	const int &goodMutableRet3();
	int *goodMutableRet4();
	int &goodMutableRet5();

public:
	int pbn;
	int *pbnp;
private:
	int n;
	int *b;
};

B::B()
{
	n = 10;
	b = new int;
	*b = 11;
	pbn = 12;
	pbnp = new int;
	*pbnp = 13;
}

B::~B()
{
	delete b;
}

int &B::badMutableRet1()
{
	return n; //违背1
	// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 公有成员函数不可返回私有的非const指针或引用变量 [gjb8114cpp-member-function-mutable-return]
}

int *B::badMutableRet2()
{
	return b; //违背2
	// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 公有成员函数不可返回私有的非const指针或引用变量 [gjb8114cpp-member-function-mutable-return]
}

int *B::badMutableRet3()
{
	return &n; //违背3
	// CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 公有成员函数不可返回私有的非const指针或引用变量 [gjb8114cpp-member-function-mutable-return]
}

int B::goodMutableRet1()
{
	return n; //遵循1
}

const int *B::goodMutableRet2()
{
	return b; //遵循2
}

const int &B::goodMutableRet3()
{
	return n; //遵循3
}

int *B::goodMutableRet4()
{
	return &pbn; //遵循4
}

int &B::goodMutableRet5()
{
	return pbn; //遵循5
}