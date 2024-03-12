// RUN: %check_clang_tidy %s misrac2012-include-ban-header %t

#include<stdio.h>
#include  <setjmp.h>    //违背1
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 禁止使用setjmp.h [misrac2012-include-ban-header]
#include <signal.h>     //违背2
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: 禁止使用signal.h [misrac2012-include-ban-header]
#include <stdarg.h>
typedef signed   char           int8_t;
typedef signed   short          int16_t;
typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;
static jmp_buf myenv;
static void jmpfunc ( int8_t p )
{
  if( p == 10 )
  {
    longjmp ( myenv, 9 );   //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 禁止使用setjmp.h中的函数longjmp [misrac2012-include-ban-header]
  }
}
void R_21_4 ( void )
{
    int16_t istat = 0;
    if ( setjmp ( myenv ) != 0 )    //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止使用setjmp.h中的函数setjmp [misrac2012-include-ban-header]
    {
      jmpfunc( 10 );
    }
}
/* end of R_21_04.c */
void R_21_5 ( void )
{
     int16_t sig;
     sig = SIGINT;
}
/* end of R_21_05.c */
void h ( va_list ap ) //违背5
// CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 禁止使用stdarg.h中的va_list [misrac2012-include-ban-header]
{
 double y;
 y = va_arg ( ap, double ); //违背6
 // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 禁止使用stdarg.h中的va_arg [misrac2012-include-ban-header]
}
void f ( uint16_t n, ... )
{
 uint32_t x;
 va_list ap;   //违背7
 // CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 禁止使用stdarg.h中的va_list [misrac2012-include-ban-header]
 va_start(ap,n);   //违背8
 // CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 禁止使用stdarg.h中的va_start [misrac2012-include-ban-header]
 x = va_arg ( ap, uint32_t );   //违背9
 // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 禁止使用stdarg.h中的va_arg [misrac2012-include-ban-header]
 h (ap);
 x = va_arg ( ap, uint32_t );   //违背10
 // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: 禁止使用stdarg.h中的va_arg [misrac2012-include-ban-header]
 va_end(ap);  //违背11
 // CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 禁止使用stdarg.h中的va_end [misrac2012-include-ban-header]
 va_list ap1;   //违背12
 // CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 禁止使用stdarg.h中的va_list [misrac2012-include-ban-header]
 va_copy(ap1, ap);  //违背13
 // CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 禁止使用stdarg.h中的va_copy [misrac2012-include-ban-header]
}
void g ( void )
{
 /* undefined - uint32_t:double type mismatch when f uses va_arg ( ) */
 f ( 1, 2.0, 3.0 );
}
