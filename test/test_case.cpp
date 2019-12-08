#include "../source/h2_unit.h"

H2UNIT (h2_case) {
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(h2_case, different number type compare)
{
   unsigned char e_unsigned_char = 2;
   unsigned char a_unsigned_char = 2;
   H2EQ(e_unsigned_char, a_unsigned_char);

   char e_char = 2;
   char a_char = 2;
   H2EQ(e_char, a_char);

   short e_short = 2;
   short a_short = 2;
   H2EQ(e_short, a_short);

   unsigned short e_unsigned_short = 2;
   unsigned short a_unsigned_short = 2;
   H2EQ(e_unsigned_short, a_unsigned_short);

   long e_int = 2;
   long a_int = 2;
   H2EQ(e_int, a_int);

   unsigned long e_unsigned_int = 2;
   unsigned long a_unsigned_int = 2;
   H2EQ(e_unsigned_int, a_unsigned_int);

   long e_long = 2;
   long a_long = 2;
   H2EQ(e_long, a_long);

   unsigned long e_unsigned_long = 2;
   unsigned long a_unsigned_long = 2;
   H2EQ(e_unsigned_long, a_unsigned_long);

   unsigned long long e_unsigned_long_long = 0x12345678900ULL;
   unsigned long long a_unsigned_long_long = 0x12345678900ULL;
   H2EQ(e_unsigned_long_long, a_unsigned_long_long);

   long long e_long_long = 0x1234567890LL;
   long long a_long_long = 0x1234567890LL;
   H2EQ(e_long_long, a_long_long);

   // char * e_char_pointer = (char*) 0;
   // char * a_char_pointer = (char*) 0;
   // H2EQ(e_char_pointer, a_char_pointer);

   int* e_int_pointer = (int*)0;
   int* a_int_pointer = (int*)0;
   H2EQ(e_int_pointer, a_int_pointer);

   double* e_double_pointer = (double*)0;
   double* a_double_pointer = (double*)0;
   H2EQ(e_double_pointer, a_double_pointer);

   void* e_void_pointer = (void*)0x12345678;
   void* a_void_pointer = (void*)0x12345678;
   H2EQ(e_void_pointer, a_void_pointer);

   enum {
      EE_A1 = 1,
      EE_B2,
      EE_C3,
      EE_D4
   };

   H2EQ(2, EE_B2);
   H2EQ(EE_D4, 4);
}
