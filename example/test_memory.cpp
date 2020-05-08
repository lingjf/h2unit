#include "h2unit.h"

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
   Case(test memory leak ok successful) { free(malloc(8)); }

   Case(test memory leak failure)
   {
      char* s = (char*)malloc(10);
      strcpy(s, "1234567");

      rectangle_t* rectangle;
      for (int i = 0; i < 3; i++) {
         rectangle = rectangle_create(1, i);
      }
      rectangle_destroy(rectangle);
   }

   Case(C++ new delete failure)
   {
      Dog* dog = new Dog(3);
      delete dog;

      Bird* bird = new Bird;
      bird->say();
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
      BLOCK(10 /* in the block , available memory only 10 bytes */)
      {
         OK(IsNull, malloc(11)); /* no enough available memory */
      }
   }

   /*
   * h2unit can modify fill of malloc.
   */

   Case(filled malloc successful)
   {
      /* in the block , malloc allocated space filled with ABC */
      BLOCK(10000000, "ABC")
      {
         char* p = (char*)malloc(8);
         OK(Me("ABCABCAB"), p);
         free(p);
      }
   }
}

SUITE(Memory symmetric allocate and free)
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
      rectangle_t* p = rectangle_create(1, 2);
      rectangle_destroy(p);
      rectangle_destroy(p);
   }
#if 0
   Case(memory underflow failure)
   {
      char* p = (char*)malloc(6);
      memcpy(p - 5, "123", 3);
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
      rectangle_t* p = rectangle_create(1, 2);
      rectangle_destroy(p);

      int width = p->width;
   }
   Case(write after free failure)
   {
      rectangle_t* p = rectangle_create(1, 2);
      rectangle_destroy(p);

      p->width = 100;
   }
#endif
}
