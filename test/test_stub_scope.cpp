#include "../source/h2_unit.cpp"

namespace {

int foo_bar1(int)
{
   return 1;
}
int foo_bar2(int)
{
   return 2;
}
int foo_bar3(int)
{
   return 3;
}
int foo_bar4(int)
{
   return 4;
}

int foo_bar1_fake(int)
{
   return -1;
}
int foo_bar2_fake(int)
{
   return -2;
}
int foo_bar3_fake(int)
{
   return -3;
}
int foo_bar4_fake(int)
{
   return -4;
}

GlobalSetup()
{
   STUB(foo_bar1, int, (int), foo_bar1_fake);
}

GlobalSuiteSetup()
{
   STUB(foo_bar2, int, (int), foo_bar2_fake);
}

GlobalCaseSetup()
{
   STUB(foo_bar3, int, (int), foo_bar3_fake);
}

SUITE(stub in shared_code)
{
   STUB(foo_bar4, int, (int), foo_bar4_fake);

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

}  // namespace
