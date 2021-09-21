#include "../source/h2_unit.cpp"

SUITE(strcmp matches)
{
   Case(Regex)
   {
      h2::h2_matches_regex a("A.*");
      OK(nullptr == a.matches("ABC", 0, false, false, false));
      OK(nullptr != a.matches("BBC", 0, false, false, false));
   }

   Case(Wildcard)
   {
      h2::h2_matches_wildcard a("A*");
      OK(nullptr == a.matches("ABC", 0, false, false, false));
      OK(nullptr != a.matches("BBC", 0, false, false, false));
   }

   Case(Substr)
   {
      h2::h2_matches_substr a("A");
      OK(nullptr == a.matches("ABC", 0, false, false, false));
      OK(nullptr != a.matches("BBC", 0, false, false, false));
   }

   Case(StartsWith)
   {
      h2::h2_matches_startswith a("A");
      OK(nullptr == a.matches("ABC", 0, false, false, false));
      OK(nullptr != a.matches("BBC", 0, false, false, false));
   }

   Case(EndsWith)
   {
      h2::h2_matches_endswith a("A");
      OK(nullptr == a.matches("CBA", 0, false, false, false));
      OK(nullptr != a.matches("ABC", 0, false, false, false));
   }

   Case(Json)
   {
      h2::h2_matches_json a("[65]", "");
      OK(nullptr == a.matches("[65]", 0, false, false, false));
      OK(nullptr != a.matches("[66]", 0, false, false, false));
   }

   Case(CaseLess)
   {
      h2::h2_caseless_matches a1(h2::h2_matcher<h2::h2_string>("A"));
      OK(nullptr == a1.matches("A", 0, false, false, false));
      OK(nullptr == a1.matches("a", 0, false, false, false));
      OK("~\"A\"", a1.expection(false, false, false).string());
   }
}

SUITE(strcmp primitive)
{
   Case(Re)
   {
      OK(Re("abc.*"), "abcdef");
      OK(!Re("A.*"), "abcdef");
   }

   Case(We)
   {
      OK(We("abc*"), "abcdef");
      OK(!We("abc?yz"), "abcdef");
   }

   Case(CaseLess)
   {
      OK(CaseLess("AbCd"), "abcd");
      OK(!CaseLess("AbCd"), "cba");
      OK(CaseLess(Substr("AbCd")), "ABCD");
      OK(CaseLess(Re("A.*")), "abcdef");
      OK(CaseLess(We("A*")), "abcdef");
   }

   Case(CaseLess ~)
   {
      // OK(~"AbCd", "abcd");
      OK(~Substr("AbCd"), "ABCD");
      OK(~Re("A.*"), "abcdef");
      OK(~We("A*"), "abcdef");
   }

   Case(CaseLess *)
   {
      // OK(*"AbCd", "abcd");
      OK(*Substr("AbCd"), "ABCD");
      OK(*Re("A.*"), "abcdef");
      OK(*We("A*"), "abcdef");
   }
}
