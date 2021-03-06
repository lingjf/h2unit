#include "../source/h2_unit.cpp"

static int foobar1(int)
{
   return 1;
}
static int foobar2(int)
{
   return 2;
}
static int foobar3(int)
{
   return 3;
}
static int foobar4(int)
{
   return 4;
}

GlobalSetup()
{
   MOCK(foobar1, int, (int), Any()) { return -1; };
}

GlobalSuiteSetup()
{
   MOCK(foobar2, int, (int), Any()) { return -2; };
}

GlobalCaseSetup()
{
   MOCK(foobar3, int, (int), Any()) { return -3; };
}

SUITE(stub in shared_code)
{
   MOCK(foobar4, int, (int), Any()) { return -4; };

   Case(a)
   {
      OK(-1, foobar1(0));
      OK(-2, foobar2(0));
      OK(-3, foobar3(0));
      OK(-4, foobar4(0));
   }

   Case(b)
   {
      OK(-1, foobar1(0));
      OK(-2, foobar2(0));
      OK(-3, foobar3(0));
      OK(-4, foobar4(0));
   }
}
