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
   STUB(int, foobar1, (int)) { return -1; };
}

GlobalSuiteSetup()
{
   STUB(int, foobar2, (int)) { return -2; };
}

GlobalCaseSetup()
{
   STUB(int, foobar3, (int)) { return -3; };
}

SUITE(stub in shared_code)
{
   STUB(int, foobar4, (int)) { return -4; };

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
