#include "../source/h2_unit.cpp"
#include "test_types.hpp"

#include <vector>
#include <deque>
#include <array>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <tuple>
#include <valarray>

// https://en.cppreference.com/w/cpp/language/types
#define I_integer I_short, I_int, I_long, I_long_long

#define I_short I_signed_short I_unsigned_short
#define I_int I_signed_int I_unsigned_int
#define I_long I_signed_long I_unsigned_long
#define I_long_long I_signed_long_long I_unsigned_long_long

#define I_signed_short short,        \
                       short int,    \
                       signed short, \
                       signed short int

#define I_unsigned_short unsigned short, \
                         unsigned short int

#define I_signed_int int,    \
                     signed, \
                     signed int

#define I_unsigned_int unsigned, \
                       unsigned int

#define I_signed_long long,        \
                      long int,    \
                      signed long, \
                      signed long int

#define I_unsigned_long unsigned long, \
                        unsigned long int

#define I_signed_long_long long long,        \
                           long long int,    \
                           signed long long, \
                           signed long long int

#define I_unsigned_long_long unsigned long long, \
                             unsigned long long int

#define I_float_point float, double, long double

SUITE(numberfy_impl)
{
   Case(int)
   {
      OK(42, h2::h2_numberfy_impl<int>::cast((int)42));
   }

   Case(int qualifiers)
   {
      int a = 42;
      OK(42, h2::h2_numberfy_impl<int>::cast(a));
      const int ca = 42;
      OK(42, h2::h2_numberfy_impl<int>::cast(ca));
      int& ar = a;
      OK(42, h2::h2_numberfy_impl<int>::cast(ar));
      int&& arr = 42;
      OK(42, h2::h2_numberfy_impl<int>::cast(arr));
      const int& car = ca;
      OK(42, h2::h2_numberfy_impl<int>::cast(car));
      const int&& carr = 42;
      OK(42, h2::h2_numberfy_impl<int>::cast(carr));
   }

   Case(int*)
   {
      OK(42, h2::h2_numberfy_impl<int*>::cast((int*)42));
   }

   Case(char*)
   {
      OK(42, h2::h2_numberfy_impl<char*>::cast((char*)42));
      // OK(42, h2::h2_numberfy_impl<char*>::cast((const char*)42));
   }

   Case(const char*)
   {
      OK(42, h2::h2_numberfy_impl<const char*>::cast((const char*)42));
      OK(42, h2::h2_numberfy_impl<const char*>::cast((char*)42));
   }

   Case(void*)
   {
      OK(42, h2::h2_numberfy_impl<void*>::cast((void*)42));
   }

   Case(double)
   {
      OK(42, h2::h2_numberfy_impl<double>::cast((double)42.0));
   }

   Case(nullptr)
   {
      OK(0, h2::h2_numberfy_impl<std::nullptr_t>::cast(nullptr));
   }

   Case(NULL)
   {
      OK(0, h2::h2_numberfy_impl<decltype(NULL)>::cast(NULL));
   }

   Case(smart ptr)
   {
      std::shared_ptr<int> a1;
      OK(0, h2::h2_numberfy_impl<decltype(a1)>::cast(a1));
   }

   Case(normal function ptr)
   {
      int (*a1)(int) = nullptr;
      OK(0, h2::h2_numberfy_impl<decltype(a1)>::cast(a1));
      auto a2 = &::sprintf;
      OK(Not(NULL), h2::h2_numberfy_impl<decltype(a2)>::cast(a2));
   }

   Case(normal member function ptr)
   {
      auto a1 = &C_OverrideClass::normal_f1;
      OK(Not(NULL), h2::h2_numberfy_impl<decltype(a1)>::cast(a1));
   }

   Case(static member function ptr)
   {
      auto a1 = &C_OverrideClass::static_f1;
      OK(Not(NULL), h2::h2_numberfy_impl<decltype(a1)>::cast(a1));
   }

   Case(virtual member function ptr)
   {
      auto a1 = &C_OverrideClass::virtual_f1;
      OK(_, h2::h2_numberfy_impl<decltype(a1)>::cast(a1));
   }

   Case(normal member object ptr)
   {
      auto a1 = &C_OverrideClass::s;
      OK(Not(NULL), h2::h2_numberfy_impl<decltype(a1)>::cast(a1));
   }
}

SUITE(numberfy)
{
   Case(nullptr to all number)
   {
      OK(0, h2::h2_numberfy<char>(nullptr));
      OK(0, h2::h2_numberfy<signed char>(nullptr));
      OK(0, h2::h2_numberfy<unsigned char>(nullptr));
      OK(0, h2::h2_numberfy<short int>(nullptr));
      OK(0, h2::h2_numberfy<unsigned short int>(nullptr));
      OK(0, h2::h2_numberfy<int>(nullptr));
      OK(0, h2::h2_numberfy<unsigned int>(nullptr));
      OK(0, h2::h2_numberfy<long int>(nullptr));
      OK(0, h2::h2_numberfy<unsigned long int>(nullptr));
      OK(0, h2::h2_numberfy<long long int>(nullptr));
      OK(0, h2::h2_numberfy<unsigned long long int>(nullptr));
      OK(0, h2::h2_numberfy<float>(nullptr));
      OK(0, h2::h2_numberfy<double>(nullptr));
      OK(0, h2::h2_numberfy<long double>(nullptr));
      OK(!h2::h2_numberfy<void*>(nullptr));
      OK(!h2::h2_numberfy<int*>(nullptr));
   }

   Case(int)
   {
      int a = 42;
      OK(42, h2::h2_numberfy<int>(a));
      const int ca = 42;
      OK(42, h2::h2_numberfy<int>(ca));
      int& ar = a;
      OK(42, h2::h2_numberfy<int>(ar));
      int&& arr = 42;
      OK(42, h2::h2_numberfy<int>(arr));
      const int& car = ca;
      OK(42, h2::h2_numberfy<int>(car));
      const int&& carr = 42;
      OK(42, h2::h2_numberfy<int>(carr));
   }

   Case(void*)
   {
      void* a = (void*)42;
      OK(42, h2::h2_numberfy<int>(a));
      const void* ca = (void*)42;
      OK(42, h2::h2_numberfy<int>(ca));
      void*& ar = a;
      OK(42, h2::h2_numberfy<int>(ar));
      void*&& arr = (void*)42;
      OK(42, h2::h2_numberfy<int>(arr));
      const void*& car = ca;
      OK(42, h2::h2_numberfy<int>(car));
      const void*&& carr = (void*)42;
      OK(42, h2::h2_numberfy<int>(carr));
   }

   Case(double)
   {
      double a = 42.0;
      OK(42, h2::h2_numberfy<int>(a));
      const double ca = 42.0;
      OK(42, h2::h2_numberfy<int>(ca));
      double& ar = a;
      OK(42, h2::h2_numberfy<int>(ar));
      double&& arr = 42.0;
      OK(42, h2::h2_numberfy<int>(arr));
      const double& car = ca;
      OK(42, h2::h2_numberfy<int>(car));
      const double&& carr = 42.0;
      OK(42, h2::h2_numberfy<int>(carr));
   }

   Case(nullptr)
   {
      OK(0, h2::h2_numberfy<int>(nullptr));
   }

   Case(NULL)
   {
      OK(0, h2::h2_numberfy<int>(NULL));
   }

   Case(smart ptr)
   {
      std::shared_ptr<int> a1;
      OK(0, h2::h2_numberfy<int>(a1));
   }

   Case(normal function ptr)
   {
      int (*a1)(int) = nullptr;
      OK(0, h2::h2_numberfy<int>(a1));
      OK(Not(NULL), h2::h2_numberfy<int>(&::sprintf));
   }

   Case(normal member function ptr)
   {
      OK(Not(NULL), h2::h2_numberfy<int>(&C_OverrideClass::normal_f1));
   }

   Case(static member function ptr)
   {
      OK(Not(NULL), h2::h2_numberfy<int>(&C_OverrideClass::static_f1));
   }

   Case(virtual member function ptr)
   {
      OK(_, h2::h2_numberfy<int>(&C_OverrideClass::virtual_f1));
   }

   Case(normal member object ptr)
   {
      OK(Not(NULL), h2::h2_numberfy<int>(&C_OverrideClass::s));
   }
}
