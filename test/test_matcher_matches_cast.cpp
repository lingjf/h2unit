#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(matches castof)
{
   Case(h2_castof_matches)
   {
   }
}

SUITE(CastOf)
{
   Case(object)
   {
      OK(CastOf<int>(1), 1.2);
   }
}
