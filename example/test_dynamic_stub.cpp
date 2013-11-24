#include "h2unit.h"
#include <stdlib.h>

extern "C" {
#include "demo_clanguage.h"
}

#include "demo_cplusplus.h"

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


H2UNIT(dynamic_stub_in_c)
{
   void setup() { }

   void teardown() { }
};


int stub_foo1(int a)
{
   return a + 1;
}
int stub_foo2(int a)
{
   return a + 2;
}
int stub_foo3(int a)
{
   return a + 3;
}

H2CASE(dynamic_stub_in_c, "stub local extern function 3 times")
{
   H2EQ_MATH(1, getSum(0));
   H2STUB(orig_foo, stub_foo1);
   H2EQ_MATH(2, getSum(0));

   H2STUB(&orig_foo, stub_foo2);
   H2EQ_MATH(3, getSum(0));

   H2STUB("orig_foo", stub_foo3);
   H2EQ_MATH(4, getSum(0));
}

int stub_bar(int b)
{
   return b + 2;
}
H2CASE(dynamic_stub_in_c, "stub local static function")
{
   H2EQ_MATH(2, getSum(0));
   H2STUB("orig_bar", stub_bar);
   H2EQ_MATH(3, getSum(0));
}

int stub_atoi(const char* s)
{
   return 1;
}

H2CASE(dynamic_stub_in_c, "stub dynamic libc function")
{
   H2EQ_MATH(0, isLegal("0"));
   H2STUB(atoi, stub_atoi);
   H2EQ_MATH(1, isLegal("0"));
   /*
    * can't use function name to dynamic stub functions in dynamic library, use function pointer instead.
    */
   H2STUB("atoi", stub_atoi);
   H2EQ_MATH(1, isLegal("0"));
}

char* stub_varg(const char* format, ...)
{
   return (char*)"h2unit hello";
}

H2CASE(dynamic_stub_in_c, "stub variable arguments function")
{
   H2EQ_STRCMP((char*)"hello h2unit", orig_varg("hello %s", "h2unit"));
   H2STUB(orig_varg, stub_varg);
   H2EQ_STRCMP((char*)"h2unit hello", orig_varg("hello %s", "h2unit"));
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

   for (int i = 3; i < 10; i++) {
      H2EQ_MATH(i + 1 + 3, getSum(i));
   }
}

H2UNIT(dynamic_stub_in_cpp)
{
   void setup() { }

   void teardown() { }
};

int stub_getResult_1st(int a, double b)
{
   return a + 1 + (int) b;
}

int stub_getResult_2nd(int a, double b)
{
   return 0;
}

H2CASE(dynamic_stub_in_cpp, "stub normal extern function")
{
   H2EQ_MATH(2, getResult(1, 1.2));
   H2STUB(getResult, stub_getResult_1st);
   H2EQ_MATH(3, getResult(1, 1.2));

   H2STUB("getResult(int, double)", stub_getResult_2nd);
   H2EQ_MATH(0, getResult(1, 1.2));
}

int stub_Rect_getEdge(Rect * thus)
{
   return 4;
}

H2CASE(dynamic_stub_in_cpp, "stub normal class member function")
{
   Rect rect(1, 3);

   H2EQ_MATH(0, rect.getEdge());
   H2STUB("Rect::getEdge(int)", stub_Rect_getEdge);
   H2EQ_MATH(4, rect.getEdge());
}

double stub_Rect_getArea(Circle * thus)
{
   return 999.0;
}

double stub_Circle_getArea(Circle * thus)
{
   return 2 * 3.14 * thus->m_radius * thus->m_radius;
}

H2CASE(dynamic_stub_in_cpp, "stub virtual class member function")
{
   Rect rect(2, 3);
   H2EQ_MATH(6, rect.getArea());
   H2STUB("Rect::getArea()", stub_Rect_getArea);
   H2EQ_MATH(999.0, rect.getArea());

   Circle cc(3);
   H2EQ_MATH(-1, cc.getArea());
   H2STUB("Circle::getArea()", stub_Circle_getArea);
   H2EQ_MATH(56.52, cc.getArea());
}

void stub_Circle_enLarge(Circle * thus, int d)
{
   thus->m_radius += d / 2;
}

H2CASE(dynamic_stub_in_cpp, "stub modify class object")
{
   Circle cc(1);
   H2EQ_STRCMP("Circle(1)", cc.toString());
   cc.enLarge(1);
   H2EQ_STRCMP("Circle(2)", cc.toString());

   H2STUB("Circle::enLarge(int)", stub_Circle_enLarge);
   cc.enLarge(2);
   H2EQ_STRCMP("Circle(3)", cc.toString());
}

