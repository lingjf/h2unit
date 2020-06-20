#include "../source/h2_unit.cpp"

SUITE(container matches)
{
   Case(ListOf)
   {
      using T1 = h2::h2_polymorphic_matcher<h2::h2_equation<int>>;

      T1 a1_ = T1(h2::h2_equation<int>(1));
      T1 a2_ = T1(h2::h2_equation<int>(2));
      T1 a3_ = T1(h2::h2_equation<int>(3));

      h2::h2_listof_matches<T1, T1, T1> a(a1_, a2_, a3_);

      int b1[] = {1, 2, 3};
      OK(nullptr == a.matches(b1));

      std::vector<int> b2 = {1, 2, 3};
      OK(nullptr == a.matches(b2));
   }

   Case(In)
   {
   }
}

SUITE(container primitive)
{
   Case(ListOf)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 2, 3), a);
   }

   Case(In)
   {
      OK(In(1, 2, 3), 2);
   }

   Case(Has)
   {
      std::vector<int> a1 = {1, 2, 3};
      OK(Has(1, 3), a1);

      std::map<int, int> a2 = {{1, 111}, {2, 222}, {3, 333}};
      OK(Has(Pair(1, 111), Pair(3, 333)), a2);
   }
}