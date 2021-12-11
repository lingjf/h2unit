#include "../source/h2_unit.cpp"

SUITE(similarity)
{
   Case(edit distance)
   {
      OK(0, h2::h2_similarity::levenshtein("a", "a", 1, 1, false));
      OK(1, h2::h2_similarity::levenshtein("a", "b", 1, 1, false));
      OK(2, h2::h2_similarity::levenshtein("a", "bc", 1, 2, false));
      OK(3, h2::h2_similarity::levenshtein("abc", "xyz", 3, 3, false));
   }

   Case(absolute match)
   {
      OK(1, h2::h2_similarity::estimate("", "", false));
      OK(1, h2::h2_similarity::estimate("a", "a", false));
      OK(1, h2::h2_similarity::estimate("ab", "ab", false));
      OK(1, h2::h2_similarity::estimate("abc", "abc", false));
   }

   Case(absolute not match)
   {
      OK(0, h2::h2_similarity::estimate("a", "b", false));
      OK(0, h2::h2_similarity::estimate("abc", "xyz", false));
   }

   Case(not match)
   {
      OK(0.75, h2::h2_similarity::estimate("abcd", "abc1", false));
      OK(0.75, h2::h2_similarity::estimate("abcd", "abce", false));
      OK(0.5, h2::h2_similarity::estimate("abcd", "ab12", false));
      OK(0.25, h2::h2_similarity::estimate("abcd", "a123", false));
      OK(0.75, h2::h2_similarity::estimate("abcd", "1bcd", false));
      OK(0.5, h2::h2_similarity::estimate("abcd", "12cd", false));
   }
}

CASE(vector reserve initialize)
{
   struct matrix {
      unsigned e : 1, p : 1, d : 6, c : 24;
      matrix() : e(1), p(1), d(5), c(55) {}
   };
   h2::h2_vector<h2::h2_vector<matrix>> m;

   m.push_back(h2::h2_vector<matrix>(3));
   for (int j = 0; j < 3; j++) {
      OK(1, m[0][j].e);
      OK(1, m[0][j].p);
      OK(5, m[0][j].d);
      OK(55, m[0][j].c);
   }
}

void __LCS_print(h2::h2_LCS& a, const char* t)
{
#define W3 " "

   for (size_t i = 0; i < a.s2.size() + 2; i++) {
      printf("%2s" W3, i < 2 ? "" : a.s2[i - 2].c_str());
   }
   printf("\n");

   for (size_t i = 0; i < a.m.size(); i++) {
      printf("%2s" W3, i == 0 ? "" : a.s1[i - 1].c_str());

      for (size_t j = 0; j < a.m[i].size(); j++) {
         if ('c' == t[0]) {
            if (i == 0 || j == 0)
               printf("\033[90m%2d\033[0m" W3, (int)a.m[i][j].c);
            else
               printf("%2d" W3, (int)a.m[i][j].c);
         } else if ('d' == t[0]) {
            if (a.m[i][j].d == 9) {
               if (a.m[i][j].p || !strchr(t, 'p'))
                  printf(" \033[32m←\033[0m" W3);
               else
                  printf(" \033[36m←\033[0m" W3);

            } else if (a.m[i][j].d == 10) {
               if (a.m[i][j].p || !strchr(t, 'p'))
                  printf(" \033[32m⬉\033[0m" W3);
               else
                  printf(" \033[36m⬉\033[0m" W3);
            } else if (a.m[i][j].d == 12) {
               if (a.m[i][j].p || !strchr(t, 'p'))
                  printf(" \033[32m↑\033[0m" W3);
               else
                  printf(" \033[36m↑\033[0m" W3);
            } else {
               printf(" \033[90m‧\033[0m" W3);
            }
         } else if ('p' == t[0]) {
            if (a.m[i][j].p)
               printf(" \033[32m*\033[0m" W3);
            else
               printf(" \033[90m‧\033[0m" W3);
         }
      }
      printf("\n");
   }
   printf("\n");
}

void LCS_print(h2::h2_LCS& a)
{
   __LCS_print(a, "c");
   __LCS_print(a, "dp");
   __LCS_print(a, "p");
}

SUITE(LCS)
{
   Case(same)
   {
      h2::h2_LCS a(h2::h2_string("abc").disperse(), h2::h2_string("abc").disperse());
      auto ret = a.lcs();
      // LCS_print(a);
      OK(Pair(ListOf(1, 1, 1), ListOf(1, 1, 1)), ret);
   }

   Case(caseless)
   {
      h2::h2_LCS a(h2::h2_string("abc").disperse(), h2::h2_string("aBC").disperse(), true);
      auto ret = a.lcs();
      // LCS_print(a);
      OK(Pair(ListOf(1, 1, 1), ListOf(1, 1, 1)), ret);
   }

   Case(utf8 中文)
   {
      h2::h2_LCS a(h2::h2_string("你好!").disperse(), h2::h2_string("您好!").disperse());
      auto ret = a.lcs();

      // LCS_print(a);
      OK(Pair(ListOf(0, 1, 1), ListOf(0, 1, 1)), ret);
   }

   Case(change char)
   {
      h2::h2_LCS a(h2::h2_string("abc").disperse(), h2::h2_string("a5c").disperse());
      auto ret = a.lcs();

      // LCS_print(a);
      OK(Pair(ListOf(1, 0, 1), ListOf(1, 0, 1)), ret);
   }

   Case(add char)
   {
      h2::h2_LCS a(h2::h2_string("abc").disperse(), h2::h2_string("abxc").disperse());
      auto ret = a.lcs();

      // LCS_print(a);
      OK(Pair(ListOf(1, 1, 1), ListOf(1, 1, 0, 1)), ret);
   }

   Case(insert word)
   {
      h2::h2_LCS a(h2::h2_string("hello world").disperse(), h2::h2_string("hello twe world").disperse());
      auto ret = a.lcs();

      // LCS_print(a);
      OK(Pair(ListOf(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1), ListOf(1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1)), ret);
   }

   Case(hello/hel1o)
   {
      h2::h2_LCS a(h2::h2_string("hello").disperse(), h2::h2_string("hel1o").disperse());
      auto ret = a.lcs();

      // LCS_print(a);
      OK(Pair(ListOf(1, 1, 1, 0, 1), ListOf(1, 1, 1, 0, 1)), ret);
   }

   Case(inside table)
   {
      h2::h2_LCS a(h2::h2_string("he1lo wor1d!").disperse(), h2::h2_string("hello the world").disperse());
      auto ret = a.lcs();

      LCS_print(a);
      OK(Pair(ListOf(1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0), ListOf(1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1)), ret);
   }
}
