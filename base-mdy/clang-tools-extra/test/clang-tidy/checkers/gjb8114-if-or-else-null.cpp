// RUN: %check_clang_tidy %s gjb8114-if-or-else-null %t 

//根据R-1-2-2要求,if/else if/else必须加大括号,由于词法分析无法获取到不加大括号的空语句的注释部分，因而注释部分的检测仅支持有大括号的情况
//CSR-172	条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明				R-1-4-2	gjb8114-if-or-else-null
//CSR-172 GJB 8114-2013 R-1-4-2
//违背示例
int main(void)
{
  int i=0,j=0,k=0;
  double x=0.0;
  //...
  if(0==i)  //遵循1
  {
    x=1.0;
  }
  else if(1==i)  //遵循2
  {
    x=2.0;
  }
  else
  {  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
    ;
  }
  if(0==j)  //遵循3
  {
    x=x+1.0;
  }
  else if(1==j)  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
  {
    ;
  }
  else
  {  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]

  }
  if(0==k)  //遵循4
  {
    x=x/2.0;
  }
  else if(1==k)  //遵循5
  {
    x=x/3.0;
  }
  else
  {  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
    /* no deal with */
  }
  if(k==2)  //违背5
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
  {

  }
  else if(k==3)  //违背6
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
  {;}
  else{  //违背7
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
    ;
  }
  if(k==5)  //违背8
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
  {;}
  if(k==5)  //违背9
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
  {;;;;;;;;;;}
  if(k==5)  //违背10
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
  {
    ;
    ;
    ;
    ;
  }
  else if(k==6)  //遵循6
  {
    ;;;;;;;;;;;
    return (0);
    ;;;;;;;
  }
  else if(k==7){  //违背11
  // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
    /* no deal with */;
  }
  else
  {  //违背12
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
    ;
  }
  if(k==1)  //遵循7
  {
    ;/* no deal with */
  }
  else if(k==2)  //遵循8
  {;/* no deal with */}
  if(k==2)  //遵循9
  {
    ;                                            /* no deal with */  
  }
  else
  {  //遵循10
    ;                      /*             no deal with                       */                  
  }
  if(k==3)  //遵循11
  {
    ;                       /*                       no                deal                 with                     */
  }
  if(k==4)  //违背13
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 条件判定分支如果为空,必须以单独一行的分号加注释进行明确说明 [gjb8114-if-or-else-null]
  {
    ;                      /*           abc            no         abc       deal         abc        with         abc            */
  }
  else; //违背14
  return (0);
}
