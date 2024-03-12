// RUN: %check_clang_tidy %s gjb8114-label-reuse-prohibit %t

//CSR-2	禁止变量名与标识名同名		4.1.1.2	gb5369-NameForbiddenReuse	R-1-13-4	gjb8114-label-reuse-prohibit	 
//CSR-2 GJB 8114-2013 R-1-13-4
//违背示例
struct POINTA
{
  unsigned int a;
  unsigned int b;
};
struct POINTB
{
  unsigned int b;
  unsigned int c;
};
int main_1(void)
{
  unsigned int POINTA;  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止变量名与标识名同名 [gjb8114-label-reuse-prohibit]
  struct POINTB POINTB;  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: 禁止变量名与标识名同名 [gjb8114-label-reuse-prohibit]
  POINTA=1;
  POINTB.b=POINTA;
  return (0);
}

enum POINTC{A,B,C};
enum POINTD{D,E,F};

int main_2(void)
{
  unsigned int POINTC;  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止变量名与标识名同名 [gjb8114-label-reuse-prohibit]
  enum POINTD POINTD;  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 禁止变量名与标识名同名 [gjb8114-label-reuse-prohibit]
  return (0);
}

union POINTE
{
  unsigned int d ;
  unsigned int e;
};
union POINTF
{
  unsigned int e ;
  unsigned int f;
};
int main_3(void)
{
  unsigned int POINTE;  //违背5
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止变量名与标识名同名 [gjb8114-label-reuse-prohibit]
  union POINTF POINTF;  //违背6
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: 禁止变量名与标识名同名 [gjb8114-label-reuse-prohibit]
  POINTE=1;
  POINTF.e=POINTE;
  return (0);
}

class POINTG
{
  public:
  void print1()
  {
  }
  int g;
};

int main_4(void)
{
  POINTG * pointg1 = new POINTG();
  pointg1->print1();
  pointg1->g=1;
  int POINTG;            //违背7
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: 禁止变量名与标识名同名 [gjb8114-label-reuse-prohibit]
  return (0);
}

//遵循示例
struct POINTA1
{
  unsigned int x;
  unsigned int y;
};
struct POINTB1
{
  unsigned int y;
  unsigned int z;
};
int main_5(void)
{
  unsigned int pot_y;    //遵循1
  struct POINTB1 spotd;  //遵循2
  pot_y=1;
  spotd.y=pot_y;
  return (0);
}
enum POINTC1{A1,B1,C1};
enum POINTD1{D1,E1,F1};

int main_6(void)
{
  unsigned int pot_y2;  //遵循3
  enum POINTD1 spotdf;  //遵循4
  return (0);
}

union POINTE1
{
  unsigned int d;
  unsigned int e;
};
union POINTF1
{
  unsigned int e;
  unsigned int f;
};
int main_7(void)
{
  unsigned int pot_y3;   //遵循5
  union POINTF1 spotdh;  //遵循6
  pot_y3=1;
  spotdh.e=pot_y3;
  return (0);
}

class POINTG1
{
  public:
  void print1()
  {
  }
  int g;
};

int main_8(void)
{
  POINTG1 * pointg1 = new POINTG1();
  pointg1->print1();
  pointg1->g=1;
  int pointg2;       //遵循7
  return (0);
}

