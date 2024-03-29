#include "../source/h2_unit.cpp"

SUITE(strcmp matches)
{
   Case(Regex)
   {
      h2::h2_matches_regex a("A.*");
      OK(nullptr == a.matches("ABC", {}));
      OK(nullptr != a.matches("BBC", {}));
   }

   Case(Wildcard)
   {
      h2::h2_matches_wildcard a("A*");
      OK(nullptr == a.matches("ABC", {}));
      OK(nullptr != a.matches("BBC", {}));
   }

   Case(Substr)
   {
      h2::h2_matches_substr a("A");
      OK(nullptr == a.matches("ABC", {}));
      OK(nullptr != a.matches("BBC", {}));
   }

   Case(StartsWith)
   {
      h2::h2_matches_startswith a("A");
      OK(nullptr == a.matches("ABC", {}));
      OK(nullptr != a.matches("BBC", {}));
   }

   Case(EndsWith)
   {
      h2::h2_matches_endswith a("A");
      OK(nullptr == a.matches("CBA", {}));
      OK(nullptr != a.matches("ABC", {}));
   }

   Case(CaseLess)
   {
      h2::h2_caseless_matches a1(h2::h2_matcher<h2::h2_string>("A"));
      OK(nullptr == a1.matches("A", {}));
      OK(nullptr == a1.matches("a", {}));
      OK("~\"A\"", a1.expection({}).string());
   }
}

SUITE(strcmp primitive)
{
   Case(Re [re])
   {
      OK(Re("abc.*"), "abcdef");
      OK(!Re("A.*"), "abcdef");
   }

   Case(We)
   {
      OK(We("abc*"), "abcdef");
      OK(!We("abc?yz"), "abcdef");
   }
}

SUITE(strcmp CaseLess)
{
   Case(CaseLess [re])
   {
      OK(CaseLess("AbCd"), "abcd");
      OK(!CaseLess("AbCd"), "cba");
      OK(CaseLess(Substr("AbCd")), "ABCD");
      OK(CaseLess(Re("A.*")), "abcdef");
      OK(CaseLess(We("A*")), "abcdef");
   }

   Case(CaseLess ~ [re])
   {
      // OK(~"AbCd", "abcd");
      OK(~Substr("AbCd"), "ABCD");
      OK(~Re("A.*"), "abcdef");
      OK(~We("A*"), "abcdef");
   }
}

SUITE(strcmp SpaceLess)
{
   Case(implicit Eq)
   {
      OK(SpaceLess("a b c"), "a  b   c");
      OK(!SpaceLess("a bc"), "a  b   c");
      OK(SpaceLess("a b c"), " a \t \n  b      c   ");
   }

   Case(explicit strcmp [re])
   {
      OK(SpaceLess(Se("a b c")), " a \t \n  b      c   ");
      OK(*(Se("a b c")), " a \t \n  b      c   ");

      OK(SpaceLess(Substr("a b ")), " a \t \n  b      c   ");
      OK(*(Substr("a b ")), " a \t \n  b      c   ");

      OK(SpaceLess(StartsWith("a b ")), " a \t \n  b      c   ");
      OK(*(StartsWith("a b ")), " a \t \n  b      c   ");

      OK(SpaceLess(EndsWith("b c")), " a \t \n  b      c   ");
      OK(*(EndsWith("b c")), " a \t \n  b      c   ");

      OK(SpaceLess(We("? ? *")), " a \t \n  b      c   ");
      OK(*(We("? ? *")), " a \t \n  b      c   ");

      OK(SpaceLess(Re(".{1} .{1} .*")), " a \t \n  b      c   ");
      OK(*(Re(".{1} .{1} .*")), " a \t \n  b      c   ");
   }
}
