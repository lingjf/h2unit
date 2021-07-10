#include "../source/h2_unit.cpp"

static int foo_bar1(int)
{
   return 1;
}
static int foo_bar2(int)
{
   return 2;
}
static int foo_bar3(int)
{
   return 3;
}
static int foo_bar4(int)
{
   return 4;
}

GlobalSetup()
{
   MOCK(foo_bar1, int, (int), Any()) { return -1; };
}

GlobalSuiteSetup()
{
   MOCK(foo_bar2, int, (int), Any()) { return -2; };
}

GlobalCaseSetup()
{
   MOCK(foo_bar3, int, (int), Any()) { return -3; };
}

SUITE(stub in shared_code)
{
   MOCK(foo_bar4, int, (int), Any()) { return -4; };

   Case(a)
   {
      OK(-1, foo_bar1(0));
      OK(-2, foo_bar2(0));
      OK(-3, foo_bar3(0));
      OK(-4, foo_bar4(0));
   }

   Case(b)
   {
      OK(-1, foo_bar1(0));
      OK(-2, foo_bar2(0));
      OK(-3, foo_bar3(0));
      OK(-4, foo_bar4(0));
   }
}
