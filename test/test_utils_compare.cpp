#include "../source/h2_unit.cpp"

SUITE(pattern match)
{
   Case(wildcard)
   {
      OK(!h2::h2_pattern::wildcard_match("hello?world*", "welcome"));
      OK(h2::h2_pattern::wildcard_match("hello?world*", "hello world"));
      OK(h2::h2_pattern::wildcard_match("hello?world*", "hello world h2unit"));
      OK(!h2::h2_pattern::wildcard_match("hello?world*", "one world"));
      OK(!h2::h2_pattern::wildcard_match("hello?world*", ""));
   }

   Case(wildcard bug)
   {
      OK(h2::h2_pattern::wildcard_match("a*1", "a*121", true));
   }

   Case(wildcard caseless)
   {
      OK(h2::h2_pattern::wildcard_match("hello?world*", "Hello World", true));
   }

   Case(regex)
   {
      const char* subject = "abcdef";
      OK(h2::h2_pattern::regex_match("a.*", subject));
      OK(h2::h2_pattern::regex_match("A.*", subject, true));
   }
}

SUITE(fuzzy match)
{
   Case(absolute match)
   {
      OK(1, h2::h2_fuzzy::likes("", ""));
      OK(1, h2::h2_fuzzy::likes("a", "a"));
      OK(1, h2::h2_fuzzy::likes("ab", "ab"));
      OK(1, h2::h2_fuzzy::likes("abc", "abc"));
   }

   Case(absolute not match)
   {
      OK(0, h2::h2_fuzzy::likes("a", "b"));
      OK(0, h2::h2_fuzzy::likes("abc", "xyz"));
   }

   Case(not match)
   {
      double abcd_abc1 = h2::h2_fuzzy::likes("abcd", "abc1");
      double abcd_abce = h2::h2_fuzzy::likes("abcd", "abce");
      double abcd_ab12 = h2::h2_fuzzy::likes("abcd", "ab12");
      double abcd_a123 = h2::h2_fuzzy::likes("abcd", "a123");
      double abcd_1bcd = h2::h2_fuzzy::likes("abcd", "1bcd");
      double abcd_12cd = h2::h2_fuzzy::likes("abcd", "12cd");

      // OK(-1, abcd_abc1);
      // OK(-1, abcd_abce);
      // OK(-1, abcd_ab12);
      // OK(-1, abcd_a123);
      // OK(-1, abcd_1bcd);
      // OK(-1, abcd_12cd);
      OK(abcd_abc1 == abcd_abce);
      OK(abcd_ab12 > abcd_a123);
   }

   Case(edit distance)
   {
      OK(1, h2::h2_fuzzy::levenshtein("a", "b", 1, 1));
      OK(2, h2::h2_fuzzy::levenshtein("a", "bc", 1, 2));
      OK(3, h2::h2_fuzzy::levenshtein("abc", "xyz", 3, 3));
   }
}
