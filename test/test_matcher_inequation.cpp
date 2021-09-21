#include "../source/h2_unit.cpp"

SUITE(inequation matches)
{
   Case(ge)
   {
      h2::h2_matches_ge<int> ge1(0);
      OK(nullptr == ge1.matches(1, 0, false, false, false));
      OK(nullptr == ge1.matches(0, 0, false, false, false));
      OK(nullptr != ge1.matches(-1, 0, false, false, false));
   }

   Case(gt)
   {
      h2::h2_matches_gt<int> gt1(0);
      OK(nullptr == gt1.matches(1, 0, false, false, false));
      OK(nullptr != gt1.matches(0, 0, false, false, false));
      OK(nullptr != gt1.matches(-1, 0, false, false, false));
   }

   Case(le)
   {
      h2::h2_matches_le<int> le1(0);
      OK(nullptr != le1.matches(1, 0, false, false, false));
      OK(nullptr == le1.matches(0, 0, false, false, false));
      OK(nullptr == le1.matches(-1, 0, false, false, false));
   }

   Case(lt)
   {
      h2::h2_matches_lt<int> lt1(0);
      OK(nullptr != lt1.matches(1, 0, false, false, false));
      OK(nullptr != lt1.matches(0, 0, false, false, false));
      OK(nullptr == lt1.matches(-1, 0, false, false, false));
   }
}
