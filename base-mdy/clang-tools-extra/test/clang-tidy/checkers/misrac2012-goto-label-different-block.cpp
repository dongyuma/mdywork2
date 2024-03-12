// RUN: %check_clang_tidy %s misrac2012-goto-label-different-block %t

typedef int int32_t;
void f1 ( int32_t a )
{
   if ( a <= 0 )
   {
     goto L2;  //违背1
     // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: goto跳转的label所在块中没有goto语句 [misrac2012-goto-label-different-block]
   }
   goto L1;  //遵循1
   if ( a == 0 )
   {
     goto L1;  //遵循2
   }
   goto L2;  //违背2
   // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: goto跳转的label所在块中没有goto语句 [misrac2012-goto-label-different-block]
  L1:
   if ( a > 0 )
   {
     L2:
     ;
   }
   int32_t x=1,y=1;
  switch ( x )
  {
   case 0:
     if ( x == y )
     {
       goto L3;  //违背3
       // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: goto跳转的label所在块中没有goto语句 [misrac2012-goto-label-different-block]
     }
     break;
   case 1:
     y = x;
     L3:
     ++x;
     break;
   case 2:
    {
        goto L4;  //遵循3
        L4:
        ;
    }
   case 3:
     goto L5;  //违背4
     // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: goto跳转的label所在块中没有goto语句 [misrac2012-goto-label-different-block]
     break;
   case 4:
     L5:
     x++;
     break;
   case 5:
     goto L6;  //遵循4
     break;
   case 6:
     L6:
     goto L4;  //遵循5
   break;
   default:
     break;
  }
}
