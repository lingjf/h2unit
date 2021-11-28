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
      CP(2017 < a1);
   }

   Case(vector great/equal than)
   {
      CP(sizeof(std::vector<int>::size_type) > 100);
   }

   Case(double)
   {
      double a1 = 3.1415926;
      OK(1, a1);
   }

   Case(approximate)
   {
      OK(Eq(3.14, 0.01%), 3.1415926) << "float precision problem " << 3.1415926;
   }

   Case(bool bool)
   {
      bool a1 = false;
      OK(true, a1);
   }

   Case(bool int)
   {
      int a1 = 3;
      OK(false, a1);
   }

   Case(bool char*)
   {
      const char* a1 = "hello";
      OK(false, a1);
   }

   Case(char)
   {
      char a1 = 'a';
      OK('b', a1) << "char is not same " << 123;
   }

   Case(void*)
   {
      void* a1 = (void*)1234;
      OK(NULL, a1);
   }

   Case(void*)
   {
      void* a1 = (void*)1234;
      OK(nullptr, a1);
   }

   Case(void*)
   {
      void* a1 = (void*)nullptr;
      OK(Not(NULL), a1);
      OK(Not(nullptr), a1);
   }
}

SUITE(string compares)
{
   Case(unprintable characters)
   {
      const char* a1 = "hel1o the word 上海welconne 귀하 !\n ";
      OK("hello world 北京 welcome そのほう !", a1);
   }

   Case(Chinese characters)
   {
      OK(Substr("兰溪"), "中国\n浙江省\t杭州市\r余杭区");
   }

   Case(squash spaces)
   {
      OK(SpaceLess("hello  world!"), "hello   world");
   }

   Case(caseless not)
   {
      OK(!~(Se("Hello World") && Substr("Hello") || Re(".*World")), "hello world");
      OK(!~AllOf("Hello World", Substr("Hello"), Re(".*World")), "hello world");
      OK(!~AnyOf("Hello World", Substr("Hello"), Re(".*World")), "hello world");
      OK(~NoneOf("Hello World", Substr("Hello"), Re(".*World")), "hello world");
   }
}
