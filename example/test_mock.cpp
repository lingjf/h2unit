#include <time.h>
#include "h2unit.h"

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
      MOCK(foobar, int, (int, const char*)).once(1, "A").returns(11);
      OK(11, foobar(2, "B"));
   }

   Case(once call failure)
   {
      MOCK(time, time_t, (time_t*)).once();
   }

   Case(twice failure)
   {
      MOCK(foobar, int, (int, const char*)).twice(Eq(1), _).returns(11);
      OK(11, foobar(1, "A"));
      OK(22, foobar(1, "BC"));  // failure
   }

   Case(3 times failure)
   {
      MOCK(foobar, int, (int, const char*)).times(3).with(Ge(1)).returns(11);
      OK(11, foobar(1, "A"));
   }

   Case(exceed call failure)
   {
      MOCK(time, time_t, (time_t*)).once();
      time(0);
      time(0);
   }

   Case(any 0 successful)
   {
      MOCK(foobar, int, (int, const char*)).any(1, "A");
   }

   Case(any 1 successful)
   {
      MOCK(foobar, int, (int, const char*)).any().with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
   }

   Case(any 2 successful)
   {
      MOCK(foobar, int, (int, const char*)).any().with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   }

   Case(atleast 2 successful)
   {
      MOCK(foobar, int, (int, const char*)).atleast(2).with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   }

   Case(atleast 3 successful)
   {
      MOCK(foobar, int, (int, const char*)).atleast(2).with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   }

   Case(atmost 1 successful)
   {
      MOCK(foobar, int, (int, const char*)).atmost(2).with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
   }

   Case(atmost 2 successful)
   {
      MOCK(foobar, int, (int, const char*)).atmost(2).with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   }

   Case(between successful)
   {
      MOCK(foobar, int, (int, const char*)).between(2, 4).with(1, "A").returns(11);
      OK(11, foobar(1, "A"));
      OK(11, foobar(1, "A"));
   }

   Case(multi - line successful)
   {
      MOCK(foobar, int, (int, const char*))
        .once(1, "A")
        .returns(11)
        .once()
        .with(2, "B")
        .returns(22)
        .twice(3, "C")
        .returns(33);

      OK(11, foobar(1, "A"));
      OK(22, foobar(2, "B"));
      OK(33, foobar(3, "C"));
      OK(33, foobar(3, "C"));
   }

   Case(th0 successful)
   {
      MOCK(foobar, int, (int, const char*)).once().th0(1);
      foobar(1, "A");
   }

   Case(th1 successful)
   {
      MOCK(foobar, int, (int, const char*)).once().th1("A");
      foobar(1, "A");
   }
}

SUITE(Mock Member)
{
   Case(static member function successful)
   {
      MOCK(Shape::born, int, (int)).once(1).returns(11);
      OK(11, Shape::born(1));
   }

   Case(member function successful)
   {
      MOCK(Rect, move, int, (int, int)).once(1, 2).returns(11);
      MOCK(Rect, move, int, (int)).once(3).returns(22);
      Rect rect(0, 0, 1, 1);
      OK(11, rect.move(1, 2));
      OK(22, rect.move(3));
   }

   Case(member function lambda failure)
   {
      MOCK(Rect, move, int, (int, int)).once(1, 2) = [](int x, int y) {
         return x + y + 1;
      };
      MOCK(Rect, move, int, (int)).once() = [](Rect* rect, int xy) {
         OK("Rect", rect->name);
         return xy + 1;
      };
      Rect rect(0, 0, 1, 1);
      OK(4, rect.move(1, 2));
      OK(3, rect.move(1));  // failure
   }

   Case(virtual member function successful)
   {
      MOCK(Rect, print, const char*, ()).once().returns("rect(0,0,1,1)");
      Rect rect(0, 0, 1, 1);
      OK("rect(0,0,1,1)", rect.print());
   }

   Case(abstract class successful)
   {
      Rect rect(0, 0, 1, 1);
      MOCK(Shape, print, const char*, ()).times(0) = []() { return "Shape"; };
      MOCK(Rect, print, const char*, ()).once() = []() { return "rect(0,0,1,1)"; };
      OK("rect(0,0,1,1)", rect.print());
   }
}
