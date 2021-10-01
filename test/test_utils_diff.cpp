#include "../source/h2_unit.cpp"

SUITE(fuzzy match)
{
   Case(edit distance)
   {
      OK(0, h2::h2_fuzzy::levenshtein("a", "a", 1, 1, false));
      OK(1, h2::h2_fuzzy::levenshtein("a", "b", 1, 1, false));
      OK(2, h2::h2_fuzzy::levenshtein("a", "bc", 1, 2, false));
      OK(3, h2::h2_fuzzy::levenshtein("abc", "xyz", 3, 3, false));
   }

   Case(absolute match)
   {
      OK(1, h2::h2_fuzzy::similarity("", "", false));
      OK(1, h2::h2_fuzzy::similarity("a", "a", false));
      OK(1, h2::h2_fuzzy::similarity("ab", "ab", false));
      OK(1, h2::h2_fuzzy::similarity("abc", "abc", false));
   }

   Case(absolute not match)
   {
      OK(0, h2::h2_fuzzy::similarity("a", "b", false));
      OK(0, h2::h2_fuzzy::similarity("abc", "xyz", false));
   }

   Case(not match)
   {
      OK(0.75, h2::h2_fuzzy::similarity("abcd", "abc1", false));
      OK(0.75, h2::h2_fuzzy::similarity("abcd", "abce", false));
      OK(0.5, h2::h2_fuzzy::similarity("abcd", "ab12", false));
      OK(0.25, h2::h2_fuzzy::similarity("abcd", "a123", false));
      OK(0.75, h2::h2_fuzzy::similarity("abcd", "1bcd", false));
      OK(0.5, h2::h2_fuzzy::similarity("abcd", "12cd", false));
   }
}

void LCS_print(h2::h2_LCS& a, h2::h2_vector<h2::h2_vector<int>>& z)
{
   for (size_t i = 0; i < a.s2.size() + 2; i++) {
      printf("%2s ", i < 2 ? "" : a.s2[i - 2].c_str());
   }
   printf("\n");

   for (size_t i = 0; i < z.size() + 1; i++) {
      printf("%2s ", i == 0 ? "" : a.s1[i - 1].c_str());

      for (auto& j : z[i]) {
         if (j == 900)
            printf(" ⇐ ");
         else if (j == 1030)
            printf(" ⇖ ");
         else if (j == 1200)
            printf(" ⇑ ");
         else
            printf("%2d ", j);
      }
      printf("\n");
   }
   printf("\n");
}

SUITE(LCS)
{
   Case(same)
   {
      h2::h2_LCS a(h2::h2_string("abc").disperse(), h2::h2_string("abc").disperse());
      auto ret = a.lcs();
      // LCS_print(a, a.c);
      // LCS_print(a, a.d);
      // LCS_print(a, a.p);
      OK(Pair(ListOf(1, 1, 1), ListOf(1, 1, 1)), ret);
   }

   Case(caseless)
   {
      h2::h2_LCS a(h2::h2_string("abc").disperse(), h2::h2_string("aBC").disperse(), true);
      auto ret = a.lcs();
      // LCS_print(a, a.c);
      // LCS_print(a, a.d);
      // LCS_print(a, a.p);
      OK(Pair(ListOf(1, 1, 1), ListOf(1, 1, 1)), ret);
   }

   Case(utf8 中文)
   {
      h2::h2_LCS a(h2::h2_string("你好").disperse(), h2::h2_string("你好").disperse());
      auto ret = a.lcs();

      // LCS_print(a, a.c);
      // LCS_print(a, a.d);
      // LCS_print(a, a.p);
      OK(Pair(ListOf(1, 1), ListOf(1, 1)), ret);
   }

   Case(change)
   {
      h2::h2_LCS a(h2::h2_string("abc").disperse(), h2::h2_string("a5c").disperse());
      auto ret = a.lcs();

      // LCS_print(a, a.c);
      // LCS_print(a, a.d);
      // LCS_print(a, a.p);
      OK(Pair(ListOf(1, 0, 1), ListOf(1, 0, 1)), ret);
   }

   Case(add)
   {
      h2::h2_LCS a(h2::h2_string("abc").disperse(), h2::h2_string("abxc").disperse());
      auto ret = a.lcs();

      // LCS_print(a, a.c);
      // LCS_print(a, a.d);
      // LCS_print(a, a.p);
      OK(Pair(ListOf(1, 1, 1), ListOf(1, 1, 0, 1)), ret);
   }

   Case(hello world)
   {
      h2::h2_LCS a(h2::h2_string("hello world").disperse(), h2::h2_string("hello twe world").disperse());
      auto ret = a.lcs();

      // LCS_print(a, a.c);
      // LCS_print(a, a.d);
      // LCS_print(a, a.p);
      OK(Pair(ListOf(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1), ListOf(1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1)), ret);
   }
}
