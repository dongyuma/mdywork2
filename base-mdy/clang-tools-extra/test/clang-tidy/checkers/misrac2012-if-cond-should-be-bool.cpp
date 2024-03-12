// RUN: %check_clang_tidy %s misrac2012-if-cond-should-be-bool %t

#include<stdio.h>
typedef signed   char           int8_t;
typedef signed   short          int16_t;
typedef signed   int            int32_t;
typedef signed   long           int64_t;
typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long           uint64_t;
typedef          int            bool_t;
void R_14_4 ( void )
{
   int32_t *p, *q;
   while ( p ) //违背1
   // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: if或while的条件应该为bool类型 [misrac2012-if-cond-should-be-bool]
   {}
   while ( q != NULL ) //遵循1
   {}
   while ( true ) //遵循2
   {}
   extern bool_t flag;
   while ( flag ) //遵循3
   {}
   int32_t i;
   if ( i ) //违背2
   // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: if或while的条件应该为bool类型 [misrac2012-if-cond-should-be-bool]
   {}
   if ( i != 0 ) //遵循4
   {}
}

