#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(bool)
{
   Case(bool)
   {
      OK(true, true);
      OK(false, false);
   }

   Case(int)
   {
      OK(true, 1);
      OK(true, 2);
      OK(false, 0);

      OK(Not(true), 0);
      OK(Not(false), 1);
   }

   Case(ptr)
   {
      const char* a1 = "";
      OK(true, a1);
      a1 = nullptr;
      OK(false, a1);

      int* a2 = (int*)1234;
      OK(true, a2);
      a2 = nullptr;
      OK(false, a2);

      void* a3 = (void*)1234;
      OK(true, a3);
      a3 = nullptr;
      OK(false, a3);
   }

   Case(MOCK)
   {
      MOCK(foobar2, int(int, const char*)).Once(true, "A").Return(11);
      OK(11, foobar2(1, "A"));
   }
}
