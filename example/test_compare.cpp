extern "C" {
#include "product_c.h"
}

#if defined H2UNIT && H2UNIT == 2
#include "../build/h2unit.hpp"
#else
#include "../h2unit.h"
#endif

SUITE(rectangle compares [pass])
{
   Case(rectangle area)
   {
      rectangle_t p1 = {2, 3};

      OK(6, rectangle_area(&p1));
      OK(6 == rectangle_area(&p1));
   }

   Case(rectangle diag length)
   {
      rectangle_t p1 = {3, 4};
      OK(5, rectangle_diag(&p1));
      rectangle_t p2 = {1, 1};
      OK(1.41421356237, rectangle_diag(&p2));
      OK(Eq(1.41, 1%), rectangle_diag(&p2));
      OK(Eq(1.4142135, 0.000001), rectangle_diag(&p2));
   }

   Case(great than)
   {
      rectangle_t p1 = {2, 3};

      OK(Gt(5), rectangle_area(&p1));
      OK(rectangle_area(&p1) > 5);
   }
}

SUITE(types compares [fail])
{
   Case(unary)
   {
      OK(2011);
      OK("");
      OK(0);
   }

   Case(integer)
   {
      int a1 = 2017;
      OK(2013, a1);
      OK(Nq(2015), 2015);
   }

   Case(integer less than)
   {
      int a1 = 2015;
      OK(2017 > a1);
      OK(2017 < a1);
   }

   Case(double)
   {
      double a1 = 3.1415926;
      OK(1, a1);
   }

   Case(approximate)
   {
      OK(Eq(3.14, 0.1%), 3.1415926);
      OK(Eq(3.14, 0.01%), 3.1415926) << "float precision problem " << 3.1415926;
   }

   Case(approximate)
   {
      OK(Eq(3.1415926, 0.000001), 3.1415926);
      OK(Eq(3.14, 0.000001), 3.1415926) << "float precision problem " << 3.1415926;
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

SUITE(String compares [fail])
{
   /*
    * OK can be used to verify Case sensitive String.
    */

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

   Case(string equal)
   {
      rectangle_t p1 = {0, 0};
      OK("Rect(0, 0)", rectangle_tostring(&p1));  // successful
      rectangle_t p2 = {1, 2};
      OK("Rect(0, 0)", rectangle_tostring(&p2));  // failure
   }

   Case(StartsWith)
   {
      rectangle_t p1 = {0, 0};
      OK(StartsWith("Rect"), rectangle_tostring(&p1));  // successful
      OK(StartsWith("Pect"), rectangle_tostring(&p1));  // failure
   }

   Case(EndsWith)
   {
      rectangle_t p1 = {0, 0};
      OK(EndsWith("(0, 0)"), rectangle_tostring(&p1));  // successful
      OK(EndsWith("(1, 1)"), rectangle_tostring(&p1));  // failure
   }

   /*
    * h2unit WILDCARD/We can be used to verify string by WildCard. support:
    *  -- ? any one char
    *  -- * any char(s)
    */
   Case(wildcard string)
   {
      rectangle_t p1 = {0, 0};
      OK(We("Rect(0, ?)"), rectangle_tostring(&p1));  // successful
      rectangle_t p2 = {1, 2};
      OK(We("Rect(*]"), rectangle_tostring(&p2));  // failure
   }

   /*
    * h2unit REGEX/Re can be used to verify string by Regular express.
    * http://www.cplusplus.com/reference/regex/ECMAScript/
    */
   Case(regex string [re])
   {
      rectangle_t p1 = {0, 0};
      OK(Re("Rect\\(0, 0\\)"), rectangle_tostring(&p1));  // successful
      rectangle_t p2 = {1, 2};
      OK(Re("Rect\\(.*"), rectangle_tostring(&p2));  // successful
      rectangle_t p3 = {3, 4};
      OK(Re("Rect\\(.*\\)"), rectangle_tostring(&p3));  // successful
      rectangle_t p4 = {5, 6};
      OK(Re("^Rect\\(.*\\)$"), rectangle_tostring(&p4));  // successful
      rectangle_t p5 = {7, 8};
      OK(Re("Rect\\([789], [0-9]\\)"), rectangle_tostring(&p5));  // successful
      rectangle_t p6 = {11, 88};
      OK(Re("Rect\\([0-9]*, [0-9]*\\)"), rectangle_tostring(&p6));  // successful
      rectangle_t p7 = {11, 88};
      OK(Re("Rect\\([0-9]+, [^6-9]+\\)"), rectangle_tostring(&p7));  // failure
   }

   /*
    * Case-insensitive string compare
    */

   Case(caseless string equal)
   {
      rectangle_t p1 = {0, 0};
      OK(CaseLess("rect(0, 0)"), rectangle_tostring(&p1));  // successful
      rectangle_t p2 = {1, 2};
      OK(CaseLess("RECT(1, 1)"), rectangle_tostring(&p2));  // failure
   }

   Case(caseless starts/ends with)
   {
      rectangle_t p1 = {0, 0};
      OK(CaseLess(StartsWith("Rect(0, 0)")), rectangle_tostring(&p1));  // successful
      rectangle_t p2 = {1, 2};
      OK(CaseLess(EndsWith("R(1, 2)")), rectangle_tostring(&p2));  // failure
   }
}

SUITE(sanity operator)
{
   Case(Any [pass])
   {
      OK(_, 0);
      OK(_, 1);
      OK(Any(), 0);
      OK(Any(), 1);
   }

   Case(Is Null [fail])
   {
      int x;
      OK(NULL, nullptr);  // successful
      OK(nullptr, &x);    // failure
   }

   Case(Not Null [fail])
   {
      int x;
      OK(Not(NULL), &x);       // successful
      OK(Not(nullptr), NULL);  // failure
   }

   Case(Is True [fail])
   {
      OK(true, true);   // successful
      OK(true, false);  // failure
   }

   Case(Is False [fail])
   {
      OK(false, false);  // successful
      OK(false, true);   // failure
   }
}
