#include "../source/h2_unit.cpp"
#include "test_types.hpp"
#include <time.h>

SUITE(UNMOCK)
{
   Case(normal function)
   {
      MOCKS(foobar2, int, (int, const char*), Once(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      UNMOCKS(foobar2, int, (int, const char*));
      OK(2, foobar2(1, "A"));
   }

   Case(normal member function)
   {
      B_DerivedClass b;
      MOCKS(B_DerivedClass, normal_f2, const char*, (int, int), Once(1, 2)) { return "+B.normal_f2"; };
      OK("+B.normal_f2", b.normal_f2(1, 2));
      UNMOCKS(B_DerivedClass, normal_f2, const char*, (int, int));
      OK("B.normal_f2", b.normal_f2(1, 2));
   }

   Case(abstract virtual member function with object)
   {
      B_DerivedClass b;
      A_AbstractClass* a = dynamic_cast<A_AbstractClass*>(&b);
      MOCK(a, A_AbstractClass, virtual_f1, const char*(int)).Once().Return("+A.virtual_f1");
      OK("+A.virtual_f1", b.virtual_f1(1));
      UNMOCK(a, A_AbstractClass, virtual_f1, const char*(int));
      OK("A.virtual_f1", b.virtual_f1(1));
   }

   Case(no default constructor with object)
   {
      D_NoConstructorClass d(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      MOCK(d, D_NoConstructorClass, virtual_f3, const char*(int, int, int)).Once().Return("+D.virtual_f3");
      OK("+D.virtual_f3", d.virtual_f3(1, 2, 3));
      UNMOCK(d, D_NoConstructorClass, virtual_f3, const char*(int, int, int));
      OK("D.virtual_f3", d.virtual_f3(1, 2, 3));
   }

   Case(template class member function)
   {
      F_TemplateClass<int> f;
      MOCKS(F_TemplateClass<int>, virtual_f1, const char*, (int a), Once()) { return "+F.virtual_f1"; };
      OK("+F.virtual_f1", f.virtual_f1(0));
      UNMOCKS(F_TemplateClass<int>, virtual_f1, const char*, (int a));
      OK("F.virtual_f1", f.virtual_f1(0));
   }

   Case(function name)
   {
      MOCKS("foobar0", int, (), Once()) { return 1; };
      OK(1, foobar0());

      UNMOCKS("foobar0");
      OK(0, foobar0());
   }
}
