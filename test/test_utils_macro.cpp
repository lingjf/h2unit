
#ifdef H2UNIT_SELF_UT

#if defined __GNUC__ || defined __clang__
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "../source/h2_unit.cpp"

#else

// debug macro only
// gcc test_utils_macro.cpp -E

#include "../source/utils/h2_macro.hpp"
#include "../source/utils/h2_macro.in.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUITE(...) int main(int argc, char** argv)
#define Case(...) for (const char* _cn_ = #__VA_ARGS__; _cn_; printf("success %s \n", _cn_), _cn_ = NULL)
#define OK(e, a)                                              \
   if ((e) != (a)) {                                          \
      printf("failed %s %s:%d \n", _cn_, __FILE__, __LINE__); \
      return 1;                                               \
   }

#endif

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

   Case(H2PP_UNIQUE)
   {
      int H2PP_UNIQUE() = 1;
      int H2PP_UNIQUE() = 1;
      int H2PP_UNIQUE(prefix) = 1;
      int H2PP_UNIQUE(prefix) = 1;
   }

   Case(H2PP_NCALL n=0)
   {
#define N0A0__0() 100
#define N0A0__1() 101
      int n0a0 = H2PP_NCALL(N0A0__, 0);
      OK(101, n0a0);
#undef N0A0__0
#undef N0A0__1

#define N0A1__0(_1) (_1) * 100
#define N0A1__1(_1) (_1) * 101
      int n0a1 = H2PP_NCALL(N0A1__, 0, 1);
      OK((1) * 100, n0a1);
#undef N0A1__0
#undef N0A1__1
   }

   Case(H2PP_NCALL n=1)
   {
#define N1A0__0() 100
#define N1A0__1() 101
      int n1a0 = H2PP_NCALL(N1A0__, 1);
      OK(100, n1a0);
#undef N1A0__0
#undef N1A0__1

#define N1A1__0(_1) (_1) * 100
#define N1A1__1(_1) (_1) * 101
      int n1a1 = H2PP_NCALL(N1A1__, 1, 1);
      OK((1) * 101, n1a1);
#undef N1A1__0
#undef N1A1__1

#define N1A2__0(_1, _2) (_1 + _2) * 100
#define N1A2__1(_1, _2) (_1 + _2) * 101
      int n1a2 = H2PP_NCALL(N1A2__, 1, 1, 2);
      OK((1 + 2) * 100, n1a2);
#undef N1A2__0
#undef N1A2__1
   }

   Case(H2PP_NCALL n=2)
   {
#define N2A0__0() 100
#define N2A0__1() 101
      int n2a0 = H2PP_NCALL(N2A0__, 2);
      OK(100, n2a0);
#undef N2A0__0
#undef N2A0__1

#define N2A1__0(_1) (_1) * 100
#define N2A1__1(_1) (_1) * 101
      int n2a1 = H2PP_NCALL(N2A1__, 2, 1);
      OK((1) * 100, n2a1);
#undef N2A1__0
#undef N2A1__1

#define N2A2__0(_1, _2) (_1 + _2) * 100
#define N2A2__1(_1, _2) (_1 + _2) * 101
      int n2a2 = H2PP_NCALL(N2A2__, 2, 1, 2);
      OK((1 + 2) * 101, n2a2);
#undef N2A2__0
#undef N2A2__1

#define N2A3__0(_1, _2, _3) (_1 + _2 + _3) * 100
#define N2A3__1(_1, _2, _3) (_1 + _2 + _3) * 101
      int n2a3 = H2PP_NCALL(N2A3__, 2, 1, 2, 3);
      OK((1 + 2 + 3) * 100, n2a3);
#undef N2A3__0
#undef N2A3__1
   }

   Case(H2PP_NCALL n=3)
   {
#define N3A0__0() 100
#define N3A0__1() 101
      int n3a0 = H2PP_NCALL(N3A0__, 3);
      OK(100, n3a0);
#undef N3A0__0
#undef N3A0__1

#define N3A1__0(_1) (_1) * 100
#define N3A1__1(_1) (_1) * 101
      int n3a1 = H2PP_NCALL(N3A1__, 3, 1);
      OK((1) * 100, n3a1);
#undef N3A1__0
#undef N3A1__1

#define N3A2__0(_1, _2) (_1 + _2) * 100
#define N3A2__1(_1, _2) (_1 + _2) * 101
      int n3a2 = H2PP_NCALL(N3A2__, 3, 1, 2);
      OK((1 + 2) * 100, n3a2);
#undef N3A2__0
#undef N3A2__1

#define N3A3__0(_1, _2, _3) (_1 + _2 + _3) * 100
#define N3A3__1(_1, _2, _3) (_1 + _2 + _3) * 101
      int n3a3 = H2PP_NCALL(N3A3__, 3, 1, 2, 3);
      OK((1 + 2 + 3) * 101, n3a3);
#undef N3A3__0
#undef N3A3__1

#define N3A4__0(_1, _2, _3, _4) (_1 + _2 + _3 + _4) * 100
#define N3A4__1(_1, _2, _3, _4) (_1 + _2 + _3 + _4) * 101
      int n3a4 = H2PP_NCALL(N3A4__, 3, 1, 2, 3, 4);
      OK((1 + 2 + 3 + 4) * 100, n3a4);
#undef N3A4__0
#undef N3A4__1
   }

   Case(H2PP_VCALL)
   {
#define M0() 0
#define M1(_1) _1
#define M2(_1, _2) _1 + _2
#define M3(_1, _2, _3) _1 + _2 + _3

      int n0 = H2PP_VCALL(M);
      OK(0, n0);

      int n1 = H2PP_VCALL(M, 1);
      OK(1, n1);

      int n2 = H2PP_VCALL(M, 1, 2);
      OK(1 + 2, n2);

      int n3 = H2PP_VCALL(M, 1, 2, 3);
      OK(1 + 2 + 3, n3);

#undef M0
#undef M1
#undef M2
#undef M3
   }

   Case(H2PP_NARG normal 0)
   {
      int n = H2PP_NARG();
      OK(0, n);
   }

   Case(H2PP_NARG normal 1)
   {
      int n = H2PP_NARG(a);
      OK(1, n);
   }

   Case(H2PP_NARG normal 2)
   {
      int n = H2PP_NARG(a, b);
      OK(2, n);
   }

   Case(H2PP_NARG normal 3)
   {
      int n = H2PP_NARG(a, b, c);
      OK(3, n);
   }

   Case(H2PP_NARG parenthesis 1)
   {
      int n = H2PP_NARG((a));
      OK(1, n);
   }

   Case(H2PP_NARG parenthesis 2)
   {
      int n = H2PP_NARG((a), (b));
      OK(2, n);
   }

   Case(H2PP_NARG two parenthesis 1)
   {
      int n = H2PP_NARG((a, b));
      OK(1, n);
   }

   Case(H2PP_NARG two parenthesis 2)
   {
      int n = H2PP_NARG((a, b), (c));
      OK(2, n);
   }

   Case(H2PP_NARG mix)
   {
      int n = H2PP_NARG((a, b), c);
      OK(2, n);
   }

   Case(H2PP_REPEAT repeat 0 times)
   {
#define RepeatCB(Dummy, i) int n##i = i;
      H2PP_REPEAT((), RepeatCB, , 0);
#undef RepeatCB

      int n0 = 0, n1 = 1, n2 = 2, n3 = 3;
      OK(0, n0);
      OK(1, n1);
      OK(2, n2);
      OK(3, n3);
   }

   Case(H2PP_REPEAT repeat 1 times)
   {
      int n0 = 0, n1 = 1, n2 = 2, n3 = 3;

#define RepeatCB(Dummy, i) n##i = 100 + i;
      H2PP_REPEAT((), RepeatCB, , 1);
#undef RepeatCB

      OK(100, n0);
      OK(1, n1);
      OK(2, n2);
      OK(3, n3);
   }

   Case(H2PP_REPEAT repeat 2 times)
   {
      int n0 = 0, n1 = 1, n2 = 2, n3 = 3;

#define RepeatCB(Dummy, i) n##i = 100 + i;
      H2PP_REPEAT((), RepeatCB, , 2);
#undef RepeatCB

      OK(100, n0);
      OK(101, n1);
      OK(2, n2);
      OK(3, n3);
   }

   Case(H2PP_REPEAT repeat 3 times)
   {
      int n0 = 0, n1 = 1, n2 = 2, n3 = 3;

#define RepeatCB(Dummy, i) n##i = 100 + i;
      H2PP_REPEAT((), RepeatCB, , 3);
#undef RepeatCB

      OK(100, n0);
      OK(101, n1);
      OK(102, n2);
      OK(3, n3);
   }

   Case(H2PP_REPEAT repeat 3 times with argument)
   {
      int n0 = 0, n1 = 1, n2 = 2, n3 = 3;

#define RepeatCB(a, i) n##i = 100 + i + a;
      H2PP_REPEAT((), RepeatCB, 20, 3);
#undef RepeatCB

      OK(120, n0);
      OK(121, n1);
      OK(122, n2);
      OK(3, n3);
   }

   Case(H2PP_FOREACH empty)
   {
      int s = 0;
#define ForEachCB(Dummy, i, x) s += x * (i + 1);
      H2PP_FOREACH(, ForEachCB, ())
#undef ForEachCB
      OK(0, s);
   }

   Case(H2PP_FOREACH zero)
   {
      int s = 0;
#define ForEachCB(Dummy, i, x) s += x * (i + 1);
      H2PP_FOREACH(, ForEachCB, (), )
#undef ForEachCB
      OK(0, s);
   }

   Case(H2PP_FOREACH no split)
   {
      int s = 0;
#define ForEachCB(Dummy, i, x) s += x * (i + 1);
      H2PP_FOREACH(, ForEachCB, (), 1, 2, 3)
#undef ForEachCB
      OK(1 * 1 + 2 * 2 + 3 * 3, s);
   }

   Case(H2PP_FOREACH empty split)
   {
      int s = 0;
#define ForEachCB(Dummy, i, x) s += x * (i + 1);
      H2PP_FOREACH((), ForEachCB, (), 1, 2, 3)
#undef ForEachCB
      OK(1 * 1 + 2 * 2 + 3 * 3, s);
   }

   Case(H2PP_FOREACH has split)
   {
      int s = 0;
#define ForEachCB(Dummy, i, x) s += x * (i + 1)
      H2PP_FOREACH((;), ForEachCB, (), 1, 2, 3);
#undef ForEachCB
      OK(1 * 1 + 2 * 2 + 3 * 3, s);
   }

   Case(H2PP_FOREACH with argument)
   {
      int s = 0;
#define ForEachCB(a, i, x) s += x * (i + 1) * a
      H2PP_FOREACH((;), ForEachCB, (2), 1, 2, 3);
#undef ForEachCB
      OK((1 * 1 + 2 * 2 + 3 * 3) * 2, s);
   }

   Case(H2PP_FOREACH index STR)
   {
      const char* e[] = {"0.a", "1.b", "2.c"};
#define ForEachCB(Dummy, i, x) OK(e[i], H2PP_STR(i.x));
      H2PP_FOREACH(, ForEachCB, (), a, b, c)
#undef ForEachCB
   }

   Case(H2PP_FOREACH index UNIQUE)
   {
      struct E3 {
         int v1[1];
         int v2[2];
         int v3[3];
      } e3;

      struct A3 {
#define ForEachCB(Dummy, i, x) int H2PP_UNIQUE(i)[x];

         H2PP_FOREACH(, ForEachCB, (), 1, 2, 3)
#undef ForEachCB
      } a3;

      struct B3 {
#define ForEachCB(Dummy, i, x) int H2PP_CAT(v, i)[x];

         H2PP_FOREACH(, ForEachCB, (), 1, 2, 3)
#undef ForEachCB
      } b3;

      OK(sizeof(e3), sizeof(a3));
      OK(sizeof(e3), sizeof(b3));
      OK(sizeof(E3), sizeof(A3));
      OK(sizeof(E3), sizeof(B3));
   }

   Case(H2PP_FULLMESH empty)
   {
      int s = 0;
#define FullMeshCB(Dummy, i, j, x, y) s += x * (i + 1) * y * (j + 1);
      H2PP_FULLMESH(, FullMeshCB, ());
#undef FullMeshCB
      OK(0, s);
   }

   int exx = 1 * 1 * 1 * 1 +
             1 * 1 * 2 * 2 +
             1 * 1 * 3 * 3 +
             2 * 2 * 1 * 1 +
             2 * 2 * 2 * 2 +
             2 * 2 * 3 * 3 +
             3 * 3 * 1 * 1 +
             3 * 3 * 2 * 2 +
             3 * 3 * 3 * 3;

   Case(H2PP_FULLMESH xx zero)
   {
      int s = 0;
#define FullMeshCB(Dummy, i, j, x, y) s += x * (i + 1) * y * (j + 1);
      H2PP_FULLMESH(, FullMeshCB, (), ());
#undef FullMeshCB
      OK(0, s);
   }

   Case(H2PP_FULLMESH xx one)
   {
      int s = 0;
#define FullMeshCB(Dummy, i, j, x, y) s += x * y * (i + 1) * (j + 1);
      H2PP_FULLMESH(, FullMeshCB, (), (1));
#undef FullMeshCB
      OK(1, s);
   }

   Case(H2PP_FULLMESH xx three)
   {
      int s = 0;
#define FullMeshCB(Dummy, i, j, x, y) s += x * (i + 1) * y * (j + 1);
      H2PP_FULLMESH(, FullMeshCB, (), (1, 2, 3));
#undef FullMeshCB
      OK(exx, s);
   }

   Case(H2PP_FULLMESH xx empty split)
   {
      int s = 0;
#define FullMeshCB(Dummy, i, j, x, y) s += x * (i + 1) * y * (j + 1);
      H2PP_FULLMESH((), FullMeshCB, (), (1, 2, 3));
#undef FullMeshCB
      OK(exx, s);
   }

   Case(H2PP_FULLMESH xx has split)
   {
      int s = 0;
#define FullMeshCB(Dummy, i, j, x, y) s += x * (i + 1) * y * (j + 1)
      H2PP_FULLMESH((;), FullMeshCB, (), (1, 2, 3));
#undef FullMeshCB
      OK(exx, s);
   }

   Case(H2PP_FULLMESH xx with argument)
   {
      int s = 0;
#define FullMeshCB(a, i, j, x, y) s += x * (i + 1) * y * (j + 1) * a
      H2PP_FULLMESH((;), FullMeshCB, (2), (1, 2, 3));
#undef FullMeshCB
      OK(exx * 2, s);
   }

   Case(H2PP_FULLMESH xx index STR)
   {
      const char* e_abc[3][3] = {
        {"0.0:a~a", "0.1:a~b", "0.2:a~c"},
        {"1.0:b~a", "1.1:b~b", "1.2:b~c"},
        {"2.0:c~a", "2.1:c~b", "2.2:c~c"}};

#define FullMeshCB(Dummy, i, j, x, y) OK(e_abc[i][j], H2PP_STR(i.j:x~y));
      H2PP_FULLMESH(, FullMeshCB, (), (a, b, c));
#undef FullMeshCB
   }

   int exy = 1 * 1 * 4 * 1 +
             1 * 1 * 5 * 2 +
             1 * 1 * 6 * 3 +
             2 * 2 * 4 * 1 +
             2 * 2 * 5 * 2 +
             2 * 2 * 6 * 3 +
             3 * 3 * 4 * 1 +
             3 * 3 * 5 * 2 +
             3 * 3 * 6 * 3;

   Case(H2PP_FULLMESH xy one)
   {
      int s = 0;
#define FullMeshCB(Dummy, i, j, x, y) s += x * y * (i + 1) * (j + 1);
      H2PP_FULLMESH(, FullMeshCB, (), (1), (1));
#undef FullMeshCB
      OK(1, s);
   }

   Case(H2PP_FULLMESH xy three)
   {
      int s = 0;
#define FullMeshCB(Dummy, i, j, x, y) s += x * (i + 1) * y * (j + 1);
      H2PP_FULLMESH(, FullMeshCB, (), (1, 2, 3), (4, 5, 6));
#undef FullMeshCB
      OK(exy, s);
   }

   Case(H2PP_FULLMESH xy empty split)
   {
      int s = 0;
#define FullMeshCB(Dummy, i, j, x, y) s += x * (i + 1) * y * (j + 1);
      H2PP_FULLMESH((), FullMeshCB, (), (1, 2, 3), (4, 5, 6));
#undef FullMeshCB
      OK(exy, s);
   }

   Case(H2PP_FULLMESH xy has split)
   {
      int s = 0;
#define FullMeshCB(Dummy, i, j, x, y) s += x * (i + 1) * y * (j + 1)
      H2PP_FULLMESH((;), FullMeshCB, (), (1, 2, 3), (4, 5, 6));
#undef FullMeshCB
      OK(exy, s);
   }

   Case(H2PP_FULLMESH xy with argument)
   {
      int s = 0;
#define FullMeshCB(a, i, j, x, y) s += x * (i + 1) * y * (j + 1) * a
      H2PP_FULLMESH((;), FullMeshCB, (2), (1, 2, 3), (4, 5, 6));
#undef FullMeshCB
      OK(exy * 2, s);
   }

   Case(H2PP_FULLMESH xy index STR)
   {
      const char* e_abcxyz[3][3] = {
        {"0.0:a~x", "0.1:a~y", "0.2:a~z"},
        {"1.0:b~x", "1.1:b~y", "1.2:b~z"},
        {"2.0:c~x", "2.1:c~y", "2.2:c~z"}};

#define FullMeshCB(Dummy, i, j, x, y) OK(e_abcxyz[i][j], H2PP_STR(i.j:x~y));
      H2PP_FULLMESH(, FullMeshCB, (), (a, b, c), (x, y, z));
#undef FullMeshCB
   }
}
