// RUN: %check_clang_tidy %s gjb8114-default-in-switch %t

//CSR-163	在switch语句中必须有default语句		4.3.1.4	ConditionalBranchChecker	R-1-4-6	gjb8114-UnionSwitch	Rule 16.4	misrac2012-DefaultLabel
//CSR-163  GJB 8114-2013 R-1-4-6
enum WorkMode {INI=0,FIGHT,MAINTAIN,TRAIN} work_state;
enum WorkMode1 {TRY,NO,YES} work_state1;
enum WorkMode2 {JOY,YESD} work_state2;
typedef enum{
    Type_A = 0,
    Type_B,
    Type_C,
    Type_D,
    Type_E
}DATA_TYPE;
int main(void)
{
    int i=0;
    int j=0;
    char word='c';
    switch(i) //违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 除枚举类型列举完全外，switch必须要有default [gjb8114-default-in-switch]
    {
      case 0:
        j=-5;
        break;
      case 1:
        j=-6;
        break;
    }
    switch(i) //遵循1
    {
      case 0:
        j=-5;
        break;
      case 1:
        j=-6;
        break;
      default:
        j=-7;
        break;
    }
    switch(word) //违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 除枚举类型列举完全外，switch必须要有default [gjb8114-default-in-switch]
    {
      case 'a':
        j=-1;
        break;
      case 'c':
        j=-2;
        break;
    }
    switch(word) //遵循2
    {
      case 'a':
        j=-1;
        break;
      case 'c':
        j=-2;
        break;
      default:
        j=-3;
        break;
    }
    switch(work_state)  //遵循3
    {
      case FIGHT:
        j=2;
        break;
      case MAINTAIN:
        j=3;
        break;
      case TRAIN:
        j=4;
        break;
      case INI:
        j=5;
        break;
    }
    switch(work_state)  //遵循4
    {
      case FIGHT:
        j=2;
        break;
      case MAINTAIN:
        j=3;
        break;
      case TRAIN:
        j=4;
        break;
      default:
        j=100;
        break;
    }
    switch(work_state)  //违背3
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 除枚举类型列举完全外，switch必须要有default [gjb8114-default-in-switch]
    {
      case TRAIN:
        j=4;
        break;
      case INI:
        j=5;
        break;
    }
    switch(work_state2)  //违背4
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 除枚举类型列举完全外，switch必须要有default [gjb8114-default-in-switch]
    {
      case JOY:
        j=4;
        break;
    }
    switch(work_state2); //违背5
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 除枚举类型列举完全外，switch必须要有default [gjb8114-default-in-switch]
    switch(i);  //违背6
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 除枚举类型列举完全外，switch必须要有default [gjb8114-default-in-switch]
    switch(work_state2); //违背7
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 除枚举类型列举完全外，switch必须要有default [gjb8114-default-in-switch]
    switch(i);  //违背8
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 除枚举类型列举完全外，switch必须要有default [gjb8114-default-in-switch]
    switch(work_state2)  //遵循5
    {
      default:
        j=9;
        break;
    }
    switch(i)  //遵循6
    {
      default:
        j=10;
        break;
    }
    return (0);
}
void R_1_4_6()
{
    DATA_TYPE dType;
    int iTemp;
    switch(dType)  //违背9
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: 除枚举类型列举完全外，switch必须要有default [gjb8114-default-in-switch]
    {
    case Type_E:
        {
            iTemp = 1;
            break;
        }
    case Type_A:
        {
            iTemp = 2;
            break;
        }
    case Type_B:
        {
            iTemp = 3;
            break;
        }
    case Type_D:
        {
            iTemp = 4;
            break;
        }
    }
}
/*
  * R-1-4-6: 除枚举类型列举完全外，switch必须要有default。
  * 错误场景: R_1_4_6_case1中枚举类型没有列举完全，R_1_4_6_case1_cmp为对照组，没有问题
  */
void R_1_4_6_case1()
{
	DATA_TYPE msg_type;
	int iTemp;
	switch (msg_type)  //违背10
  // CHECK-MESSAGES: :[[@LINE-1]]:2: warning: 除枚举类型列举完全外，switch必须要有default [gjb8114-default-in-switch]
	{
	case Type_E:
		{
			iTemp = 1;
			break;
		}
	case Type_A:
		{
			iTemp = 2;
			break;
		}
	case Type_B:
		{
			iTemp = 3;
			break;
		}
	case Type_D:
		{
			iTemp = 4;
			break;
		}
	}
}

void R_1_4_6_case1_cmp()
{
	DATA_TYPE msg_type;
	int iTemp;
	switch (msg_type)
	{
	case Type_E:
		{
			iTemp = 1;
			break;
		}
	case Type_A:
		{
			iTemp = 2;
			break;
		}
	case Type_B:
		{
			iTemp = 3;
			break;
		}
	case Type_C:
		{
			iTemp = 4;
			break;
		}
	case Type_D:
		{
			iTemp = 5;
			break;
		}
	}
}