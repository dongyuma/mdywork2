// RUN: %check_clang_tidy %s misrac2012-persistent-side-effects-not-same %t

int f(int a,int b)
{
  return a+b;
}
int main()
{
  extern volatile int violate_1, violate_2;
  int result_1;
  result_1=violate_1+violate_2;  //违背1
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 在所有允许的评估顺序下，表达式的值及其持续的副作用应是相同的 [misrac2012-persistent-side-effects-not-same]
  int normal_1,normal_2;
  int result_2;
  result_2=violate_1+normal_2;  //遵循1
  result_2=normal_1+normal_2;  //遵循2
  result_1=violate_1+normal_1+normal_2+violate_1;  //违背2
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 在所有允许的评估顺序下，表达式的值及其持续的副作用应是相同的 [misrac2012-persistent-side-effects-not-same]
  int result_3=violate_1+violate_2;  //违背3
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 在所有允许的评估顺序下，表达式的值及其持续的副作用应是相同的 [misrac2012-persistent-side-effects-not-same]
  int i=1;
  f(i++,i);  //违背4
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 在所有允许的评估顺序下，表达式的值及其持续的副作用应是相同的 [misrac2012-persistent-side-effects-not-same]
  f(i,i);  //遵循3
  f(i++,++i);  //违背5
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: 在所有允许的评估顺序下，表达式的值及其持续的副作用应是相同的 [misrac2012-persistent-side-effects-not-same]
}
