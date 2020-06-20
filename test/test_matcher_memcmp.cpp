#include "../source/h2_unit.cpp"

SUITE(memory compares, memcmp)
{
   Case(M8e)
   {
      const unsigned char e1[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      const char* e2 = "\xA1\xB2\xC3\xD4\xE5";
      const unsigned char a[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      OK(M8e(e1, 5), a);
      OK(M8e(e2, 5), a);
   }

   Case(M16e)
   {
      short e1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      const char* e2 = "\x01\x00\x02\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08\x00\x09\x00";
      short a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(M16e(e1, 9), a);
      OK(M16e(e2, 9), a);
   }

   Case(M32e)
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(M32e(e, 9), a);
   }

   Case(M64e)
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      long long a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      OK(M64e(e, 9), a);
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

SUITE(memory compares, M1e, deduce content by string)
{
   Case(A1B2C3D4E5)
   {
      unsigned char a[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      OK(M1e("1010 0001 1011 0010 1100 0011 1101 0100 1110 0101"), a);
      OK(M1e("A1B2C3D4E5"), a);
   }

   Case(A1B2C3D4E5 explicit length)
   {
      unsigned char a[] = {0xA1, 0xB2, 0xC3, 0xD4, 0x55};
      OK(M1e("1010 0001 1011 0010 1100 0011 1101 0100 1110 0101", 40 - 8), a);
      OK(M1e("A1B2C3D4E5", 40 - 8), a);
      unsigned char e[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      OK(M1e(e, 40 - 8), a);
   }
}

SUITE(memory compares, M8e, deduce content by string)
{
   Case(A1B2C3D4E5)
   {
      const char* e = "A1B2C3D4E5";
      const unsigned char a1[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      OK(M8e(e), a1);
      const char* a2 = "A1B2C3D4E5";
      OK(M8e(e), a2);
   }

   Case(A1B2C3D4E5 explicit length)
   {
      const char* e = "A1B2C3D4E5";
      const unsigned char a1[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      OK(M8e(e, 5), a1);
      const char* a2 = "A1B2C3D4E5";
      OK(M8e(e, 10 - 1), a2);
   }
}

SUITE(memory compares, M16e, deduce content by string)
{
   Case(A1B2C3D4E5F6)
   {
      const char* e = "A1B2C3D4E5F6";
      const unsigned short a1[] = {0xB2A1, 0xD4C3, 0xF6E5};
      OK(M16e(e), a1);
      const char* a2 = "A1B2C3D4E5F6";
      OK(M16e(e), a2);  // only want to display as word wide
   }

   Case(A1B2C3D4E5F6 explicit length)
   {
      const char* e = "A1B2C3D4E5F6";
      const unsigned short a1[] = {0xB2A1, 0xD4C3, 0xF6E5};
      OK(M16e(e, 3), a1);
      const char* a2 = "A1B2C3D4E5F6";
      OK(M16e(e, 3 - 1), a2);
   }
}

SUITE(memory compares, M32e, deduce content by string)
{
   Case(A1B2C3D4A1B2C3D4)
   {
      const char* e = "A1B2C3D4A1B2C3D4";
      const unsigned int a1[] = {0xD4C3B2A1, 0xD4C3B2A1};
      OK(M32e(e), a1);
      const char* a2 = "A1B2C3D4A1B2C3D4";
      OK(M32e(e), a2);
   }
   Case(A1B2C3D4A1B2C3D4 explicit length)
   {
      const char* e = "A1B2C3D4A1B2C3D4";
      const unsigned int a1[] = {0xD4C3B2A1, 0xD4C3B2A1};
      OK(M32e(e, 2), a1);
      const char* a2 = "A1B2C3D4A1B2C3D4";
      OK(M32e(e, 2 - 1), a2);
   }
}

SUITE(memory compares, M64e, deduce content by string)
{
   Case(A1B2C3D40000E5F6A1B2C3D40000E5F6)
   {
      const char* e = "A1B2C3D40000E5F6A1B2C3D40000E5F6";
      const unsigned long long a1[] = {0xF6E50000D4C3B2A1ULL, 0xF6E50000D4C3B2A1ULL};
      OK(M64e(e), a1);
      const char* a2 = "A1B2C3D40000E5F6A1B2C3D40000E5F6";
      OK(M64e(e), a2);
   }

   Case(A1B2C3D40000E5F6A1B2C3D40000E5F6 explicit length)
   {
      const char* e = "A1B2C3D40000E5F6A1B2C3D40000E5F6";
      const unsigned long long a1[] = {0xF6E50000D4C3B2A1ULL, 0xF6E50000D4C3B2A1ULL};
      OK(M64e(e, 2), a1);
      const char* a2 = "A1B2C3D40000E5F6A1B2C3D40000E5F6";
      OK(M64e(e, 2 - 1), a2);
   }
}

SUITE(memory compares, deduce content by string)
{
   Case(1010 0001)
   {
      unsigned char a[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      OK(Me("1010 0001 1011 0010 1100 0011 1101 0100 1110 0101"), a);
   }

   Case(1010 0001 explicit length)
   {
      unsigned char a[] = {0xA1, 0xB2, 0xC3, 0xD4, 0x55};
      OK(Me("1010 0001 1011 0010 1100 0011 1101 0100 1110 0101", 32), a);
   }

   Case(A1B2C3D4E5)
   {
      unsigned char a1[] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5};
      const char* a2 = "A1B2C3D4E5";
      OK(Me("A1B2C3D4E5"), a1);
      OK(Me("A1B2C3D4E5"), a2);
   }

   Case(A1B2C3D4E5 explicit length)
   {
      unsigned char a1[] = {0xA1, 0xB2, 0xC3, 0xD4, 0x55};
      const char* a2 = "A1B2C3D455";
      OK(Me("A1B2C3D4E5", 4), a1);
      OK(Me("A1B2C3D4E5", 8), a2);
   }
}
