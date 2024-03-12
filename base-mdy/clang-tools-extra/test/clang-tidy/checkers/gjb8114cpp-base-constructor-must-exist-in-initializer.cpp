// RUN: %check_clang_tidy %s gjb8114cpp-base-constructor-must-exist-in-initializer %t
/// GJB 8114-2013 R-2-2-5 派生类构造函数必须在初始化列表中说明直接基类构造函数。
#include <iostream>
using namespace std;
class Document
{
public:
    int docid;
    Document(void):docid(0)
    {}
    explicit Document(int);
};

Document::Document(int var)
{
    docid = var;
}
class Book : public Document
{
public:
    int bookid;
    Book(void) : bookid(1)  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 派生类构造函数必须在初始化列表中说明直接基类构造函数。 [gjb8114cpp-base-constructor-must-exist-in-initializer]
    {}
    Book(int id) //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 派生类构造函数必须在初始化列表中说明直接基类构造函数。 [gjb8114cpp-base-constructor-must-exist-in-initializer]
    {
        bookid = id;
    }
};

int main(void)
{
    Book mybook;
    return (0);
}

class Book2 : public Document
{
public:
    int bookid;
    Book2(void) : Document(1), bookid(1)
    {}
};
int main1(void)
{
    Book2 mybook;
    return (0);
}

