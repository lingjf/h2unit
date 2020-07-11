#include "../source/h2_unit.cpp"

SUITE(string compares)
{
   Case(unprintable characters)
   {
      OK("a752a5eee24a149f899\r3e48ce81b77b200b212b23c2402c221ddc4260\na608cf32"
         "8a2133386477eb7a65958893726741cb9223419a7a0224fe7af2866944197ee4ab18e"
         "3cae8507bb306a284f",
         "a752a5eeX24a149f899\t3e48ce81b77b200b212b23c2402c2Y1ddc4260\na608cf32"
         "8a2133386477eb7a6595889372674Zcb9223419a7a0224fe7af2866944197ee4ab18e"
         "3cae8507bb306a284f");
   }

   Case(caseless not)
   {
      OK(!~(Se("Hello World") && Substr("Hello") || Re(".*World")), "hello world");
      OK(!~AllOf("Hello World", Substr("Hello"), Re(".*World")), "hello world");
      OK(!~AnyOf("Hello World", Substr("Hello"), Re(".*World")), "hello world");
      OK(~NoneOf("Hello World", Substr("Hello"), Re(".*World")), "hello world");
   }
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
      OK(M1e("1000 1110 1110 1000 1000 1111 1100 1000 0000 1"), a1);
   }
   Case(short failure)
   {
      short e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      short a2[] = {1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0};
      OK(M16e(e, 100), a2);
   }
   Case(int failure)
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      int a2[] = {1, 2, 3, 0x55555555, 5, 6, 7, 8, 9, 0};
      OK(M32e(e, 10), a2);
   }
   Case(long long failure)
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      long long a2[] = {1, 2, 3, 0x5555555555555555LL, 5, 6, 7, 8, 9, 0};
      OK(M64e(e, 10), a2);
   }
}
