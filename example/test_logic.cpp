
extern "C" {
#include "product_c.h"
}

#if defined H2UNIT && H2UNIT == 2
#include "../build/h2unit.hpp"
#else
#include "../h2unit.h"
#endif

SUITE(Logic)
{
   Case(Not successful[pass])
   {
      rectangle_t p1 = {2, 3};
      OK(Not(5), rectangle_diag(&p1));
   }

   Case(Not Not [fail])
   {
      OK(!Not(5), 6);
   }

   Case(Not string equal [fail])
   {
      const char* p = "Hello,您好,ボンジュール,안녕하세요.5eee24a149f8993e48ce81b77b200b212b23c2402c221ddc4"
                      "260a608cf328a2133386477eb7a65958893726741cb9223419a7a0224f"
                      "e7af2866944197ee4ab18e25913cae8507bb306a284f";
      OK(Not(p), p);
   }

   Case(one and [fail])
   {
      int a = 0;
      int b = -2;
      OK(Ge(a) && Eq(b), -1);
   }

   Case(one or [fail])
   {
      int a = 0;
      int b = -2;
      OK(Ge(a) || Eq(b), -1) << ">=0 or ==-2";
   }

   Case(two logic [fail])
   {
      int a = 0;
      int b = -1;
      OK(-3 && Ge(a) && !Eq(b), -1);
   }

   Case(two logic [fail])
   {
      int a = 0;
      int b = -1;
      OK((-3 || Ge(a)) && !Eq(b), -1);
   }

   Case(!&& || [fail])
   {
      int a = 0;
      int b = -2;
      OK(!(Le(a) && !Eq(b)) || -3, -1);
   }

   Case(AllOf [fail])
   {
      rectangle_t p1 = {0, 0};
      OK(AllOf("Rect(0, 0)", StartsWith("Rect")), rectangle_tostring(&p1));  // successful

      rectangle_t p2 = {1, 2};
      OK(AllOf("Rect(0, 0)", _, EndsWith("R(1, 2)")), rectangle_tostring(&p2));  // failure
   }

   Case(AnyOf successful [pass])
   {
      rectangle_t p1 = {0, 0};
      OK(AnyOf("Rect(0, 0)", StartsWith("Rect")), rectangle_tostring(&p1));
      OK(AnyOf("Rect(0, 1)", StartsWith("Rect")), rectangle_tostring(&p1));
   }

   Case(NoneOf successful [pass])
   {
      rectangle_t p1 = {0, 0};
      OK(NoneOf("Rect(0, 1)", EndsWith("R(1, 2)")), rectangle_tostring(&p1));
   }

   Case(Conditional [pass])
   {
      int height = 1;

      height = 1;
      rectangle_t p1 = {0, height};
      OK(Conditional(height, "Rect(0, 1)", "Rect(0, 0)"), rectangle_tostring(&p1));

      height = 0;
      rectangle_t p2 = {0, height};
      OK(Conditional(height, "Rect(0, 1)", "Rect(0, 0)"), rectangle_tostring(&p2));
   }
}

SUITE(In)
{
   Case(In [fail])
   {
      OK(AnyOf(1, Le(2), Pair(2, 3)), 5);
   }

   Case(In [fail])
   {
      std::pair<int, std::string> a1 = std::make_pair(9, std::string("nine"));
      OK(AnyOf(Pair(1, "one"), Pair(2, "two")), a1);
   }
}
