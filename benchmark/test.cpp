
#if H2UNIT == 1
#include "../h2unit.h"
#endif

#if H2UNIT == 2
#include "../build/h2unit.hpp"
#endif

GlobalSetup()
{
}

GlobalCleanup()
{
}

SUITE(normal)
{
   Case(case 1)
   {
      OK(true);
      OK("a", "b");
   }

   Case(case 2)
   {
      OK(Ge(0), 1);
      OK(Je("{}"), "{}");
   }

   Case(case 3)
   {
      OK(_, 0) << "hello user message";
   }

   Case(case 4)
   {
      OK(AnyOf(_), 1);
   }

   Case(case 5)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 6, 3), a);
   }


   Case(case 6)
   {
      BLOCK()
      {
      }
   }
}

static int foobar1(int a) { return a + 1; }
static int foobar2(int a) { return a + 2; }
static int foobar3(int a) { return a + 3; }
static int foobar4(int a) { return a + 4; }
static int foobar_fake(int a) { return 0; }

CASE(stub & mock)
{
   MOCK(foobar1, int(int)).Once().Return(0);
   MOCKS(foobar2, int, (int), Once()) { return 0; };
   STUB(foobar3, int(int), foobar_fake);
   STUBS(foobar4, int, (int)) { return 0; };

   foobar1(1);
   foobar2(1);
   foobar3(1);
   foobar4(1);
}
