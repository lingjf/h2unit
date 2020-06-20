#include "../source/h2_unit.cpp"

SUITE(strcmp matches)
{
   Case(Regex)
   {
      h2::h2_matches_regex a("A.*");
      OK(nullptr == a.matches("ABC"));
      OK(nullptr != a.matches("BBC"));
   }

   Case(Wildcard)
   {
      h2::h2_matches_wildcard a("A*");
      OK(nullptr == a.matches("ABC"));
      OK(nullptr != a.matches("BBC"));
   }

   Case(Substr)
   {
      h2::h2_matches_substr a("A");
      OK(nullptr == a.matches("ABC"));
      OK(nullptr != a.matches("BBC"));
   }

   Case(StartsWith)
   {
      h2::h2_matches_startswith a("A");
      OK(nullptr == a.matches("ABC"));
      OK(nullptr != a.matches("BBC"));
   }

   Case(EndsWith)
   {
      h2::h2_matches_endswith a("A");
      OK(nullptr == a.matches("CBA"));
      OK(nullptr != a.matches("ABC"));
   }

   Case(Json)
   {
      h2::h2_matches_json a("[65]");
      OK(nullptr == a.matches("[65]"));
      OK(nullptr != a.matches("[66]"));
   }

   Case(CaseLess)
   {
      h2::h2_caseless_matches a1(h2::h2_matcher<h2::h2_string>("A"));
      OK(nullptr == a1.matches("A"));
      OK(nullptr == a1.matches("a"));
      OK("~\"A\"", a1.expects("A"));
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
      OK(!CaseLess("AbCd"), "cba");
      OK(~Substr("AbCd"), "ABCD");
      OK(~Re("A.*"), "abcdef");
      OK(~We("A*"), "abcdef");
   }
}
