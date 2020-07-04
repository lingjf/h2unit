#include "../source/h2_unit.cpp"

namespace {

int foobar1(int)
{
   return 1;
}
int foobar2(int)
{
   return 2;
}
int foobar3(int)
{
   return 3;
}
int foobar4(int)
{
   return 4;
}

GlobalSetup()
{
   MOCK(foobar1, int, (int)).any().returns(-1);
}

GlobalSuiteSetup()
{
   MOCK(foobar2, int, (int)).any().returns(-2);
}

GlobalCaseSetup()
{
   MOCK(foobar3, int, (int)).any().returns(-3);
}

SUITE(stub in shared_code)
{
   MOCK(foobar4, int, (int)).any().returns(-4);

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

}  // namespace
