// RUN: %check_clang_tidy %s gjb8114-error-declare-of-struct %t

//CSR-48	结构体初始化的嵌套结构必须与定义一致	R-1-11-3	gjb8114-error-declare-of-struct
//CSR-48 GJB 8114-2013 R-1-11-3
//违背示例
struct Spixel
{
  unsigned int colour0;
  unsigned int colour1;
  unsigned int colour2;
  unsigned int colour3;
  unsigned int colour4;
  struct Scoords
  {
    unsigned int x;
    unsigned int y;
    struct Scoords1
    {
      unsigned int z;
      unsigned int w;
    }coords1;
    
  }coords;
  unsigned int colour5;
  unsigned int colour6;
};
struct struct1
{
  int data1;
  unsigned int data2;
  int data3;
  unsigned int data4;
  struct struct2
  {
    int data5;
    int data6;
  }s2;
  int data7;
  double data8;
};
struct struct3
{
  int data1;
  long data2;
  int data3;
  struct struct4
  {
    unsigned short data4;
    short data5;
  }s4;
  double data6;
};
struct ST_TABLE_ITEM
{
    int iV1;
    struct SUBITEM
    {
        int iV2;
        int iV3;
    }subITEM;
};
int main(void)
{
  struct Spixel pixel0={1,2,3,4,5,6,7,8,9,10,11};  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:24: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-error-declare-of-struct]
  struct Spixel pixel1={1,2,3};  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:24: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-error-declare-of-struct]
  struct Spixel pixel2={1,2,3,4,5,{6,7,8,9},10,11};  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:24: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-error-declare-of-struct]
  struct Spixel pixel3={1,2,3,4,5,{6,7,{8,9}},10,11};  //遵循1
  struct struct1 pixel4={1,2,3,4,5,{{8,9}},10,10.5};  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-error-declare-of-struct]
  struct struct1 pixel5={1,2,3,4,5,6,7,10.5};  //违背5
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-error-declare-of-struct]
  struct struct1 pixel6={1,2,3,4,{5,6},7,10.5};  //遵循2
  struct struct1 pixel7={1,2,3,4,{55,67},9,10.5};  //遵循3
  struct struct1 pixel8={1,2,3,4,{{{{55}}},{67}},9,10.5};  //违背6
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-error-declare-of-struct]
  struct struct3 pixel9={1,2,3,{4,5},10.5};  //遵循4
  pixel0={1,2,3,4,5,6,7,8,9,10,11};  //违背7
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-error-declare-of-struct]
  pixel1={1,2,3};  //违背8
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-error-declare-of-struct]
  pixel2={1,2,3,4,5,{6,7,8,9},10,11};  //违背9
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-error-declare-of-struct]
  pixel3={1,2,3,4,5,{6,7,{8,9}},10,11};  //遵循5
  pixel3={56,45,66,7,8,{9,0,{88,66}},160,116};  //遵循6
  int data1=1;
  return (0);
}
void R_1_11_3()
{
    struct ST_TABLE_ITEM stItem = {1 , 2 , 3};  //违背10
    // CHECK-MESSAGES: :[[@LINE-1]]:35: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-error-declare-of-struct]
    struct ST_TABLE_ITEM stItem1 = {1 ,{ 2,   3}};  //遵循7
    struct ST_TABLE_ITEM stItem2 = {1,{2,3} };  //遵循8
    struct ST_TABLE_ITEM stItem3 = {1,{2,3}};  //遵循9
    struct ST_TABLE_ITEM stItem4 = {1,2, 3};  //违背11
    // CHECK-MESSAGES: :[[@LINE-1]]:36: warning: 结构体初始化的嵌套结构必须与定义一致 [gjb8114-error-declare-of-struct]
}