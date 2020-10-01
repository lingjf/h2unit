#include "../source/h2_unit.cpp"

SUITE(assert utility)
{
   Case(find_outer_comma)
   {
      OK(",b", h2::find_outer_comma("a,b"));
      OK(",b", h2::find_outer_comma("\"a\",b"));
      OK(",b", h2::find_outer_comma("\"a,\",b"));
      OK(",b", h2::find_outer_comma("a\\\",b"));
      OK(",b", h2::find_outer_comma("'a',b"));
      OK(",b", h2::find_outer_comma("(a,),b"));
      OK(",b", h2::find_outer_comma("<a,>,b"));
   }
}
