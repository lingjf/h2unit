#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(type name)
{
   Case(basic)
   {
      OK("char", h2::h2_cxa::type_name<char>(buffer));
      OK("signed char", h2::h2_cxa::type_name<signed char>(buffer));
      OK("unsigned char", h2::h2_cxa::type_name<unsigned char>(buffer));
      OK("short", h2::h2_cxa::type_name<short>(buffer));
      OK("short", h2::h2_cxa::type_name<signed short>(buffer));
      OK("unsigned short", h2::h2_cxa::type_name<unsigned short>(buffer));
      OK("short", h2::h2_cxa::type_name<short int>(buffer));
      OK("short", h2::h2_cxa::type_name<signed short int>(buffer));
      OK("unsigned short", h2::h2_cxa::type_name<unsigned short int>(buffer));
      OK("int", h2::h2_cxa::type_name<int>(buffer));
      OK("int", h2::h2_cxa::type_name<signed int>(buffer));
      OK("unsigned int", h2::h2_cxa::type_name<unsigned int>(buffer));
      OK("long", h2::h2_cxa::type_name<long>(buffer));
      OK("long", h2::h2_cxa::type_name<signed long>(buffer));
      OK("unsigned long", h2::h2_cxa::type_name<unsigned long>(buffer));
      OK("long", h2::h2_cxa::type_name<long int>(buffer));
      OK("long", h2::h2_cxa::type_name<signed long int>(buffer));
      OK("unsigned long", h2::h2_cxa::type_name<unsigned long int>(buffer));
      OK(_long_long, h2::h2_cxa::type_name<long long>(buffer));
      OK(_long_long, h2::h2_cxa::type_name<signed long long>(buffer));
      OK("unsigned " _long_long, h2::h2_cxa::type_name<unsigned long long>(buffer));
      OK(_long_long, h2::h2_cxa::type_name<long long int>(buffer));
      OK(_long_long, h2::h2_cxa::type_name<signed long long int>(buffer));
      OK("unsigned " _long_long, h2::h2_cxa::type_name<unsigned long long int>(buffer));
      OK("float", h2::h2_cxa::type_name<float>(buffer));
      OK("double", h2::h2_cxa::type_name<double>(buffer));
      OK("bool", h2::h2_cxa::type_name<bool>(buffer));
      OK("unsigned int", h2::h2_cxa::type_name<unsigned>(buffer));
   }

   Case(void)
   {
      OK("void", h2::h2_cxa::type_name<void>(buffer));
   }

   Case(enum)
   {
      OK(_enum "A_Enum", h2::h2_cxa::type_name<A_Enum>(buffer));
   }

   Case(struct)
   {
      OK(_struct "A_PlainStruct", h2::h2_cxa::type_name<A_PlainStruct>(buffer));
      OK(_struct "A_PlainStruct", h2::h2_cxa::type_name<struct A_PlainStruct>(buffer));
   }

   Case(class)
   {
      OK(_class "A_AbstractClass", h2::h2_cxa::type_name<A_AbstractClass>(buffer));
      OK(_class "B_DerivedClass", h2::h2_cxa::type_name<class B_DerivedClass>(buffer));
      OK(_class "C_OverrideClass", h2::h2_cxa::type_name<C_OverrideClass>(buffer));
      OK(_class "D_NoConstructorClass", h2::h2_cxa::type_name<class D_NoConstructorClass>(buffer));
   }

   Case(typedef)
   {
      typedef char TypeDef_char;
      OK("char", h2::h2_cxa::type_name<TypeDef_char>(buffer));
      typedef unsigned int TypeDef_unsigned_int;
      OK("unsigned int", h2::h2_cxa::type_name<TypeDef_unsigned_int>(buffer));
      typedef A_PlainStruct TypeDef_A_PlainStruct;
      OK(_struct "A_PlainStruct", h2::h2_cxa::type_name<TypeDef_A_PlainStruct>(buffer));
      typedef class A_AbstractClass TypeDef_A_AbstractClass;
      OK(_class "A_AbstractClass", h2::h2_cxa::type_name<TypeDef_A_AbstractClass>(buffer));
      typedef char A_CharArray[8];
      OK("char [8]", h2::h2_cxa::type_name<A_CharArray>(buffer));
   }

   Case(pointer)
   {
      OK(Se("char" _pointer), h2::h2_cxa::type_name<char*>(buffer));
      OK(Se("unsigned long" _pointer), h2::h2_cxa::type_name<unsigned long*>(buffer));
      OK(Se(_struct "A_PlainStruct" _pointer), h2::h2_cxa::type_name<struct A_PlainStruct*>(buffer));
      OK(Se(_class "A_AbstractClass" _pointer), h2::h2_cxa::type_name<class A_AbstractClass*>(buffer));
   }

   Case(double pointer)
   {
      OK(Se("char" _pointer _pointer), h2::h2_cxa::type_name<char**>(buffer));
      OK(Se("unsigned long" _pointer _pointer), h2::h2_cxa::type_name<unsigned long**>(buffer));
      OK(Se(_struct "A_PlainStruct" _pointer _pointer), h2::h2_cxa::type_name<struct A_PlainStruct**>(buffer));
      OK(Se(_class "A_AbstractClass" _pointer _pointer), h2::h2_cxa::type_name<class A_AbstractClass**>(buffer));
   }

   Case(triple pointer)
   {
      OK(Se("char" _pointer _pointer _pointer), h2::h2_cxa::type_name<char***>(buffer));
      OK(Se("unsigned long" _pointer _pointer _pointer), h2::h2_cxa::type_name<unsigned long***>(buffer));
      OK(Se(_struct "A_PlainStruct" _pointer _pointer _pointer), h2::h2_cxa::type_name<struct A_PlainStruct***>(buffer));
      OK(Se(_class "A_AbstractClass" _pointer _pointer _pointer), h2::h2_cxa::type_name<class A_AbstractClass***>(buffer));
   }

   Case(function pointer)
   {
      OK(AnyOf(Se("int (*)(int, int)"), Se("int (__cdecl*)(int,int)")), h2::h2_cxa::type_name<A_FunctionPointer>(buffer));
   }

   Case(reference)
   {
      OK("char&", h2::h2_cxa::type_name<char&>(buffer));
      OK("unsigned long&", h2::h2_cxa::type_name<unsigned long&>(buffer));
      OK(_struct "A_PlainStruct&", h2::h2_cxa::type_name<struct A_PlainStruct&>(buffer));
      OK(_class "A_AbstractClass&", h2::h2_cxa::type_name<class A_AbstractClass&>(buffer));
   }

   Case(const)
   {
      OK("const char", h2::h2_cxa::type_name<const char>(buffer));
      OK("const unsigned long", h2::h2_cxa::type_name<const unsigned long>(buffer));
      OK("const " _struct "A_PlainStruct", h2::h2_cxa::type_name<const A_PlainStruct>(buffer));
      OK("const " _class "A_AbstractClass", h2::h2_cxa::type_name<const A_AbstractClass>(buffer));
   }

   Case(const pointer)
   {
      OK(Se("char const" _pointer), h2::h2_cxa::type_name<const char*>(buffer));
      OK(Se("unsigned long const" _pointer), h2::h2_cxa::type_name<const unsigned long*>(buffer));
      OK(Se(_struct "A_PlainStruct const" _pointer), h2::h2_cxa::type_name<const A_PlainStruct*>(buffer));
      OK(Se(_class "A_AbstractClass const" _pointer), h2::h2_cxa::type_name<const A_AbstractClass*>(buffer));
   }

   Case(const pointer reference)
   {
      OK(Se("char const" _pointer "&"), h2::h2_cxa::type_name<const char*&>(buffer));
      OK(Se("unsigned long const" _pointer "&"), h2::h2_cxa::type_name<const unsigned long*&>(buffer));
      OK(Se(_struct "A_PlainStruct const" _pointer "&"), h2::h2_cxa::type_name<const A_PlainStruct*&>(buffer));
      OK(Se(_class "A_AbstractClass const" _pointer "&"), h2::h2_cxa::type_name<const A_AbstractClass*&>(buffer));
   }
}

#if !defined _MSC_VER
SUITE(demangle)
{
   Case(demangle namespace class member function)
   {
      char mangled[] = "_ZN2h24unit7executeEv";
      char demangled[1024] = "1";

      size_t len = sizeof(demangled);
      int status = 0;
      abi::__cxa_demangle(mangled, demangled, &len, &status);

      OK(0, status);
      OK("h2::unit::execute()", demangled);
   }

   Case(demangle namespace class member function one parameter)
   {
      char mangled[] = "_ZN2h24unit6mallocEm";
      char* demangled = h2::h2_cxa::demangle(mangled);
      OK("h2::unit::malloc(unsigned long)", demangled);
   }

   Case(demangle c - function)
   {
      char mangled[] = "malloc";
      char* demangled = h2::h2_cxa::demangle(mangled);
      OK("malloc", demangled);
   }
}
#endif
