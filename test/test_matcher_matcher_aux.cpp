#include "../source/h2_unit.cpp"

struct Gf {
   const char* path;
   size_t len = 0;
   size_t ret = 0;

   Gf(const char* filename, unsigned char* data, size_t size) : path(filename), len(size)
   {
      FILE* filp = ::fopen(path, "wb");
      if (filp) {
         ret = fwrite(data, 1, len, filp);
         fclose(filp);
      }
   }

   ~Gf()
   {
      ::remove(path);
   }
};

SUITE(File primitive [api])
{
   Case(strcmp)
   {
      Gf f1("_temp_full.txt", (unsigned char*)"hello h2unit!", 13);
      OK(File("_temp_full.txt"), "hello h2unit!");
      OK(Se(File("_temp_full.txt")), "hello h2unit!");

      Gf f2("_temp_sub.txt", (unsigned char*)"hello", 5);
      OK(Substr(File("_temp_sub.txt")), "hello h2unit!");

      Gf f3("_temp_wildcard.txt", (unsigned char*)"hello?h*!", 9);
      OK(We(File("_temp_wildcard.txt")), "hello h2unit!");
   }

   Case(memcmp)
   {
      Gf f1("_temp_bytes.dat", (unsigned char*)"hello\0h2unit!\0", 14);
      OK(Me(File("_temp_bytes.dat")), "hello\0h2unit!\0");
   }
}

SUITE(IsEven IsOdd primitive [api])
{
   Case(zero)
   {
      OK(IsEven(), 0);
      OK(!IsOdd(), 0);
   }

   Case(positive integer)
   {
      OK(!IsEven(), 1);
      OK(IsOdd(), 1);
      OK(IsEven(), 2);
      OK(!IsOdd(), 2);
      OK(!IsEven(), 3);
      OK(IsOdd(), 3);
      OK(IsEven(), 4);
      OK(!IsOdd(), 4);
   }

   Case(negtive integer)
   {
      OK(!IsEven(), -1);
      OK(IsOdd(), -1);
      OK(IsEven(), -2);
      OK(!IsOdd(), -2);
   }

   // Case(double)
   // {
   //    OK(!IsEven(), 1.2);
   // }
}

SUITE(IsDivable primitive [api])
{
   Case(positive integer)
   {
      OK(IsDivable(2), 4);
      OK(!IsDivable(3), 4);
   }

   Case(negtive integer)
   {
      OK(IsDivable(2), -4);
      OK(!IsDivable(3), -4);
      OK(IsDivable(-2), -4);
      OK(!IsDivable(-3), -4);
   }

   // Case(double)
   // {
   //    OK(!IsEven(), 1.2);
   // }
}

SUITE(IsInteger primitive [api])
{
   Case(positive integer)
   {
      OK(IsInteger(), 4);
      OK(!IsInteger(), 4.2);
   }

   Case(negtive integer)
   {
      OK(IsInteger(), -4);
      OK(!IsInteger(), -4.2);
   }
}

SUITE(IsNaN primitive [api])
{
   Case(positive integer)
   {
      OK(IsNaN(), NAN);
   }
}
