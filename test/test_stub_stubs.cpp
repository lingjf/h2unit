#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(STUBS)
{
   Case(lambdas normal function)
   {
      STUBS(foobar2, int, (int a, const char* b)) { return -222; };
      OK(-222, foobar2(1, "2"));

      UNSTUB(foobar2, int, (int a, const char* b));
      OK(2, foobar2(1, "2"));
   }

   Case(lambdas normal member function)
   {
      B_DerivedClass b;

      OK("A.normal_f1", b.normal_f1(1));

      STUBS(B_DerivedClass, normal_f1, const char*, (int a)) { return "-B.normal_f1"; };
      OK("-B.normal_f1", b.normal_f1(1));

      UNSTUB(B_DerivedClass, normal_f1, const char*, (int a));
      OK("A.normal_f1", b.normal_f1(1));
   }

   Case(lambdas virtual member function)
   {
      B_DerivedClass b;

      OK("B.virtual_f2", b.virtual_f2(1, 2));

      STUBS(B_DerivedClass, virtual_f2, const char*, (int a, int b)) { return "-B.virtual_f1"; };
      OK("-B.virtual_f1", b.virtual_f2(1, 2));

      UNSTUB(B_DerivedClass, virtual_f2, const char*, (int a, int b));
      OK("A.virtual_f1", b.virtual_f1(1));
   }

   Case(lambdas static member function)
   {
      STUBS(B_DerivedClass::static_f2, const char*, (int a, int b)) { return "-B.static_f2"; };
      OK("-B.static_f2", B_DerivedClass::static_f2(1, 2));

      UNSTUB(B_DerivedClass::static_f2, const char*, (int a, int b));
      OK("B.static_f2", B_DerivedClass::static_f2(1, 2));
   }
}
