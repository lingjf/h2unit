
SUITE(An)
{
  Case(List)
  {
    int n0 = H2List_An();
    OK(0, n0);
    int n1 = H2List_An(1, 2, 3);
    OK(1, n1);
  }
  Case(ForEach)
  {
    int n0 = H2ForEach_An();
    OK(0, n0);
    int n1 = H2ForEach_An(1, 2, 3);
    OK(1, n1);
    int n2 = H2ForEach_An((1, 2, 3));
    OK(1, n2);
  }
  Case(Fullmesh)
  {
    int nx0 = H2Fullmesh_Ax();
    OK(0, nx0);
    int ny0 = H2Fullmesh_Ay();
    OK(0, ny0);

    int nx1 = H2Fullmesh_Ax(1, 2, 3);
    OK(1, nx1);
    int ny1 = H2Fullmesh_Ay(1, 2, 3);
    OK(3, ny1);

    int nx2 = H2Fullmesh_Ax((1, 2, 3));
    OK(1, nx2);
    int ny2 = H2Fullmesh_Ay((1, 2, 3));
    OK(1, ny2);
    int nx3 = H2Fullmesh_Ax((1, 2, 3), (4, 5, 6));
    OK(1, nx3);
    int ny3 = H2Fullmesh_Ay((1, 2, 3), (4, 5, 6));
    OK(4, ny3);
  }
}

CASE(ForForEach)
{
  int s = 0;
#define FOO(x) s += x;
  ForForEach(FOO, 1, 2, 3)
#undef FOO
    OK(6, s);
}

CASE(ForFullmesh)
{
  int s = 0;
#define FOO(x, y) s += x * y;
  ForFullmesh(FOO, 1, 2, 3);
#undef FOO
  OK(1 * 1 + 1 * 2 + 1 * 3 + 2 * 1 + 2 * 2 + 2 * 3 + 3 * 1 + 3 * 2 + 3 * 3, s);
}

#define BAR(x, y)                                                              \
  CASE(generator x y) {}

ForFullmesh(BAR, (A, B, C), (1, 2, 3))
#undef BAR

  CASES(test foreach CASE, 1, 2, 3)
{
  OK(Gt(0), x);
}
CASES(test foreach CASE, 1, 2, 3)
{
  OK(Gt(0), x);
}

CASESS(test fullmesh CASE, (1, 2, 3), (4, 5, 6))
{
  OK(x < y);
}

CASESS(test fullmesh CASE, 1, 2, 3)
{
  OK(1 < x + y);
}

SUITE()
{
  Cases(test foreach Case, 1, 2, 3) { OK(Gt(0), x); };
  Cases(test foreach Case, 1, 2, 3) { OK(Gt(0), x); };
}

SUITE()
{
  Casess(test fullmesh Case, 1, 2, 3) { OK(Gt(1), x + y); }
}
