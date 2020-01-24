#include "../source/h2_unit.h"
using namespace h2;

SUITE(eq_matches)
{
   Case(Integer) {
      h2_eq_matches<int> a1(123);
      OK(nullptr == a1.matches(123));
      h2_eq_matches<const int> a2(123);
      OK(nullptr == a2.matches(123));
      h2_eq_matches<unsigned long long> a3(123);
      OK(nullptr == a3.matches(123));

      OK(Eq(123), 123);
      short b1 = 123;
      OK(Eq(b1), 123);
      const long int b2 = 123;
      OK(Eq(b2), 123);
   };

   Case(Boolean) {
      h2_eq_matches<bool> a1(false);
      OK(nullptr == a1.matches(false));
      h2_eq_matches<const bool> a2(false);
      OK(nullptr == a2.matches(false));

      const bool b2 = true;
      OK(Eq(b2), true);
   };

   Case(Float) {
      h2_eq_matches<float> a1(65.000000001);
      OK(nullptr == a1.matches(65));
      h2_eq_matches<double> a2(65.000000001);
      OK(nullptr == a2.matches(65));

      const float b1 = 65.000000001;
      OK(Eq(b1), 65);
      const double b2 = 65.000000001;
      OK(Eq(b2), 65);
      const long double b3 = 65.000000001;
      OK(Eq(b3), 65);
   };

   Case(String) {
      h2_eq_matches<char*> a1("abc");
      OK(nullptr == a1.matches("abc"));
      h2_eq_matches<h2_string> a2("abc");
      OK(nullptr == a2.matches("abc"));
      h2_eq_matches<std::string> a3("abc");
      OK(nullptr == a3.matches("abc"));

      OK(Eq("abc"), "abc");
      const char* b1 = "abc";
      OK(Eq(b1), "abc");
      const h2_string b2 = "abc";
      OK(Eq(b2), "abc");
      const std::string b3 = "abc";
      OK(Eq(b3), "abc");
   };
}

SUITE(CaseLess)
{
   Case(direct)
   {
      OK(CaseLess("AbCd"), "ABcd");
   };
   Case("Eq")
   {
      OK(CaseLess(Eq("AbCd")), "ABcd");
   };
}

SUITE(matcher_Eq)
{
   Setup(){};

   Teardown(){};

   Case(Eq number)
   {
      // 65 is A
      OK(nullptr == h2_matcher<int>(Eq(65)).matches(65));
      OK(nullptr == h2_matcher<double>(Eq(65)).matches(65.0));
      OK(nullptr == h2_matcher<float>(65.000000001).matches(65));
      OK(nullptr == h2_matcher<double>(65.000000001).matches(65));
      OK(nullptr == h2_matcher<char>(Eq(65)).matches('A'));
      OK(nullptr == h2_matcher<unsigned long long>(Eq(65)).matches(65ULL));

      OK(nullptr != h2_matcher<int>(Eq(65)).matches(66));
   };

   Case(Eq bool)
   {
      OK(nullptr == h2_matcher<bool>(Eq(true)).matches(true));
      OK(nullptr == h2_matcher<bool>(Eq(true)).matches(2));

      OK(nullptr != h2_matcher<bool>(Eq(true)).matches(false));
   };

   Case(Eq string)
   {
      char a[1024];
      sprintf(a, "A");
      OK(nullptr == h2_matcher<std::string>(Eq("A")).matches("A"));
      OK(nullptr == h2_matcher<const char*>(Eq("A")).matches(a));
      OK(nullptr != h2_matcher<const char*>(Eq("A")).matches("B"));
   };
}

SUITE(matcher_Me)
{
   Setup(){};

   Teardown(){};

   Case(Me Memory buffer)
   {
      unsigned char t1[] = {1, 2, 3, 4, 5, 6, 7, 8};
      unsigned char t2[] = {1, 2, 3, 4, 5, 6, 7, 8};

      OK(nullptr == h2_matcher<unsigned char*>(Me(t1, 8)).matches(t2));
   };
}

SUITE(matcher_Pe_Pointee)
{
   Setup(){};

   Teardown(){};

   Case(success)
   {
      // 65 is A
      int a65 = 65;
      int a66 = 66;

      OK(nullptr == h2_matcher<int*>(Pe(65)).matches(&a65));
      OK(nullptr == h2_matcher<int*>(Pe(Gt(65))).matches(&a66));

      OK(nullptr != h2_matcher<int*>(Pe(65)).matches(&a66));
   };
}

SUITE(matcher_Logic)
{
   Setup(){};

   Teardown(){};

   Case(Not)
   {
      // 65 is A
      OK(nullptr == h2_matcher<int>(Not(65)).matches(11));
      OK(nullptr == h2_matcher<int>(Not(Gt(65))).matches(11));
      OK(nullptr == h2_matcher<const char*>(Not(Lt("A"))).matches("B"));

      OK(nullptr != h2_matcher<int>(Not(65)).matches(65));

      OK(Not(65), 11);
      OK(Not("abc"), "xyz");
      const std::string b3 = "abc";
      OK(Not(b3), "xyz");
   };

   Case(AllOf)
   {
      // (65, 75)
      OK(nullptr == h2_matcher<int>(AllOf(Gt(65), Lt(75))).matches(66));

      OK(nullptr != h2_matcher<int>(AllOf(Gt(65), Lt(75))).matches(11));
   };

   Case(AnyOf)
   {
      // (65, 75)
      OK(nullptr == h2_matcher<int>(AnyOf(Gt(65), Lt(75))).matches(66));
      OK(nullptr == h2_matcher<int>(AnyOf(Gt(65), Lt(55))).matches(66));

      OK(nullptr != h2_matcher<int>(AnyOf(Gt(65), Lt(55))).matches(60));
   };

   Case(NoneOf)
   {
      // <65, 75<
      OK(nullptr == h2_matcher<int>(NoneOf(Lt(65), Gt(75))).matches(70));

      OK(nullptr != h2_matcher<int>(NoneOf(Lt(65), Gt(75))).matches(60));
   };

   Case(ListOf)
   {
      int a[] = {1, 3, 5};
      OK(nullptr == h2_matcher<int*>(ListOf(1, 3, 5)).matches(a));

      std::vector<int> b = {1, 3, 5};
      OK(nullptr == h2_matcher<std::vector<int>>(ListOf(1, 3, 5)).matches(b));
   };
}

SUITE(matcher_String)
{
   Setup(){};

   Teardown(){};

   Case(Eq)
   {
      OK(nullptr == h2_matcher<const char*>("abcd").matches("abcd"));
      OK(nullptr == h2_matcher<const char*>(CaseLess("AbCd")).matches("ABcd"));
      std::string abcd = "abcd";
      OK(nullptr == h2_matcher<std::string>(abcd).matches("abcd"));

      OK(nullptr != h2_matcher<const char*>("abcd").matches("abcD"));
      OK(nullptr != h2_matcher<const char*>(CaseLess("AbCd")).matches("ABcX"));
   };

   Case(CaseLess)
   {
      OK(nullptr == h2_matcher<const char*>(CaseLess("AbCd")).matches("ABcd"));
      std::string AbCd = "AbCd";
      OK(nullptr == h2_matcher<const char*>(CaseLess(AbCd)).matches("ABcd"));
      OK(nullptr == h2_matcher<std::string>(CaseLess("ABcd")).matches(AbCd));
   };

   Case(Regex)
   {
      OK(nullptr == h2_matcher<const char*>(Re("abc.*")).matches("abcdef"));
      OK(nullptr != h2_matcher<const char*>(Re("A.*")).matches("abcdef"));
   };

   Case(Wildcard)
   {
      OK(nullptr == h2_matcher<const char*>(We("abc*")).matches("abcdef"));
      OK(nullptr != h2_matcher<const char*>(We("abc?yz")).matches("abcdef"));
   };

   Case(Contains)
   {
      OK(nullptr == h2_matcher<const char*>(Contains("cd")).matches("abcdef"));
      OK(nullptr != h2_matcher<const char*>(Contains("cc")).matches("abcdef"));
      std::string cd = "cd";
      OK(nullptr == h2_matcher<const char*>(Contains(cd)).matches("abcdef"));
      OK(nullptr == h2_matcher<const char*>(CaseLess(Contains("cd"))).matches("ABCDEF"));
   };

   Case(StartsWith)
   {
      OK(nullptr == h2_matcher<const char*>(StartsWith("abc")).matches("abcdef"));
      std::string abc = "abc";
      OK(nullptr == h2_matcher<const char*>(StartsWith(abc)).matches("abcdef"));
      OK(nullptr == h2_matcher<const char*>(CaseLess(StartsWith("abc"))).matches("ABCDEF"));
   };

   Case(EndsWith)
   {
      OK(nullptr == h2_matcher<const char*>(EndsWith("def")).matches("abcdef"));
      OK(nullptr == h2_matcher<const char*>(CaseLess(EndsWith("def"))).matches("ABCDEF"));
   };
}

SUITE(std string)
{
   Case(Eq)
   {
      std::string a = "123";
      OK("123", a);
      OK(a, "123");
   };
}
