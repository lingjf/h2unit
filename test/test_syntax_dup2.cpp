#include "../source/h2_unit.cpp"

CASE() {}

SUITE() {}

CASE(syntax) {}

SUITE(syntax) {}

SUITE(syntax)
{
   Setup() {}
}
SUITE(syntax)
{
   Cleanup() {}
}
SUITE(syntax)
{
   Setup(int a, int b) {}
   Cleanup(1 2, 3) {}
}
SUITE(syntax)
{
   Cleanup() {}
   Setup() {}
}

#define D101 101

SUITE(syntax D101)
{
   Setup(a b)
   {
   }
   Cleanup("a b")
   {
   }
   Case(D101) {}
   Case(2)
   {
      int e = 1, a = 1;
      OK(e, a) << e << " not equals " << a;
   }

   Todo(1) {}
   Todo(2) {}
}

GlobalSetup()
{
}

GlobalCleanup()
{
}

GlobalSuiteSetup()
{
}

GlobalSuiteCleanup()
{
}

GlobalCaseSetup()
{
}

GlobalCaseCleanup()
{
}

TODO(syntax) {}
