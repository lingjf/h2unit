#include "h2unit.h"
#include <stdlib.h>
#include <string.h>

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
 * -- CoTaskMemAlloc/CoTaskMemRealloc
 * -- GlobalAlloc
 * -- HeapAlloc/HeapReAlloc
 * -- LocalAlloc/LocalReAlloc
 */

extern "C" {
#include "product_c.h"
}

H2UNIT (memory_leak) {
   void setup() {}
   void teardown() {}
};

/*
 * Memory leak detection on Test Case Level
 */
H2CASE(memory_leak, test memory leak ok)
{
   free(malloc(8));
}

H2CASE(memory_leak, test memory leak)
{
   char* s = (char*)malloc(8);
   strcpy(s, "1234567");

   for (int i = 0; i < 3; i++) {
      create_point(1, i);
   }
}

/*
 * Memory leak detection on User-Defined Block Level
 */
H2CASE(memory_leak, test memory leak block)
{
   void *c1, *c2, *c3, *c4;

   c1 = malloc(1);

   H2BLOCK()
   {
      c2 = malloc(3);
      free(c2);
   }

   H2BLOCK()
   {
      c3 = malloc(5);
      free(c3);
      H2BLOCK()
      {
         c4 = malloc(7);
      }
   }

   free(c4);
   free(c1);
}

/*
 * h2unit can modify the total memory resource to make malloc() failed.
 */

H2CASE(memory_leak, malloc faulty injection)
{
   H2BLOCK(10 /* in the block , available memory only 10 bytes */)
   {
      H2EQ(NULL == malloc(11)); /* no enough available memory */
   }
}

/*
 * h2unit can modify fill of malloc.
 */

H2CASE(memory_leak, filled malloc)
{
   H2BLOCK(10000000, "ABC" /* in the block , malloc allocated space filled with ABC */)
   {
      char* p = (char*)malloc(9); /* no enough available memory */
      H2EQ(Me("ABCABCABC"), p);
      free(p);
   }
}

/*
 * h2unit can detect out-of-bound access.
 *
 * OverFlow : access out-of-bound at end.
 *
 * UnderFlow : access out-of-bound at before.
 *
 */

H2UNIT (memory_out_of_bound) {
   void setup() {}
   void teardown() {}
};

H2CASE(memory_out_of_bound, test memory overflow)
{
   char* c1 = (char*)malloc(6);
   memcpy(c1, "1234567", 7);
   free(c1);
}
