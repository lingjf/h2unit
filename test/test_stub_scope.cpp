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

int foobar1_fake(int)
{
   return -1;
}
int foobar2_fake(int)
{
   return -2;
}
int foobar3_fake(int)
{
   return -3;
}
int foobar4_fake(int)
{
   return -4;
}

GlobalSetup()
{
   STUB(foobar1, int, (int), foobar1_fake);
}

GlobalSuiteSetup()
{
   STUB(foobar2, int, (int), foobar2_fake);
}

GlobalCaseSetup()
{
   STUB(foobar3, int, (int), foobar3_fake);
}

SUITE(stub in shared_code)
{
   STUB(foobar4, int, (int), foobar4_fake);

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
