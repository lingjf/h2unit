#include "../h2unit.h"

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

SUITE(Memory Leak)
{
   /*
    * Memory leak detection on Test Case Level
    */
   Case(test memory leak ok successful)
   {
      free(malloc(8));
   }

   Case(test memory leak failure)
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
   Case(test memory leak block failure)
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

   Case(malloc faulty injection successful)
   {
      BLOCK(limit = 10 /* in the block , available memory only 10 bytes */)
      {
         OK(IsNull, malloc(11)); /* no enough available memory */
      }
   }

   /*
    * h2unit can modify fill of malloc.
    */

   Case(filled malloc successful)
   {
      /* in the block , malloc allocated space filled with 0xABCD */
      BLOCK(limit = 10000000, fill = 0xABCD)
      {
         char* p = (char*)malloc(8);
         OK(Me("\xAB\xCD\xAB\xCD"), p);
         free(p);
      }
   }

   Case(exempt leak function)
   {
      UNMEM(rectangle_create);

      rectangle_t* rectangle = rectangle_create(1, 2);
      OK(2, rectangle_area(rectangle));
   }
}

SUITE(Memory Check)
{
   Case(asymmetric malloc and delete failure)
   {
      char* p = (char*)malloc(100);
      delete p;
   }

   Case(double free failure)
   {
      rectangle_t* p = rectangle_create(1, 2);
      rectangle_destroy(p);
      rectangle_destroy(p);
   }

   Case(use after free failure)
   {
      rectangle_t* p = rectangle_create(1, 2);
      rectangle_destroy(p);

      int height = p->height;
      p->height = height;
   }
}
