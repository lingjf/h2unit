#include "../source/h2_unit.cpp"

SUITE(logic matches)
{
   int int65 = 65;

   Case(Not)
   {
      h2::h2_not_matches<h2::h2_polymorphic_matcher<h2::h2_equation<int>>> a1(h2::h2_polymorphic_matcher<h2::h2_equation<int>>{h2::h2_equation<int>(65)});
      OK(nullptr != a1.matches(65, 0, {}));
      OK(nullptr == a1.matches(66, 0, {}));
      OK("≠65", a1.expection({}).string());

      h2::h2_not_matches<h2::h2_polymorphic_matcher<h2::h2_matches_null>> a2(h2::h2_polymorphic_matcher<h2::h2_matches_null>{h2::h2_matches_null(false)});
      OK(nullptr != a2.matches(nullptr, 0, {}));
      OK(nullptr == a2.matches(&int65, 0, {}));
      OK("NotNull", a2.expection({}).string());

      h2::h2_not_matches<h2::h2_polymorphic_matcher<h2::h2_matches_substr>> a3(h2::h2_polymorphic_matcher<h2::h2_matches_substr>{h2::h2_matches_substr("A")});
      OK(nullptr != a3.matches("ABC", 0, {}));
      OK(nullptr == a3.matches("BBC", 0, {}));
   }

   Case(AllOf)
   {
      using T1 = h2::h2_polymorphic_matcher<h2::h2_matches_null>;
      using T2 = h2::h2_polymorphic_matcher<h2::h2_matches_any>;
      h2::h2_allof_matches<T1, T2> a(T1{h2::h2_matches_null(false)}, T2{h2::h2_matches_any()});
      OK(nullptr == a.matches(nullptr, 0, {}));
   }

   Case(AnyOf)
   {
      using T1 = h2::h2_polymorphic_matcher<h2::h2_matches_null>;
      using T2 = h2::h2_polymorphic_matcher<h2::h2_matches_any>;
      h2::h2_anyof_matches<T1, T2> a(T1{h2::h2_matches_null(false)}, T2{h2::h2_matches_any()});
      OK(nullptr == a.matches(nullptr, 0, {}));
      OK(nullptr == a.matches(NULL, 0, {}));
   }

   Case(NoneOf)
   {
      using T1 = h2::h2_polymorphic_matcher<h2::h2_matches_null>;
      using T2 = h2::h2_polymorphic_matcher<h2::h2_pointee_matches<h2::h2_matcher<int>>>;
      h2::h2_pointee_matches<h2::h2_matcher<int>> a_(h2::h2_matcher<int>(66));
      h2::h2_noneof_matches<T1, T2> a(T1{h2::h2_matches_null(false)}, T2(a_));
      OK(nullptr == a.matches(&int65, 0, {}));
   }
}

SUITE(logic primitive)
{
   Case(Not)
   {
      OK(Not(1), 2);
      OK(Not("A"), "a");
      OK(Not(Not(2)), 2);
      OK((Not(1)), 2);
   }

   Case(Not !)
   {
      OK(!1, 0);  // normal !
      OK(!(Not("A")), "A");
      OK(!(Not(1)), 1);
   }

   Case(&&Primitive)
   {
      OK(Ge(-1) && Le(1), 0);
      OK(CaseLess("AbCd") && Eq("abcd"), "abcd");
      OK(Ge(-1) && Le(1) && Eq(0), 0);
   }

   Case(&&naive)
   {
      OK(Ge(-1) && 1, 1);
      OK(1 && Ge(-1), 1);
      OK(CaseLess("AbCd") && "abcd", "abcd");
      OK(CaseLess("AbCd") && "abcd" && StartsWith("ab"), "abcd");
   }

   Case(|| Primitive)
   {
      OK(Ge(-1) || Le(1), 0);
      OK(CaseLess("AbCd") || Eq("abcd"), "abcd");
   }

   Case(|| naive)
   {
      OK(Ge(-1) || 1, 1);
      OK(1 || Ge(-1), 1);
      OK(CaseLess("AbCd") || "abcd", "abcd");
   }

   Case(&& || !)
   {
      OK(Ge(0) || -1 && Eq(1), 1);
      OK(!(Ge(0) || -1), -2);
   }
}

SUITE(logic sugar primitive)
{
   Case(AllOf &&&&&&)
   {
      OK(AllOf(2, Ge(0), Lt(3)), 2);
   }

   Case(AnyOf ||||||)
   {
      OK(AnyOf(2, Ge(0), Pair(2, 3)), 2);
   }

   Case(NoneOf !||||||)
   {
      OK(NoneOf(1, Ge(3), Pair(2, 3)), 2);
   }
}
