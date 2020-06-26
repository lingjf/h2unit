#include "../source/h2_unit.cpp"

namespace {
int foobar(int a, const char* b)
{
   return 0;
}

struct a_struct {
   char a[100];
   int b;
   char c[200];
};

class a_construct_class {
 public:
   a_construct_class(int, int, int, int, int, int, int, int, int, int, int) {}
   int func() { return 0; }
};

class a_abstract_class {
 public:
   a_abstract_class() {}
   virtual ~a_abstract_class() {}
   virtual int func() = 0;
};

#if 0

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
      const unsigned char e[] = "abcdefghijklmnopqrstuvwxyz";
      OK(Me(e, sizeof(e)), "abcdEfghijklmnopqrsTuvwxyz");  // failure
   }
   Case(bits failure)
   {
      unsigned char a1[] = {0x8E, 0xC8, 0x8E, 0xC8, 0xF8};
      OK(M1e("1000 1110 1110 1000 1000 1111 1100 1000 0000 1"), a1);  // failure
   }
   Case(short failure)
   {
      short e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      short a2[] = {1, 2, 3, 0x5555, 5, 6, 7, 8, 9};
      OK(M16e(e, 9), a2);  // failure
   }
   Case(int failure)
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      int a2[] = {1, 2, 3, 0x55555555, 5, 6, 7, 8, 9};
      OK(M32e(e, 9), a2);  // failure
   }
   Case(long long failure)
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      long long a2[] = {1, 2, 3, 0x5555555555555555LL, 5, 6, 7, 8, 9};
      OK(M64e(e, 9), a2);  // failure
   }
}

SUITE(Memory asymmetric allocate and free)
{
   Case(malloc - delete failure)
   {
      char* p = (char*)malloc(100);
      delete p;
   }
   Case(malloc - delete[] failure)
   {
      char* p = (char*)malloc(100);
      delete[] p;
   }
   Case(new - free failure)
   {
      char* p = (char*)new char;
      free(p);
   }
   Case(new - delete[] failure)
   {
      char* p = (char*)new char;
      delete[] p;
   }
   Case(new[] - free failure)
   {
      char* p = (char*)new char[100];
      free(p);
   }
}

SUITE(Ilegal Access)
{
   Case(double free failure)
   {
      void* p = malloc(100);
      free(p);
      free(p);
   }

   Case(memory underflow failure)
   {
      char* p = (char*)malloc(6);
      memcpy(p - 50, "123", 3);
      free(p);
   }

   Case(memory overflow failure)
   {
      char* p = (char*)malloc(6);
      memcpy(p, "12345678901234567890123456789012345678901234567890", 50);
      free(p);
   }

   Case(read after free failure)
   {
      a_struct* p = (a_struct*)malloc(sizeof(a_struct));
      free(p);

      int b = p->b;
   }
   Case(write after free failure)
   {
      a_struct* p = (a_struct*)malloc(sizeof(a_struct));
      free(p);

      p->b = 100;
   }
}

SUITE(MOCK fails)
{
   Case(greed true)
   {
      MOCK(foobar, int(int, const char*))
        .greed(true)
        .between(1, 3)
        .with(1, "A")
        .returns(11)
        .once(1, _)
        .returns(22);

      OK(11, foobar(1, "A"));
      OK(22, foobar(1, "A"));
   }
}

#endif

}  // namespace
