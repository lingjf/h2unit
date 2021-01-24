#include "../source/h2_unit.cpp"

SUITE(tool)
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

   Case(once)
   {
      h2::h2_once once;
      OK(once);
      OK(!once);
      OK(!once);
   }
}

CASE(H2Foreach)
{
   int s = 0;
#define FOO(x) s += x;
   H2Foreach(FOO, 1, 2, 3)
#undef FOO
     OK(6, s);
}

CASE(H2Fullmesh)
{
   int s = 0;
#define FOO(x, y) s += x * y;
   H2Fullmesh(FOO, 1, 2, 3);
#undef FOO
   OK(1 * 1 + 1 * 2 + 1 * 3 + 2 * 1 + 2 * 2 + 2 * 3 + 3 * 1 + 3 * 2 + 3 * 3, s);
}

#define BAR(x, y) \
   CASE(generator x y) {}

H2Fullmesh(BAR, (A, B, C), (1, 2, 3))
#undef BAR
