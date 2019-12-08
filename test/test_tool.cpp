#include "../source/h2_unit.h"

H2UNIT (h2_tool) {
   char t1[1024];
   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(h2_tool, H2_PP_0ARGS)
{
   int n0 = H2_PP_0ARGS();
   int n1 = H2_PP_0ARGS(a);
   int n2 = H2_PP_0ARGS(a, b);
   int n3 = H2_PP_0ARGS(a, b, c);

   H2EQ(1, n0);
   H2EQ(0, n1);
   H2EQ(0, n2);
   H2EQ(0, n3);
}

H2CASE(h2_tool, PP_NARGS)
{
   int n0 = H2_PP_NARGS();
   int n1 = H2_PP_NARGS(a);
   int n2 = H2_PP_NARGS(a, b);
   int n3 = H2_PP_NARGS(a, b, c);

   H2EQ(0, n0);
   H2EQ(1, n1);
   H2EQ(2, n2);
   H2EQ(3, n3);
}

H2CASE(h2_tool, PP_CAT)
{
   int H2_PP_CAT(a, b, c, d) = 4;

   H2EQ(4, abcd);
}

H2CASE(h2_tool, h2_style)
{
   H2EQ("\033[31m", h2_style("red", t1));
   H2EQ("\033[31;1m", h2_style("red,bold", t1));
}

H2CASE(h2_tool, wildcard)
{
   H2EQ(!h2_wildcard_match("hello?world*", "welcome"));
   H2EQ(h2_wildcard_match("hello?world*", "hello world"));
   H2EQ(h2_wildcard_match("hello?world*", "hello world h2unit"));
   H2EQ(!h2_wildcard_match("hello?world*", "one world"));
   H2EQ(!h2_wildcard_match("hello?world*", ""));
}

H2CASE(h2_tool, acronym)
{
   H2EQ("01234", h2_acronym_string("01234"));
   H2EQ("0123456789...", h2_acronym_string("0123456789ABCDEF"));
}

H2CASE(h2_tool, center)
{
   H2EQ("ab", h2_center_string("ab", 2, t1));
   H2EQ("ab ", h2_center_string("ab", 3, t1));
   H2EQ(" ab ", h2_center_string("ab", 4, t1));
   H2EQ(" ab  ", h2_center_string("ab", 5, t1));
   H2EQ("  ab  ", h2_center_string("ab", 6, t1));

   H2EQ("abc", h2_center_string("abc", 3, t1));
   H2EQ("abc ", h2_center_string("abc", 4, t1));
   H2EQ(" abc ", h2_center_string("abc", 5, t1));
   H2EQ(" abc  ", h2_center_string("abc", 6, t1));
}
