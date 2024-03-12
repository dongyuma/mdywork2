// RUN: %check_clang_tidy %s gjb8114-bool-switch %t

//CSR-173	禁止对bool量使用switch语句				R-1-4-4	gjb8114-bool-switch	Rule 16.7	misrac2012-BoolSwitch
//CSR-173  GJB 8114-2013 R-1-4-4
//违背示例
//禁止对 bool 量使用 switch 语句。违背示例: 
int main_1(void)
{
    int x=0;
    int y=0;
    switch ( 0 == x )//违背1
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: 禁止对bool量使用switch语句 [gjb8114-bool-switch]
    {
        case 1:
        y = 1; 
        break;
        default:
        y = 2; 
        break;
    }
    return (0); 
}

int main_2(void)
{
    int x=0;
    int y=0;
    switch ( !x )//违背2
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: 禁止对bool量使用switch语句 [gjb8114-bool-switch]
    {
        case 1:
        y = 1; 
        break;
        default:
        y = 2; 
        break;
    }
    switch ( (!x) );//违背2 
    switch ( !(x) )//违背2-1 
    // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: 禁止对bool量使用switch语句 [gjb8114-bool-switch]
    {
        case 1:
        y = 1; 
        break;
        default:
        y = 2; 
        break;
    }
    return (0); 
}

void intAsBoolAsSwitchCondition(int c) {
  switch ((bool)c) { // expected-warning {{switch condition has boolean value}}
  // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: 禁止对bool量使用switch语句 [gjb8114-bool-switch]
  case 0:
    break;
  }

  switch ((int)(bool)c) { // no-warning
    case 0:
      break;
  }
}