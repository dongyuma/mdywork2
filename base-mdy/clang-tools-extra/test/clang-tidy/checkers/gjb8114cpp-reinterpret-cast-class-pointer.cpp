// RUN: %check_clang_tidy %s gjb8114cpp-reinterpret-cast-class-pointer %t

#include <iostream>
namespace
{
	struct S
	{
		int i;
		int j;
		int k;
	};

	class A
	{
	public:
		A() :i(0), j(0), k(0) {}
		int i;
		int j;
		int k;
	};

	class B :public A
	{
	public:
		B() :A(), b(0) {}
		int b;
	};

	class C : public A
	{
	public:
		C() :A(), c(0) {}
		int c;
	};

	class D
	{
	public:
		D() :d(0) {}
		int d;
	};

	void badReinterpretCast1()
	{
		S *s = new S;
		A *a = reinterpret_cast<A *>(s); //违背 1
		// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止将不相关的指针类型强制转换为对象指针类型 [gjb8114cpp-reinterpret-cast-class-pointer]
	}

	void badReinterpretCast2()
	{
		S *s = new S;
		s->i = 0;
		s->j = 0;
		s->k = 0;
		A *a = reinterpret_cast<A *>(&(s->i)); //违背2
		// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止将不相关的指针类型强制转换为对象指针类型 [gjb8114cpp-reinterpret-cast-class-pointer]
	}

	void badReinterpretCast3()
	{
		A *a = new A;
		D *d = reinterpret_cast<D *>(a); //违背3
		// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止将不相关的指针类型强制转换为对象指针类型 [gjb8114cpp-reinterpret-cast-class-pointer]
	}

	void goodReinterpretCast1()
	{
		A *a = new A;
		B *b = reinterpret_cast<B *>(a); //遵循1 A为B的基类 相关
	}

	void goodReinterpretCast2()
	{
		A *a = new A;
		C *c = reinterpret_cast<C *>(a); //遵循2 A为C的基类 相关
	}

	void goodReinterpretCast3()
	{
		C *c = new C;
		B *b = reinterpret_cast<B *>(c); //遵循1 B和C有共同基类A 相关
	}

}


void R_2_5_1()
{
}
