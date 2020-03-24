#include "../source/h2_unit.hpp"
#include "../source/h2_unit.cpp"

SUITE(h2_tool)
{
   char t1[1024];

   Case(h2_style)
   {
      OK("", h2::h2_style("", t1));
      OK("\033[31m", h2::h2_style("red", t1));
      OK("\033[31;1m", h2::h2_style("red,bold", t1));
   };

   Case(wildcard)
   {
      OK(!h2::h2_wildcard_match("hello?world*", "welcome"));
      OK(h2::h2_wildcard_match("hello?world*", "hello world"));
      OK(h2::h2_wildcard_match("hello?world*", "hello world h2unit"));
      OK(!h2::h2_wildcard_match("hello?world*", "one world"));
      OK(!h2::h2_wildcard_match("hello?world*", ""));
   };
}
