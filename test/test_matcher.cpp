#include "../source/h2_unit.h"

H2UNIT (h2_matcher_Eq) {
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(h2_matcher_Eq, Eq number)
{
   // 65 is A
   H2EQ(nullptr == h2_matcher<int>(Eq(65)).matches(65));
   H2EQ(nullptr == h2_matcher<double>(Eq(65)).matches(65.0));
   H2EQ(nullptr == h2_matcher<float>(65.000000001).matches(65));
   H2EQ(nullptr == h2_matcher<double>(65.000000001).matches(65));
   H2EQ(nullptr == h2_matcher<char>(Eq(65)).matches('A'));
   H2EQ(nullptr == h2_matcher<unsigned long long>(Eq(65)).matches(65ULL));

   H2EQ(nullptr != h2_matcher<int>(Eq(65)).matches(66));
}

H2CASE(h2_matcher_Eq, Eq bool)
{
   H2EQ(nullptr == h2_matcher<bool>(Eq(true)).matches(true));
   H2EQ(nullptr == h2_matcher<bool>(Eq(true)).matches(2));

   H2EQ(nullptr != h2_matcher<bool>(Eq(true)).matches(false));
}

H2CASE(h2_matcher_Eq, Eq string)
{
   char a[1024];
   sprintf(a, "A");
   H2EQ(nullptr == h2_matcher<std::string>(Eq("A")).matches("A"));
   H2EQ(nullptr == h2_matcher<const char*>(Eq("A")).matches(a));
   H2EQ(nullptr != h2_matcher<const char*>(Eq("A")).matches("B"));
}

H2UNIT (h2_matcher_Me) {
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(h2_matcher_Me, Me Memory buffer)
{
   unsigned char t1[] = {1, 2, 3, 4, 5, 6, 7, 8};
   unsigned char t2[] = {1, 2, 3, 4, 5, 6, 7, 8};

   H2EQ(nullptr == h2_matcher<unsigned char*>(Me(t1, 8)).matches(t2));
}

H2UNIT (h2_matcher_Pe_Pointee) {
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(h2_matcher_Pe_Pointee, success)
{
   // 65 is A
   int a65 = 65;
   int a66 = 66;

   H2EQ(nullptr == h2_matcher<int*>(Pe(65)).matches(&a65));
   H2EQ(nullptr == h2_matcher<int*>(Pe(Gt(65))).matches(&a66));

   H2EQ(nullptr != h2_matcher<int*>(Pe(65)).matches(&a66));
}

H2UNIT (h2_matcher_Logic) {
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(h2_matcher_Logic, Not success)
{
   // 65 is A
   H2EQ(nullptr == h2_matcher<int>(Not(65)).matches(11));
   H2EQ(nullptr == h2_matcher<int>(Not(Gt(65))).matches(11));
   H2EQ(nullptr == h2_matcher<const char*>(Not(Lt("A"))).matches("B"));

   H2EQ(nullptr != h2_matcher<int>(Not(65)).matches(65));
}

H2CASE(h2_matcher_Logic, AllOf)
{
   // (65, 75)
   H2EQ(nullptr == h2_matcher<int>(AllOf(Gt(65), Lt(75))).matches(66));

   H2EQ(nullptr != h2_matcher<int>(AllOf(Gt(65), Lt(75))).matches(11));
}

H2CASE(h2_matcher_Logic, AnyOf)
{
   // (65, 75)
   H2EQ(nullptr == h2_matcher<int>(AnyOf(Gt(65), Lt(75))).matches(66));
   H2EQ(nullptr == h2_matcher<int>(AnyOf(Gt(65), Lt(55))).matches(66));

   H2EQ(nullptr != h2_matcher<int>(AnyOf(Gt(65), Lt(55))).matches(60));
}

H2CASE(h2_matcher_Logic, NoneOf)
{
   // <65, 75<
   H2EQ(nullptr == h2_matcher<int>(NoneOf(Lt(65), Gt(75))).matches(70));

   H2EQ(nullptr != h2_matcher<int>(NoneOf(Lt(65), Gt(75))).matches(60));
}

H2CASE(h2_matcher_Logic, ListOf)
{
   int a[] = {1, 3, 5};
   H2EQ(nullptr == h2_matcher<int*>(ListOf(1, 3, 5)).matches(a));

   std::vector<int> b = {1, 3, 5};
   H2EQ(nullptr == h2_matcher<std::vector<int>>(ListOf(1, 3, 5)).matches(b));
}

H2UNIT (h2_matcher_String) {
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(h2_matcher_String, Eq)
{
   H2EQ(nullptr == h2_matcher<const char*>("abcd").matches("abcd"));
   H2EQ(nullptr == h2_matcher<const char*>(CaseLess("AbCd")).matches("ABcd"));

   H2EQ(nullptr != h2_matcher<const char*>("abcd").matches("abcD"));
   H2EQ(nullptr != h2_matcher<const char*>(CaseLess("AbCd")).matches("ABcX"));
}

H2CASE(h2_matcher_String, CaseLess)
{
   H2EQ(nullptr == h2_matcher<const char*>(CaseLess("AbCd")).matches("ABcd"));
}

H2CASE(h2_matcher_String, Regex)
{
   H2EQ(nullptr == h2_matcher<const char*>(Re("abc.*")).matches("abcdef"));
   H2EQ(nullptr != h2_matcher<const char*>(Re("A.*")).matches("abcdef"));
}

H2CASE(h2_matcher_String, Wildcard)
{
   H2EQ(nullptr == h2_matcher<const char*>(We("abc*")).matches("abcdef"));
   H2EQ(nullptr != h2_matcher<const char*>(We("abc?yz")).matches("abcdef"));
}

H2CASE(h2_matcher_String, HasSubstr)
{
   H2EQ(nullptr == h2_matcher<const char*>(HasSubstr("cd")).matches("abcdef"));
   H2EQ(nullptr == h2_matcher<const char*>(CaseLess(HasSubstr("cd"))).matches("ABCDEF"));
}

H2CASE(h2_matcher_String, StartsWith)
{
   H2EQ(nullptr == h2_matcher<const char*>(StartsWith("abc")).matches("abcdef"));
   H2EQ(nullptr == h2_matcher<const char*>(CaseLess(StartsWith("abc"))).matches("ABCDEF"));
}

H2CASE(h2_matcher_String, EndsWith)
{
   H2EQ(nullptr == h2_matcher<const char*>(EndsWith("def")).matches("abcdef"));
   H2EQ(nullptr == h2_matcher<const char*>(CaseLess(EndsWith("def"))).matches("ABCDEF"));
}

H2UNIT (h2_matcher_lambda) {
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(h2_matcher_lambda, Eq)
{
   H2EQ(nullptr == h2_matcher<const char*>([](const char* a) -> bool {
                      return strcmp("abcdef", a) == 0;
                   })
                     .matches("abcdef"));
}
