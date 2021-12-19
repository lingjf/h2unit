#if defined __clang__
#pragma GCC diagnostic ignored "-Wrange-loop-analysis"
#endif

#include "../source/h2_unit.cpp"
#include "test_cplusplus.hpp"

SUITE(container bool problem)
{
   // https://isocpp.org/blog/2012/11/on-vectorbool

   Case(C/C++ generic array)
   {
      bool a1[] = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }

   Case(vector)
   {
      std::vector<bool> a1 = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }

   Case(array)
   {
      std::array<bool, 3> a1 = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }

   Case(deque)
   {
      std::deque<bool> a1 = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }

   Case(forward_list)
   {
      std::forward_list<bool> a1 = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }

   Case(list)
   {
      std::list<bool> a1 = {true, false, true};
      OK(ListOf(1, 0, 1), a1);
      OK(CountOf(3), a1);
      OK(Has(false), a1);
   }
}
