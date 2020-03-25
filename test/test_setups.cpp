#include <math.h>

#include "../source/h2_unit.hpp"
#include "../source/h2_unit.cpp"

double STUB_sin(double x)
{
   return 3.14;
}

double STUB_cos(double x)
{
   return 3.14;
}

double STUB_tan(double x)
{
   return 3.14;
}

int STUB_rand(void)
{
   return 1024;
}


GlobalSetup()
{
   STUB((double(*)(double))sin, STUB_sin);
   MOCK((double(*)(double))asin, double(double)).any().returns(3.14);
}

GlobalSuiteSetup() {
   STUB((double(*)(double))cos, STUB_cos);
   MOCK((double(*)(double))acos, double(double)).any().returns(3.14);
}

GlobalCaseSetup() {
   STUB((double(*)(double))tan, STUB_tan);
   MOCK((double(*)(double))atan, double(double)).any().returns(3.14);
}

SUITE(Stub/Mock in Setups)
{
   STUB(rand, STUB_rand);
   MOCK(time, time_t(time_t*)).any().returns(1024);

   Case(a)
   {
      OK(3.14, sin(1));
      OK(3.14, cos(1));
      OK(3.14, tan(1));
      OK(1024, rand());

      OK(3.14, asin(1));
      OK(3.14, acos(1));
      OK(3.14, atan(1));
      OK(1024, time(NULL));
   }

   Case(b)
   {
      OK(3.14, sin(1));
      OK(3.14, cos(1));
      OK(3.14, tan(1));
      OK(1024, rand());

      OK(3.14, asin(1));
      OK(3.14, acos(1));
      OK(3.14, atan(1));
      OK(1024, time(NULL));
   }
}
