#include "../build/h2unit.hpp"

SUITE(Logic)
{
   Case(Not string equal)
   {
      const char* p = "Hello,您好,ボンジュール,안녕하세요.5eee24a149f8993e48ce81b77b200b212b23c2402c221ddc4"
                      "260a608cf328a2133386477eb7a65958893726741cb9223419a7a0224f"
                      "e7af2866944197ee4ab18e25913cae8507bb306a284f";
      OK(Not(p), p);
   }

   Case(&&)
   {
      int a = 0;
      int b = -2;
      OK(Ge(a) && Eq(b), -1);
   }

   Case(||)
   {
      int a = 0;
      int b = -2;
      OK(Ge(a) || Eq(b), -1);
   }

   Case(|| &&!)
   {
      int a = 0;
      int b = -2;
      OK(-3 || Ge(a) && !Eq(b), -1) << "-3 || (>=0 && !-2)";
   }

   Case(!&& ||)
   {
      int a = 0;
      int b = -2;
      OK(!(Le(a) && !Eq(b)) || -3, -1);
   }

   Case(AllOf)
   {
      OK(AllOf("Rect(0, 0)", _, EndsWith("R(1, 2)")), "Rect()");
   }

   Case(Not AllOf)
   {
      OK(!AllOf("Rect(0, 0)", _, EndsWith("(0, 0)")), "Rect(0, 0)");
   }

   Case(AnyOf)
   {
      OK(AnyOf("Rect(0, 1)", StartsWith("Rect")), "Pect(0, 1)");
   }

   Case(NoneOf)
   {
      OK(NoneOf("Rect(0, 1)", EndsWith("R(1, 2)")), "Rect(0, 1)");
   }
}

SUITE(In)
{
   Case(In)
   {
      OK(AnyOf(1, Le(2), Pair(2, 3)), 5);
   }

   Case(In)
   {
      std::pair<int, std::string> a1 = std::make_pair(9, std::string("nine"));
      OK(AnyOf(Pair(1, "one"), Pair(2, "two")), a1);
   }
}
