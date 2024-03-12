// RUN: %check_clang_tidy %s gjb8114cpp-const-cast-pointer-reference %t

#include <iostream>


namespace
{
	class A
	{
	public:
		explicit A(int a);
		A();
		~A();

		int ma;
	};

	A::A(int a)
	{
		ma = a;
	}


	A::A()
	{
		ma = 0;
	}

	A::~A()
	{

	}
}

void R_2_5_2()
{
	A const a1 = A(10);
	A *a2 = const_cast<A *>(&a1); //Υ��1
	// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: ָ������õ�����ת���н�ֹ�Ƴ�const��volatile���� [gjb8114cpp-const-cast-pointer-reference]
	a2->ma = 11;
	A &a3 = const_cast<A &>(a1); //Υ��2
	// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: ָ������õ�����ת���н�ֹ�Ƴ�const��volatile���� [gjb8114cpp-const-cast-pointer-reference]
	a3.ma = 12;
	A a4 = A(100);
	A *a5 = const_cast<A *>(&a4); //��ѭ1
	a5->ma = 101;
	A &a6 = const_cast<A &>(a4); //��ѭ2
	a6.ma = 102;
	A *a7 = new A;
	A *a8 = const_cast<A *>(a7); //��ѭ3
	a8->ma = 103;
	const A *a9 = a7;
	A *a10 = const_cast<A *>(a9); //Υ��3
	// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: ָ������õ�����ת���н�ֹ�Ƴ�const��volatile���� [gjb8114cpp-const-cast-pointer-reference]
	a10->ma = 104;
	volatile int x1 = 10;
	int &x2 = const_cast<int &>(x1); //Υ��4
	// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: ָ������õ�����ת���н�ֹ�Ƴ�const��volatile���� [gjb8114cpp-const-cast-pointer-reference]
	volatile int &x3 = const_cast<volatile int &>(x1); //��ѭ4
}

