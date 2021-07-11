#include "../source/h2_unit.cpp"
#include "test_types.hpp"
#include <time.h>

SUITE(UNMOCK)
{
   Case(normal function)
   {
      MOCKS(foobar2, int, (int, const char*), Once(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      UNMOCK(foobar2);
      OK(2, foobar2(1, "A"));
   }

   Case(normal member function)
   {
      B_DerivedClass b;
      MOCKS(B_DerivedClass, normal_f2, const char*, (int, int), Once(1, 2)) { return "MOCK"; };
      OK("MOCK", b.normal_f2(1, 2));
      UNMOCK(B_DerivedClass, normal_f2, const char*, (int, int));
      OK("B.normal_f2", b.normal_f2(1, 2));
   }

#ifndef _WIN32
   Case(template class member function)
   {
      F_TemplateClass<int> f;
      MOCKS(F_TemplateClass<int>, virtual_f1, const char*, (int a)) { return "MOCK"; };
      OK("MOCK", f.virtual_f1(0));
      UNMOCK(F_TemplateClass<int>, virtual_f1, const char*, (int a));
      OK("F.virtual_f1", f.virtual_f1(0));
   }
#endif

   Case(function name)
   {
      MOCKS("foobar0", int, (), Once()) { return 1; };
      OK(1, foobar0());

      UNMOCK("foobar0");
      OK(0, foobar0());
   }
}
