#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(Matcher)
{
   Case(Eq number)
   {
      // 65 is A
      OK(nullptr == h2::h2_matcher<int>(Eq(65)).matches(65));
      OK(nullptr == h2::h2_matcher<double>(Eq(65)).matches(65.0));
      OK(nullptr == h2::h2_matcher<float>(65.000000001).matches(65));
      OK(nullptr == h2::h2_matcher<double>(65.000000001).matches(65));
      OK(nullptr == h2::h2_matcher<char>(Eq(65)).matches('A'));
      OK(nullptr == h2::h2_matcher<unsigned long long>(Eq(65)).matches(65ULL));

      OK(nullptr != h2::h2_matcher<int>(Eq(65)).matches(66));
   }

   Case(Eq bool)
   {
      OK(nullptr == h2::h2_matcher<bool>(Eq(true)).matches(true));
      OK(nullptr == h2::h2_matcher<bool>(Eq(true)).matches(2));

      OK(nullptr != h2::h2_matcher<bool>(Eq(true)).matches(false));
   }

   Case(Eq string)
   {
      char a[1024];
      sprintf(a, "A");
      OK(nullptr == h2::h2_matcher<std::string>(Eq("A")).matches("A"));
      OK(nullptr == h2::h2_matcher<const char*>(Eq("A")).matches(a));
      OK(nullptr != h2::h2_matcher<const char*>(Eq("A")).matches("B"));

      OK(nullptr == h2::h2_matcher<const char*>("abcd").matches("abcd"));
      std::string abcd = "abcd";
      OK(nullptr == h2::h2_matcher<std::string>(abcd).matches("abcd"));
   }

   Case(CaseLess)
   {
      OK(nullptr == h2::h2_matcher<const char*>(CaseLess("AbCd")).matches("ABcd"));
      std::string AbCd = "AbCd";
      OK(nullptr == h2::h2_matcher<const char*>(CaseLess(AbCd)).matches("ABcd"));
      OK(nullptr == h2::h2_matcher<std::string>(CaseLess("ABcd")).matches(AbCd));
   }

   Case(Regex)
   {
      OK(nullptr == h2::h2_matcher<const char*>(Re("abc.*")).matches("abcdef"));
      OK(nullptr != h2::h2_matcher<const char*>(Re("A.*")).matches("abcdef"));
   }

   Case(Wildcard)
   {
      OK(nullptr == h2::h2_matcher<const char*>(We("abc*")).matches("abcdef"));
      OK(nullptr != h2::h2_matcher<const char*>(We("abc?yz")).matches("abcdef"));
   }

   Case(Substr)
   {
      OK(nullptr == h2::h2_matcher<const char*>(Substr("cd")).matches("abcdef"));
      OK(nullptr != h2::h2_matcher<const char*>(Substr("cc")).matches("abcdef"));
      std::string cd = "cd";
      OK(nullptr == h2::h2_matcher<const char*>(Substr(cd)).matches("abcdef"));
      OK(nullptr == h2::h2_matcher<const char*>(CaseLess(Substr("cd"))).matches("ABCDEF"));
   }

   Case(StartsWith)
   {
      OK(nullptr == h2::h2_matcher<const char*>(StartsWith("abc")).matches("abcdef"));
      std::string abc = "abc";
      OK(nullptr == h2::h2_matcher<const char*>(StartsWith(abc)).matches("abcdef"));
      OK(nullptr == h2::h2_matcher<const char*>(CaseLess(StartsWith("abc"))).matches("ABCDEF"));
   }

   Case(EndsWith)
   {
      OK(nullptr == h2::h2_matcher<const char*>(EndsWith("def")).matches("abcdef"));
      OK(nullptr == h2::h2_matcher<const char*>(CaseLess(EndsWith("def"))).matches("ABCDEF"));
   }

   Case(Me)
   {
      unsigned char t1[] = {1, 2, 3, 4, 5, 6, 7, 8};
      unsigned char t2[] = {1, 2, 3, 4, 5, 6, 7, 8};

      OK(nullptr == h2::h2_matcher<unsigned char*>(Me(t1, 8)).matches(t2));
   }

   Case(Pointee)
   {
      // 65 is A
      int a65 = 65;
      int a66 = 66;

      OK(nullptr == h2::h2_matcher<int*>(Pointee(65)).matches(&a65));
      OK(nullptr == h2::h2_matcher<int*>(Pointee(Gt(65))).matches(&a66));

      OK(nullptr != h2::h2_matcher<int*>(Pointee(65)).matches(&a66));
   }

   Case(Not)
   {
      // 65 is A
      OK(nullptr == h2::h2_matcher<int>(Not(65)).matches(11));
      OK(nullptr == h2::h2_matcher<int>(Not(Gt(65))).matches(11));
      OK(nullptr == h2::h2_matcher<const char*>(Not(Lt("A"))).matches("B"));

      OK(nullptr != h2::h2_matcher<int>(Not(65)).matches(65));
   }

   Case(AllOf)
   {
      // (65, 75)
      OK(nullptr == h2::h2_matcher<int>(AllOf(Gt(65), Lt(75))).matches(66));

      OK(nullptr != h2::h2_matcher<int>(AllOf(Gt(65), Lt(75))).matches(11));
   }

   Case(AnyOf)
   {
      // (65, 75)
      OK(nullptr == h2::h2_matcher<int>(AnyOf(Gt(65), Lt(75))).matches(66));
      OK(nullptr == h2::h2_matcher<int>(AnyOf(Gt(65), Lt(55))).matches(66));

      OK(nullptr != h2::h2_matcher<int>(AnyOf(Gt(65), Lt(55))).matches(60));
   }

   Case(NoneOf)
   {
      // <65, 75<
      OK(nullptr == h2::h2_matcher<int>(NoneOf(Lt(65), Gt(75))).matches(70));

      OK(nullptr != h2::h2_matcher<int>(NoneOf(Lt(65), Gt(75))).matches(60));
   }

   Case(ListOf)
   {
      int a[] = {1, 3, 5};
      OK(nullptr == h2::h2_matcher<int*>(ListOf(1, 3, 5)).matches(a));

      std::vector<int> b = {1, 3, 5};
      OK(nullptr == h2::h2_matcher<std::vector<int>>(ListOf(1, 3, 5)).matches(b));
   }
}

namespace {

int foobar(int a, const char* b)
{
   return 100;
}

CASE(Any matcher{})
{
   MOCK(foobar, int(int, const char*)).Once({}, {}).Return(11);
   OK(11, foobar(1, "A"));
}

}  // namespace
