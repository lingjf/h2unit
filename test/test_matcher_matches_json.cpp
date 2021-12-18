#include "../source/h2_unit.cpp"

SUITE(json matches)
{
   Case(Json)
   {
      h2::h2_matches_json a("[65]", "");
      OK(nullptr == a.matches("[65]", {}));
      OK(nullptr != a.matches("[66]", {}));
   }
}

SUITE(Je primitive [api])
{
   Case(Je)
   {
   }
}
