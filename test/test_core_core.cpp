#include "../source/h2_unit.cpp"

SUITE(An)
{
   Case()
   {
      int n0 = _H2_An();
      OK(0, n0);
      int n1 = _H2_An(1, 2, 3);
      OK(1, n1);
   }

   Case(ForEach)
   {
      int n0 = _H2_An_ForEach();
      OK(0, n0);
      int n1 = _H2_An_ForEach(1, 2, 3);
      OK(1, n1);
      int n2 = _H2_An_ForEach((1, 2, 3));
      OK(1, n2);
   }

   Case(Fullmesh)
   {
      int nx0 = _H2_An_Fullmeshx();
      OK(0, nx0);
      int ny0 = _H2_An_Fullmeshy();
      OK(0, ny0);

      int nx1 = _H2_An_Fullmeshx(1, 2, 3);
      OK(1, nx1);
      int ny1 = _H2_An_Fullmeshy(1, 2, 3);
      OK(3, ny1);

      int nx2 = _H2_An_Fullmeshx((1, 2, 3));
      OK(1, nx2);
      int ny2 = _H2_An_Fullmeshy((1, 2, 3));
      OK(1, ny2);
      int nx3 = _H2_An_Fullmeshx((1, 2, 3), (4, 5, 6));
      OK(1, nx3);
      int ny3 = _H2_An_Fullmeshy((1, 2, 3), (4, 5, 6));
      OK(4, ny3);
   }
}

CASES(1, 2, 3)
{
   OK(Gt(0), x);
}

CASES(1, 2, 3)
{
   OK(Gt(0), x);
}

CASESS((1, 2, 3), (4, 5, 6))
{
   OK(x < y);
}

CASESS(1, 2, 3)
{
   OK(1 < x + y);
}

SUITE(test foreach)
{
   Cases(1, 2, 3)
   {
      OK(Gt(0), x);
   }

   Cases(1, 2, 3)
   {
      OK(Gt(0), x);
   }
}

SUITE(test fullmesh)
{
   Casess(1, 2, 3)
   {
      OK(Gt(1), x + y);
   }
}

CASES_T(int, long, double)
{
   x value = 1;
   OK(Gt(0), value);
}

CASESS_T(int, long, double)
{
   x value_x = 1;
   y value_y = 1;
   OK(value_x, value_y);
}
