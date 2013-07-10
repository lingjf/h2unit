#include "h2unit.h"
#include <stdlib.h>

extern "C" {
#include "demo_dynamic_stub.h"
}

#include "demo_dynamic_stub.hpp"

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


H2UNIT(mock_with_dynamic_stub)
{
   void setup() { }
   void teardown() { }
};

int stub_foo_mock1(int a)
{
   H2EQUAL(1, a); /* check the input parameter */
   return 5; /* return wanted return value */
}


H2CASE(mock_with_dynamic_stub, "act mock")
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

H2CASE(mock_with_dynamic_stub, "act mock n call")
{
   H2STUB(orig_foo, stub_foo_mock2);
   H2EQUAL(3, getSum(1));
   H2EQUAL(5, getSum(2));

   for (int i = 3; i < 1000; i++) {
      H2EQUAL(i + 1 + 3, getSum(i));
   }
}

H2UNIT(dynamic_stub_in_cpp)
{
   void setup() { }

   void teardown() { }
};

int stub_Rect_getCode1()
{
   return 1;
}

int stub_Rect_getCode2(Rect * thus)
{
   return 2;
}

int stub_Rect_getPage(Rect * thus, int v)
{
   return v + 1;
}

H2CASE(dynamic_stub_in_cpp, "stub normal class member function")
{
   Rect rect(1,2,3,4);

   H2EQUAL(0, rect.getCode());
   H2STUB("Rect::getCode()", stub_Rect_getCode1);
   H2EQUAL(1, rect.getCode());

   H2STUB("Rect::getCode()", stub_Rect_getCode2);
   H2EQUAL(2, rect.getCode());


   H2EQUAL(1, rect.getPage(1));
   H2STUB("Rect::getPage(int)", stub_Rect_getPage);
   H2EQUAL(2, rect.getPage(1));
}

int stub_Rect_getEdge(Rect * thus)
{
   return 999;
}

int stub_Triangle_getEdge(Rect * thus)
{
   return 888;
}

H2CASE(dynamic_stub_in_cpp, "stub virtual class member function")
{
   Rect rect(1,2,3,4);
   H2EQUAL(4, rect.getEdge());
   H2STUB("Rect::getEdge()", stub_Rect_getEdge);
   H2EQUAL(999, rect.getEdge());

   Triangle tr;
   H2EQUAL(3, tr.getEdge());
   H2STUB("Triangle::getEdge()", stub_Triangle_getEdge);
   H2EQUAL(888, tr.getEdge());
}
