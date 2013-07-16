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
 * The 2nd is not support on Windows/MacOS currently.
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
   H2EQ_MATH(2, getSum(0));
   H2STUB(orig_foo, stub_foo);
   H2EQ_MATH(3, getSum(0));
}

H2CASE(dynamic_stub, "test dynamic stub local static function")
{
   H2EQ_MATH(2, getSum(0));
   H2STUB("orig_bar", stub_bar);
   H2EQ_MATH(3, getSum(0));
}

H2CASE(dynamic_stub, "test dynamic stub mix")
{
   H2EQ_MATH(2, getSum(0));
   H2STUB(orig_foo, stub_foo);
   H2EQ_MATH(3, getSum(0));
   H2STUB("orig_bar", stub_bar);
   H2EQ_MATH(4, getSum(0));
}

H2CASE(dynamic_stub, "test dynamic stub unknown function")
{
   H2EQ_MATH(2, getSum(0));
   H2STUB("orig_fun", stub_bar);
   H2EQ_MATH(3, getSum(0));
}

H2CASE(dynamic_stub, "test dynamic stub libc function")
{
   H2EQ_MATH(0, isLegal("0"));
   H2STUB(atoi, stub_atoi);
   H2EQ_MATH(1, isLegal("0"));
}


H2UNIT(mock_with_dynamic_stub)
{
   void setup() { }
   void teardown() { }
};

int stub_foo_mock1(int a)
{
   H2EQ_MATH(1, a); /* check the input parameter */
   return 5; /* return wanted return value */
}

H2CASE(mock_with_dynamic_stub, "act mock")
{
   H2STUB(orig_foo, stub_foo_mock1);
   H2EQ_MATH(7, getSum(1));
   H2EQ_MATH(8, getSum(2));
}

int stub_foo_mock2(int a)
{
   static int call_seq = 0;
   call_seq++;

   if (call_seq == 1) {
      H2EQ_MATH(1, a); /* check the input parameter */
      return 1; /* return wanted return value */
   } else if (call_seq == 2) {
      H2EQ_MATH(2, a); /* check the input parameter */
      return 2; /* return wanted return value */
   } else {
      H2EQ_MATH(call_seq, a); /* check the input parameter */
      return 3; /* return wanted return value */
   }

   return 0;
}

H2CASE(mock_with_dynamic_stub, "act mock n call")
{
   H2STUB(orig_foo, stub_foo_mock2);
   H2EQ_MATH(3, getSum(1));
   H2EQ_MATH(5, getSum(2));

   for (int i = 3; i < 1000; i++) {
      H2EQ_MATH(i + 1 + 3, getSum(i));
   }
}

H2UNIT(dynamic_stub_in_cpp)
{
   void setup() { }

   void teardown() { }
};

int stub_dog(int a, double b)
{
   return a + 1 + (int) b;
}

int stub_cat(int a, double b)
{
   return 0;
}

H2CASE(dynamic_stub_in_cpp, "stub normal extern function")
{
   H2EQ_MATH(2, orig_dog(1, 1.2));
   H2STUB(orig_dog, stub_dog);
   H2EQ_MATH(3, orig_dog(1, 1.2));
   H2STUB("orig_dog(int, double)", stub_cat);
   H2EQ_MATH(0, orig_dog(1, 1.2));
}

int stub_Rect_getPage(Rect * thus, int v)
{
   return v + 1;
}

H2CASE(dynamic_stub_in_cpp, "stub normal class member function")
{
   Rect rect(1,2,3,4);

   H2EQ_MATH(1, rect.getPage(1));
   H2STUB("Rect::getPage(int)", stub_Rect_getPage);
   H2EQ_MATH(2, rect.getPage(1));
}

int stub_Rect_getEdge(Circle * thus)
{
   return 999;
}

int stub_Circle_getEdge(Circle * thus)
{
   return 888;
}

H2CASE(dynamic_stub_in_cpp, "stub virtual class member function")
{
   Rect rect(1,2,3,4);
   H2EQ_MATH(4, rect.getEdge());
   H2STUB("Rect::getEdge()", stub_Rect_getEdge);
   H2EQ_MATH(999, rect.getEdge());

   Circle cc(3);
   H2EQ_MATH(1, cc.getEdge());
   H2STUB("Circle::getEdge()", stub_Circle_getEdge);
   H2EQ_MATH(888, cc.getEdge());
}

void stub_Circle_enlarge(Circle * thus, int d)
{
   thus->m_r += d / 2;
}

H2CASE(dynamic_stub_in_cpp, "stub modify class object")
{
   Circle cc(1);
   H2EQ_STRCMP("Circle(1)", cc.tuString());
   cc.enlarge(1);
   H2EQ_STRCMP("Circle(2)", cc.tuString());
   H2STUB("Circle::enlarge(int)", stub_Circle_enlarge);
   cc.enlarge(2);
   H2EQ_STRCMP("Circle(3)", cc.tuString());
}
