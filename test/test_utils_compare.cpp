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
