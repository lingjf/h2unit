#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(matcher cast)
{
   Case(1.1)
   {
      h2::h2_matcher<int> m2 = h2::h2_matcher_cast_impl<int, double>::cast(123.45);
      OK(nullptr == m2.matches(123));
   }

   Case(1.2)
   {
   }

   Case(1.3)
   {
   }

   Case(2)
   {
      h2::h2_matcher<double> m1(Eq(123));
      h2::h2_matcher<int> m2 = h2::h2_matcher_cast_impl<int, h2::h2_matcher<double>>::cast(m1);
      OK(nullptr == m2.matches(123));
   }

   Case(3)
   {
      h2::h2_matcher<double> m1(Eq(123.45));
      h2::h2_matcher<double> m2 = h2::h2_matcher_cast_impl<double, h2::h2_matcher<double>>::cast(m1);
      OK(nullptr == m2.matches(123.45));
   }
}
