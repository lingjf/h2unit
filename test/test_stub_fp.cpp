#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(normal fp)
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

SUITE(constructible and destructible)
{
   Case(constructible)
   {
      A_PlainStruct* a_PlainStruct = h2::h2_constructible<A_PlainStruct>::O(alloca(sizeof(A_PlainStruct)));
      OK(NotNull, a_PlainStruct);
      h2::h2_destructible<A_PlainStruct>(a_PlainStruct);

      B_ClassStruct* a_ClassStruct = h2::h2_constructible<B_ClassStruct>::O(alloca(sizeof(B_ClassStruct)));
      OK(NotNull, a_ClassStruct);
      h2::h2_destructible<B_ClassStruct>(a_ClassStruct);

      A_AbstractClass* a_AbstractClass = h2::h2_constructible<A_AbstractClass>::O(alloca(sizeof(A_AbstractClass)));
      OK(1, (long long)a_AbstractClass);

      B_DerivedClass* b_DerivedClass = h2::h2_constructible<B_DerivedClass>::O(alloca(sizeof(B_DerivedClass)));
      OK(NotNull, b_DerivedClass);
      h2::h2_destructible<B_DerivedClass>(b_DerivedClass);

      C_OverrideClass* c_OverrideClass = h2::h2_constructible<C_OverrideClass>::O(alloca(sizeof(C_OverrideClass)));
      OK(NotNull, c_OverrideClass);
      h2::h2_destructible<C_OverrideClass>(c_OverrideClass);

      D_NoConstructorClass* d_NoConstructorClass = h2::h2_constructible<D_NoConstructorClass>::O(alloca(sizeof(D_NoConstructorClass)));
      OK(2, (long long)d_NoConstructorClass);
   }
}

SUITE(mfp for static member function)
{
   Case(get address of abstract class)
   {
      void* A_AbstractClass__static_f1 = h2::h2_fp<A_AbstractClass, const char*(int)>::A(&A_AbstractClass::static_f1);
      OK(NotNull, A_AbstractClass__static_f1);
      typedef const char* (*A_AbstractClass__static_f1_Type)(int);
      OK("A.static_f1(1)", ((A_AbstractClass__static_f1_Type)A_AbstractClass__static_f1)(1));
   }

   Case(get address of derived class)
   {
      void* B_DerivedClass__static_f1 = h2::h2_fp<B_DerivedClass, const char*(int)>::A(&B_DerivedClass::static_f1);
      OK(NotNull, B_DerivedClass__static_f1);
      typedef const char* (*B_DerivedClass__static_f1_Type)(int);
      OK("A.static_f1(1)", ((B_DerivedClass__static_f1_Type)B_DerivedClass__static_f1)(1));

      void* B_DerivedClass__static_f2 = h2::h2_fp<B_DerivedClass, const char*(int, int)>::A(&B_DerivedClass::static_f2);
      OK(NotNull, B_DerivedClass__static_f2);
      typedef const char* (*B_DerivedClass__static_f2_Type)(int, int);
      OK("B.static_f2(1,2)", ((B_DerivedClass__static_f2_Type)B_DerivedClass__static_f2)(1, 2));
   }

   Case(get address of override class)
   {
      void* C_OverrideClass__static_f1 = h2::h2_fp<C_OverrideClass, const char*(int)>::A(&C_OverrideClass::static_f1);
      OK(NotNull, C_OverrideClass__static_f1);
      typedef const char* (*C_OverrideClass__static_f1_Type)(int);
      OK("C.static_f1(1)", ((C_OverrideClass__static_f1_Type)C_OverrideClass__static_f1)(1));

      void* C_OverrideClass__static_f2 = h2::h2_fp<C_OverrideClass, const char*(int, int)>::A(&C_OverrideClass::static_f2);
      OK(NotNull, C_OverrideClass__static_f2);
      typedef const char* (*C_OverrideClass__static_f2_Type)(int, int);
      OK("C.static_f2(1,2)", ((C_OverrideClass__static_f2_Type)C_OverrideClass__static_f2)(1, 2));
   }

   Case(get address of no constructor class)
   {
      void* D_NoConstructorClass__static_f3 = h2::h2_fp<D_NoConstructorClass, const char*(int, int, int)>::A(&D_NoConstructorClass::static_f3);
      OK(NotNull, D_NoConstructorClass__static_f3);
      typedef const char* (*D_NoConstructorClass__static_f3_Type)(int, int, int);
      OK("D.static_f3(1,2,3)", ((D_NoConstructorClass__static_f3_Type)D_NoConstructorClass__static_f3)(1, 2, 3));
   }
}

SUITE(mfp for normal member function)
{
   B_DerivedClass b;
   C_OverrideClass c;

   Case(abstract class)
   {
      void* A_AbstractClass__normal_f1 = h2::h2_fp<A_AbstractClass, const char*(int)>::A(&A_AbstractClass::normal_f1);
      OK(NotNull, A_AbstractClass__normal_f1);
      typedef const char* (*A_AbstractClass__normal_f1_Type)(A_AbstractClass*, int);
      OK("A.normal_f1(1)a", ((A_AbstractClass__normal_f1_Type)A_AbstractClass__normal_f1)((A_AbstractClass*)&b, 1));
   }

   Case(derived class)
   {
      void* B_DerivedClass__normal_f1 = h2::h2_fp<B_DerivedClass, const char*(int)>::A(&B_DerivedClass::normal_f1);
      OK(NotNull, B_DerivedClass__normal_f1);
      typedef const char* (*B_DerivedClass__normal_f1_Type)(B_DerivedClass*, int);
      OK("A.normal_f1(1)a", ((B_DerivedClass__normal_f1_Type)B_DerivedClass__normal_f1)(&b, 1));

      void* B_DerivedClass__normal_f2 = h2::h2_fp<B_DerivedClass, const char*(int, int)>::A(&B_DerivedClass::normal_f2);
      OK(NotNull, B_DerivedClass__normal_f2);
      typedef const char* (*B_DerivedClass__normal_f2_Type)(B_DerivedClass*, int, int);
      OK("B.normal_f2(1,2)b", ((B_DerivedClass__normal_f2_Type)B_DerivedClass__normal_f2)(&b, 1, 2));
   }

   Case(override class)
   {
      void* C_OverrideClass__normal_f1 = h2::h2_fp<C_OverrideClass, const char*(int)>::A(&C_OverrideClass::normal_f1);
      OK(NotNull, C_OverrideClass__normal_f1);
      typedef const char* (*C_OverrideClass__normal_f1_Type)(C_OverrideClass*, int);
      OK("C.normal_f1(1)c", ((C_OverrideClass__normal_f1_Type)C_OverrideClass__normal_f1)(&c, 1));

      void* C_OverrideClass__normal_f2 = h2::h2_fp<C_OverrideClass, const char*(int, int)>::A(&C_OverrideClass::normal_f2);
      OK(NotNull, C_OverrideClass__normal_f2);
      typedef const char* (*C_OverrideClass__normal_f2_Type)(C_OverrideClass*, int, int);
      OK("C.normal_f2(1,2)c", ((C_OverrideClass__normal_f2_Type)C_OverrideClass__normal_f2)(&c, 1, 2));
   }
}

SUITE(mfp for virtual member function)
{
   B_DerivedClass b;
   C_OverrideClass c;
   D_NoConstructorClass d(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
   A_AbstractClass* a = dynamic_cast<A_AbstractClass*>(&b);

#if !defined _WIN32
   Case(abstract class)
   {
      void* A_AbstractClass__virtual_f1 = h2::h2_fp<A_AbstractClass, const char*(int)>::A(&A_AbstractClass::virtual_f1);
      OK(NotNull, A_AbstractClass__virtual_f1);
      typedef const char* (*A_AbstractClass__virtual_f1_Type)(A_AbstractClass*, int);
      OK("A.virtual_f1(1)a", ((A_AbstractClass__virtual_f1_Type)A_AbstractClass__virtual_f1)(a, 1));
   }
#endif

   Case(abstract class with object)
   {
      void* A_AbstractClass__virtual_f1 = h2::h2_fp<A_AbstractClass, const char*(int)>::B(a, &A_AbstractClass::virtual_f1);
      OK(NotNull, A_AbstractClass__virtual_f1);
      typedef const char* (*A_AbstractClass__virtual_f1_Type)(A_AbstractClass*, int);
      OK("A.virtual_f1(1)a", ((A_AbstractClass__virtual_f1_Type)A_AbstractClass__virtual_f1)(a, 1));
   }

   Case(derived class)
   {
      void* B_DerivedClass__virtual_f1 = h2::h2_fp<B_DerivedClass, const char*(int)>::A(&B_DerivedClass::virtual_f1);
      OK(NotNull, B_DerivedClass__virtual_f1);
      typedef const char* (*B_DerivedClass__virtual_f1_Type)(B_DerivedClass*, int);
      OK("A.virtual_f1(1)a", ((B_DerivedClass__virtual_f1_Type)B_DerivedClass__virtual_f1)(&b, 1));

      void* B_DerivedClass__virtual_f2 = h2::h2_fp<B_DerivedClass, const char*(int, int)>::A(&B_DerivedClass::virtual_f2);
      OK(NotNull, B_DerivedClass__virtual_f2);
      typedef const char* (*B_DerivedClass__virtual_f2_Type)(B_DerivedClass*, int, int);
      OK("B.virtual_f2(1,2)b", ((B_DerivedClass__virtual_f2_Type)B_DerivedClass__virtual_f2)(&b, 1, 2));
   }

   Case(override class)
   {
      void* C_OverrideClass__virtual_f1 = h2::h2_fp<C_OverrideClass, const char*(int)>::A(&C_OverrideClass::virtual_f1);
      OK(NotNull, C_OverrideClass__virtual_f1);
      typedef const char* (*C_OverrideClass__virtual_f1_Type)(C_OverrideClass*, int);
      OK("C.virtual_f1(1)c", ((C_OverrideClass__virtual_f1_Type)C_OverrideClass__virtual_f1)(&c, 1));

      void* C_OverrideClass__virtual_f2 = h2::h2_fp<C_OverrideClass, const char*(int, int)>::A(&C_OverrideClass::virtual_f2);
      OK(NotNull, C_OverrideClass__virtual_f2);
      typedef const char* (*C_OverrideClass__virtual_f2_Type)(C_OverrideClass*, int, int);
      OK("C.virtual_f2(1,2)c", ((C_OverrideClass__virtual_f2_Type)C_OverrideClass__virtual_f2)(&c, 1, 2));
   }

#if !defined _WIN32
   Case(no default constructor class)
   {
      void* D_NoConstructorClass__virtual_f1 = h2::h2_fp<D_NoConstructorClass, const char*(int)>::A(&D_NoConstructorClass::virtual_f1);
      OK(NotNull, D_NoConstructorClass__virtual_f1);
      typedef const char* (*D_NoConstructorClass__virtual_f1_Type)(D_NoConstructorClass*, int);
      OK("D.virtual_f1(1)d", ((D_NoConstructorClass__virtual_f1_Type)D_NoConstructorClass__virtual_f1)(&d, 1));

      void* D_NoConstructorClass__virtual_f2 = h2::h2_fp<D_NoConstructorClass, const char*(int, int)>::A(&D_NoConstructorClass::virtual_f2);
      OK(NotNull, D_NoConstructorClass__virtual_f2);
      typedef const char* (*D_NoConstructorClass__virtual_f2_Type)(D_NoConstructorClass*, int, int);
      OK("D.virtual_f2(1,2)d", ((D_NoConstructorClass__virtual_f2_Type)D_NoConstructorClass__virtual_f2)(&d, 1, 2));

      void* D_NoConstructorClass__virtual_f3 = h2::h2_fp<D_NoConstructorClass, const char*(int, int, int)>::A(&D_NoConstructorClass::virtual_f3);
      OK(NotNull, D_NoConstructorClass__virtual_f3);
      typedef const char* (*D_NoConstructorClass__virtual_f3_Type)(D_NoConstructorClass*, int, int, int);
      OK("D.virtual_f3(1,2,3)d", ((D_NoConstructorClass__virtual_f3_Type)D_NoConstructorClass__virtual_f3)(&d, 1, 2, 3));
   }
#endif

   Case(no default constructor class with object)
   {
      void* D_NoConstructorClass__virtual_f1 = h2::h2_fp<D_NoConstructorClass, const char*(int)>::B(&d, &D_NoConstructorClass::virtual_f1);
      OK(NotNull, D_NoConstructorClass__virtual_f1);
      typedef const char* (*D_NoConstructorClass__virtual_f1_Type)(D_NoConstructorClass*, int);
      OK("D.virtual_f1(1)d", ((D_NoConstructorClass__virtual_f1_Type)D_NoConstructorClass__virtual_f1)(&d, 1));

      void* D_NoConstructorClass__virtual_f2 = h2::h2_fp<D_NoConstructorClass, const char*(int, int)>::B(&d, &D_NoConstructorClass::virtual_f2);
      OK(NotNull, D_NoConstructorClass__virtual_f2);
      typedef const char* (*D_NoConstructorClass__virtual_f2_Type)(D_NoConstructorClass*, int, int);
      OK("D.virtual_f2(1,2)d", ((D_NoConstructorClass__virtual_f2_Type)D_NoConstructorClass__virtual_f2)(&d, 1, 2));

      void* D_NoConstructorClass__virtual_f3 = h2::h2_fp<D_NoConstructorClass, const char*(int, int, int)>::B(&d, &D_NoConstructorClass::virtual_f3);
      OK(NotNull, D_NoConstructorClass__virtual_f3);
      typedef const char* (*D_NoConstructorClass__virtual_f3_Type)(D_NoConstructorClass*, int, int, int);
      OK("D.virtual_f3(1,2,3)d", ((D_NoConstructorClass__virtual_f3_Type)D_NoConstructorClass__virtual_f3)(&d, 1, 2, 3));
   }
}
