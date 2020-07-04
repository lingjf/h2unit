#include "../source/h2_unit.cpp"

SUITE(Check Utility)
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

SUITE(OK Primitive)
{
   Case(Any)
   {
      OK(_, 1);
      OK(Any, "A");
      OK((_), "A");
      OK((Any), "A");
   }

   Case(IsNull NotNull)
   {
      OK(IsNull, 0);
      OK((NotNull), 1);
   }
}
