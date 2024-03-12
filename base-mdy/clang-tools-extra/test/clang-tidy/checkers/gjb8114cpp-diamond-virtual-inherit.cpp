// RUN: %check_clang_tidy %s gjb8114cpp-diamond-virtual-inherit %t

#include <iostream>

namespace
{
	class A
	{
	public:
		A() :a(0) {}
		int a;
	};

	class B1 :public A //违背1
	// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 基类 'class (anonymous namespace)::A' 在被菱形继承时应使用virtual进行修饰 [gjb8114cpp-diamond-virtual-inherit]
	{
	public:
		B1() :A(), b1(0) {}
		int b1;
	};

	class B2 :public A //违背2
	// CHECK-MESSAGES: :[[@LINE-1]]:8: 基类 'class (anonymous namespace)::A' 在被菱形继承时应使用virtual进行修饰 [gjb8114cpp-diamond-virtual-inherit]
	{
	public:
		B2() :A(), b2(0) {}
		int b2;
	};

	class C1 :public B1, public B2
	{
	public:
		C1() :B1(), B2(), c1(0) {}
		int c1;
	};


	class B3 :public virtual A //遵循1
	{
	public:
		B3() :A(), b3(0) {}
		int b3;
	};

	class B4 :public virtual A //遵循2
	{
	public:
		B4() :A(), b4(0) {}
		int b4;
	};

	class C2 :public B3, public B4
	{
	public:
		C2() :B3(), B4(), c2(0) {}
		int c2;
	};
}


void R_2_1_3()
{
	//菱形层次结构的派生类设计，对基类派生必须使用virtual说明
	return;
}