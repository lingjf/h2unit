
#ifdef JUSTPP
// debug macro only
// gcc test_utils_macro.cpp -DJUSTPP

#include "../source/utils/h2_macro.hpp"
#define SUITE(...) int main(int argc, char** argv)
#define Case(...) for (const char* cn = #__VA_ARGS__; cn; printf("success %s \n", cn), cn = NULL)
#define OK(e, a)                                            \
   if ((e) != (a)) {                                        \
      printf("failed %s %s:%d \n", cn, __FILE__, __LINE__); \
      exit(1);                                              \
   }
#else
#include "../source/h2_unit.cpp"
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////

SUITE(macro)
{
   Case(H2PP_CAT)
   {
#define CAT1 1
#define CAT13 -13
      int m12 = _H2PP_CAT2(1, 2);
      OK(12, m12);

      int m13 = _H2PP_CAT2(CAT1, 3);
      OK(-13, m13);

      int n12 = H2PP_CAT2(1, 2);
      OK(12, n12);

      int n13 = H2PP_CAT2(CAT1, 3);
      OK(13, n13);

#undef CAT1
#undef CAT13
   }

   Case(H2PP_HAS_COMMA)
   {
      int s0 = H2PP_HAS_COMMA();
      OK(0, s0);

      int s1 = H2PP_HAS_COMMA(a);
      OK(0, s1);

      int s2 = H2PP_HAS_COMMA(a, b);
      OK(1, s2);

      int s3 = H2PP_HAS_COMMA(a, b, c);
      OK(1, s3);
   }

   Case(H2PP_IS_EMPTY)
   {
      int s0 = H2PP_IS_EMPTY();
      OK(1, s0);

      int s1 = H2PP_IS_EMPTY(a);
      OK(0, s1);

      int s2 = H2PP_IS_EMPTY(a, b);
      OK(0, s2);

      int s3 = H2PP_IS_EMPTY(a, b, c);
      OK(0, s3);
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
      OK(1, H2PP_EQ(0, 0));
      OK(1, H2PP_EQ(1, 1));
      OK(1, H2PP_EQ(2, 2));
   }

   Case(H2PP_IS_BEGIN_PARENTHESIS)
   {
      OK(0, H2PP_IS_BEGIN_PARENTHESIS());
      OK(0, H2PP_IS_BEGIN_PARENTHESIS(a));
      OK(1, H2PP_IS_BEGIN_PARENTHESIS((a)));
      OK(1, H2PP_IS_BEGIN_PARENTHESIS((a, b)));
      OK(1, H2PP_IS_BEGIN_PARENTHESIS(()));

      /* OK(1 , H2PP_IS_BEGIN_PARENTHESIS((a, b), 1)) ; // compile error */
   }

   Case(H2PP_REMOVE_PARENTHESES)
   {
      int n[] = {H2PP_REMOVE_PARENTHESES((1, 2, 3))};
      OK(1, n[0]);
      OK(2, n[1]);
      OK(3, n[2]);
   }

   Case(H2PP_REMOVE_PARENTHESES_IF)
   {
      int n[] = {H2PP_REMOVE_PARENTHESES_IF((1, 2, 3))};
      OK(1, n[0]);
      OK(2, n[1]);
      OK(3, n[2]);

      // int m[] = { H2PP_REMOVE_PARENTHESES_IF(1, 2, 3) };
      // OK(1 , m[0]);
      // OK(2 , m[1]);
      // OK(3 , m[2]);
   }

   Case(H2PP_TH)
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
      OK(9, n10);
   }

   Case(H2PP_HEAD)
   {
      int n0[3] = {H2PP_HEAD()};
      OK(0, n0[0]);

      int n1 = H2PP_HEAD(1);
      OK(1, n1);
      int n3 = H2PP_HEAD(1, 2, 3);
      OK(1, n3);
   }

   Case(H2PP_TAIL)
   {
      int n0[3] = {H2PP_TAIL()};
      OK(0, n0[0]);

      int n[] = {H2PP_TAIL(1, 2, 3)};
      OK(2, n[0]);
      OK(3, n[1]);
   }

   Case(H2PP_LAST)
   {
      int n0[3] = {H2PP_LAST()};
      OK(0, n0[0]);

      int n1 = H2PP_LAST(1);
      OK(1, n1);
      int n3 = H2PP_LAST(1, 2, 3);
      OK(3, n3);
   }

   Case(H2PP_IF_ELSE)
   {
#define X7_TRUE "Yes"
#define X7_FALSE "No"

      const char* yes = H2PP_IF_ELSE(1, X7_TRUE, X7_FALSE);
      OK(0, strcmp("Yes", yes));
      const char* no = H2PP_IF_ELSE(0, X7_TRUE, X7_FALSE);
      OK(0, strcmp("No", no));

#undef X7_TRUE
#undef X7_FALSE
   }

   Case(H2PP_IF)
   {
      int n0[3] = {H2PP_IF(0)(10)};
      OK(0, n0[0]);
      int n1[3] = {H2PP_IF(1)(11)};
      OK(11, n1[0]);
      int n2[3] = {H2PP_IF(13)(12)};
      OK(12, n2[0]);
   }

   Case(H2PP_VARIADIC_CALL)
   {
#define M0() 0
#define M1(_1) _1
#define M2(_1, _2) _1 + _2
#define M3(_1, _2, _3) _1 + _2 + _3

      int n0 = H2PP_VARIADIC_CALL(M);
      OK(0, n0);

      int n1 = H2PP_VARIADIC_CALL(M, 1);
      OK(1, n1);

      int n2 = H2PP_VARIADIC_CALL(M, 1, 2);
      OK(1 + 2, n2);

      int n3 = H2PP_VARIADIC_CALL(M, 1, 2, 3);
      OK(1 + 2 + 3, n3);

#undef M0
#undef M1
#undef M2
#undef M3
   }

   Case(H2PP_NARG)
   {
      int n0 = H2PP_NARG();
      int n1a = H2PP_NARG(a);
      int n1b = H2PP_NARG((a));
      int n1c = H2PP_NARG((a, b));
      int n1d = H2PP_NARG((a, b, c));
      int n2a = H2PP_NARG(a, b);
      int n2b = H2PP_NARG((a, b), c);
      int n3 = H2PP_NARG(a, b, c);

      OK(0, n0);
      OK(1, n1a);
      OK(1, n1b);
      OK(1, n1c);
      OK(1, n1d);
      OK(2, n2a);
      OK(2, n2b);
      OK(3, n3);
   }

   Case(H2PP_REPEAT)
   {
#define M0(Dummy, i) int n##i = i;
      H2PP_REPEAT((), M0, , 0);
#undef M0
      int n0, n1, n2, n3;

#define Ma(Dummy, i) n##i = 100 + i
      H2PP_REPEAT((), Ma, , 1);
      OK(100, n0);
#undef Ma

#define Mb(a, i) n##i = 200 + i + a
      H2PP_REPEAT((), Mb, 0, 1);
      OK(200, n0);
#undef Mb

#define Mc(Dummy, i) n##i = 300 + i
      H2PP_REPEAT((, ), Mc, , 2);
      OK(300, n0);
      OK(301, n1);
#undef Mc
   }

   Case(H2PP_FOREACH)
   {
      int sa = 0;
#define Fora(Dummy, i, x) sa += x * i;
      H2PP_FOREACH(, Fora, (), 1, 2, 3)
#undef Fora
      OK(1 * 0 + 2 * 1 + 3 * 2, sa);

      int sb = 0;
#define Forb(Dummy, i, x) sb += x * i;
      H2PP_FOREACH((), Forb, (), 1, 2, 3)
#undef Forb
      OK(1 * 0 + 2 * 1 + 3 * 2, sb);

      int sc = 0;
#define Forc(Dummy, i, x) sc += x * i
      H2PP_FOREACH((;), Forc, (), 1, 2, 3);
#undef Forc
      OK(1 * 0 + 2 * 1 + 3 * 2, sc);
   }

   Case(H2PP_UNIQUE)
   {
      int H2PP_UNIQUE() = 1;
      int H2PP_UNIQUE() = 1;
      int H2PP_UNIQUE(prefix) = 1;
      int H2PP_UNIQUE(prefix) = 1;
   }
}

SUITE(Iterator i j)
{
   Case(H2PP_STR)
   {
      const char* e[] = {"0.a", "1.b", "2.c"};
#define Fora(Dummy, i, x) OK(e[i], H2PP_STR(i.x));
      H2PP_FOREACH(, Fora, (), a, b, c)
#undef Fora
   }

   Case(H2PP_UNIQUE)
   {
      struct S {
#define Fora(Dummy, i, x) int H2PP_UNIQUE(x) = i;

         H2PP_FOREACH(, Fora, (), a, b, c)
#undef Fora
      } s;

      OK(0, ((int*)(&s))[0]);
      OK(1, ((int*)(&s))[1]);
      OK(2, ((int*)(&s))[2]);
   }

   Case(H2PP_UNIQUE)
   {
      struct S {
#define Fora(Dummy, i, x) int H2PP_UNIQUE(i) = i;

         H2PP_FOREACH(, Fora, (), a, b, c)
#undef Fora
      } s;

      OK(0, ((int*)(&s))[0]);
      OK(1, ((int*)(&s))[1]);
      OK(2, ((int*)(&s))[2]);
   }
}

SUITE(H2PP_FULLMESH)
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

   Case(empty)
   {
      int sa = 0;
#define Fma(Dummy, i, j, x, y) sa += x * y * i * j;
      H2PP_FULLMESH(, Fma, ());
#undef Fma
      OK(0, sa);
   }

   Case(zero)
   {
      int sa = 0;
#define Fma(Dummy, i, j, x, y) sa += x * y * i * j;
      H2PP_FULLMESH(, Fma, (), ());
#undef Fma
      OK(0, sa);
   }

   Case(one)
   {
      int sa = 0;
#define Fma(Dummy, i, j, x, y) sa += x * y * (i + 1) * (j + 1);
      H2PP_FULLMESH(, Fma, (), (1));
#undef Fma
      OK(1, sa);
   }

   Case(no split)
   {
      int sa = 0;
#define Fma(Dummy, i, j, x, y) sa += x * y * i * j;
      H2PP_FULLMESH(, Fma, (), (1, 2, 3));
#undef Fma
      OK(e, sa);
   }

   Case(empty split)
   {
      int sb = 0;
#define Fmb(Dummy, i, j, x, y) sb += x * y * i * j;
      H2PP_FULLMESH((), Fmb, (), (1, 2, 3));
#undef Fmb
      OK(e, sb);
   }

   Case(has split)
   {
      int sc = 0;
#define Fmc(Dummy, i, j, x, y) sc += x * y * i * j
      H2PP_FULLMESH((;), Fmc, (), (1, 2, 3));
#undef Fmc
      OK(e, sc);
   }
}