#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(type name)
{
   Case(basic)
   {
      OK("char", h2::h2_cxa::type_name<char>());
      OK("signed char", h2::h2_cxa::type_name<signed char>());
      OK("unsigned char", h2::h2_cxa::type_name<unsigned char>());
      OK("short", h2::h2_cxa::type_name<short>());
      OK("short", h2::h2_cxa::type_name<signed short>());
      OK("unsigned short", h2::h2_cxa::type_name<unsigned short>());
      OK("short", h2::h2_cxa::type_name<short int>());
      OK("short", h2::h2_cxa::type_name<signed short int>());
      OK("unsigned short", h2::h2_cxa::type_name<unsigned short int>());
      OK("int", h2::h2_cxa::type_name<int>());
      OK("int", h2::h2_cxa::type_name<signed int>());
      OK("unsigned int", h2::h2_cxa::type_name<unsigned int>());
      OK("long", h2::h2_cxa::type_name<long>());
      OK("long", h2::h2_cxa::type_name<signed long>());
      OK("unsigned long", h2::h2_cxa::type_name<unsigned long>());
      OK("long", h2::h2_cxa::type_name<long int>());
      OK("long", h2::h2_cxa::type_name<signed long int>());
      OK("unsigned long", h2::h2_cxa::type_name<unsigned long int>());
      OK(_long_long, h2::h2_cxa::type_name<long long>());
      OK(_long_long, h2::h2_cxa::type_name<signed long long>());
      OK("unsigned " _long_long, h2::h2_cxa::type_name<unsigned long long>());
      OK(_long_long, h2::h2_cxa::type_name<long long int>());
      OK(_long_long, h2::h2_cxa::type_name<signed long long int>());
      OK("unsigned " _long_long, h2::h2_cxa::type_name<unsigned long long int>());
      OK("float", h2::h2_cxa::type_name<float>());
      OK("double", h2::h2_cxa::type_name<double>());
      OK("bool", h2::h2_cxa::type_name<bool>());
      OK("unsigned int", h2::h2_cxa::type_name<unsigned>());
   }

   Case(void)
   {
      OK("void", h2::h2_cxa::type_name<void>());
   }

   Case(enum)
   {
      OK(_enum "A_Enum", h2::h2_cxa::type_name<A_Enum>());
   }

   Case(struct)
   {
      OK(_struct "A_PlainStruct", h2::h2_cxa::type_name<A_PlainStruct>());
      OK(_struct "A_PlainStruct", h2::h2_cxa::type_name<struct A_PlainStruct>());
   }

   Case(class)
   {
      OK(_class "A_AbstractClass", h2::h2_cxa::type_name<A_AbstractClass>());
      OK(_class "B_DerivedClass", h2::h2_cxa::type_name<class B_DerivedClass>());
      OK(_class "C_OverrideClass", h2::h2_cxa::type_name<C_OverrideClass>());
      OK(_class "D_NoConstructorClass", h2::h2_cxa::type_name<class D_NoConstructorClass>());
   }

   Case(typedef)
   {
      typedef char TypeDef_char;
      OK("char", h2::h2_cxa::type_name<TypeDef_char>());
      typedef unsigned int TypeDef_unsigned_int;
      OK("unsigned int", h2::h2_cxa::type_name<TypeDef_unsigned_int>());
      typedef A_PlainStruct TypeDef_A_PlainStruct;
      OK(_struct "A_PlainStruct", h2::h2_cxa::type_name<TypeDef_A_PlainStruct>());
      typedef class A_AbstractClass TypeDef_A_AbstractClass;
      OK(_class "A_AbstractClass", h2::h2_cxa::type_name<TypeDef_A_AbstractClass>());
      typedef char A_CharArray[8];
      OK("char [8]", h2::h2_cxa::type_name<A_CharArray>());
   }

   Case(pointer)
   {
      OK("char" _pointer, h2::h2_cxa::type_name<char*>());
      OK("unsigned long" _pointer, h2::h2_cxa::type_name<unsigned long*>());
      OK(_struct "A_PlainStruct" _pointer, h2::h2_cxa::type_name<struct A_PlainStruct*>());
      OK(_class "A_AbstractClass" _pointer, h2::h2_cxa::type_name<class A_AbstractClass*>());
   }

   Case(double pointer)
   {
      OK("char" _pointer _pointer, h2::h2_cxa::type_name<char**>());
      OK("unsigned long" _pointer _pointer, h2::h2_cxa::type_name<unsigned long**>());
      OK(_struct "A_PlainStruct" _pointer _pointer, h2::h2_cxa::type_name<struct A_PlainStruct**>());
      OK(_class "A_AbstractClass" _pointer _pointer, h2::h2_cxa::type_name<class A_AbstractClass**>());
   }

   Case(triple pointer)
   {
      OK("char" _pointer _pointer _pointer, h2::h2_cxa::type_name<char***>());
      OK("unsigned long" _pointer _pointer _pointer, h2::h2_cxa::type_name<unsigned long***>());
      OK(_struct "A_PlainStruct" _pointer _pointer _pointer, h2::h2_cxa::type_name<struct A_PlainStruct***>());
      OK(_class "A_AbstractClass" _pointer _pointer _pointer, h2::h2_cxa::type_name<class A_AbstractClass***>());
   }

   Case(function pointer)
   {
      OK(AnyOf("int (*)(int, int)", "int (__cdecl*)(int,int)"), h2::h2_cxa::type_name<A_FunctionPointer>());
   }

   Case(reference)
   {
      OK("char&", h2::h2_cxa::type_name<char&>());
      OK("unsigned long&", h2::h2_cxa::type_name<unsigned long&>());
      OK(_struct "A_PlainStruct&", h2::h2_cxa::type_name<struct A_PlainStruct&>());
      OK(_class "A_AbstractClass&", h2::h2_cxa::type_name<class A_AbstractClass&>());
   }

   Case(const)
   {
      OK("const char", h2::h2_cxa::type_name<const char>());
      OK("const unsigned long", h2::h2_cxa::type_name<const unsigned long>());
      OK("const " _struct "A_PlainStruct", h2::h2_cxa::type_name<const A_PlainStruct>());
      OK("const " _class "A_AbstractClass", h2::h2_cxa::type_name<const A_AbstractClass>());
   }

   Case(const pointer)
   {
      OK("char const" _pointer, h2::h2_cxa::type_name<const char*>());
      OK("unsigned long const" _pointer, h2::h2_cxa::type_name<const unsigned long*>());
      OK(_struct "A_PlainStruct const" _pointer, h2::h2_cxa::type_name<const A_PlainStruct*>());
      OK(_class "A_AbstractClass const" _pointer, h2::h2_cxa::type_name<const A_AbstractClass*>());
   }

   Case(const pointer reference)
   {
      OK("char const" _pointer "&", h2::h2_cxa::type_name<const char*&>());
      OK("unsigned long const" _pointer "&", h2::h2_cxa::type_name<const unsigned long*&>());
      OK(_struct "A_PlainStruct const" _pointer "&", h2::h2_cxa::type_name<const A_PlainStruct*&>());
      OK(_class "A_AbstractClass const" _pointer "&", h2::h2_cxa::type_name<const A_AbstractClass*&>());
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
