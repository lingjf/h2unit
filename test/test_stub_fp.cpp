#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(fp)
{
   void* fp;

   Case(one candidate)
   {
      fp = h2::h2_fp<int(int, const char*)>::A(foobar2);
      OK(fp);
      int (*fa)(int, const char*) = (int (*)(int, const char*))fp;
      OK(2, fa(0, ""));
   }

   Case(two candidate)
   {
      fp = h2::h2_fp<int(int)>::A(foobar3);
      OK(fp);
      int (*fa)(int) = (int (*)(int))fp;
      OK(3, fa(0));

      fp = h2::h2_fp<const char*(const char*)>::A(foobar3);
      OK(fp);
      const char* (*fb)(const char*) = (const char* (*)(const char*))fp;
      OK("3", fb(""));
   }

   Case(funtion fp)
   {
      fp = h2::h2_fp<void>::A(foobar0);
      OK(fp);
      int (*fa)() = (int (*)())fp;
      OK(0, fa());
   }

   Case(funtion name)
   {
      fp = h2::h2_fp<>::A("foobar0");
      OK(fp);
      int (*fa)() = (int (*)())fp;
      OK(0, fa());
   }
}
