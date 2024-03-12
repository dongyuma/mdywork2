// RUN: %check_clang_tidy %s gjb8114-case-in-different-layer %t

//CSR-175	字符型变量必须明确定义是有符号还是无符号		5.4.1.8	gb5369-NameForbiddenReuse	R-1-4-8	gjb8114-case-in-different-layer	
//CSR-175  GJB 8114-2013 R-1-4-8
int main_1(void)
{
  int x=2;
  int y=0;
  int z=0;
  int m=0;
  //...
  switch(x)
  {
    case 1:  //遵循1
      if(0==y)
      {
        case 2:  //违背1
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: switch语句的所有分支必须具有相同的层次范围 [gjb8114-case-in-different-layer]
        z=1;
        break;
      }
      z=2;
      break;
    case 3:  //遵循2
      {
        default:  //违背2
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: switch语句的所有分支必须具有相同的层次范围 [gjb8114-case-in-different-layer]
        z=6;
      }
      z=3;
      break;
    case 4:  //遵循3
    case 5:  //遵循4
    case 6:  //遵循5
      z=9;
      break;
    case 7:  //遵循6
      if(z==9)
      {
        case 8:  //违背3
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: switch语句的所有分支必须具有相同的层次范围 [gjb8114-case-in-different-layer]
          z++;
          break;
      }
    case 9:  //遵循7
      break;
    case 10:  //遵循8
      m=m+2;
      break;
    case 11:  //遵循9
      m=m+3;
    case 12:
      m++;
      break;
    case 13:  
      while(z>=-1)
      {
        z--;
        if(z==0)
          case 14:  //违背4
          // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: switch语句的所有分支必须具有相同的层次范围 [gjb8114-case-in-different-layer]
            z++;
            break;
      }
    case 15:
      switch(m)
      {
        case 20:  //遵循10
          break;
      }
      z++;
    case 16:
      z--;
      do
      {
        case 17:  //违背5
        // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: switch语句的所有分支必须具有相同的层次范围 [gjb8114-case-in-different-layer]
          z++;
      }while(z<0);
  }
  return (0);
}
