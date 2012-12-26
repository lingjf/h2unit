#include "h2unit.h"
#include <stdlib.h>

extern "C" {
#include "product_1.h"
}

int gv;

H2UNIT(getEven)
{
   int uv;
   void setup()
   {
      uv = 2;
      gv = 3;
   }

   void teardown()
   {
      uv = 0;
      gv = 1;
   }
};

H2CASE(getEven,"test for h2unit varible and setup")
{
   H2CHECK(2 == uv);
}

H2CASE(getEven,"test for global varible and h2check")
{
   H2CHECK(4 == gv);
}

H2CASE(getEven,"test for general H2EQUAL")
{
   H2EQUAL(6, getEven(6));
}

H2CASE(getEven,"test for H2EQUAL_INTEGER")
{
   H2EQUAL_INTEGER(4, getEven(7));
}

H2UNIT(getCeil)
{
};

H2CASE(getCeil, "test for general H2EQUAL")
{
   H2EQUAL_DOUBLE(6.0, getCeil(6.2), 0.000001);
   H2EQUAL(6.0, getCeil(6.4));
}

H2CASE(getCeil, "test for H2EQUAL_DOUBLE")
{
   H2EQUAL_DOUBLE(6.8, getCeil(6.8), 0.000001);
}

H2UNIT(dynamic_stub)
{
   void setup()
   {
   }

   void teardown()
   {
   }
};

int stub_func(int a)
{
   return a + 2;
}

int stub_atoi(const char* s)
{
   return 1;
}

H2CASE(dynamic_stub, "test dynamic stub 1")
{
   H2EQUAL(1, isLegal(0, "0"));
   H2STUB(orig_func, stub_func);
   H2EQUAL(2, isLegal(0, "0"));
}

H2CASE(dynamic_stub, "test dynamic stub 2")
{
   H2EQUAL(1, isLegal(0, "0"));
   H2STUB(atoi, stub_atoi);
   H2EQUAL(2, isLegal(0, "0"));
}

