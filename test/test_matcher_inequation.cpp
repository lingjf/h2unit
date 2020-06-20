#include "../source/h2_unit.cpp"

SUITE(inequation matches)
{
   Case(ge)
   {
      h2::h2_matches_ge<int> ge1(0);
      OK(nullptr == ge1.matches(1));
      OK(nullptr == ge1.matches(0));
      OK(nullptr != ge1.matches(-1));
   }

   Case(gt)
   {
      h2::h2_matches_gt<int> gt1(0);
      OK(nullptr == gt1.matches(1));
      OK(nullptr != gt1.matches(0));
      OK(nullptr != gt1.matches(-1));
   }

   Case(le)
   {
      h2::h2_matches_le<int> le1(0);
      OK(nullptr != le1.matches(1));
      OK(nullptr == le1.matches(0));
      OK(nullptr == le1.matches(-1));
   }

   Case(lt)
   {
      h2::h2_matches_lt<int> lt1(0);
      OK(nullptr != lt1.matches(1));
      OK(nullptr != lt1.matches(0));
      OK(nullptr == lt1.matches(-1));
   }
}
