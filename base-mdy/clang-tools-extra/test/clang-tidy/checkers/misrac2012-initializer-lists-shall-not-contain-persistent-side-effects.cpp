// RUN: %check_clang_tidy %s misrac2012-initializer-lists-shall-not-contain-persistent-side-effects %t

// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'f' is insufficiently awesome [misrac2012-initializer-lists-shall-not-contain-persistent-side-effects]
volatile int v1;
void f ( void )
{
 /* Non-compliant - volatile access is persistent side effect */
 int a[ 2 ] = { v1, 0 };
}
void g ( int x, int y )
{
 /* Compliant - no side effects */
 int a[ 2 ] = { x + y, x - y };
}
int x = 0u;
extern void p ( int a[ 2 ] );
void h ( void )
{
 /* Non-compliant - two side effects */
 p ( ( int[ 2 ] ) { x++, x++ } );
}