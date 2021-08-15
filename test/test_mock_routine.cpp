#include "../source/h2_unit.cpp"
#include "test_types.hpp"

static void void_foobar(int& a)
{
   a = 42;
}

static const int int_foobar(int a, int b)
{
   return 42;
}

static const A_PlainStruct obj_foobar(int a, int b)
{
   A_PlainStruct v = {42, 3.14};
   return v;
}

static const A_PlainStruct& ref_foobar(int a, int b)
{
   static A_PlainStruct v = {42, 3.14};
   return v;
}

static void void_class_foobar(B_DerivedClass*, int& a)
{
   a = 42;
}

static int int_class_foobar(B_DerivedClass*, int a, int b)
{
   return 42;
}

static A_PlainStruct obj_class_foobar(B_DerivedClass*, int a, int b)
{
   A_PlainStruct v = {42, 3.14};
   return v;
}

static A_PlainStruct& ref_class_foobar(B_DerivedClass*, int a, int b)
{
   static A_PlainStruct v = {42, 3.14};
   return v;
}

SUITE(routine)
{
   int a;

   A_PlainStruct a_PlainStruct;
   a_PlainStruct.a = 42;
   a_PlainStruct.b = 3.14;

   B_DerivedClass b_DerivedClass;

   Case(normal function bind value)
   {
      h2::h2_routine<std::false_type, int(int, const char*)> R1(42);
      OK(42, R1(nullptr, 1, ""));

      h2::h2_routine<std::false_type, const char*(int, const char*)> R2("h2unit");
      OK("h2unit", R2(nullptr, 1, ""));

      h2::h2_routine<std::false_type, std::string(int, const char*)> R3("h2unit");
      OK("h2unit", R3(nullptr, 1, ""));

      h2::h2_routine<std::false_type, A_PlainStruct(int, const char*)> R4(a_PlainStruct);
      auto r4 = R4(nullptr, 1, "");
      OK(42, r4.a);
      OK(3.14, r4.b);

      h2::h2_routine<std::false_type, const A_PlainStruct(int, const char*)> R5(a_PlainStruct);
      auto r5 = R5(nullptr, 1, "");
      OK(42, r5.a);
      OK(3.14, r5.b);

      h2::h2_routine<std::false_type, A_PlainStruct&(int, const char*)> R6(a_PlainStruct);
      auto r6 = R6(nullptr, 1, "");
      OK(42, r6.a);
      OK(3.14, r6.b);

      h2::h2_routine<std::false_type, const A_PlainStruct&(int, const char*)> R7(a_PlainStruct);
      auto r7 = R7(nullptr, 1, "");
      OK(42, r7.a);
      OK(3.14, r7.b);
   }

   Case(class member function bind value)
   {
      h2::h2_routine<B_DerivedClass, const char*(int, int)> R1("h2unit");
      OK("h2unit", R1(&b_DerivedClass, 1, 2));

      h2::h2_routine<B_DerivedClass, A_PlainStruct(int, int)> R2(a_PlainStruct);
      auto r2 = R2(&b_DerivedClass, 1, 2);
      OK(42, r2.a);
      OK(3.14, r2.b);

      h2::h2_routine<B_DerivedClass, const A_PlainStruct&(int, int)> R3(a_PlainStruct);
      auto r3 = R3(&b_DerivedClass, 1, 2);
      OK(42, r3.a);
      OK(3.14, r3.b);
   }

   Case(normal function bind function)
   {
      h2::h2_routine<std::false_type, void(int&)> R1(void_foobar);
      R1(nullptr, a);
      OK(42, a);

      h2::h2_routine<std::false_type, const int(int, int)> R2(int_foobar);
      OK(42, R2(nullptr, 1, 2));

      h2::h2_routine<std::false_type, const A_PlainStruct(int, int)> R3(obj_foobar);
      auto r3 = R3(nullptr, 1, 2);
      OK(42, r3.a);
      OK(3.14, r3.b);

      h2::h2_routine<std::false_type, const A_PlainStruct&(int, int)> R4(ref_foobar);
      auto r4 = R4(nullptr, 1, 2);
      OK(42, r4.a);
      OK(3.14, r4.b);
   }

   Case(class member function bind function)
   {
      h2::h2_routine<B_DerivedClass, void(int&)> R1(void_class_foobar);
      R1(&b_DerivedClass, a);
      OK(42, a);

      h2::h2_routine<B_DerivedClass, int(int, int)> R2(int_class_foobar);
      OK(42, R2(&b_DerivedClass, 1, 2));

      h2::h2_routine<B_DerivedClass, A_PlainStruct(int, int)> R3(obj_class_foobar);
      auto r3 = R3(&b_DerivedClass, 1, 2);
      OK(42, r3.a);
      OK(3.14, r3.b);

      h2::h2_routine<B_DerivedClass, A_PlainStruct&(int, int)> R4(ref_class_foobar);
      auto r4 = R4(&b_DerivedClass, 1, 2);
      OK(42, r4.a);
      OK(3.14, r4.b);
   }

   Case(lambda function)
   {
      h2::h2_routine<std::false_type, void(int&)> R1([](int& a) -> void {
         a = 42;
      });
      R1(nullptr, a);
      OK(42, a);

      h2::h2_routine<B_DerivedClass, void(int&)> R2([](int& a) -> void {
         a = 42;
      });
      R2(&b_DerivedClass, a);
      OK(42, a);

      h2::h2_routine<std::false_type, int(int, const char*)> R3([](int a, const char* b) -> int {
         return 42;
      });
      OK(42, R3(nullptr, 1, "2"));

      h2::h2_routine<B_DerivedClass, int(int, const char*)> R4([](int a, const char* b) -> int {
         return 42;
      });
      OK(42, R4(&b_DerivedClass, 1, "2"));

      h2::h2_routine<std::false_type, const A_PlainStruct(int, int)> R5([](int a, int d) -> const A_PlainStruct {
         A_PlainStruct v = {42, 3.14};
         return v;
      });
      auto r5 = R5(nullptr, 1, 2);
      OK(42, r5.a);
      OK(3.14, r5.b);

      h2::h2_routine<B_DerivedClass, const A_PlainStruct(int, int)> R6([](int a, int d) -> const A_PlainStruct {
         A_PlainStruct v = {42, 3.14};
         return v;
      });
      auto r6 = R6(&b_DerivedClass, 1, 2);
      OK(42, r6.a);
      OK(3.14, r6.b);
   }
}
