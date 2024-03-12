// RUN: %check_clang_tidy %s bz12-struct-default-alignment %t

struct my_struct
{
    char a[5];
    char b;
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 结构体定义中的 'b' 项会出现结构体对齐现象 [bz12-struct-default-alignment]
    int c;
    int d;
};

struct my_struct1
{
    char struct1_a;
    int struct1_b;
};
struct my_struct2
{
    struct my_struct1 a;
    int struct2_a;
    char struct2_b;
    char struct2_c;
    // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: 结构体定义中的 'struct2_c' 项会出现结构体对齐现象 [bz12-struct-default-alignment]
};

struct my_struct3
{
    int a;
    int b;
    int c;
    int d;
};

struct my_struct4
{
    int a;
    int b;
    // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: 结构体定义中的 'b' 项会出现结构体对齐现象 [bz12-struct-default-alignment]
    int c;
    long e;
};

struct my_struct5
{ 
    char a[11]; 
    int b; 
    char c; 
}; 
