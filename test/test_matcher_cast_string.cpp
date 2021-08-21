#include "../source/h2_unit.cpp"
#if !defined __CYGWIN__

#include "test_types.hpp"

SUITE(Cast String)
{
   STRING_DECL_LIST;

   Case(OK)
   {
#define TheCheck(x, y)   \
   OK(x, y);             \
   OK(CaseLess(x), y);   \
   OK(Substr(x), y);     \
   OK(Re(x), y);         \
   OK(AnyOf(x), y);

      H2Fullmesh(TheCheck, STRING_VAR_LIST);
#undef TheCheck
   }

   Case(OK)
   {
#define TheCheck(x, y)   \
   OK(Eq(x), y);         \
   OK(CaseLess(x), y);   \
   OK(Substr(x), y);     \
   OK(Re(x), y);         \
   OK(AllOf(_, x), y);   \
   OK(Not(!NoneOf(Nq(x))), y);

      H2Fullmesh(TheCheck, STRING_VAR_LIST);
#undef TheCheck
   }
}
#endif
