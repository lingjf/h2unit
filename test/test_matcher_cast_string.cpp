#include "../source/h2_unit.cpp"

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

#if !defined WIN32
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
#endif
}
