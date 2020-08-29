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

CASE(bugfix
     : Eq integer with float)
{
   float uv = 2.00001;
   OK(Eq(2, 0.0001), uv);
}

CASE(bugfix
     : MOCK return reference)
{
   NoDefaultConstructorClass x(1, 1.1);
   MOCK(foobar, NoDefaultConstructorClass&, (int, NoDefaultConstructorClass&)).once(_, _).returns(x);
   foobar(1, x);
}

CASE(bugfix
     : OK(<, >))
{
   OK(std::is_convertible<int, long>::value);
}

CASE(bugfix
     : OK uint8_t print error)
{
   // actual is type = 3
   // OK( 7==>, <==type ) at ...
   uint8_t type = 7;
   OK(7, type);
}

CASE(bugfix
     : OK with "\"" print error)
{
   // OK("\"", ",");
   // OK( "\"", "==>""", ","<==" ) at ...
   OK("\"", "\"");
}

CASE(bugfix
     : MOCK without matcher cause crash)
{
   // MOCK(foobar, int, (int, const char*)).times(0);
   // foobar(1, "A");
}
