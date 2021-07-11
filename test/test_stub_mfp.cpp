#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(mfp for static member function)
{
   Case(is static member function)
   {
      OK(h2::h2_mfp<A_AbstractClass, const char*(int)>::is_static_member_function(&A_AbstractClass::static_f1));
      OK(!h2::h2_mfp<A_AbstractClass, const char*(int)>::is_static_member_function(&A_AbstractClass::normal_f1));
      OK(!h2::h2_mfp<A_AbstractClass, const char*(int)>::is_static_member_function(&A_AbstractClass::virtual_f1));

      OK(h2::h2_mfp<B_DerivedClass, const char*(int)>::is_static_member_function(&B_DerivedClass::static_f1));
      OK(h2::h2_mfp<B_DerivedClass, const char*(int, int)>::is_static_member_function(&B_DerivedClass::static_f2));
      OK(!h2::h2_mfp<B_DerivedClass, const char*(int)>::is_static_member_function(&B_DerivedClass::normal_f1));
      OK(!h2::h2_mfp<B_DerivedClass, const char*(int, int)>::is_static_member_function(&B_DerivedClass::normal_f2));
      OK(!h2::h2_mfp<B_DerivedClass, const char*(int)>::is_static_member_function(&B_DerivedClass::virtual_f1));
      OK(!h2::h2_mfp<B_DerivedClass, const char*(int, int)>::is_static_member_function(&B_DerivedClass::virtual_f2));
   }

   Case(get address of abstract class)
   {
      void* A_AbstractClass__static_f1 = h2::h2_mfp<A_AbstractClass, const char*(int)>::A(&A_AbstractClass::static_f1);
      OK(NotNull, A_AbstractClass__static_f1);
      typedef const char* (*A_AbstractClass__static_f1_Type)(A_AbstractClass*, int);
      OK("A.static_f1", ((A_AbstractClass__static_f1_Type)A_AbstractClass__static_f1)(nullptr, 1));
   }

   Case(get address of derived class)
   {
      void* B_DerivedClass__static_f1 = h2::h2_mfp<B_DerivedClass, const char*(int)>::A(&B_DerivedClass::static_f1);
      OK(NotNull, B_DerivedClass__static_f1);
      typedef const char* (*B_DerivedClass__static_f1_Type)(B_DerivedClass*, int);
      OK("A.static_f1", ((B_DerivedClass__static_f1_Type)B_DerivedClass__static_f1)(nullptr, 1));

      void* B_DerivedClass__static_f2 = h2::h2_mfp<B_DerivedClass, const char*(int, int)>::A(&B_DerivedClass::static_f2);
      OK(NotNull, B_DerivedClass__static_f2);
      typedef const char* (*B_DerivedClass__static_f2_Type)(B_DerivedClass*, int, int);
      OK("B.static_f2", ((B_DerivedClass__static_f2_Type)B_DerivedClass__static_f2)(nullptr, 1, 2));
   }

   Case(get address of override class)
   {
      void* C_OverrideClass__static_f1 = h2::h2_mfp<C_OverrideClass, const char*(int)>::A(&C_OverrideClass::static_f1);
      OK(NotNull, C_OverrideClass__static_f1);
      typedef const char* (*C_OverrideClass__static_f1_Type)(C_OverrideClass*, int);
      OK("C.static_f1", ((C_OverrideClass__static_f1_Type)C_OverrideClass__static_f1)(nullptr, 1));

      void* C_OverrideClass__static_f2 = h2::h2_mfp<C_OverrideClass, const char*(int, int)>::A(&C_OverrideClass::static_f2);
      OK(NotNull, C_OverrideClass__static_f2);
      typedef const char* (*C_OverrideClass__static_f2_Type)(C_OverrideClass*, int, int);
      OK("C.static_f2", ((C_OverrideClass__static_f2_Type)C_OverrideClass__static_f2)(nullptr, 1, 2));
   }

   Case(get address of no constructor class)
   {
      void* D_NoConstructorClass__static_f3 = h2::h2_mfp<D_NoConstructorClass, const char*(int, int, int)>::A(&D_NoConstructorClass::static_f3);
      OK(NotNull, D_NoConstructorClass__static_f3);
      typedef const char* (*D_NoConstructorClass__static_f3_Type)(D_NoConstructorClass*, int, int, int);
      OK("D.static_f3", ((D_NoConstructorClass__static_f3_Type)D_NoConstructorClass__static_f3)(nullptr, 1, 2, 3));
   }
}

SUITE(mfp for normal member function)
{
   B_DerivedClass b;
   C_OverrideClass c;

   Case(abstract class)
   {
      void* A_AbstractClass__normal_f1 = h2::h2_mfp<A_AbstractClass, const char*(int)>::A(&A_AbstractClass::normal_f1);
      OK(NotNull, A_AbstractClass__normal_f1);
      typedef const char* (*A_AbstractClass__normal_f1_Type)(A_AbstractClass*, int);
      OK("A.normal_f1", ((A_AbstractClass__normal_f1_Type)A_AbstractClass__normal_f1)(nullptr, 1));
   }

   Case(derived class)
   {
      void* B_DerivedClass__normal_f1 = h2::h2_mfp<B_DerivedClass, const char*(int)>::A(&B_DerivedClass::normal_f1);
      OK(NotNull, B_DerivedClass__normal_f1);
      typedef const char* (*B_DerivedClass__normal_f1_Type)(B_DerivedClass*, int);
      OK("A.normal_f1", ((B_DerivedClass__normal_f1_Type)B_DerivedClass__normal_f1)(&b, 1));

      void* B_DerivedClass__normal_f2 = h2::h2_mfp<B_DerivedClass, const char*(int, int)>::A(&B_DerivedClass::normal_f2);
      OK(NotNull, B_DerivedClass__normal_f2);
      typedef const char* (*B_DerivedClass__normal_f2_Type)(B_DerivedClass*, int, int);
      OK("B.normal_f2", ((B_DerivedClass__normal_f2_Type)B_DerivedClass__normal_f2)(&b, 1, 2));
   }

   Case(override class)
   {
      void* C_OverrideClass__normal_f1 = h2::h2_mfp<C_OverrideClass, const char*(int)>::A(&C_OverrideClass::normal_f1);
      OK(NotNull, C_OverrideClass__normal_f1);
      typedef const char* (*C_OverrideClass__normal_f1_Type)(C_OverrideClass*, int);
      OK("C.normal_f1", ((C_OverrideClass__normal_f1_Type)C_OverrideClass__normal_f1)(&c, 1));

      void* C_OverrideClass__normal_f2 = h2::h2_mfp<C_OverrideClass, const char*(int, int)>::A(&C_OverrideClass::normal_f2);
      OK(NotNull, C_OverrideClass__normal_f2);
      typedef const char* (*C_OverrideClass__normal_f2_Type)(C_OverrideClass*, int, int);
      OK("C.normal_f2", ((C_OverrideClass__normal_f2_Type)C_OverrideClass__normal_f2)(&c, 1, 2));
   }
}

#if !defined WIN32
SUITE(mfp for virtual member function)
{
   B_DerivedClass b;
   C_OverrideClass c;
   D_NoConstructorClass d(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

   Case(abstract class)
   {
      void* A_AbstractClass__virtual_f1 = h2::h2_mfp<A_AbstractClass, const char*(int)>::A(&A_AbstractClass::virtual_f1);
      OK(NotNull, A_AbstractClass__virtual_f1);
      typedef const char* (*A_AbstractClass__virtual_f1_Type)(A_AbstractClass*, int);
      OK("A.virtual_f1", ((A_AbstractClass__virtual_f1_Type)A_AbstractClass__virtual_f1)(nullptr, 1));
   }

   Case(derived class)
   {
      void* B_DerivedClass__virtual_f1 = h2::h2_mfp<B_DerivedClass, const char*(int)>::A(&B_DerivedClass::virtual_f1);
      OK(NotNull, B_DerivedClass__virtual_f1);
      typedef const char* (*B_DerivedClass__virtual_f1_Type)(B_DerivedClass*, int);
      OK("A.virtual_f1", ((B_DerivedClass__virtual_f1_Type)B_DerivedClass__virtual_f1)(&b, 1));

      void* B_DerivedClass__virtual_f2 = h2::h2_mfp<B_DerivedClass, const char*(int, int)>::A(&B_DerivedClass::virtual_f2);
      OK(NotNull, B_DerivedClass__virtual_f2);
      typedef const char* (*B_DerivedClass__virtual_f2_Type)(B_DerivedClass*, int, int);
      OK("B.virtual_f2", ((B_DerivedClass__virtual_f2_Type)B_DerivedClass__virtual_f2)(&b, 1, 2));
   }

   Case(override class)
   {
      void* C_OverrideClass__virtual_f1 = h2::h2_mfp<C_OverrideClass, const char*(int)>::A(&C_OverrideClass::virtual_f1);
      OK(NotNull, C_OverrideClass__virtual_f1);
      typedef const char* (*C_OverrideClass__virtual_f1_Type)(C_OverrideClass*, int);
      OK("C.virtual_f1", ((C_OverrideClass__virtual_f1_Type)C_OverrideClass__virtual_f1)(&c, 1));

      void* C_OverrideClass__virtual_f2 = h2::h2_mfp<C_OverrideClass, const char*(int, int)>::A(&C_OverrideClass::virtual_f2);
      OK(NotNull, C_OverrideClass__virtual_f2);
      typedef const char* (*C_OverrideClass__virtual_f2_Type)(C_OverrideClass*, int, int);
      OK("C.virtual_f2", ((C_OverrideClass__virtual_f2_Type)C_OverrideClass__virtual_f2)(&c, 1, 2));
   }

   Case(no default constructor class)
   {
      void* D_NoConstructorClass__virtual_f1 = h2::h2_mfp<D_NoConstructorClass, const char*(int)>::A(&D_NoConstructorClass::virtual_f1);
      OK(NotNull, D_NoConstructorClass__virtual_f1);
      typedef const char* (*D_NoConstructorClass__virtual_f1_Type)(D_NoConstructorClass*, int);
      OK("D.virtual_f1", ((D_NoConstructorClass__virtual_f1_Type)D_NoConstructorClass__virtual_f1)(&d, 1));

      void* D_NoConstructorClass__virtual_f2 = h2::h2_mfp<D_NoConstructorClass, const char*(int, int)>::A(&D_NoConstructorClass::virtual_f2);
      OK(NotNull, D_NoConstructorClass__virtual_f2);
      typedef const char* (*D_NoConstructorClass__virtual_f2_Type)(D_NoConstructorClass*, int, int);
      OK("D.virtual_f2", ((D_NoConstructorClass__virtual_f2_Type)D_NoConstructorClass__virtual_f2)(&d, 1, 2));

      void* D_NoConstructorClass__virtual_f3 = h2::h2_mfp<D_NoConstructorClass, const char*(int, int, int)>::A(&D_NoConstructorClass::virtual_f3);
      OK(NotNull, D_NoConstructorClass__virtual_f3);
      typedef const char* (*D_NoConstructorClass__virtual_f3_Type)(D_NoConstructorClass*, int, int, int);
      OK("D.virtual_f3", ((D_NoConstructorClass__virtual_f3_Type)D_NoConstructorClass__virtual_f3)(&d, 1, 2, 3));
   }
}
#endif
