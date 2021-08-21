#include "../source/h2_unit.cpp"
#if !defined __CYGWIN__

#include "test_types.hpp"
NUMBER0_DECL_LIST;
NUMBER1_DECL_LIST;

#define TheCheck(x) static void foo1##x(decltype(x) v){};
H2Foreach(TheCheck, NUMBER0_VAR_LIST);
#undef TheCheck

SUITE(Same Number Type)
{
   Case(signed)
   {
      OK(std::is_same<short, short int>::value);
      OK(std::is_same<long, long int>::value);
      OK(std::is_same<long long, long long int>::value);
      OK(std::is_same<signed, int>::value || std::is_same<signed, long int>::value || std::is_same<signed, long long int>::value);
      OK(std::is_same<intptr_t, int>::value || std::is_same<intptr_t, long int>::value || std::is_same<intptr_t, long long int>::value);
      OK(std::is_same<ssize_t, int>::value || std::is_same<ssize_t, long int>::value || std::is_same<ssize_t, long long int>::value);
   }

   Case(unsigned)
   {
      OK((std::is_same<unsigned short, unsigned short int>::value));
      OK((std::is_same<unsigned long, unsigned long int>::value));
      OK((std::is_same<unsigned long long, unsigned long long int>::value));
      OK((std::is_same<unsigned, unsigned int>::value) || (std::is_same<unsigned, unsigned long int>::value) || (std::is_same<unsigned, unsigned long long int>::value));
      OK((std::is_same<uintptr_t, unsigned int>::value) || (std::is_same<uintptr_t, unsigned long int>::value) || (std::is_same<uintptr_t, unsigned long long int>::value));
      OK((std::is_same<size_t, unsigned int>::value) || (std::is_same<size_t, unsigned long int>::value) || (std::is_same<size_t, unsigned long long int>::value));
   }

   Case(const)
   {
      // OK((std::is_same<int, const int>::value));
      // OK((std::is_same<int, int&>::value));
   }
}

SUITE(number[integer, float, enum, bool])
{
   Case(OK1)
   {
#define TheCheck(x) \
   OK(x);           \
   OK(IsTrue, x);   \
   OK(!IsFalse, x);
      H2Foreach(TheCheck, NUMBER1_VAR_LIST);
#undef TheCheck
   }

   Case(OK2)
   {
#define TheCheck(x, y) \
   OK(x, y);           \
   OK(Ge(x) || Gt(x) || Le(x) || Lt(x) || Nq(x) || Eq(x) && Ge(0), y);
      H2Fullmesh(TheCheck, NUMBER1_VAR_LIST);
#undef TheCheck
   }

   Case(MOCK)
   {
#define TheCheck(x)                          \
   MOCK(foo1##x, void(decltype(x))).Once(x); \
   foo1##x(x);
      H2Foreach(TheCheck, NUMBER0_VAR_LIST);
#undef TheCheck
   }
}
#endif
