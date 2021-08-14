#include "../build/h2unit.hpp"
#include <time.h>

int foo(int a, const char* b)
{
   return 0;
}

class bar {
 public:
   int fx(int a, const char* b)
   {
      return 0;
   }
};

SUITE(MOCK)
{
   Case(argument)
   {
      OK(11, foo(2, "B"));
      MOCK(foo, int, (int, const char*)).Once(1, "A").Return(11);
      OK(22, foo(2, "B"));
   }

   Case(call times)
   {
      MOCK(foo, int, (int, const char*)).Times(2).Return(0);
      foo(1, "A");
   }

   Case(unexpect call)
   {
      MOCK(foo, int, (int, const char*)).Times(1).With(1, "A").Return(0);
      foo(1, "A");
      OK(2, foo(2, "B"));
   }

   Case(c++ class member)
   {
      MOCK(bar, fx, int, (int, const char*)).Once(1, "A").Return(11);

      bar f;
      OK(11, f.fx(2, "B"));
   }

   Case(OK in mock)
   {
      MOCKS(foo, int, (int a, const char* b), Once())
      {
         OK(1, a);
         OK("a", b);
         return 11;
      };

      OK(11, foo(2, "B"));
   }

   Case(greed true)
   {
      MOCK(foo, int, (int, const char*)).greed(true).Between(1, 3).With(1, "A").Return(11).Once(1, _).Return(22);

      OK(11, foo(1, "A"));
      OK(22, foo(1, "A"));
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

CASE("foobar")
{
   STUB("foobar", STUB_foobar);
   OK(-1, foobar(0));
}
