#include <time.h>
#include "../h2unit.h"

extern "C" {
#include "product_c.h"
}

#include "product_cpp.h"

int foobar(int a, const char* b)
{
   return 0;
}

SUITE(Mock Function)
{
   Case(once argument failure)
   {
      MOCK(foobar, int, (int, const char*)).Once(1, "A").Return(11);
      OK(11, foobar(2, "B"));
   }

   Case(once call failure)
   {
      MOCK(time, time_t, (time_t*)).Once().Return((time_t)0);
   }

   Case(twice failure)
   {
      MOCK(foobar, int, (int, const char*)).Twice(Eq(1), _).Return(11);
      OK(11, foobar(1, "A"));
      OK(22, foobar(1, "BC"));  // failure
   }

   Case(3 times failure)
   {
      MOCK(foobar, int, (int, const char*)).Times(3).With(Ge(1)).Return(11);
      OK(11, foobar(1, "A"));
   }

   Case(exceed call failure)
   {
      MOCK(time, time_t, (time_t*)).Once();
      time(0);
      time(0);
   }

   Case(any 0 successful)
   {
      MOCK(foobar, int, (int, const char*)).Any(1, "A");
   }

   Case(any 1 successful)
   {
      MOCK(foobar, int, (int, const char*)).Any().With(1, "A").Return(11);
      OK(11, foobar(1, "A"));
   }

   Case(any 2 successful)
   {
      MOCK(foobar, int, (int, const char*)).Any().With(1, "A").Return(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   }

   Case(atleast 2 successful)
   {
      MOCK(foobar, int, (int, const char*)).Atleast(2).With(1, "A").Return(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   }

   Case(atleast 3 successful)
   {
      MOCK(foobar, int, (int, const char*)).Atleast(2).With(1, "A").Return(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   }

   Case(atmost 1 successful)
   {
      MOCK(foobar, int, (int, const char*)).Atmost(2).With(1, "A").Return(11);
      OK(11, foobar(1, "A"));
   }

   Case(atmost 2 successful)
   {
      MOCK(foobar, int, (int, const char*)).Atmost(2).With(1, "A").Return(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   }

   Case(between successful)
   {
      MOCK(foobar, int, (int, const char*)).Between(2, 4).With(1, "A").Return(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   }

   Case(multi - line successful)
   {
      MOCK(foobar, int, (int, const char*)).Once(1, "A").Return(11).Once().With(2, "B").Return(22).Twice(3, "C").Return(33);

      OK(11, foobar(1, "A"));
      OK(22, foobar(2, "B"));
      OK(33, foobar(3, "C"));
      OK(33, foobar(3, "C"));
   }

   Case(th0 successful)
   {
      MOCK(foobar, int, (int, const char*)).Once().Th0(1);
      foobar(1, "A");
   }

   Case(th1 successful)
   {
      MOCK(foobar, int, (int, const char*)).Once().Th1("A");
      foobar(1, "A");
   }
}

SUITE(Mock Member)
{
   Case(static member function successful)
   {
      MOCK(Shape::born, int, (int)).Once(1).Return(11);
      OK(11, Shape::born(1));
   }

   Case(member function successful)
   {
      MOCK(Rect, move, int, (int, int)).Once(1, 2).Return(11);
      MOCK(Rect, move, int, (int)).Once(3).Return(22);
      Rect rect(0, 0, 1, 1);
      OK(11, rect.move(1, 2));
      OK(22, rect.move(3));
   }

   Case(member function lambda failure)
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

   Case(virtual member function successful)
   {
      MOCK(Rect, print, const char*, ()).Once().Return("rect(0,0,1,1)");
      Rect rect(0, 0, 1, 1);
      OK("rect(0,0,1,1)", rect.print());
   }

   Case(abstract class successful)
   {
      Rect rect(0, 0, 1, 1);
      MOCK(Shape, print, const char*, ()).Times(0).Return("Shape");
      MOCK(Rect, print, const char*, ()).Once().Return("rect(0,0,1,1)");
      OK("rect(0,0,1,1)", rect.print());
   }
}
