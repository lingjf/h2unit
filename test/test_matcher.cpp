#include "../source/h2_unit.cpp"

SUITE(Matches)
{
  Case(Equal)
  {
    h2::h2_equal_matches<int> a1(123);
    OK(nullptr == a1.matches(123));
    h2::h2_equal_matches<const int> a2(123);
    OK(nullptr == a2.matches(123));
    h2::h2_equal_matches<unsigned long long> a3(123);
    OK(nullptr == a3.matches(123));

    h2::h2_equal_matches<bool> b1(false);
    OK(nullptr == b1.matches(false));
    h2::h2_equal_matches<const bool> b2(false);
    OK(nullptr == b2.matches(false));

    h2::h2_equal_matches<float> c1(65.000000001);
    OK(nullptr == c1.matches(65));
    h2::h2_equal_matches<double> c2(65.000000001);
    OK(nullptr == c2.matches(65));

    h2::h2_equal_matches<char*> d1("abc");
    OK(nullptr == d1.matches("abc"));
    h2::h2_equal_matches<h2::h2_string> d2("abc");
    OK(nullptr == d2.matches("abc"));
    h2::h2_equal_matches<std::string> d3("abc");
    OK(nullptr == d3.matches("abc"));

    h2::h2_equal_matches<char*> d4("*bc");
    OK(nullptr == d4.matches("abc"));

    h2::h2_equal_matches<char*> d5(".*bc");
    OK(nullptr == d5.matches("abc"));
  }

  Case(Any)
  {
    h2::h2_any_matches a;
    OK(nullptr == a.matches(65));
    OK(nullptr == a.matches(65.000000001));
    OK(nullptr == a.matches(true));
    OK(nullptr == a.matches("abc"));
    OK(nullptr == a.matches(NULL));
    OK(nullptr == a.matches(nullptr));
  }

  Case(Null)
  {
    int int65 = 65;

    h2::h2_null_matches a;
    OK(nullptr == a.matches(NULL));
    OK(nullptr == a.matches(nullptr));
    OK(nullptr != a.matches(&int65));

    h2::h2_null_matches _a(true);
    OK(nullptr == _a.matches(&int65));
    OK(nullptr != _a.matches(nullptr));
  }

  Case(Boolean)
  {
    h2::h2_boolean_matches istrue(true);
    OK(nullptr == istrue.matches(true));
    OK(nullptr != istrue.matches(false));

    h2::h2_boolean_matches isfalse(false);
    OK(nullptr == isfalse.matches(false));
    OK(nullptr != isfalse.matches(true));
  }

  Case(ge, gt, le, lt)
  {
    h2::h2_ge_matches<int> ge1(0);
    OK(nullptr == ge1.matches(1));
    OK(nullptr == ge1.matches(0));
    OK(nullptr != ge1.matches(-1));

    h2::h2_gt_matches<int> gt1(0);
    OK(nullptr == gt1.matches(1));
    OK(nullptr != gt1.matches(0));
    OK(nullptr != gt1.matches(-1));

    h2::h2_le_matches<int> le1(0);
    OK(nullptr != le1.matches(1));
    OK(nullptr == le1.matches(0));
    OK(nullptr == le1.matches(-1));

    h2::h2_lt_matches<int> lt1(0);
    OK(nullptr != lt1.matches(1));
    OK(nullptr != lt1.matches(0));
    OK(nullptr == lt1.matches(-1));
  }

  Case(memcmp)
  {
    unsigned char t1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    unsigned char t2[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    unsigned char t3[] = { 5, 5, 5, 5, 5, 5, 5, 5 };

    h2::h2_memcmp_matches a(t1, 8);
    OK(nullptr == a.matches(t2));
    OK(nullptr != a.matches(t3));
  }

  Case(Regex)
  {
    h2::h2_regex_matches a("A.*");
    OK(nullptr == a.matches("ABC"));
    OK(nullptr != a.matches("BBC"));
  }

  Case(Wildcard)
  {
    h2::h2_wildcard_matches a("A*");
    OK(nullptr == a.matches("ABC"));
    OK(nullptr != a.matches("BBC"));
  }

  Case(Contains)
  {
    h2::h2_contains_matches a("A");
    OK(nullptr == a.matches("ABC"));
    OK(nullptr != a.matches("BBC"));
  }

  Case(StartsWith)
  {
    h2::h2_startswith_matches a("A");
    OK(nullptr == a.matches("ABC"));
    OK(nullptr != a.matches("BBC"));
  }

  Case(EndsWith)
  {
    h2::h2_endswith_matches a("A");
    OK(nullptr == a.matches("CBA"));
    OK(nullptr != a.matches("ABC"));
  }

  Case(Json)
  {
    h2::h2_json_matches a("[65]");
    OK(nullptr == a.matches("[65]"));
    OK(nullptr != a.matches("[66]"));
  }
}

SUITE(Polymorphic Matcher)
{
  h2::h2_equal_matches<int> a__(65);
  h2::h2_polymorphic_matcher<h2::h2_equal_matches<int>> a(a__);

  Case(int)
  {
    h2::h2_matcher<int> b(a);
    OK(nullptr == b.matches(65));
  }

  Case(reverse)
  {
    h2::h2_matcher<int> b(!a);
    OK(nullptr != b.matches(65));
  }
}

SUITE(Combine Matches)
{
  int int65 = 65, int66 = 66;
  Case(Pointee)
  {
    h2::h2_pointee_matches<
      h2::h2_polymorphic_matcher<h2::h2_equal_matches<int>>>
      a(h2::h2_polymorphic_matcher<h2::h2_equal_matches<int>>{
        h2::h2_equal_matches<int>(65) });

    OK(nullptr == a.matches(&int65));
    OK(nullptr != a.matches(&int66));
  }

  Case(CaseLess)
  {
    h2::h2_caseless_matches a(h2::h2_matcher<h2::h2_string>("A"));
    OK(nullptr == a.matches("A"));
    OK(nullptr == a.matches("a"));
  }

  Case(Not)
  {
    h2::h2_not_matches<h2::h2_polymorphic_matcher<h2::h2_equal_matches<int>>>
      a1(h2::h2_polymorphic_matcher<h2::h2_equal_matches<int>>{
        h2::h2_equal_matches<int>(65) });
    OK(nullptr != a1.matches(65));
    OK(nullptr == a1.matches(66));

    h2::h2_not_matches<h2::h2_polymorphic_matcher<h2::h2_null_matches>> a2(
      h2::h2_polymorphic_matcher<h2::h2_null_matches>{ h2::h2_null_matches() });
    OK(nullptr != a2.matches(nullptr));
    OK(nullptr == a2.matches(&int65));

    h2::h2_not_matches<h2::h2_polymorphic_matcher<h2::h2_contains_matches>> a3(
      h2::h2_polymorphic_matcher<h2::h2_contains_matches>{
        h2::h2_contains_matches("A") });
    OK(nullptr != a3.matches("ABC"));
    OK(nullptr == a3.matches("BBC"));
  }

  Case(AllOf)
  {
    using T1 = h2::h2_polymorphic_matcher<h2::h2_null_matches>;
    using T2 = h2::h2_polymorphic_matcher<h2::h2_any_matches>;
    h2::h2_allof_matches<T1, T2> a(T1{ h2::h2_null_matches() },
                                   T2{ h2::h2_any_matches() });
    OK(nullptr == a.matches(nullptr));
  }

  Case(AnyOf)
  {
    using T1 = h2::h2_polymorphic_matcher<h2::h2_null_matches>;
    using T2 = h2::h2_polymorphic_matcher<h2::h2_any_matches>;
    h2::h2_anyof_matches<T1, T2> a(T1{ h2::h2_null_matches() },
                                   T2{ h2::h2_any_matches() });
    OK(nullptr == a.matches(nullptr));
    OK(nullptr == a.matches(NULL));
  }

  Case(NoneOf)
  {
    using T1 = h2::h2_polymorphic_matcher<h2::h2_null_matches>;
    using T2 =
      h2::h2_polymorphic_matcher<h2::h2_pointee_matches<h2::h2_matcher<int>>>;
    h2::h2_pointee_matches<h2::h2_matcher<int>> a_(h2::h2_matcher<int>(66));
    h2::h2_noneof_matches<T1, T2> a(T1{ h2::h2_null_matches() }, T2(a_));
    OK(nullptr == a.matches(&int65));
  }

  Case(ListOf)
  {
    using T1 = h2::h2_polymorphic_matcher<h2::h2_equal_matches<int>>;

    T1 a1_ = T1(h2::h2_equal_matches<int>(1));
    T1 a2_ = T1(h2::h2_equal_matches<int>(2));
    T1 a3_ = T1(h2::h2_equal_matches<int>(3));

    h2::h2_listof_matches<T1, T1, T1> a(a1_, a2_, a3_);

    int b1[] = { 1, 2, 3 };
    OK(nullptr == a.matches(b1));

    std::vector<int> b2 = { 1, 2, 3 };
    OK(nullptr == a.matches(b2));
  }
}

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
    OK(nullptr ==
       h2::h2_matcher<const char*>(CaseLess("AbCd")).matches("ABcd"));
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

  Case(Contains)
  {
    OK(nullptr ==
       h2::h2_matcher<const char*>(Contains("cd")).matches("abcdef"));
    OK(nullptr !=
       h2::h2_matcher<const char*>(Contains("cc")).matches("abcdef"));
    std::string cd = "cd";
    OK(nullptr == h2::h2_matcher<const char*>(Contains(cd)).matches("abcdef"));
    OK(nullptr ==
       h2::h2_matcher<const char*>(CaseLess(Contains("cd"))).matches("ABCDEF"));
  }

  Case(StartsWith)
  {
    OK(nullptr ==
       h2::h2_matcher<const char*>(StartsWith("abc")).matches("abcdef"));
    std::string abc = "abc";
    OK(nullptr ==
       h2::h2_matcher<const char*>(StartsWith(abc)).matches("abcdef"));
    OK(nullptr == h2::h2_matcher<const char*>(CaseLess(StartsWith("abc")))
                    .matches("ABCDEF"));
  }

  Case(EndsWith)
  {
    OK(nullptr ==
       h2::h2_matcher<const char*>(EndsWith("def")).matches("abcdef"));
    OK(
      nullptr ==
      h2::h2_matcher<const char*>(CaseLess(EndsWith("def"))).matches("ABCDEF"));
  }

  Case(Me)
  {
    unsigned char t1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    unsigned char t2[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

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
    int a[] = { 1, 3, 5 };
    OK(nullptr == h2::h2_matcher<int*>(ListOf(1, 3, 5)).matches(a));

    std::vector<int> b = { 1, 3, 5 };
    OK(nullptr == h2::h2_matcher<std::vector<int>>(ListOf(1, 3, 5)).matches(b));
  }
}
