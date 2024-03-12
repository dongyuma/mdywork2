// RUN: %check_clang_tidy %s misrac2012-ban-goto %t

void f ( void )
{
 int j = 0;
L1:
 ++j;
 if ( 10 == j )
 {
 goto L2; //遵循1
 }
 goto L1; //违背1
 // CHECK-MESSAGES: :[[@LINE-1]]:2: warning: Goto应该跳转到同一个函数中在goto之后声明的一个label [misrac2012-ban-goto]
L2 :
 ++j;
}