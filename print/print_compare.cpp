#include "../build/h2unit.hpp"

SUITE(number compares)
{
   Case(integer)
   {
      int a1 = 2017;
      OK(2013, a1);
      OK(Nq(2015), 2015);
   }

   Case(integer less than)
   {
      int a1 = 2015;
      Ok(2017 < a1);
   }

   Case(vector great/equal than)
   {
      Ok(sizeof(std::vector<int>::size_type) > 100);
   }

   Case(double)
   {
      double a1 = 3.1415926;
      OK(1, a1);
   }

   Case(bool)
   {
      bool a1 = false;
      OK(true, a1);
   }

   Case(char)
   {
      char a1 = 'a';
      OK('b', a1) << "char is not same " << 123;
   }
}

SUITE(string compares)
{
   Case(unprintable characters)
   {
      const char* a1 = "hello the world 北京 welcone you\n!";
      OK("hello world 上海 welcome you!", a1);
   }

   Case(Chinese characters)
   {
      OK(Substr("兰溪"), "中国\n浙江省\t杭州市\r余杭区");
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
