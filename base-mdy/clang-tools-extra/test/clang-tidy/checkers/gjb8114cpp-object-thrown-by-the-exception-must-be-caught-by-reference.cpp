// RUN: %check_clang_tidy %s gjb8114cpp-object-thrown-by-the-exception-must-be-caught-by-reference %t

//违背示例:
// #include <iostream>
// using namespace std;
// class Document
// {
//   public:
//   Document(void) :docid(0)
//   {
//   }
//   int get_docid(void);
//   private:
//   int docid;
// };
// int Document::get_docid(void)
// {
//   return docid;
// }
// int main(void)
// {
// try
// {
//   Document mydoc1;
//   throw &mydoc1; //违背1  
//   // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 异常抛出的对象必须使用引用方式捕获 [gjb8114cpp-object-thrown-by-the-exception-must-be-caught-by-reference]
// }
// catch (Document *d) //违背2 
// // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 异常抛出的对象必须使用引用方式捕获 [gjb8114cpp-object-thrown-by-the-exception-must-be-caught-by-reference]
// {
//   cout<<d->get_docid()<<endl;
// }
// return (0);
// }

//遵循示例
#include <iostream> 
using namespace std;
class Document
{
  public:
  Document(void) :docid(0)
  {
  }
  int get_docid(void);
  private:
  int docid;
};
int Document::get_docid(void)
{
  return docid;
}
int main(void)
{
try
{
  Document mydoc1;
  throw mydoc1; //违背1 
}
catch (Document &d) //违背2 
{
  cout<<d.get_docid()<<endl;
}
return (0);
}