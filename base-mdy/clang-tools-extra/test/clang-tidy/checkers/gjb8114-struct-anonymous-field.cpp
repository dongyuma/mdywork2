// RUN: %check_clang_tidy %s gjb8114-struct-anonymous-field %t


// CSR-3 结构体定义中禁止含有无名结构体 GJB R-1-1-9

// 违背示例
struct Sdata_v1
{
    unsigned char id;
    struct Scoor
    {
        unsigned char xs;
        unsigned char yx;
        unsigned char zs;
    };     // 违背1
    // CHECK-MESSAGES: :[[@LINE-6]]:12: warning: 结构体定义中禁止包含无名结构体。 [gjb8114-struct-anonymous-field]
};

int main_v1(void)
{
    struct Sdata_v1 data;
    data.id = 1;
    return(0);
}

// 遵循示例
struct Sdata_v2
{
    unsigned char id;
    struct Scoor
    {
        unsigned char xs;
        unsigned char yx;
        unsigned char zs;
    } coor;     // 遵循1
};

int main_v2(void)
{
    struct Sdata_v2 data;
    data.id = 1;
    return(0);
}