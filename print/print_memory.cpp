#include "../h2unit.h"

struct A_struct {
   char a[100];
   int b;
   char c[200];
};

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
}

CASE(read after free failure)
{
   A_struct* p = (A_struct*)malloc(sizeof(A_struct));
   free(p);

   int b = p->b;
}

CASE(write after free failure)
{
   A_struct* p = (A_struct*)malloc(sizeof(A_struct));
   free(p);

   p->b = 100;
}
