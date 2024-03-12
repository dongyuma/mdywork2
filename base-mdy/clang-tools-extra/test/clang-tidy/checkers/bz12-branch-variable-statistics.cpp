// RUN: %check_clang_tidy %s bz12-branch-variable-statistics %t

void func1()
{
    int x=0, y=1, z=2;
    int m = 10;
    if(m > 0) {
        x = 100;
        z = 1;
    } else {
        y = 20;
        z = 2;
    }

    if(m > 0) {
        x = 100;
        z = 1;
    } else if(m > -100){
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: If分支可能需要对以下变量进行赋值操作：x [bz12-branch-variable-statistics]
        y = 20;
        z = 2;
    } else {
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: Else分支可能需要对以下变量进行赋值操作：z [bz12-branch-variable-statistics]
        x = 10;
    }

    switch (m) {
    case 1:
        x = 1;
        y = 1;
        break;
    case 10:
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Switch结构的当前Case分支可能需要对以下变量进行赋值操作：x, y [bz12-branch-variable-statistics]
        z = 1;
        break;
    case 100:
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Switch结构的当前Case分支可能需要对以下变量进行赋值操作：y [bz12-branch-variable-statistics]
        x = 1;
        break;
    default:
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Switch结构的当前Default分支可能需要对以下变量进行赋值操作：x [bz12-branch-variable-statistics]
        y = 1;
        break;
    }
}

void func2()
{
    int x = 0, y = 1, z = 2;
    int m = 10;
    if (m > 0) {
        x = 100;
        z = 1;
    }
    else {
        y = 20;
        z = 2;
    }

    if (m > 0) {
        x++;
        z = 1;
    }
    else if (m > -100) {
        // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: If分支可能需要对以下变量进行赋值操作：x [bz12-branch-variable-statistics]
        y = 20;
        --z;
    }
    else {
        // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: Else分支可能需要对以下变量进行赋值操作：z [bz12-branch-variable-statistics]
        x++;
    }

    switch (m) {
    case 1:
        x++;
        y--;
        break;
    case 10:
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Switch结构的当前Case分支可能需要对以下变量进行赋值操作：x, y [bz12-branch-variable-statistics]
        z = 1;
        break;
    case 100:
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Switch结构的当前Case分支可能需要对以下变量进行赋值操作：y [bz12-branch-variable-statistics]
        x = 1;
        break;
    default:
        // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Switch结构的当前Default分支可能需要对以下变量进行赋值操作：x [bz12-branch-variable-statistics]
        y = 1;
        break;
    }
}

void func3()
{
    int x=0, y=1, z=2;
    int m = 10;
    if(m > 0) {
        x += 100;
        z = 1;
    } else {
        y = 20;
        z = 2;
    }

    if(m > 0) {
        x += 100;
        z -= 1;
    } else if(m > -100){
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: If分支可能需要对以下变量进行赋值操作：x [bz12-branch-variable-statistics]
        y *= 20;
        z = 2;
    } else {
// CHECK-MESSAGES: :[[@LINE-1]]:12: warning: Else分支可能需要对以下变量进行赋值操作：z [bz12-branch-variable-statistics]
        x /= 10;
    }

    switch (m) {
    case 1:
        x += 1;
        y = 1;
        break;
    case 10:
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Switch结构的当前Case分支可能需要对以下变量进行赋值操作：x, y [bz12-branch-variable-statistics]
        z = 1;
        break;
    case 100:
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Switch结构的当前Case分支可能需要对以下变量进行赋值操作：y [bz12-branch-variable-statistics]
        x -= 1;
        break;
    default:
// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: Switch结构的当前Default分支可能需要对以下变量进行赋值操作：x [bz12-branch-variable-statistics]
        y = 1;
        break;
    }
}