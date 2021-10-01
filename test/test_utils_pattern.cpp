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
