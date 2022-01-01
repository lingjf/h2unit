
extern "C" {
#include "product_c.h"
}

#include "product_cpp.h"

#if defined H2UNIT && H2UNIT == 2
#include "../build/h2unit.hpp"
#else
#include "../h2unit.h"
#endif

static int foo(int a, const char* b)
{
   return 0;
}

/*
 * h2unit Me (Memory equal) can be used to verify memory buffer.
 */
SUITE(memory compares)
{
   Case(bytes successfull [pass])
   {
      const unsigned char e[] = "abcdefghijklmnopqrstuvwxyz";
      OK(Me(e, sizeof(e)), "abcdefghijklmnopqrstuvwxyz");
   }

   Case(bytes [fail])
   {
      const unsigned char e[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
      OK(Me(e, sizeof(e)), "abcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyz");
   }

   Case(bits [fail])
   {
      unsigned char a1[] = {0x8E, 0xC8, 0x8E, 0xC8, 0xF8};
      OK(Me("1000 1110 1100 1000 1000 1110 1100 1000 1111 1"), a1);
      OK(Me("1000 1110 1110 1000 1000 1111 1100 1000 0000 1"), a1);
   }

   Case(short successfull [pass])
   {
      short e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      short a1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(Me(e, 9), a1);
   }

   Case(short [fail])
   {
      short e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      short a2[] = {1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0};
      OK(Me(e), a2);
   }

   Case(int successfull [pass])
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      int a1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(Me(e, 9), a1);
   }

   Case(int [fail])
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      int a2[] = {1, 2, 3, 0x55555555, 5, 6, 7, 8, 9, 0};
      OK(Me(e, 10), a2);
   }

   Case(long long successfull [pass])
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      long long a1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(Me(e, 9), a1);  // successful
   }

   Case(long long [fail])
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      long long a2[] = {1, 2, 3, 0x5555555555555555LL, 5, 6, 7, 8, 9, 0};
      OK(Me(e, 10), a2);
   }

   Case(guess successfull [pass])
   {
      unsigned char a1[] = {0x8E, 0xC8, 0x8E, 0xC8, 0xF8};
      OK(Me("1000 1110 1100 1000 1000 1110 1100 1000 1111 1"), a1);  // successful
      OK(Me("8EC88EC8F8"), a1);                                      // successful
   }

   Case(mock [fail])
   {
      MOCK(foo, int(int, const char*)).Once(1, Me("abcdefghijKlmnopqrszuvwxyz1234567890"));
      foo(1, "abcdefghijklmnopqrszuvwxyz1234567890");
   }

   Case(load expection from File)
   {
      Rect p1 = {0, 0, 1, 1};
      p1.dump("_temp_p1.model");

      unsigned char data[1024];
      p1.serialize(data, sizeof(data));

      OK(Me(File("_temp_p1.model")), data);
   }
}
