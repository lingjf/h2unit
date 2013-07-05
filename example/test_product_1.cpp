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

H2CASE(getEven,"test for global varible and H2EQUAL[_TRUE]")
{
   H2EQUAL(4 == gv);
}

H2CASE(getEven,"test for H2EQUAL_INTEGER")
{
   H2EQUAL_INTEGER(4, getEven(7));
}

H2CASE(getEven,"test for general H2EQUAL[_INTEGER]")
{
   H2EQUAL(6, getEven(6));
}

H2CASE(getEven,"test for H2EQUAL_INTEGER/H2EQUAL with byte")
{
   unsigned char e1 = 2;
   unsigned char a1 = 2;
   H2EQUAL_INTEGER(e1, a1);

   char e2 = 2;
   char a2 = 2;
   H2EQUAL(e2, a2);
}

H2CASE(getEven,"test for H2EQUAL_INTEGER/H2EQUAL with short")
{
   short e1 = 2;
   short a1 = 2;
   H2EQUAL_INTEGER(e1, a1);

   unsigned short e2 = 2;
   unsigned short a2 = 2;
   H2EQUAL(e2, a2);
}

H2CASE(getEven,"test for H2EQUAL_INTEGER/H2EQUAL with long")
{
   long e1 = 2;
   long a1 = 2;
   H2EQUAL_INTEGER(e1, a1);

   unsigned long e2 = 2;
   unsigned long a2 = 2;
   H2EQUAL(e2, a2);
}

H2CASE(getEven,"test for H2EQUAL_INTEGER/H2EQUAL with long long")
{
   long long e1 = 2;
   long long a1 = 2;
   H2EQUAL_INTEGER(e1, a1);

   unsigned long long e2 = 0x12345678900L;
   unsigned long long a2 = 12345678900L;
   H2EQUAL(e2, a2);
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
   void setup() { }

   void teardown() { }
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


H2UNIT(dynamic_stub_act_mock)
{
   void setup() { }
   void teardown() { }
};

int stub_foo_mock1(int a)
{
   H2EQUAL(1, a); /* check the input parameter */
   return 5; /* return wanted return value */
}


H2CASE(dynamic_stub_act_mock, "act mock")
{
   H2STUB(orig_foo, stub_foo_mock1);
   H2EQUAL(7, getSum(1));
   H2EQUAL(8, getSum(2));
}

int stub_foo_mock2(int a)
{
   static int call_seq = 0;
   call_seq++;

   if (call_seq == 1) {
      H2EQUAL(1, a); /* check the input parameter */
      return 1; /* return wanted return value */
   } else if (call_seq == 2) {
      H2EQUAL(2, a); /* check the input parameter */
      return 2; /* return wanted return value */
   } else {
      H2EQUAL(call_seq, a); /* check the input parameter */
      return 3; /* return wanted return value */
   }

   return 0;
}

H2CASE(dynamic_stub_act_mock, "act mock n call")
{
   H2STUB(orig_foo, stub_foo_mock2);
   H2EQUAL(3, getSum(1));
   H2EQUAL(5, getSum(2));

   for (int i = 3; i < 1000; i++) {
      H2EQUAL(i + 1 + 3, getSum(i));
   }
}

