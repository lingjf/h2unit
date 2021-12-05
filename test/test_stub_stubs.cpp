#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(STUBS)
{
   Case(lambdas normal function)
   {
      STUBS(foobar2, int, (int a, const char* b)) { return -222; };
      OK(-222, foobar2(1, "2"));

      UNSTUBS(foobar2, int, (int a, const char* b));
      OK(2, foobar2(1, "2"));
   }

   Case(lambdas normal member function)
   {
      B_DerivedClass b;

      OK("A.normal_f1(1)a", b.normal_f1(1));

      STUBS(B_DerivedClass, normal_f1, const char*, (int x))
      {
         return sprintf(buffer, "@B.normal_f1(%d)%c", x, This->b), buffer;
      };
      OK("@B.normal_f1(1)b", b.normal_f1(1));

      UNSTUBS(B_DerivedClass, normal_f1, const char*, (int a));
      OK("A.normal_f1(1)a", b.normal_f1(1));
   }

   Case(lambdas virtual member function)
   {
      B_DerivedClass b;

      OK("B.virtual_f2(1,2)b", b.virtual_f2(1, 2));

      STUBS(B_DerivedClass, virtual_f2, const char*, (int x, int y))
      {
         return sprintf(buffer, "@B.virtual_f2(%d,%d)%c", x, y, This->b), buffer;
      };
      OK("@B.virtual_f2(1,2)b", b.virtual_f2(1, 2));
      UNSTUBS(B_DerivedClass, virtual_f2, const char*, (int a, int b));
      OK("B.virtual_f2(1,2)b", b.virtual_f2(1, 2));
   }

   Case(lambdas static member function)
   {
      STUBS(B_DerivedClass::static_f2, const char*, (int x, int y))
      {
         return sprintf(buffer, "@B.static_f2(%d,%d)", x, y), buffer;
      };
      OK("@B.static_f2(1,2)", B_DerivedClass::static_f2(1, 2));

      UNSTUBS(B_DerivedClass::static_f2, const char*, (int a, int b));
      OK("B.static_f2(1,2)", B_DerivedClass::static_f2(1, 2));
   }

   Case(abstract virtual member function with object)
   {
      B_DerivedClass b;
      OK("A.virtual_f1(1)a", b.virtual_f1(1));
      A_AbstractClass* a = dynamic_cast<A_AbstractClass*>(&b);

      STUBS(a, A_AbstractClass, virtual_f1, const char*, (int x))
      {
         return sprintf(buffer, "@A.virtual_f1(%d)%c", x, This->a), buffer;
      };
      OK("@A.virtual_f1(1)a", b.virtual_f1(1));
      UNSTUBS(a, A_AbstractClass, virtual_f1, const char*, (int a));
      OK("A.virtual_f1(1)a", b.virtual_f1(1));
   }

   Case(no default constructor with object)
   {
      D_NoConstructorClass d(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      STUBS(d, D_NoConstructorClass, virtual_f3, const char*, (int x, int y, int z))
      {
         return sprintf(buffer, "@D.virtual_f3(%d,%d,%d)%c", x, y, z, This->d), buffer;
      };
      OK("@D.virtual_f3(1,2,3)d", d.virtual_f3(1, 2, 3));
      UNSTUBS(d, D_NoConstructorClass, virtual_f3, const char*, (int, int, int));
      OK("D.virtual_f3(1,2,3)d", d.virtual_f3(1, 2, 3));
   }
}
