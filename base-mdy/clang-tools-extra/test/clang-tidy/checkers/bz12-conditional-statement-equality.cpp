// RUN: %check_clang_tidy %s bz12-conditional-statement-equality %t

// AS-15：检测条件语句使用=而不是==

void ifStmtFunction() {
    int x = 10;
    if(x=1) {
    // CHECK-MESSAGES: :[[@LINE-1]]:8: warning: 在条件语句中不建议使用符号:= . [bz12-conditional-statement-equality]
        ;
    } else if(x=2) {
    // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: 在条件语句中不建议使用符号:= . [bz12-conditional-statement-equality]
        ;
    } else {
        ;
    } 
}

void forStmtFunction() {
    int i = 10;
    for(i=1; i=3; i++) {
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: 在条件语句中不建议使用符号:= . [bz12-conditional-statement-equality]
        ;
    }
}

void whileStmtFunction() {
    int x = 10;
    while(x=4) {
    // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 在条件语句中不建议使用符号:= . [bz12-conditional-statement-equality]
        ;
    }
}

void doWhileStmtFunction() {
    int x = 10;
    do {
        ;
    } while(x=5);
    // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: 在条件语句中不建议使用符号:= . [bz12-conditional-statement-equality]
}
