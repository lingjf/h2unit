#include "../source/h2_unit.cpp"

SUITE(memory compares explicitly)
{
   Case(bytes)
   {
      const unsigned char e[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      const unsigned char a[] = {0xA1, 0xB2, 0xC3, 0xD4, 0x5E};
      OK(Me(e, 4, 8), a);
   }

   Case(short)
   {
      short e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      short a[] = {1, 2, 3, 4, 5, 6, 7, 8, 90};
      OK(Me(e, 8, 16), a);
   }

   Case(int)
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 90};
      OK(Me(e, 7, 32), a);
   }

   Case(long long)
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      long long a[] = {1, 2, 3, 4, 5, 6, 7, 8, 90};
      OK(Me(e, 8, 64), a);
   }

   Case(bit)
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 0x90};
      int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 0x9F};
      OK(Me(e, 8 * 32 + 4, 1), a);
   }
}

SUITE(memory compares, deduce width by date type)
{
   Case(unsigned char)
   {
      const unsigned char e[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      const unsigned char a[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      OK(Me(e, 5), a);
   }

   Case(short)
   {
      short e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      short a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(Me(e, 9), a);
   }

   Case(int)
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(Me(e, 9), a);
   }

   Case(long long)
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      long long a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(Me(e, 9), a);
   }
}

SUITE(memory compares, deduce width by date type, deduce length by array size)
{
   Case(unsigned char)
   {
      const unsigned char e[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      const unsigned char a[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      OK(Me(e), a);
   }

   Case(short)
   {
      short e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      short a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(Me(e), a);
   }

   Case(int)
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(Me(e), a);
   }

   Case(long long)
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      long long a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(Me(e), a);
   }
}

SUITE(memory compares, deduce width and length by string format)
{
   Case("1010 0001")
   {
      unsigned char a1[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      OK(Me("1010 0001 1011 0010 1100 0011 1101 0100 1110 0101"), a1);
   }

   Case("A1B2C3D4E5")
   {
      unsigned char a1[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      OK(Me("A1B2C3D4E5"), a1);
   }

   Case(fallback)
   {
      const char* a2 = "A1B2C3D4E5";
      OK(Me("A1B2C3D4E5"), a2);
   }
}

SUITE(memory compares, string)
{
   Case(with length)
   {
      const char* a2 = "A1B2C3Dxxx";
      OK(Me("A1B2C3Dyyy", 7), a2);
   }
}
