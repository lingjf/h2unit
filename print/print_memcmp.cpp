#include "../build/h2unit.hpp"

static int foo(int a, const char* b)
{
   return 0;
}

SUITE(memory compares)
{
   Case(bytes failure)
   {
      const unsigned char e[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
      OK(Me(e, sizeof(e)), "abcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyz");
   }

   Case(bits failure)
   {
      unsigned char a1[] = {0x8E, 0xC8, 0x8E, 0xC8, 0xF8};
      OK(Me("1000 1110 1110 1000 1000 1111 1100 1000 0000 1"), a1);
   }

   Case(short failure)
   {
      short e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      short a2[] = {1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0};
      OK(Me(e), a2);
   }

   Case(int failure)
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      int a2[] = {1, 2, 3, 0x55555555, 5, 6, 7, 8, 9, 0};
      OK(Me(e, 10), a2);
   }

   Case(long long failure)
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      long long a2[] = {1, 2, 3, 0x5555555555555555LL, 5, 6, 7, 8, 9, 0};
      OK(Me(e, 10), a2);
   }

   Case(mock failure)
   {
      MOCK(foo, int(int, const char*)).Once(1, Me("abcdefghijKlmnopqrszuvwxyz1234567890"));
      foo(1, "abcdefghijklmnopqrszuvwxyz1234567890");
   }
}

CASE(memory compares, missing length)
{
   int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
   int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
   int* e1 = e;
   OK(!Me(e1), a);
}
