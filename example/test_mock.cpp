
extern "C" {
#include "product_c.h"
}

#include "product_cpp.h"

#if defined H2UNIT && H2UNIT == 2
#include "../build/h2unit.hpp"
#else
#include "../h2unit.h"
#endif

static int foo(int a, const char* b)
{
   return 0;
}

SUITE(Mock Function)
{
   Case(one argument [fail])
   {
      OK(11, foo(2, "B"));
      MOCK(foo, int(int, const char*)).Once(1, "A").Return(11);
      OK(22, foo(2, "B"));
   }

   Case(twice call [fail])
   {
      MOCK(foo, int(int, const char*)).Twice(Eq(1), _).Return(11);
      OK(11, foo(1, "A"));
      OK(22, foo(1, "BC"));
   }

   Case(call times [fail])
   {
      MOCK(foo, int(int, const char*)).Times(2).Return(0);
      foo(1, "A");
   }

   Case(unexpect call [fail])
   {
      MOCK(foo, int(int, const char*)).Times(1).With(1, "A").Return(0);
      foo(1, "A");
      OK(2, foo(2, "B"));
   }

   Case(any 0 successful [pass])
   {
      MOCK(foo, int(int, const char*)).Any(1, "A");
   }

   Case(any 1 successful [pass])
   {
      MOCK(foo, int(int, const char*)).Any().With(1, "A").Return(11);
      OK(11, foo(1, "A"));
   }

   Case(any 2 successful [pass])
   {
      MOCK(foo, int(int, const char*)).Any().With(1, "A").Return(11);
      OK(11, foo(1, "A"));
      OK(11, foo(1, "A"));
   }

   Case(atleast 2 successful [pass])
   {
      MOCK(foo, int(int, const char*)).Atleast(2).With(1, "A").Return(11);
      OK(11, foo(1, "A"));
      OK(11, foo(1, "A"));
   }

   Case(atleast 3 successful [pass])
   {
      MOCK(foo, int(int, const char*)).Atleast(2).With(1, "A").Return(11);
      OK(11, foo(1, "A"));
      OK(11, foo(1, "A"));
      OK(11, foo(1, "A"));
   }

   Case(atmost 1 successful [pass])
   {
      MOCK(foo, int(int, const char*)).Atmost(2).With(1, "A").Return(11);
      OK(11, foo(1, "A"));
   }

   Case(atmost 2 successful [pass])
   {
      MOCK(foo, int(int, const char*)).Atmost(2).With(1, "A").Return(11);
      OK(11, foo(1, "A"));
      OK(11, foo(1, "A"));
   }

   Case(between successful [pass])
   {
      MOCK(foo, int(int, const char*)).Between(2, 4).With(1, "A").Return(11);
      OK(11, foo(1, "A"));
      OK(11, foo(1, "A"));
   }

   Case(OK in mock [fail])
   {
      MOCKS(foo, int, (int a, const char* b), Once())
      {
         OK(1, a);
         OK("a", b);
         return 11;
      };

      OK(11, foo(2, "B"));
   }

   Case(greed true [fail])
   {
      MOCK(foo, int(int, const char*)).greed(true).Between(1, 3).With(1, "A").Return(11).Once(1, _).Return(22);

      OK(11, foo(1, "A"));
      OK(22, foo(1, "A"));
   }

   Case(multi-line successful [pass])
   {
      MOCK(foo, int(int, const char*)).Once(1, "A").Return(11).Once().With(2, "B").Return(22).Twice(3, "C").Return(33);

      OK(11, foo(1, "A"));
      OK(22, foo(2, "B"));
      OK(33, foo(3, "C"));
      OK(33, foo(3, "C"));
   }

   Case(th0 successful [pass])
   {
      MOCK(foo, int(int, const char*)).Once().Th0(1);
      foo(1, "A");
   }

   Case(th1 successful [pass])
   {
      MOCK(foo, int(int, const char*)).Once().Th1("A");
      foo(1, "A");
   }
}

SUITE(Mock Member)
{
   Case(static member function [pass])
   {
      MOCK(Shape::born, int(int)).Once(1).Return(11);
      OK(11, Shape::born(1));
   }

   Case(member function [pass])
   {
      MOCK(Rect, move, int(int, int)).Once(1, 2).Return(11);
      MOCK(Rect, move, int(int)).Once(3).Return(22);
      Rect rect(0, 0, 1, 1);
      OK(11, rect.move(1, 2));
      OK(22, rect.move(3));
   }

   Case(member function lambda [fail])
   {
      MOCKS(Rect, move, int, (int x, int y), Once(1, 2))
      {
         return x + y + 1;
      };
      MOCKS(Rect, move, int, (int xy), Once())
      {
         OK("Rect", This->name);
         return xy + 1;
      };
      Rect rect(0, 0, 1, 1);
      OK(4, rect.move(1, 2));
      OK(3, rect.move(1));  // failure
   }

   Case(virtual member function [pass])
   {
      MOCK(Rect, print, const char*()).Once().Return("rect(0,0,1,1)");
      Rect rect(0, 0, 1, 1);
      OK("rect(0,0,1,1)", rect.print());
   }

   Case(abstract class [pass])
   {
      Rect rect(0, 0, 1, 1);
      MOCK(Shape, print, const char*()).Times(0).Return("Shape");
      MOCK(Rect, print, const char*()).Once().Return("rect(0,0,1,1)");
      OK("rect(0,0,1,1)", rect.print());
   }
}

static time_t STUB_time(time_t* x)
{
   return 42;
}

CASE(no function)
{
   STUB("time", STUB_time);
   OK(42, time(NULL));
}

int foobar(int a)
{
   return 0;
}

int foobar(int a, const char* b)
{
   return 0;
}

int STUB_foobar(int a)
{
   return -1;
}

CASE("foobar" ambiguous [fail])
{
   STUB("foobar", STUB_foobar);
   OK(-1, foobar(0));
}
