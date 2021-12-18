#include "../build/h2unit.hpp"

SUITE(ListOf)
{
   Case(only one not same)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 6, 3), a);
   }

   Case(two not same)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 6, _), a);
   }

   Case(only one miss)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 2, 3, 4), a);
   }

   Case(Not)
   {
      std::vector<int> a = {1, 2, 3};
      OK(!ListOf(1, 2, 3), a);
   }

   Case(C/C++ native array)
   {
      int a[] = {1, 2, 3};
      OK(ListOf(1, 7, 3), a);
   }
}

SUITE(Has)
{
   Case(miss only one)
   {
      std::vector<int> a = {1, 2, 3};
      OK(Has(8), a);
   }

   Case(not has)
   {
      std::vector<int> a = {1, 2, 3};
      OK(!Has(2), a);
   }

   Case(C/C++ generic array)
   {
      int a[] = {1, 2, 3};
      OK(Has(4), a);
   }
}
