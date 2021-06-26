// gcc test_utils_macro.cpp -DJUSTPP

#ifdef JUSTPP
#   include "../source/utils/h2_macro.hpp"
#else
#   include "../source/h2_unit.cpp"
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int test_H2PP_CAT()
{
#define CAT1 1
#define CAT13 -13
   int m12 = _H2PP_CAT2(1, 2);
   if (m12 != 12) return __LINE__;

   int m13 = _H2PP_CAT2(CAT1, 3);
   if (m13 != -13) return __LINE__;

   int n12 = H2PP_CAT2(1, 2);
   if (n12 != 12) return __LINE__;

   int n13 = H2PP_CAT2(CAT1, 3);
   if (n13 != 13) return __LINE__;

#undef CAT1
#undef CAT13
   return 0;
}

static int test_H2PP_HAS_COMMA()
{
   int s0 = H2PP_HAS_COMMA();
   if (s0 > 0) return __LINE__;

   int s1 = H2PP_HAS_COMMA(a);
   if (s1 > 0) return __LINE__;

   int s2 = H2PP_HAS_COMMA(a, b);
   if (s2 == 0) return __LINE__;

   int s3 = H2PP_HAS_COMMA(a, b, c);
   if (s3 == 0) return __LINE__;

   return 0;
}

static int test_H2PP_IS_EMPTY()
{
   int s0 = H2PP_IS_EMPTY();
   if (s0 == 0) return __LINE__;

   int s1 = H2PP_IS_EMPTY(a);
   if (s1) return __LINE__;

   int s2 = H2PP_IS_EMPTY(a, b);
   if (s2) return __LINE__;

   int s3 = H2PP_IS_EMPTY(a, b, c);
   if (s3) return __LINE__;

   return 0;
}

static int test_H2PP_NOT()
{
   int n0 = H2PP_NOT(0);
   if (!(1 == n0)) return __LINE__;

   int n1 = H2PP_NOT(1);
   if (!(0 == n1)) return __LINE__;
   int n2 = H2PP_NOT(2);
   if (!(0 == n2)) return __LINE__;

   return 0;
}

static int test_H2PP_COMPL()
{
   int n0 = H2PP_COMPL(1);
   if (!(0 == n0)) return __LINE__;
   int n1 = H2PP_COMPL(0);
   if (!(1 == n1)) return __LINE__;

   return 0;
}

static int test_H2PP_BOOL()
{
   int n0 = H2PP_BOOL(0);
   if (!(0 == n0)) return __LINE__;

   int n1 = H2PP_BOOL(1);
   if (!(1 == n1)) return __LINE__;
   int n2 = H2PP_BOOL(2);
   if (!(1 == n2)) return __LINE__;

   return 0;
}

static int test_H2PP_AND()
{
   int n11 = H2PP_AND(1, 1);
   if (1 != n11) return __LINE__;
   int n10 = H2PP_AND(1, 0);
   if (0 != n10) return __LINE__;
   int n01 = H2PP_AND(0, 1);
   if (0 != n01) return __LINE__;
   int n00 = H2PP_AND(0, 0);
   if (0 != n00) return __LINE__;

   int n22 = H2PP_AND(2, 2);
   if (1 != n22) return __LINE__;
   int n20 = H2PP_AND(2, 0);
   if (0 != n20) return __LINE__;

   return 0;
}

static int test_H2PP_OR()
{
   int n11 = H2PP_OR(1, 1);
   if (1 != n11) return __LINE__;
   int n10 = H2PP_OR(1, 0);
   if (1 != n10) return __LINE__;
   int n01 = H2PP_OR(0, 1);
   if (1 != n01) return __LINE__;
   int n00 = H2PP_OR(0, 0);
   if (0 != n00) return __LINE__;

   int n22 = H2PP_OR(2, 2);
   if (1 != n22) return __LINE__;
   int n20 = H2PP_OR(2, 0);
   if (1 != n20) return __LINE__;

   return 0;
}

static int test_H2PP_EQ()
{
   if (!(H2PP_EQ(0, 0))) return __LINE__;
   if (!(H2PP_EQ(1, 1))) return __LINE__;
   if (!(H2PP_EQ(2, 2))) return __LINE__;

   return 0;
}

static int test_H2PP_IS_BEGIN_PARENTHESIS()
{
   if (0 != H2PP_IS_BEGIN_PARENTHESIS()) return __LINE__;
   if (0 != H2PP_IS_BEGIN_PARENTHESIS(a)) return __LINE__;
   if (1 != H2PP_IS_BEGIN_PARENTHESIS((a))) return __LINE__;
   if (1 != H2PP_IS_BEGIN_PARENTHESIS((a, b))) return __LINE__;
   if (1 != H2PP_IS_BEGIN_PARENTHESIS(())) return __LINE__;

   /* if(1 != H2PP_IS_BEGIN_PARENTHESIS((a, b), 1)) return false; // compile error */
   return 0;
}

static int test_H2PP_REMOVE_PARENTHESES()
{
   int n[] = {H2PP_REMOVE_PARENTHESES((1, 2, 3))};
   if ((1 != n[0]) &&
       (2 != n[1]) &&
       (3 != n[2])) return __LINE__;
   return 0;
}

static int test_H2PP_REMOVE_PARENTHESES_IF()
{
   int n[] = {H2PP_REMOVE_PARENTHESES_IF((1, 2, 3))};
   if ((1 != n[0]) && (2 != n[1]) && (3 != n[2])) return __LINE__;

   //  int m[] = { H2PP_REMOVE_PARENTHESES_IF(1, 2, 3) };
   //  if ((1 != m[0]) && (2 != m[1])  && (3 != m[2])) return __LINE__;

   return 0;
}

static int test_H2PP_TH()
{
   int n0 H2PP_TH9();
   int n1 H2PP_TH9(0);
   int n2 H2PP_TH9(0, 1);
   int n3 H2PP_TH9(0, 1, 2);
   int n4 H2PP_TH9(0, 1, 2, 3);
   int n5 H2PP_TH9(0, 1, 2, 3, 4);
   int n6 H2PP_TH9(0, 1, 2, 3, 4, 5);
   int n7 H2PP_TH9(0, 1, 2, 3, 4, 5, 6);
   int n8 H2PP_TH9(0, 1, 2, 3, 4, 5, 6, 7);
   int n9 H2PP_TH9(0, 1, 2, 3, 4, 5, 6, 7, 8);
   int n10 = H2PP_TH9(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
   if (9 != n10) return __LINE__;

   return 0;
}

static int test_H2PP_HEAD()
{
   int n0[3] = {H2PP_HEAD()};
   if (0 != n0[0]) return __LINE__;

   int n1 = H2PP_HEAD(1);
   if (1 != n1) return __LINE__;
   int n3 = H2PP_HEAD(1, 2, 3);
   if (1 != n3) return __LINE__;
   return 0;
}

static int test_H2PP_TAIL()
{
   int n0[3] = {H2PP_TAIL()};
   if (0 != n0[0]) return __LINE__;

   int n[] = {H2PP_TAIL(1, 2, 3)};
   if (2 != n[0]) return __LINE__;
   if (3 != n[1]) return __LINE__;
   return 0;
}

static int test_H2PP_LAST()
{
   int n0[3] = {H2PP_LAST()};
   if (0 != n0[0]) return __LINE__;

   int n1 = H2PP_LAST(1);
   if (1 != n1) return __LINE__;
   int n3 = H2PP_LAST(1, 2, 3);
   if (3 != n3) return __LINE__;
   return 0;
}

static int test_H2PP_IF_ELSE()
{
#define X7_TRUE "Yes"
#define X7_FALSE "No"

   const char* yes = H2PP_IF_ELSE(1, X7_TRUE, X7_FALSE);
   if (strcmp("Yes", yes)) return __LINE__;
   const char* no = H2PP_IF_ELSE(0, X7_TRUE, X7_FALSE);
   if (strcmp("No", no)) return __LINE__;

#undef X7_TRUE
#undef X7_FALSE

   return 0;
}

static int test_H2PP_IF()
{
   int n0[3] = {H2PP_IF(0)(10)};
   if (0 != n0[0]) return __LINE__;
   int n1[3] = {H2PP_IF(1)(11)};
   if (11 != n1[0]) return __LINE__;
   int n2[3] = {H2PP_IF(13)(12)};
   if (12 != n2[0]) return __LINE__;

   return 0;
}

static int test_H2PP_VARIADIC_CALL()
{
#define M0() 0
#define M1(_1) _1
#define M2(_1, _2) _1 + _2
#define M3(_1, _2, _3) _1 + _2 + _3

   int n0 = H2PP_VARIADIC_CALL(M);
   if (0 != n0) return __LINE__;

   int n1 = H2PP_VARIADIC_CALL(M, 1);
   if (1 != n1) return __LINE__;

   int n2 = H2PP_VARIADIC_CALL(M, 1, 2);
   if (1 + 2 != n2) return __LINE__;

   int n3 = H2PP_VARIADIC_CALL(M, 1, 2, 3);
   if (1 + 2 + 3 != n3) return __LINE__;

#undef M0
#undef M1
#undef M2
#undef M3

   return 0;
}

static int test_H2PP_NARG()
{
   int n0 = H2PP_NARG();
   int n1a = H2PP_NARG(a);
   int n1b = H2PP_NARG((a));
   int n1c = H2PP_NARG((a, b));
   int n1d = H2PP_NARG((a, b, c));
   int n2a = H2PP_NARG(a, b);
   int n2b = H2PP_NARG((a, b), c);
   int n3 = H2PP_NARG(a, b, c);

   if (0 != n0) return __LINE__;
   if (1 != n1a) return __LINE__;
   if (1 != n1b) return __LINE__;
   if (1 != n1c) return __LINE__;
   if (1 != n1d) return __LINE__;
   if (2 != n2a) return __LINE__;
   if (2 != n2b) return __LINE__;
   if (3 != n3) return __LINE__;

   return 0;
}

static int test_H2PP_REPEAT()
{
#define M0(Dummy, i) int n##i = i;
   H2PP_REPEAT((), M0, , 0);
#undef M0
   int n0, n1, n2, n3;

#define Ma(Dummy, i) n##i = 100 + i
   H2PP_REPEAT((), Ma, , 1);
   if (100 != n0) return __LINE__;
#undef Ma

#define Mb(a, i) n##i = 200 + i + a
   H2PP_REPEAT((), Mb, 0, 1);
   if (200 != n0) return __LINE__;
#undef Mb

#define Mc(Dummy, i) n##i = 300 + i
   H2PP_REPEAT((, ), Mc, , 2);
   if (300 != n0) return __LINE__;
   if (301 != n1) return __LINE__;
#undef Mc

   return 0;
}

static int test_H2PP_FOREACH()
{
   int sa = 0;
#define Fora(Dummy, i, x) sa += x * i;
   H2PP_FOREACH(, Fora, (), 1, 2, 3)
#undef Fora
   if (1 * 0 + 2 * 1 + 3 * 2 != sa) return __LINE__;

   int sb = 0;
#define Forb(Dummy, i, x) sb += x * i;
   H2PP_FOREACH((), Forb, (), 1, 2, 3)
#undef Forb
   if (1 * 0 + 2 * 1 + 3 * 2 != sb) return __LINE__;

   int sc = 0;
#define Forc(Dummy, i, x) sc += x * i
   H2PP_FOREACH((;), Forc, (), 1, 2, 3);
#undef Forc
   if (1 * 0 + 2 * 1 + 3 * 2 != sc) return __LINE__;

   return 0;
}

static int test_H2PP_FULLMESH()
{
   int e = 1 * 1 * 0 * 0 +
           1 * 2 * 0 * 1 +
           1 * 3 * 0 * 2 +
           2 * 1 * 1 * 0 +
           2 * 2 * 1 * 1 +
           2 * 3 * 1 * 2 +
           3 * 1 * 2 * 0 +
           3 * 2 * 2 * 1 +
           3 * 3 * 2 * 2;

   int sa = 0;
#define Fma(Dummy, i, j, x, y) sa += x * y * i * j;
   H2PP_FULLMESH((, Fma, (), 1, 2, 3));
#undef Fma
   if (e != sa) return __LINE__;

   int sb = 0;
#define Fmb(Dummy, i, j, x, y) sb += x * y * i * j;
   H2PP_FULLMESH(((), Fmb, (), 1, 2, 3));
#undef Fmb
   if (e != sb) return __LINE__;

   int sc = 0;
#define Fmc(Dummy, i, j, x, y) sc += x * y * i * j
   H2PP_FULLMESH(((;), Fmc, (), 1, 2, 3));
#undef Fmc
   if (e != sc) return __LINE__;

   return 0;
}

static int test_H2PP_UNIQUE()
{
   int H2PP_UNIQUE() = 1;
   int H2PP_UNIQUE() = 1;
   int H2PP_UNIQUE(prefix) = 1;
   int H2PP_UNIQUE(prefix) = 1;

   return 0;
}

////////////////////////////////////////////////////////////////

#ifdef JUSTPP

int main(int argc, char **argv)
{
   int ret;
   if ((ret = test_H2PP_CAT())) goto failed;
   if ((ret = test_H2PP_HAS_COMMA())) goto failed;
   if ((ret = test_H2PP_IS_EMPTY())) goto failed;
   if ((ret = test_H2PP_NOT())) goto failed;
   if ((ret = test_H2PP_COMPL())) goto failed;
   if ((ret = test_H2PP_BOOL())) goto failed;
   if ((ret = test_H2PP_AND())) goto failed;
   if ((ret = test_H2PP_OR())) goto failed;
   if ((ret = test_H2PP_EQ())) goto failed;
   if ((ret = test_H2PP_IS_BEGIN_PARENTHESIS())) goto failed;
   if ((ret = test_H2PP_REMOVE_PARENTHESES())) goto failed;
   if ((ret = test_H2PP_REMOVE_PARENTHESES_IF())) goto failed;
   if ((ret = test_H2PP_TH())) goto failed;
   if ((ret = test_H2PP_HEAD())) goto failed;
   if ((ret = test_H2PP_TAIL())) goto failed;
   if ((ret = test_H2PP_LAST())) goto failed;
   if ((ret = test_H2PP_IF_ELSE())) goto failed;
   if ((ret = test_H2PP_IF())) goto failed;
   if ((ret = test_H2PP_VARIADIC_CALL())) goto failed;
   if ((ret = test_H2PP_NARG())) goto failed;
   if ((ret = test_H2PP_REPEAT())) goto failed;
   if ((ret = test_H2PP_FOREACH())) goto failed;
   if ((ret = test_H2PP_FULLMESH())) goto failed;
   if ((ret = test_H2PP_UNIQUE())) goto failed;

   printf("success \n");
   return 0;
failed:
   printf("failed line %d \n", ret);

   return ret;
}

#else

CASE(macro)
{
   OK(0, test_H2PP_CAT());
   OK(0, test_H2PP_HAS_COMMA());
   OK(0, test_H2PP_IS_EMPTY());
   OK(0, test_H2PP_NOT());
   OK(0, test_H2PP_COMPL());
   OK(0, test_H2PP_BOOL());
   OK(0, test_H2PP_AND());
   OK(0, test_H2PP_OR());
   OK(0, test_H2PP_EQ());
   OK(0, test_H2PP_IS_BEGIN_PARENTHESIS());
   OK(0, test_H2PP_REMOVE_PARENTHESES());
   OK(0, test_H2PP_REMOVE_PARENTHESES_IF());
   OK(0, test_H2PP_TH());
   OK(0, test_H2PP_HEAD());
   OK(0, test_H2PP_TAIL());
   OK(0, test_H2PP_LAST());
   OK(0, test_H2PP_IF_ELSE());
   OK(0, test_H2PP_IF());
   OK(0, test_H2PP_VARIADIC_CALL());
   OK(0, test_H2PP_NARG());
   OK(0, test_H2PP_REPEAT());
   OK(0, test_H2PP_FOREACH());
   OK(0, test_H2PP_FULLMESH());
   OK(0, test_H2PP_UNIQUE());
}
#endif
