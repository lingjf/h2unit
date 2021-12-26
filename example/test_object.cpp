
#include "product_cpp.h"

#if defined H2UNIT && H2UNIT == 2
#include "../build/h2unit.hpp"
#else
#include "../h2unit.h"
#endif

SUITE(ListOf)
{
   Case(only one not same [fail])
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 6, 3), a);
   }

   Case(two not same [fail])
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 6, _), a);
   }

   Case(only one miss [fail])
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 2, 3, 4), a);
   }

   Case(Not [fail])
   {
      std::vector<int> a = {1, 2, 3};
      OK(!ListOf(1, 2, 3), a);
   }

   Case(C/C++ native array [fail])
   {
      int a[] = {1, 2, 3};
      OK(ListOf(1, 7, 3), a);
   }
}

SUITE(Has)
{
   Case(miss only one [fail])
   {
      std::vector<int> a = {1, 2, 3};
      OK(Has(8), a);
   }

   Case(not has [fail])
   {
      std::vector<int> a = {1, 2, 3};
      OK(!Has(2), a);
   }

   Case(has two [pass])
   {
      std::vector<int> a = {1, 2, 2};
      OK(Has(2) * 2, a);
   }

   Case(has three [fail])
   {
      std::vector<int> a = {1, 2, 2};
      OK(Has(2) * 3, a);
   }

   Case(C/C++ generic array [fail])
   {
      int a[] = {1, 2, 3};
      OK(Has(4), a);
   }
}

CASE(Every [fail])
{
   std::vector<int> a = {1, 2, 3};
   OK(Every(Ge(2)), a);
}

CASE(CountOf [fail])
{
   std::vector<int> a = {1, 2, 3};
   OK(CountOf(8), a);
}

CASE(MaxOf [fail])
{
   std::vector<int> a = {1, 2, 3};
   OK(MaxOf(1), a);
}

CASE(MinOf [fail])
{
   std::vector<int> a = {1, 2, 3};
   OK(MinOf(3), a);
}

CASE(AvgOf [fail])
{
   std::vector<int> a = {1, 2, 3};
   OK(AvgOf(-2), a);
}

CASE(MeanOf [fail])
{
   std::vector<int> a = {1, 2, 3};
   OK(MeanOf(-2), a);
}

CASE(MedianOf [fail])
{
   std::vector<int> a = {1, 2, 3};
   OK(MedianOf(-2), a);
}

SUITE(specified subset of container)
{
   Case(Has with C/C++ generic array [fail])
   {
      int a[] = {1, 2, 3};

      // way 1 : ' /size '
      OK(Has(2) / 2, a);

      // way 2 : python range style 1 argument
      OK(Has(2)(2), a);

      // way 3 : python range style 2 argument
      OK(Has(1)(1, 2), a);

      // way 4 : python range style in template parameter 1 argument
      OK(Has<2>(2), a);

      // way 5 : python range style in template parameter 2 argument
      OK((Has<1, 2>(1)), a);
   }

   Case(Every with vector [pass])
   {
      std::vector<int> a = {1, 2, 3};

      // way 1 : ' /size '
      OK(Every(Ge(0)) / 2, a);

      // way 2 : python range style 1 argument
      OK(Every(Ge(0))(2), a);

      // way 3 : python range style 2 argument
      OK(Every(Ge(0))(1, 2), a);

      // way 4 : python range style in template parameter 1 argument
      OK(Every<2>(Ge(0)), a);

      // way 5 : python range style in template parameter 2 argument
      OK((Every<1, 2>(Ge(0))), a);
   }
}

SUITE(Member)
{
   Rect a(1, 1, 3, 3);

   Case(data [fail])
   {
      OK(Member(30, &Rect::width), a);
   }

#if __cplusplus >= 201402L || (defined _MSVC_LANG && _MSVC_LANG >= 201402L)
   Case(function 0 [pass])
   {
      OK(Member("Rect", &Rect::print), a);
      OK(Member("Rect", &Rect::print, ()), a);
   }

   Case(function 1 [pass])
   {
      OK(Member(0, &Shape::move, 2), a);
      OK(Member(0, &Shape::move, (2)), a);
   }

   Case(function 2 [fail])
   {
      OK(Member(42, &Shape::move, 2, (int)2LL), a);
      OK(Member(42, &Shape::move, (2, (int)2LL)), a);
   }
#endif
}

/*
 * h2unit Pointee can be used to verify the data pointer point to.
 */
CASE(point to compares [fail])
{
   int x = 1;
   OK(Pointee(2), &x);
}
