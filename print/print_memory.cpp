#include "../build/h2unit.hpp"

#if defined(__GNUC__) && __GNUC__ >= 7 && !defined(__clang__) // gcc >= 7
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif

#if defined(__GNUC__) && __GNUC__ >= 11 && !defined(__clang__) // gcc >= 11
#pragma GCC diagnostic ignored "-Wmismatched-new-delete"
#endif

SUITE(Memory asymmetric allocate and free)
{
   Case(malloc delete failure)
   {
      char* p = (char*)malloc(100);
      delete p;
   }

   Case(malloc "delete[]" failure)
   {
      char* p = (char*)malloc(100);
      delete[] p;
   }

   Case(new free failure)
   {
      char* p = (char*)new char;
      free(p);
   }

   Case(new "delete[]" failure)
   {
      char* p = (char*)new char;
      delete[] p;
   }

   Case("new[]" free failure)
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
      int* p = (int*)malloc(6);
      p[5] = 32717;
      free(p);
   }

   Case(use after free failure)
   {
      struct tm* read_after_free = (struct tm*)malloc(sizeof(struct tm));
      free(read_after_free);
      int b = read_after_free->tm_sec;

      struct tm* write_after_free = (struct tm*)malloc(sizeof(struct tm));
      free(write_after_free);
      write_after_free->tm_sec = b;
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
