#include "../h2unit.h"

int foobar(int a, const char* b)
{
   return 0;
}

SUITE(MOCK)
{
   Case(argument)
   {
      MOCK(foobar, int, (int, const char*)).once(1, "A").returns(11);
      OK(11, foobar(2, "B"));
   }

   Case(call times)
   {
      MOCK(foobar, int, (int, const char*)).times(2).once();
      foobar(1, "A");
   }

   Case(unexpect call)
   {
      MOCK(foobar, int, (int, const char*)).times(1).with(1, "A");
      foobar(1, "A");
      foobar(2, "B");
   }

   Case(OK in mock)
   {
      MOCK(foobar, int, (int, const char*)).once() = [](int a, const char* b) {
         OK(1, a);
         OK("a", b);
         return 11;
      };

      OK(11, foobar(2, "B"));
   }

   Case(greed true)
   {
      MOCK(foobar, int, (int, const char*))
        .greed(true)
        .between(1, 3)
        .with(1, "A")
        .returns(11)
        .once(1, _)
        .returns(22);

      OK(11, foobar(1, "A"));
      OK(22, foobar(1, "A"));
   }
}
