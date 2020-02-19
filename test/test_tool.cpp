#include "../source/h2_unit.h"

using namespace h2;

SUITE(h2_tool)
{
   char t1[1024];
   Setup(){};

   Teardown(){};

   Case(H2PP_0ARGS)
   {
      int n0 = H2PP_0ARGS();
      int n1 = H2PP_0ARGS(a);
      int n2 = H2PP_0ARGS(a, b);
      int n3 = H2PP_0ARGS(a, b, c);

      OK(1, n0);
      OK(0, n1);
      OK(0, n2);
      OK(0, n3);
   };

   Case(H2PP_NARGS)
   {
      int n0 = H2PP_NARGS();
      int n1 = H2PP_NARGS(a);
      int n2 = H2PP_NARGS(a, b);
      int n3 = H2PP_NARGS(a, b, c);

      OK(0, n0);
      OK(1, n1);
      OK(2, n2);
      OK(3, n3);
   };

   Case(h2_style)
   {
      OK("", h2_style("", t1));
      OK("\033[31m", h2_style("red", t1));
      OK("\033[31;1m", h2_style("red,bold", t1));
   };

   Case(wildcard)
   {
      OK(!h2_wildcard_match("hello?world*", "welcome"));
      OK(h2_wildcard_match("hello?world*", "hello world"));
      OK(h2_wildcard_match("hello?world*", "hello world h2unit"));
      OK(!h2_wildcard_match("hello?world*", "one world"));
      OK(!h2_wildcard_match("hello?world*", ""));
   };

   Case(acronym)
   {
      OK("01234", h2_acronym_string("01234"));
      OK("0123456789...", h2_acronym_string("0123456789ABCDEF"));
   };

   Case(center)
   {
      OK("ab", h2_center_string("ab", 2, t1));
      OK("ab ", h2_center_string("ab", 3, t1));
      OK(" ab ", h2_center_string("ab", 4, t1));
      OK(" ab  ", h2_center_string("ab", 5, t1));
      OK("  ab  ", h2_center_string("ab", 6, t1));

      OK("abc", h2_center_string("abc", 3, t1));
      OK("abc ", h2_center_string("abc", 4, t1));
      OK(" abc ", h2_center_string("abc", 5, t1));
      OK(" abc  ", h2_center_string("abc", 6, t1));
   };
}
