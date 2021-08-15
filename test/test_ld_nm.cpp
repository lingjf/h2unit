#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(type name)
{
   Case(basic)
   {
      OK("char", h2::h2_type_name<char>((char*)alloca(256)));
      OK("signed char", h2::h2_type_name<signed char>((char*)alloca(256)));
      OK("unsigned char", h2::h2_type_name<unsigned char>((char*)alloca(256)));
      OK("short", h2::h2_type_name<short>((char*)alloca(256)));
      OK("short", h2::h2_type_name<signed short>((char*)alloca(256)));
      OK("unsigned short", h2::h2_type_name<unsigned short>((char*)alloca(256)));
      OK("short", h2::h2_type_name<short int>((char*)alloca(256)));
      OK("short", h2::h2_type_name<signed short int>((char*)alloca(256)));
      OK("unsigned short", h2::h2_type_name<unsigned short int>((char*)alloca(256)));
      OK("int", h2::h2_type_name<int>((char*)alloca(256)));
      OK("int", h2::h2_type_name<signed int>((char*)alloca(256)));
      OK("unsigned int", h2::h2_type_name<unsigned int>((char*)alloca(256)));
      OK("long", h2::h2_type_name<long>((char*)alloca(256)));
      OK("long", h2::h2_type_name<signed long>((char*)alloca(256)));
      OK("unsigned long", h2::h2_type_name<unsigned long>((char*)alloca(256)));
      OK("long", h2::h2_type_name<long int>((char*)alloca(256)));
      OK("long", h2::h2_type_name<signed long int>((char*)alloca(256)));
      OK("unsigned long", h2::h2_type_name<unsigned long int>((char*)alloca(256)));
      OK(_long_long, h2::h2_type_name<long long>((char*)alloca(256)));
      OK(_long_long, h2::h2_type_name<signed long long>((char*)alloca(256)));
      OK("unsigned " _long_long, h2::h2_type_name<unsigned long long>((char*)alloca(256)));
      OK(_long_long, h2::h2_type_name<long long int>((char*)alloca(256)));
      OK(_long_long, h2::h2_type_name<signed long long int>((char*)alloca(256)));
      OK("unsigned " _long_long, h2::h2_type_name<unsigned long long int>((char*)alloca(256)));
      OK("float", h2::h2_type_name<float>((char*)alloca(256)));
      OK("double", h2::h2_type_name<double>((char*)alloca(256)));
      OK("bool", h2::h2_type_name<bool>((char*)alloca(256)));
      OK("unsigned int", h2::h2_type_name<unsigned>((char*)alloca(256)));
   }

   Case(void)
   {
      OK("void", h2::h2_type_name<void>((char*)alloca(256)));
   }

   Case(enum)
   {
      OK(_enum "A_Enum", h2::h2_type_name<A_Enum>((char*)alloca(256)));
   }

   Case(struct)
   {
      OK(_struct "A_PlainStruct", h2::h2_type_name<A_PlainStruct>((char*)alloca(256)));
      OK(_struct "A_PlainStruct", h2::h2_type_name<struct A_PlainStruct>((char*)alloca(256)));
   }

   Case(class)
   {
      OK(_class "A_AbstractClass", h2::h2_type_name<A_AbstractClass>((char*)alloca(256)));
      OK(_class "B_DerivedClass", h2::h2_type_name<class B_DerivedClass>((char*)alloca(256)));
      OK(_class "C_OverrideClass", h2::h2_type_name<C_OverrideClass>((char*)alloca(256)));
      OK(_class "D_NoConstructorClass", h2::h2_type_name<class D_NoConstructorClass>((char*)alloca(256)));
   }

   Case(typedef)
   {
      typedef char TypeDef_char;
      OK("char", h2::h2_type_name<TypeDef_char>((char*)alloca(256)));
      typedef unsigned int TypeDef_unsigned_int;
      OK("unsigned int", h2::h2_type_name<TypeDef_unsigned_int>((char*)alloca(256)));
      typedef A_PlainStruct TypeDef_A_PlainStruct;
      OK(_struct "A_PlainStruct", h2::h2_type_name<TypeDef_A_PlainStruct>((char*)alloca(256)));
      typedef class A_AbstractClass TypeDef_A_AbstractClass;
      OK(_class "A_AbstractClass", h2::h2_type_name<TypeDef_A_AbstractClass>((char*)alloca(256)));
      typedef char A_CharArray[8];
      OK("char [8]", h2::h2_type_name<A_CharArray>((char*)alloca(256)));
   }

   Case(pointer)
   {
      OK("char" _pointer, h2::h2_type_name<char*>((char*)alloca(256)));
      OK("unsigned long" _pointer, h2::h2_type_name<unsigned long*>((char*)alloca(256)));
      OK(_struct "A_PlainStruct" _pointer, h2::h2_type_name<struct A_PlainStruct*>((char*)alloca(256)));
      OK(_class "A_AbstractClass" _pointer, h2::h2_type_name<class A_AbstractClass*>((char*)alloca(256)));
   }

   Case(double pointer)
   {
      OK("char" _pointer _pointer, h2::h2_type_name<char**>((char*)alloca(256)));
      OK("unsigned long" _pointer _pointer, h2::h2_type_name<unsigned long**>((char*)alloca(256)));
      OK(_struct "A_PlainStruct" _pointer _pointer, h2::h2_type_name<struct A_PlainStruct**>((char*)alloca(256)));
      OK(_class "A_AbstractClass" _pointer _pointer, h2::h2_type_name<class A_AbstractClass**>((char*)alloca(256)));
   }

   Case(triple pointer)
   {
      OK("char" _pointer _pointer _pointer, h2::h2_type_name<char***>((char*)alloca(256)));
      OK("unsigned long" _pointer _pointer _pointer, h2::h2_type_name<unsigned long***>((char*)alloca(256)));
      OK(_struct "A_PlainStruct" _pointer _pointer _pointer, h2::h2_type_name<struct A_PlainStruct***>((char*)alloca(256)));
      OK(_class "A_AbstractClass" _pointer _pointer _pointer, h2::h2_type_name<class A_AbstractClass***>((char*)alloca(256)));
   }

   Case(function pointer)
   {
      OK(AnyOf("int (*)(int, int)", "int (__cdecl*)(int,int)"), h2::h2_type_name<A_FunctionPointer>((char*)alloca(256)));
   }

   Case(reference)
   {
      OK("char&", h2::h2_type_name<char&>((char*)alloca(256)));
      OK("unsigned long&", h2::h2_type_name<unsigned long&>((char*)alloca(256)));
      OK(_struct "A_PlainStruct&", h2::h2_type_name<struct A_PlainStruct&>((char*)alloca(256)));
      OK(_class "A_AbstractClass&", h2::h2_type_name<class A_AbstractClass&>((char*)alloca(256)));
   }

   Case(const)
   {
      OK("const char", h2::h2_type_name<const char>((char*)alloca(256)));
      OK("const unsigned long", h2::h2_type_name<const unsigned long>((char*)alloca(256)));
      OK("const " _struct "A_PlainStruct", h2::h2_type_name<const A_PlainStruct>((char*)alloca(256)));
      OK("const " _class "A_AbstractClass", h2::h2_type_name<const A_AbstractClass>((char*)alloca(256)));
   }

   Case(const pointer)
   {
      OK("char const" _pointer, h2::h2_type_name<const char*>((char*)alloca(256)));
      OK("unsigned long const" _pointer, h2::h2_type_name<const unsigned long*>((char*)alloca(256)));
      OK(_struct "A_PlainStruct const" _pointer, h2::h2_type_name<const A_PlainStruct*>((char*)alloca(256)));
      OK(_class "A_AbstractClass const" _pointer, h2::h2_type_name<const A_AbstractClass*>((char*)alloca(256)));
   }

   Case(const pointer reference)
   {
      OK("char const" _pointer "&", h2::h2_type_name<const char*&>((char*)alloca(256)));
      OK("unsigned long const" _pointer "&", h2::h2_type_name<const unsigned long*&>((char*)alloca(256)));
      OK(_struct "A_PlainStruct const" _pointer "&", h2::h2_type_name<const A_PlainStruct*&>((char*)alloca(256)));
      OK(_class "A_AbstractClass const" _pointer "&", h2::h2_type_name<const A_AbstractClass*&>((char*)alloca(256)));
   }
}

SUITE(nm)
{
   Case(get_by_name)
   {
      h2::h2_symbol* res[16];
      int n = h2::h2_nm::get_by_name("foobar0", res, 16);
      OK(1, n);
      OK(NotNull, res[0]->addr);
   }
}

#if !defined _WIN32
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
      char* demangled = h2::h2_nm::demangle(mangled);
      OK("h2::unit::malloc(unsigned long)", demangled);
   }

   Case(demangle c - function)
   {
      char mangled[] = "malloc";
      char* demangled = h2::h2_nm::demangle(mangled);
      OK("malloc", demangled);
   }
}
#endif
