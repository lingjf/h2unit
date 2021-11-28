#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(Any)
{
   Case(OK)
   {
      OK(_, 0);
      OK(_(), 1);
      OK(_()(), 42);
      OK(Any, 0);
      OK(Any(), 1);
      OK(Any()(), 42);
   }

   Case(MOCK)
   {
      MOCK(foobar2, int(int, const char*)).Once(_, Any()()).Return(11);
      OK(11, foobar2(1, "A"));
   }
}
