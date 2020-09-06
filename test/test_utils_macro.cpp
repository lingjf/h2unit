#include "../source/h2_unit.cpp"

SUITE(macro)
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

   Case(H2PP_TH0)
   {
      int n0 H2PP_TH0();
      int n1 = H2PP_TH0(0);
      OK(0, n1);
      int n2 = H2PP_TH0(0, 1);
      OK(0, n2);
   }

   Case(H2PP_TH1)
   {
      int n0 H2PP_TH1();
      int n1 H2PP_TH1(0);
      int n2 = H2PP_TH1(0, 1);
      OK(1, n2);
      int n3 = H2PP_TH1(0, 1, 2);
      OK(1, n3);
   }

   Case(H2PP_TH9)
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

   Case(H2PP_TH32)
   {
      int n0 = H2PP_TH0(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(0, n0);
      int n1 = H2PP_TH1(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(1, n1);
      int n2 = H2PP_TH2(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(2, n2);
      int n3 = H2PP_TH3(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(3, n3);
      int n4 = H2PP_TH4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(4, n4);
      int n5 = H2PP_TH5(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(5, n5);
      int n6 = H2PP_TH6(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(6, n6);
      int n7 = H2PP_TH7(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(7, n7);
      int n8 = H2PP_TH8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(8, n8);
      int n9 = H2PP_TH9(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(9, n9);

      int n10 = H2PP_TH10(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(10, n10);
      int n11 = H2PP_TH11(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(11, n11);
      int n12 = H2PP_TH12(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(12, n12);
      int n13 = H2PP_TH13(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(13, n13);
      int n14 = H2PP_TH14(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(14, n14);
      int n15 = H2PP_TH15(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(15, n15);
      int n16 = H2PP_TH16(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(16, n16);
      int n17 = H2PP_TH17(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(17, n17);
      int n18 = H2PP_TH18(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(18, n18);
      int n19 = H2PP_TH19(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(19, n19);

      int n20 = H2PP_TH20(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(20, n20);
      int n21 = H2PP_TH21(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(21, n21);
      int n22 = H2PP_TH22(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(22, n22);
      int n23 = H2PP_TH23(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(23, n23);
      int n24 = H2PP_TH24(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(24, n24);
      int n25 = H2PP_TH25(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(25, n25);
      int n26 = H2PP_TH26(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(26, n26);
      int n27 = H2PP_TH27(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(27, n27);
      int n28 = H2PP_TH28(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(28, n28);
      int n29 = H2PP_TH29(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(29, n29);

      int n30 = H2PP_TH30(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(30, n30);
      int n31 = H2PP_TH31(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(31, n31);
      int n32 = H2PP_TH32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
      OK(32, n32);
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

   Case(H2PP_REMOVE_PARENTHESES)
   {
      int n[] = {H2PP_REMOVE_PARENTHESES((1, 2, 3))};
      OK(1, n[0]);
      OK(2, n[1]);
      OK(3, n[2]);
   }

   Case(H2PP_RPS)
   {
      int n[] = {H2PP_RPS((1, 2, 3))};
      OK(1, n[0]);
      OK(2, n[1]);
      OK(3, n[2]);
   }

   Case(H2PP_IBP)
   {
      OK(0, H2PP_IBP());
      OK(0, H2PP_IBP(a));
      OK(1, H2PP_IBP((a)));
      OK(1, H2PP_IBP((a, b)));
      // OK(1, H2PP_IBP((a, b), 1)); // compile error
      OK(1, H2PP_IBP(()));
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
      int n0[3] = {H2PP_IF(0)(10)};
      OK(0, n0[0]);
      int n1[3] = {H2PP_IF(1)(11)};
      OK(11, n1[0]);
      int n2[3] = {H2PP_IF(13)(12)};
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
#define M0() \
   {         \
   }
#define M1(_1) \
   {           \
      _1       \
   }
#define M2(_1, _2) \
   {               \
      _1, _2       \
   }
#define M3(_1, _2, _3) \
   {                   \
      _1, _2, _3       \
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
#undef M0
#undef M1
#undef M2
#undef M3
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

   Case(H2PP_REPEAT 0)
   {
#define M0(i) int n##i = i;
      H2PP_REPEAT(0, (), M0);
#undef M0
   }

   Case(H2PP_REPEAT 1)
   {
      int n0;

#define Ma(i) n##i = i
      H2PP_REPEAT(1, (), Ma);
      OK(0, n0);
#undef Ma

#define Mb(i) n##i = i
      H2PP_REPEAT(1, (), Mb, );
      OK(0, n0);
#undef Mb

#define Mc(i, a, b) n##i = i + a + b
      H2PP_REPEAT(1, (), Mc, 0, 0);
      OK(0, n0);
#undef Mc

#define Md(i) n##i = i
      H2PP_REPEAT(1, (, ), Md);
      OK(0, n0);
#undef Md
   }

   Case(H2PP_REPEAT 2)
   {
      int n0, n1;

#define Ma(i) n##i = i
      H2PP_REPEAT(2, (, ), Ma);
      OK(0, n0);
      OK(1, n1);
#undef Ma

#define Mb(i, a, b) n##i = i + a + b
      H2PP_REPEAT(1, (, ), Mb, 0, 0);
      OK(0, n0);
      OK(1, n1);
#undef Mb
   }

   Case(H2PP_REPEAT)
   {
      int n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16, n17, n18, n19, n20, n21, n22, n23, n24, n25, n26, n27, n28, n29, n30, n31;
      int n[32];
      for (int i = 0; i < 32; i++) n[i] = -1;
#define Ma(i) n[i] = i
      H2PP_REPEAT(32, (, ), Ma);
      for (int i = 0; i < 32; i++) {
         OK(i, n[i]);
      }
#undef Ma

#define Mb(i, a) n[i] = i + a
      H2PP_REPEAT(32, (, ), Mb, 1);
      for (int i = 0; i < 32; i++) {
         OK(i + 1, n[i]);
      }
#undef Mb

#define Mc(i) n[i] = i;
      H2PP_REPEAT(32, (), Mc);
      for (int i = 0; i < 32; i++) {
         OK(i, n[i]);
      }
#undef Mc

#define Md(i, a) n[i] = i + a;
      H2PP_REPEAT(32, , Md, 1);
      for (int i = 0; i < 32; i++) {
         OK(i + 1, n[i]);
      }
#undef Md
   }
}

CASE(H2Q generate unique symbol)
{
   int H2Q() = 1;
   int H2Q() = 1;
   int H2Q(prefix) = 1;
   int H2Q(prefix) = 1;
}
