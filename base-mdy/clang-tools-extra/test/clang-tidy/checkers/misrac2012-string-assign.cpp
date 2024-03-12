// RUN: %check_clang_tidy %s misrac2012-string-assign %t


void fun()
{
    char *s1;
    s1="string";  //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 字符串应该被赋值给const类型的指针 [misrac2012-string-assign]
}
char *s = "string";   //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 字符串应该被赋值给const类型的指针 [misrac2012-string-assign]
const volatile char *p = "string";  //遵循1
extern void f1 ( char *s1 );
extern void f2 ( const char *s2 );
void g ( void )
{
 f1 ("string");   //违背3
 // CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 字符串应该被赋值给const类型的指针 [misrac2012-string-assign]
 f2 ( "string" );   //遵循2
}
char *name1 ( void )
{
 return ("MISRA");  //违背4
 // CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 字符串应该被赋值给const类型的指针 [misrac2012-string-assign]
}
const char *name2 ( void )
{
 return ( "MISRA" ); /* Compliant */
}