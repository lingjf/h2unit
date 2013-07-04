#include "h2unit.h"
#include <stdlib.h>

extern "C" {
#include "product_1.h"
}

int gv;

/*
 * H2UNIT is a unit test Suite, which contains several test cases.
 */
H2UNIT(getEven)
{
   int uv;

   /*
    * setup() will be executed before every test case belong to this suite.
    * Typically it is used to prepare preconditions.
    */
   void setup()
   {
      uv = 2;
      gv = 3;
   }
   /*
    * teardown() will be executed after every test case belong to this suite
    * whatever test case passed or failed.
    * Typically it is used to release resource.
    */
   void teardown()
   {
      uv = 0;
      gv = 1;
   }
};

/*
 * H2CASE is a unit test case
 */
H2CASE(getEven,"test for h2unit varible and setup")
{
   H2EQUAL_TRUE(2 == uv);
}

H2CASE(getEven,"test for global varible and h2check")
{
   H2EQUAL(4 == gv);
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
  /*
   * setup() and teardown() can be omitted.
   */
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

/*
 * h2unit can replace function at runtime dynamically.
 * Which this feature, it is helpful to stub functions.
 *
 * H2STUB(function_pointer, new_function)
 *
 * H2STUB("function_name_string", new_function)
 *
 * The 2nd is not support on Windows currently.
 *
 */


H2UNIT(dynamic_stub)
{
   void setup()
   {
   }

   void teardown()
   {
   }
};

int stub_foo(int a)
{
   return a + 2;
}

int stub_bar(int b)
{
   return b + 2;
}

int stub_atoi(const char* s)
{
   return 1;
}

H2CASE(dynamic_stub, "test dynamic stub local extern function")
{
   H2EQUAL(2, getSum(0));
   H2STUB(orig_foo, stub_foo);
   H2EQUAL(3, getSum(0));
}

H2CASE(dynamic_stub, "test dynamic stub local static function")
{
   H2EQUAL(2, getSum(0));
   H2STUB("orig_bar", stub_bar);
   H2EQUAL(3, getSum(0));
}

H2CASE(dynamic_stub, "test dynamic stub mix")
{
   H2EQUAL(2, getSum(0));
   H2STUB(orig_foo, stub_foo);
   H2EQUAL(3, getSum(0));
   H2STUB("orig_bar", stub_bar);
   H2EQUAL(4, getSum(0));
}

H2CASE(dynamic_stub, "test dynamic stub unknown function")
{
   H2EQUAL(2, getSum(0));
   H2STUB("orig_fun", stub_bar);
   H2EQUAL(3, getSum(0));
}

H2CASE(dynamic_stub, "test dynamic stub libc function")
{
   H2EQUAL(0, isLegal("0"));
   H2STUB(atoi, stub_atoi);
   H2EQUAL(1, isLegal("0"));
}

