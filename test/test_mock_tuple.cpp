#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(mock tuple)
{
   Case(tuple_types)
   {
      h2::h2_vector<h2::h2_string> names;
      h2::tuple_types<std::tuple<int, char*, struct tm*, B_DerivedClass, C_OverrideClass&>>(names);

      OK(5, names.size());
      OK("int", names[0]);
      OK(Se("char" _pointer), names[1]);
      OK(Se(_struct "tm" _pointer), names[2]);
      OK(_class "B_DerivedClass", names[3]);
      OK(_class "C_OverrideClass&", names[4]);
   }
}
