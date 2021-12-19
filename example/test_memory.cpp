
/*
 * h2unit can detect memory leak which is allocated by:
 *
 * -- malloc
 * -- calloc
 * -- realloc
 * -- strdup
 * -- strndup
 * -- memalign
 * -- posix_memalign
 * -- aligned_alloc
 * -- new
 * -- new[]
 */

extern "C" {
#include "product_c.h"
}

#include "product_cpp.h"

#if defined H2UNIT && H2UNIT == 2
#include "../build/h2unit.hpp"
#else
#include "../h2unit.h"
#endif

SUITE(Memory Leak)
{
   /*
    * Memory leak detection on Test Case Level
    */
   Case(test memory leak ok successful [pass])
   {
      free(malloc(8));
   }

   Case(1 times leak [fail])
   {
      int* p = (int*)malloc(10);
      *p = 0;
   }

   Case(20 times leak [fail])
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

   Case(test memory leak [fail])
   {
      char* s = (char*)malloc(10);
      strcpy(s, "1234567");

      rectangle_t* rectangle;
      for (int i = 0; i < 3; i++) {
         rectangle = rectangle_create(1, i);
      }
      rectangle_destroy(rectangle);

      Rect* rect1 = new Rect(0, 0, 1, 1);
      OK("Rect", rect1->print());
      delete rect1;

      Rect* rect2 = new Rect(0, 0, 1, 1);
      OK("Rect", rect2->print());
   }

   /*
    * Memory leak detection on User-Defined Block Level
    */
   Case(test memory leak block [fail])
   {
      void *c1, *c2, *c3, *c4;

      c1 = malloc(1);

      BLOCK()
      {
         c2 = malloc(3);
         free(c2);
      }

      BLOCK()
      {
         c3 = malloc(5);
         free(c3);
         BLOCK() { c4 = malloc(7); }
      }

      free(c4);
      free(c1);
   }

   /*
    * h2unit can modify the total memory resource to make malloc() failed.
    */

   Case(malloc faulty injection successful [pass])
   {
      BLOCK(limit = 10 /* in the block , available memory only 10 bytes */)
      {
         OK(NULL, malloc(11)); /* no enough available memory */
      }
   }

   /*
    * h2unit can modify fill of malloc.
    */

   Case(filled malloc successful [pass])
   {
      /* in the block , malloc allocated space filled with 0xABCD */
      BLOCK(limit = 10000000, fill = 0xABCD)
      {
         char* p = (char*)malloc(8);
         OK(Me("ABCDABCD"), p);
         free(p);
      }
   }

   Case(exempt leak function [pass])
   {
      UNMEM(rectangle_create);

      rectangle_t* rectangle = rectangle_create(1, 2);
      OK(2, rectangle_area(rectangle));
   }
}

SUITE(Memory asymmetric allocate and free [fail])
{
   Case(malloc delete)
   {
      char* p = (char*)malloc(100);
      delete p;
   }

   Case(malloc "delete[]")
   {
      char* p = (char*)malloc(100);
      delete[] p;
   }

   Case(new free)
   {
      char* p = (char*)new char;
      free(p);
   }

   Case("new[]" free)
   {
      char* p = (char*)new char[100];
      free(p);
   }
}

SUITE(Ilegal Access [fail])
{
   int t;

   Case(double free)
   {
      rectangle_t* p = rectangle_create(1, 2);
      rectangle_destroy(p);
      rectangle_destroy(p);
   }

   Case(underflow)
   {
      char* p = (char*)malloc(6);
      strcpy(p - 50, "underflow");
      free(p);
   }

   Case(overflow)
   {
      int* p = (int*)malloc(6);
      p[5] = 32717;
      free(p);
   }

   Case(read after free)
   {
      rectangle_t* p = rectangle_create(1, 2);
      rectangle_destroy(p);
      t = p->height;
   }

   Case(write after free)
   {
      rectangle_t* p = rectangle_create(1, 2);
      rectangle_destroy(p);
      p->height = 42;
   }

   Case(readwrite after free)
   {
      rectangle_t* p = rectangle_create(1, 2);
      rectangle_destroy(p);
      t = p->height;
      p->height = t;
   }
}
