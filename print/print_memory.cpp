#include "../build/h2unit.hpp"

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
      strcpy(p - 50, "underflow");
      free(p);
   }

   Case(memory overflow failure)
   {
      char* p = (char*)malloc(6);
      strcpy(p, "________10________20________20________40overflow50");
      free(p);
   }

   Case(use after free failure)
   {
      A_struct* read_after_free = (A_struct*)malloc(sizeof(A_struct));
      free(read_after_free);
      int b = read_after_free->b;

      A_struct* write_after_free = (A_struct*)malloc(sizeof(A_struct));
      free(write_after_free);
      write_after_free->b = 100;
   }
}

SUITE(Memory Leak)
{
   Case(1 times leak)
   {
      int* p = (int*)malloc(10);
      *p = 0;
   }

   Case(20 times leak)
   {
      for (int i = 0; i < 10; ++i) {
         int* p = (int*)malloc(10);
         *p = i;
      }

      for (int i = 0; i < 10; ++i) {
         char* p = (char*)malloc(i + 1);
         *p = i;
      }
   }
}
