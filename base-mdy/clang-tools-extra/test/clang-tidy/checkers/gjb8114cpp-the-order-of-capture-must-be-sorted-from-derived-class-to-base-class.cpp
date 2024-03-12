// RUN: %check_clang_tidy %s gjb8114cpp-the-order-of-capture-must-be-sorted-from-derived-class-to-base-class %t

//CSR-323	捕获的顺序必须按由派生类到基类的次序排序		R-2-8-1	 gjb8114cpp-the-order-of-capture-must-be-sorted-from-derived-class-to-base-class
//CSR-323  GJB 8114-2013 R-2-8-1
//违背示例:
#include<iostream>
using namespace std; 
class Document
{
public:
  Document(void):docid(0)
  {}
  int get_docid(void); 
private:
  int docid;
};
int Document::get_docid(void)
{
  return docid;
}
class Book:public Document
{
public:
  Book(void):Document(),bookid(0)
  {}
  int get_bookid(void);
private:
  int bookid;
};
int Book::get_bookid(void)
{
  return bookid;
}
int main(void)
{
Book mybook;
try
{
  throw mybook;
}
catch (Document &d) //违背 1 
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 捕获的顺序必须按由派生类到基类的次序排序 [gjb8114cpp-the-order-of-capture-must-be-sorted-from-derived-class-to-base-class]
{
  cout<<d.get_docid()<<endl;
}
catch(Book &b) //违背 2 
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 捕获的顺序必须按由派生类到基类的次序排序 [gjb8114cpp-the-order-of-capture-must-be-sorted-from-derived-class-to-base-class]
{
  cout<<b.get_bookid()<<endl;
}
return (0);
}


// //遵循示例
// #include<iostream>
// using namespace std; 
// class Document
// {
// public:
//   Document(void):docid(0)
//   {}
//   int get_docid(void); 
// private:
//   int docid;
// };
// int Document::get_docid(void)
// {
//   return docid;
// }
// class Book:public Document
// {
// public:
//   Book(void):Document(),bookid(0)
//   {}
//   int get_bookid(void);
// private:
//   int bookid;
// };
// int Book::get_bookid(void)
// {
//   return bookid;
// }
// int main(void)
// {
// Book mybook;
// try
// {
//   throw mybook;
// }
// catch(Book &b) //遵循1 
// {
//   cout<<b.get_bookid()<<endl;
// }
// catch (Document &d) //遵循2
// {
//   cout<<d.get_docid()<<endl;
// }

// return (0);
// }