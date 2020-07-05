#include "../source/h2_unit.cpp"

SUITE(ListOf)
{
   Case(only one not same)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 6, 3), a);
   }
   Case(two not same)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 6, 9), a);
   }
   Case(only one miss)
   {
      std::vector<int> a = {1, 2, 3};
      OK(ListOf(1, 2, 3, 4), a);
   }
   Case(Not)
   {
      std::vector<int> a = {1, 2, 3};
      OK(!ListOf(1, 2, 3), a);
   }
}

SUITE(Has)
{
   Case(miss only one)
   {
      std::vector<int> a = {1, 2, 3};
      OK(Has(8), a);
   }
   Case(miss two)
   {
      std::vector<int> a = {1, 2, 3};
      OK(Has(8, 9), a);
   }
   Case(miss two)
   {
      std::vector<int> a = {1, 2, 3};
      OK(!Has(2), a);
   }
}

SUITE(Logic)
{
   Case(Not string equal failure)
   {
      const char* p = "a7af3452a5eee24a149f8993e48ce81b77b200b212b23c2402c221ddc4"
                      "260a608cf328a2133386477eb7a65958893726741cb9223419a7a0224f"
                      "e7af2866944197ee4ab18e25913cae8507bb306a284f";
      OK(Not(p), p);
   }

   Case(&& || !failure)
   {
      OK(Ge(0) && Eq(-2) || -4, -1);  // failure
   }

   Case(AllOf)
   {
      OK(AllOf("Rect(0, 0)", _, EndsWith("R(1, 2)")), "Rect()");
   }
   Case(Not AllOf)
   {
      OK(!AllOf("Rect(0, 0)", _, EndsWith("(0, 0)")), "Rect(0, 0)");
   }

   Case(AnyOf)
   {
      OK(AnyOf("Rect(0, 1)", StartsWith("Rect")), "Pect(0, 1)");
   }

   Case(NoneOf successful)
   {
      OK(NoneOf("Rect(0, 1)", EndsWith("R(1, 2)")), "Rect(0, 1)");  // successful
   }
}

SUITE(memory compares)
{
   Case(bytes failure)
   {
      const unsigned char e[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
      OK(Me(e, sizeof(e)), "abcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyzabcdEfghijklmnopqrsTuvwxyz");
   }
   Case(bits failure)
   {
      unsigned char a1[] = {0x8E, 0xC8, 0x8E, 0xC8, 0xF8};
      OK(M1e("1000 1110 1110 1000 1000 1111 1100 1000 0000 1"), a1);
   }
   Case(short failure)
   {
      short e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      short a2[] = {1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0, 1, 2, 3, 0x5555, 5, 6, 7, 8, 9, 0};
      OK(M16e(e, 100), a2);
   }
   Case(int failure)
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      int a2[] = {1, 2, 3, 0x55555555, 5, 6, 7, 8, 9, 0};
      OK(M32e(e, 10), a2);
   }
   Case(long long failure)
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
      long long a2[] = {1, 2, 3, 0x5555555555555555LL, 5, 6, 7, 8, 9, 0};
      OK(M64e(e, 10), a2);
   }
}
