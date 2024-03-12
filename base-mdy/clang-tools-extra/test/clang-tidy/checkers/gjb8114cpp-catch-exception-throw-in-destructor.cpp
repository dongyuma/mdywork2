// RUN: %check_clang_tidy %s gjb8114cpp-catch-exception-throw-in-destructor %t
#include <iostream>
using namespace std;

// CSR-310	析构函数中禁止存在不是由自身捕获处理的异常。R-2-3-2 GJB8114 2013

class Foo
{
public:
    int a;
    Foo(void);
    ~Foo();
};

Foo::Foo(void)
{
    a = 1;
}

Foo::~Foo()
{
    if (1 == a)
    {
        throw 0; // 违背1
// CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 析构函数中禁止存在不是由自身捕获处理的异常。 [gjb8114cpp-catch-exception-throw-in-destructor]
    }
    
}
int main(void)
{
    try
    {
        Foo f;
    }
    catch(double e)
    {
        //...
    }
    return (0);
}

class Foo2
{
public:
    int a;
    Foo2(void);
    ~Foo2();
};

Foo2::Foo2(void)
{
    a = 1;
}

Foo2::~Foo2()
{
    try
    {
        if( 1 == a)
        {
            throw 0;
        }
    }
    catch(int e) //遵循1
    {
        //...
    }
}

int main2(void)
{
    try
    {
        Foo2 f;
    }
    catch(int e)
    {
        //...
    }
    return (0);
}

class Foo3
{
public:
    int a;
    Foo3(void);
    ~Foo3();
};

Foo3::Foo3(void)
{
    a = 1;
}

Foo3::~Foo3()
{
    try
    {
        if( 1 == a)
        {
            throw 0;
        }
    }
    catch(int e) //遵循1
    {
        //...
    }
    throw 0; //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 析构函数中禁止存在不是由自身捕获处理的异常。 [gjb8114cpp-catch-exception-throw-in-destructor]
}

class Foo4
{
public:
    int a;
    Foo4(void);
    ~Foo4();
};

Foo4::Foo4(void)
{
    a = 1;
}

Foo4::~Foo4()
{
    try
    {
        if( 1 == a)
        {
            throw 0;//违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: 析构函数中禁止存在不是由自身捕获处理的异常。 [gjb8114cpp-catch-exception-throw-in-destructor]
        }

    }
    catch(float e) 
    {
        //...
    }
    catch(double e) 
    {
        //...
    }
    // try
    // {
    //     if( 1 == a)
    //     {
    //         throw 0;//违背3
    //     }
    // }    
    // 上面这种情况编译会出错
}