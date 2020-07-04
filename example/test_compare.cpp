#include "h2unit.h"

extern "C" {
#include "product_c.h"
}

SUITE(Number compares)
{
   Case(Calculate rectangle area successful)
   {
      rectangle_t p1 = {2, 3};

      OK(6, rectangle_area(&p1));  // successful
   }

   Case(get rectangle diag length failure)
   {
      rectangle_t p1 = {3, 4};
      OK(5, rectangle_diag(&p1));  // successful
      rectangle_t p2 = {1, 1};
      OK(Eq(1.41, 0.000001), rectangle_diag(&p2))
        << "float precision problem " << 1.41421356237;  // failure
   }

   Case(test for great than failure)
   {
      rectangle_t p1 = {2, 3};

      OK(Gt(5), rectangle_area(&p1));   // successful
      OK(!Gt(5), rectangle_area(&p1));  // failure
   }
}

SUITE(String compares)
{
   /*
   * OK can be used to verify Case sensitive String.
   */
   Case(string equal failure)
   {
      rectangle_t p1 = {0, 0};
      OK("Rect(0, 0)", rectangle_tostring(&p1));  // successful
      rectangle_t p2 = {1, 2};
      OK("Rect(0, 0)", rectangle_tostring(&p2));  // failure
   }

   Case(demo Substr failure)
   {
      rectangle_t p1 = {0, 0};
      OK(Substr("(0, 0)"), rectangle_tostring(&p1));  // successful
      rectangle_t p2 = {1, 2};
      OK(Substr("(1, 1)"), rectangle_tostring(&p2));  // failure
   }

   Case(demo StartsWith failure)
   {
      rectangle_t p1 = {0, 0};
      OK(StartsWith("Rect"), rectangle_tostring(&p1));  // successful
      OK(StartsWith("Pect"), rectangle_tostring(&p1));  // failure
   }

   Case(demo EndsWith failure)
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
   Case(wildcard string failure)
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
   Case(regex string failure)
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
   * Case-Less string compare
   */

   Case(caseless string equal failure)
   {
      rectangle_t p1 = {0, 0};
      OK(CaseLess("rect(0, 0)"), rectangle_tostring(&p1));  // successful
      rectangle_t p2 = {1, 2};
      OK(CaseLess("RECT(1, 1)"), rectangle_tostring(&p2));  // failure
   }

   Case(caseless starts / end with failure)
   {
      rectangle_t p1 = {0, 0};
      OK(CaseLess(StartsWith("Rect(0, 0)")), rectangle_tostring(&p1));  // successful
      rectangle_t p2 = {1, 2};
      OK(CaseLess(EndsWith("R(1, 2)")), rectangle_tostring(&p2));  // failure
   }
}

CASE(string compares failure)
{
   OK("a752a5eee24a149f899\r3e48ce81b77b200b212b23c2402c221ddc4260\na608cf32"
      "8a2133386477eb7a65958893726741cb9223419a7a0224fe7af2866944197ee4ab18e"
      "3cae8507bb306a284f",
      "a752a5eeX24a149f899\t3e48ce81b77b200b212b23c2402c2Y1ddc4260\na608cf32"
      "8a2133386477eb7a6595889372674Zcb9223419a7a0224fe7af2866944197ee4ab18e"
      "3cae8507bb306a284f");
}
/*
 * h2unit Me (Memory equal) can be used to verify memory buffer.
 */
SUITE(memory compares)
{
   Case(bytes failure)
   {
      const unsigned char e[] = "abcdefghijklmnopqrstuvwxyz";
      OK(M8e(e, sizeof(e)), "abcdefghijklmnopqrstuvwxyz");  // successful
      OK(M8e(e, sizeof(e)), "abcdEfghijklmnopqrsTuvwxyz");  // failure
   }
   Case(bits successfull)
   {
      unsigned char a1[] = {0x8E, 0xC8, 0x8E, 0xC8, 0xF8};
      OK(M1e("1000 1110 1100 1000 1000 1110 1100 1000 1111 1"), a1);  // successful
   }
   Case(short successfull)
   {
      short e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      short a1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(M16e(e, 9), a1);  // successful
   }
   Case(int successfull)
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      int a1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(M32e(e, 9), a1);  // successful
   }
   Case(long long successfull)
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      long long a1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(M64e(e, 9), a1);  // successful
   }

   Case(Me guess successfull)
   {
      unsigned char a1[] = {0x8E, 0xC8, 0x8E, 0xC8, 0xF8};
      OK(Me("1000 1110 1100 1000 1000 1110 1100 1000 1111 1"), a1);  // successful
      OK(Me("8EC88EC8F8"), a1);                                      // successful

      OK(Me("abcdefghijklmnopqrstuvwxyz"), "abcdefghijklmnopqrstuvwxyz");  // successful
   }
}
/*
 * h2unit Pointee can be used to verify the data pointer point to.
 */
CASE(point to compares failure)
{
   int x = 1;
   OK(Pointee(2), &x);  // failure
}

SUITE(zero operator)
{
   Case(Any)
   {
      OK(_, 0);
      OK(_, 1);
      OK(Any, 0);
      OK(Any, 1);
   }

   Case(IsNull failure)
   {
      int x;
      OK(IsNull, nullptr);  // successful
      OK(IsNull, &x);       // failure
   }

   Case(NotNull failure)
   {
      int x;
      OK(NotNull, &x);    // successful
      OK(NotNull, NULL);  // failure
   }

   Case(IsTrue failure)
   {
      OK(IsTrue, true);   // successful
      OK(IsTrue, false);  // failure
   }

   Case(IsFalse failure)
   {
      OK(IsFalse, false);  // successful
      OK(IsFalse, true);   // failure
   }
}
