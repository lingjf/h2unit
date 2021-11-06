#include "../source/h2_unit.cpp"

struct NoDefaultConstructorClass {
   int a;
   float b;
   char c[100];

   NoDefaultConstructorClass() = delete;
   NoDefaultConstructorClass(int _a, float _b) : a(_a), b(_b) {}
};

static NoDefaultConstructorClass& foobar(int a, NoDefaultConstructorClass& x)
{
   x.a = a;
   x.b += a;
   sprintf(x.c, "%d", a);
   return x;
}

SUITE(bugfix)
{
   Case(Eq integer with float)
   {
      float uv = 2.00001f;
      OK(Eq(2, 0.0001), uv);
   }

   Case(MOCK return reference)
   {
      NoDefaultConstructorClass x(1, 1.1f);
      MOCK(foobar, NoDefaultConstructorClass & (int, NoDefaultConstructorClass&)).Once(_, _).Return(x);
      foobar(1, x);
   }

   Case(OK(<, >))
   {
      OK((std::is_convertible<int, long>::value));
   }

   Case(OK uint8_t print error)
   {
      // actual is type = 3
      // OK( 7==>, <==type ) at ...
      uint8_t type = 7;
      OK(7, type);
   }

   Case(OK with "\"" print error)
   {
      // OK("\"", ",");
      // OK( "\"", "==>""", ","<==" ) at ...
      OK("\"", "\"");
   }

   Case(MOCK without matcher cause crash)
   {
      // MOCK(foobar, int(int, const char*)).Times(0);
      // foobar(1, "A");
   }

   Case(parse json very large number)
   {
      JE("[1912000101600571]", "[1912000101600571]");
   }

   Case(parse json '{},{}')
   {
      // Passed before fixed
      // JE("{}", "{}, {}");
   }

   Case(vector bool)
   {
      /* https://isocpp.org/blog/2012/11/on-vectorbool */

      std::vector<bool> a = {true, false, true};
      // for (auto& k : a) {
      // }
      for (auto&& k : a) {
         OK(_, k);
      }
   }

   Case(strcmp with LCS)
   {
      // OK("hello", "hel1o");
   }
}
