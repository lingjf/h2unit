#include "../source/h2_unit.cpp"

SUITE(Matches)
{
   int int65 = 65, int66 = 66;
   Case(Pointee)
   {
      h2::h2_pointee_matches<h2::h2_polymorphic_matcher<h2::h2_equals<int>>>
        a1(h2::h2_polymorphic_matcher<h2::h2_equals<int>>{h2::h2_equals<int>(65)});

      OK(nullptr == a1.matches(&int65));
      OK(nullptr != a1.matches(&int66));
      OK("65", a1.expects(&int65));
   }

   Case(CaseLess)
   {
      h2::h2_caseless_matches a1(h2::h2_matcher<h2::h2_string>("A"));
      OK(nullptr == a1.matches("A"));
      OK(nullptr == a1.matches("a"));
      OK("~\"A\"", a1.expects("A"));
   }

   Case(Not)
   {
      h2::h2_not_matches<h2::h2_polymorphic_matcher<h2::h2_equals<int>>>
        a1(h2::h2_polymorphic_matcher<h2::h2_equals<int>>{h2::h2_equals<int>(65)});
      OK(nullptr != a1.matches(65));
      OK(nullptr == a1.matches(66));
      OK("!65", a1.expects(65));

      h2::h2_not_matches<h2::h2_polymorphic_matcher<h2::h2_matchee_null>> a2(
        h2::h2_polymorphic_matcher<h2::h2_matchee_null>{h2::h2_matchee_null(false)});
      OK(nullptr != a2.matches(nullptr));
      OK(nullptr == a2.matches(&int65));
      OK("NotNull", a2.expects(nullptr));

      h2::h2_not_matches<h2::h2_polymorphic_matcher<h2::h2_matchee_substr>> a3(
        h2::h2_polymorphic_matcher<h2::h2_matchee_substr>{h2::h2_matchee_substr("A")});
      OK(nullptr != a3.matches("ABC"));
      OK(nullptr == a3.matches("BBC"));
   }

   Case(AllOf)
   {
      using T1 = h2::h2_polymorphic_matcher<h2::h2_matchee_null>;
      using T2 = h2::h2_polymorphic_matcher<h2::h2_matchee_any>;
      h2::h2_allof_matches<T1, T2> a(T1{h2::h2_matchee_null(false)}, T2{h2::h2_matchee_any()});
      OK(nullptr == a.matches(nullptr));
   }

   Case(AnyOf)
   {
      using T1 = h2::h2_polymorphic_matcher<h2::h2_matchee_null>;
      using T2 = h2::h2_polymorphic_matcher<h2::h2_matchee_any>;
      h2::h2_anyof_matches<T1, T2> a(T1{h2::h2_matchee_null(false)}, T2{h2::h2_matchee_any()});
      OK(nullptr == a.matches(nullptr));
      OK(nullptr == a.matches(NULL));
   }

   Case(NoneOf)
   {
      using T1 = h2::h2_polymorphic_matcher<h2::h2_matchee_null>;
      using T2 = h2::h2_polymorphic_matcher<h2::h2_pointee_matches<h2::h2_matcher<int>>>;
      h2::h2_pointee_matches<h2::h2_matcher<int>> a_(h2::h2_matcher<int>(66));
      h2::h2_noneof_matches<T1, T2> a(T1{h2::h2_matchee_null(false)}, T2(a_));
      OK(nullptr == a.matches(&int65));
   }

   Case(ListOf)
   {
      using T1 = h2::h2_polymorphic_matcher<h2::h2_equals<int>>;

      T1 a1_ = T1(h2::h2_equals<int>(1));
      T1 a2_ = T1(h2::h2_equals<int>(2));
      T1 a3_ = T1(h2::h2_equals<int>(3));

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
