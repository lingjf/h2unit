#include "../source/h2_unit.cpp"

SUITE(H2_A)
{
   Case(empty)
   {
      int n = H2_A();
      OK(0, n);
   }

   Case(one)
   {
      int n = H2_A(1);
      OK(1, n);
   }

   Case(two)
   {
      int n = H2_A(1, 2);
      OK(1, n);
   }

   Case(empty in parenthesis)
   {
      int n = H2_A(());
      OK(0, n);
   }

   Case(one in parenthesis)
   {
      int n = H2_A((1));
      OK(1, n);
   }

   Case(two in parenthesis)
   {
      int n = H2_A((1, 2));
      OK(1, n);
   }
}

SUITE(H2_A x y)
{
   Case(empty)
   {
      int x = H2_XA();
      OK(0, x);
      int y = H2_YA();
      OK(0, y);
   }

   Case(one)
   {
      int x = H2_XA(1);
      OK(1, x);
      int y = H2_YA(1);
      OK(1, y);
   }

   Case(two)
   {
      int x = H2_XA(1, 2);
      OK(1, x);
      int y = H2_YA(1, 2);
      OK(2, y);
   }

   Case(empty in 1 parenthesis)
   {
      int x = H2_XA(());
      OK(0, x);
      int y = H2_YA(());
      OK(0, y);
   }

   Case(one in 1 parenthesis)
   {
      int x = H2_XA((1));
      OK(1, x);
      int y = H2_YA((1));
      OK(1, y);
   }

   Case(two in 1 parenthesis)
   {
      int x = H2_XA((1, 2));
      OK(1, x);
      int y = H2_YA((1, 2));
      OK(1, y);
   }

   Case(empty in 2 parenthesis)
   {
      int x = H2_XA((), ());
      OK(0, x);
      int y = H2_YA((), ());
      OK(0, y);
   }

   Case(one in 2 parenthesis)
   {
      int x = H2_XA((1), (2));
      OK(1, x);
      int y = H2_YA((1), (2));
      OK(2, y);
   }

   Case(two in 2 parenthesis)
   {
      int x = H2_XA((1, 2), (3, 4));
      OK(1, x);
      int y = H2_YA((1, 2), (3, 4));
      OK(3, y);
   }

   Case(three in 2 parenthesis)
   {
      int x = H2_XA((1, 2, 3), (4, 5, 6));
      OK(1, x);
      int y = H2_YA((1, 2, 3), (4, 5, 6));
      OK(4, y);
   }
}

CASES(parameterized, (1, 2, 3))
{
   OK(Gt(0), x);
}

CASES(parameterized, (1, 2, 3))
{
   OK(Gt(0), x);
}

CASES(parameterized, ())
{
   OK(Gt(10), x);
}

CASES(parameterized)
{
   OK(Gt(10), x);
}

TODOS(parameterized, (1, 2, 3))
{
   OK(Gt(10), x) << "error";
}

CASESS(parameterized, (1, 2, 3), (4, 5, 6))
{
   OK(x < y);
}

CASESS(parameterized, (1, 2, 3), ())
{
   OK(x < y);
}

CASESS(parameterized, (), (4, 5, 6))
{
   OK(x < y);
}

CASESS(parameterized, (), ())
{
   OK(x < y);
}

CASESS(parameterized, (1, 2, 3))
{
   OK(1 < x + y);
}

CASESS(parameterized, ())
{
   OK(1 < x + y);
}

CASESS(parameterized)
{
   OK(1 < x + y);
}

TODOSS(parameterized, (1, 2, 3), (4, 5, 6))
{
   OK(x > y) << "error";
}

TODOSS(parameterized, (1, 2, 3))
{
   OK(10 < x + y) << "error";
}

SUITE(test foreach)
{
   Cases(parameterized, (1, 2, 3))
   {
      OK(Gt(0), x);
   }

   Cases(parameterized, (1, 2, 3))
   {
      OK(Gt(0), x);
   }

   Todos(parameterized, (1, 2, 3))
   {
      OK(Gt(10), x) << "error";
   }

   // Cases_t(parameterized, (int, long, double))
   // {
   // }
}

SUITE(test fullmesh)
{
   Casess(parameterized, (1, 2, 3))
   {
      OK(Gt(1), x + y);
   }

   Todoss(parameterized, (1, 2, 3))
   {
      OK(Gt(10), x + y) << "error";
   }
}

CASES_T(parameterized, (int, long, double))
{
   x value = 1;
   OK(Gt(0), value);
}

TODOS_T(parameterized, (int, long, double))
{
   x value = 1;
   OK(Gt(10), value) << "error";
}

CASESS_T(parameterized, (int, long, double))
{
   x value_x = 1;
   y value_y = 1;
   OK(value_x, value_y);
}

TODOSS_T(parameterized, (int, long, double))
{
   x value_x = 1;
   y value_y = 2;
   OK(value_x, value_y) << "error";
}
