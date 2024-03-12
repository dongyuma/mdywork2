// RUN: %check_clang_tidy %s misrac2012-cast-const %t

typedef signed   char           int8_t;
typedef signed   short          int16_t;
typedef signed   int            int32_t;
typedef signed   long           int64_t;
typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long           uint64_t;
typedef          char           char_t;
typedef signed   long long      int128_t;
typedef unsigned long long      uint128_t;
typedef          float          float32_t;
typedef          double         float64_t;
typedef          long double    float128_t;
void fun()
{
   uint16_t x;
   uint16_t * const cpi = &x; /* const pointer */
   uint16_t * const *pcpi; /* pointer to const pointer */
   uint16_t * const *pcpi1; /* pointer to const pointer */
   uint16_t * *ppi;
   const uint16_t *pci; /* pointer to const */
   volatile uint16_t *pvi; /* pointer to volatile */
   uint16_t *pi;
   pi = cpi;  //遵循1
   pi = (uint16_t *)pci; //违背1
   // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: 指针转换不应该转换掉const或者volatile限定符 [misrac2012-cast-const]
   pi = (uint16_t *)pvi; //违背2
   // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: 指针转换不应该转换掉const或者volatile限定符 [misrac2012-cast-const]
   ppi = (uint16_t * *)pcpi; //违背3
   // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: 指针转换不应该转换掉const或者volatile限定符 [misrac2012-cast-const]
   pcpi=pcpi1;  //遵循2
   pci=(uint16_t *)pvi;  //违背4
   // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: 指针转换不应该转换掉const或者volatile限定符 [misrac2012-cast-const]
}