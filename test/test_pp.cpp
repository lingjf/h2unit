#include "../source/h2_unit.cpp"

SUITE(preprocessor)
{
#define X1 1
#define X13 -13

  Case(H2PP__CAT2)
  {
    int n12 = H2PP__CAT2(1, 2);
    OK(12, n12);

    int i13 = H2PP__CAT2(X1, 3);
    OK(-13, i13);
  }

  Case(H2PP_CAT2)
  {
    int n12 = H2PP_CAT2(1, 2);
    OK(12, n12);

    int n13 = H2PP_CAT2(X1, 3);
    OK(13, n13);
  }

  Case(H2PP_HEAD)
  {
    int n0[3] = { H2PP_HEAD() };
    OK(0, n0[0]);

    int n1 = H2PP_HEAD(1);
    OK(1, n1);
    int n3 = H2PP_HEAD(1, 2, 3);
    OK(1, n3);
  }

  Case(H2PP_TAIL)
  {
    int n0[3] = { H2PP_TAIL() };
    OK(0, n0[0]);

    int n[] = { H2PP_TAIL(1, 2, 3) };
    OK(2, n[0]);
    OK(3, n[1]);
  }

  Case(H2PP_LAST)
  {
    int n0[3] = { H2PP_LAST() };
    OK(0, n0[0]);

    int n1 = H2PP_LAST(1);
    OK(1, n1);
    int n3 = H2PP_LAST(1, 2, 3);
    OK(3, n3);
  }

  Case(H2PP_REMOVE_PARENTHESES)
  {
    int n[] = { H2PP_REMOVE_PARENTHESES((1, 2, 3)) };
    OK(1, n[0]);
    OK(2, n[1]);
    OK(3, n[2]);
  }

  Case(H2PP_IS_BEGIN_PARENTHESES)
  {
    OK(0, H2PP_IS_BEGIN_PARENTHESES());
    OK(0, H2PP_IS_BEGIN_PARENTHESES(a));
    OK(1, H2PP_IS_BEGIN_PARENTHESES((a)));
    OK(1, H2PP_IS_BEGIN_PARENTHESES((a, b)));
    // OK(1, H2PP_IS_BEGIN_PARENTHESES((a, b), 1)); // compile error
    OK(1, H2PP_IS_BEGIN_PARENTHESES(()));
  }

  Case(H2PP_NOT)
  {
    int n0 = H2PP_NOT(0);
    OK(1, n0);

    int n1 = H2PP_NOT(1);
    OK(0, n1);
    int n2 = H2PP_NOT(2);
    OK(0, n2);
  }

  Case(H2PP_COMPL)
  {
    int n0 = H2PP_COMPL(1);
    OK(0, n0);
    int n1 = H2PP_COMPL(0);
    OK(1, n1);
  }

  Case(H2PP_BOOL)
  {
    int n0 = H2PP_BOOL(0);
    OK(0, n0);

    int n1 = H2PP_BOOL(1);
    OK(1, n1);
    int n2 = H2PP_BOOL(2);
    OK(1, n2);
  }

  Case(H2PP_AND)
  {
    int n11 = H2PP_AND(1, 1);
    OK(1, n11);
    int n10 = H2PP_AND(1, 0);
    OK(0, n10);
    int n01 = H2PP_AND(0, 1);
    OK(0, n01);
    int n00 = H2PP_AND(0, 0);
    OK(0, n00);

    int n22 = H2PP_AND(2, 2);
    OK(1, n22);
    int n20 = H2PP_AND(2, 0);
    OK(0, n20);
  }

  Case(H2PP_OR)
  {
    int n11 = H2PP_OR(1, 1);
    OK(1, n11);
    int n10 = H2PP_OR(1, 0);
    OK(1, n10);
    int n01 = H2PP_OR(0, 1);
    OK(1, n01);
    int n00 = H2PP_OR(0, 0);
    OK(0, n00);

    int n22 = H2PP_OR(2, 2);
    OK(1, n22);
    int n20 = H2PP_OR(2, 0);
    OK(1, n20);
  }

  Case(H2PP_EQ)
  {
    OK(H2PP_EQ(0, 0));
    OK(H2PP_EQ(1, 1));
    OK(H2PP_EQ(2, 2));
  }

  Case(H2PP_IF_ELSE)
  {
#define X7_YES "Yes"
#define X7_NO "No"

    const char* yes = H2PP_IF_ELSE(1, X7_YES, X7_NO);
    OK("Yes", yes);
    const char* no = H2PP_IF_ELSE(0, X7_YES, X7_NO);
    OK("No", no);
  }

  Case(H2PP_IF)
  {
    int n0[3] = { H2PP_IF(0)(10) };
    OK(0, n0[0]);
    int n1[3] = { H2PP_IF(1)(11) };
    OK(11, n1[0]);
    int n2[3] = { H2PP_IF(13)(12) };
    OK(12, n2[0]);
  }

  Case(H2PP_IS_EMPTY)
  {
    int n0 = H2PP_IS_EMPTY();
    int n1 = H2PP_IS_EMPTY(a);
    int n2 = H2PP_IS_EMPTY(a, b);
    int n3 = H2PP_IS_EMPTY(a, b, c);

    OK(1, n0);
    OK(0, n1);
    OK(0, n2);
    OK(0, n3);
  }

  Case(H2PP_VARIADIC_CALL)
  {
#define M0()                                                                   \
  {}
#define M1(_1)                                                                 \
  {                                                                            \
    _1                                                                         \
  }
#define M2(_1, _2)                                                             \
  {                                                                            \
    _1, _2                                                                     \
  }
#define M3(_1, _2, _3)                                                         \
  {                                                                            \
    _1, _2, _3                                                                 \
  }
    int n0[9] = H2PP_VARIADIC_CALL(M);
    OK(0, n0[0]);
    OK(0, n0[1]);
    OK(0, n0[2]);

    int n1[9] = H2PP_VARIADIC_CALL(M, 1);
    OK(1, n1[0]);
    OK(0, n1[1]);
    OK(0, n1[2]);

    int n2[9] = H2PP_VARIADIC_CALL(M, 1, 2);
    OK(1, n2[0]);
    OK(2, n2[1]);
    OK(0, n2[2]);

    int n3[9] = H2PP_VARIADIC_CALL(M, 1, 2, 3);
    OK(1, n3[0]);
    OK(2, n3[1]);
    OK(3, n3[2]);
  }

  Case(H2PP_NARG)
  {
    int n0 = H2PP_NARG();
    int n1 = H2PP_NARG(a);
    int n2 = H2PP_NARG(a, b);
    int n3 = H2PP_NARG(a, b, c);

    OK(0, n0);
    OK(1, n1);
    OK(2, n2);
    OK(3, n3);
  }
}
