#include "../h2unit.h"

int foobar(int a, const char* b)
{
   return 0;
}

class foo {
 public:
   virtual int bar(int a, const char* b)
   {
      return 0;
   }
};

SUITE(MOCK)
{
   Case(argument)
   {
      MOCK(foobar, int, (int, const char*), Once(1, "A")) { return 11; };
      OK(11, foobar(2, "B"));
   }

   Case(call times)
   {
      MOCK(foobar, int, (int, const char*), Times(2), Once()){};
      foobar(1, "A");
   }

   Case(unexpect call)
   {
      MOCK(foobar, int, (int, const char*), Times(1).With(1, "A")){};
      foobar(1, "A");
      foobar(2, "B");
   }

   Case(c++ class member)
   {
      MOCK(foo, bar, int, (int, const char*), Once(1, "A")) { return 11; };

      foo f;
      OK(11, f.bar(2, "B"));
   }

   Case(OK in mock)
   {
      MOCK(foobar, int, (int a, const char* b), Once())
      {
         OK(1, a);
         OK("a", b);
         return 11;
      };

      OK(11, foobar(2, "B"));
   }

   Case(greed true)
   {
      MOCK(foobar, int, (int, const char*),
           greed(true)
             .Between(1, 3)
             .With(1, "A")
             .Return(11)
             .Once(1, _)
             .Return(22)){};

      OK(11, foobar(1, "A"));
      OK(22, foobar(1, "A"));
   }
}
