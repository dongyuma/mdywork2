
unsigned char U8;
signed char S8;

void assign(unsigned U, signed char S) {

 if (S < -10)
    U8 = S; // case 1 {{Loss of sign in implicit conversion}}
  if (U > 300)
    U8 = U; // case 2 {{Loss of precision in implicit conversion}}

  unsigned T = 0;
  S8 = S; // case 1 compliant
  T = U; // case 2 compliant

 
}

void mulAssign(unsigned U, signed char S) {

  unsigned long L = 1000;
  int I = -1;
  U8 *= L; // case 4 {{Loss of precision in implicit conversion}}
  L *= I;  // case 3 {{Loss of sign in implicit conversion}}
  
  unsigned long L2 = 1;
  L2 *= L; // case 4 compliant
  unsigned long I2 = 1;
  L *= I2;  // case 3 compliant
   
}

void init() 
{
  signed char C = -1;
  unsigned char Z = C;// case 5 {{Loss of sign in implicit conversion}}
  long long A = 1LL << 60;
  short X = A; // case 6 {{Loss of precision in implicit conversion}}
  
  signed char C2 = 1;
 // signed char Z = C2;// case 5 compliant
  long long X2 = A; // case 6 compliant
}

void f(unsigned x) {}


void functioncall() {
  signed x = -1;
  f(x); // case 7 {{Loss of sign in implicit conversion}}
  unsigned long long y = 1LL << 60;
  f(y); // case 8 {{Loss of precision in implicit conversion}}
  
  unsigned z = 1;
  f(z); // case 7 case 8 compliant
  
}

void floatconversion() {
  float f = (float)1.1;
  int a = 1; 
  a = f; //case 9
  f = a; //case 10
  
  float f2 = (float)2.2;
  int a2 = 2;
  a = a2; //case 9 compliant
  f = f2; //case 10 compliant
  f = (float)2.2; //case 11 compliant
  
}