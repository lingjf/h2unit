#include "h2unit.h"
#include <stdlib.h>
#include <string.h>

void allocation();


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
 * -- CoTaskMemAlloc/CoTaskMemRealloc
 * -- GlobalAlloc
 * -- HeapAlloc/HeapReAlloc
 * -- LocalAlloc/LocalReAlloc
 * ?? brk/sbrk not support
 * ?? mmap/mnumap not support
 * ?? VirtualAlloc not support
 */

H2UNIT(memory_leak)
{
   void setup() { }
   void teardown() { }
};

/*
 * Memory leak detection on Test Case Level
 */
H2CASE(memory_leak, "test memory leak")
{
   allocation();
}


/*
 * Memory leak detection on User-Defined Block Level
 */
H2CASE(memory_leak, "test memory leak block")
{
   void * c1, * c2, * c3, * c4;

   c1 = malloc(1);

   H2LEAK_BLOCK() {
      c2 = malloc(3);
      free(c2);
   }

   H2LEAK_BLOCK() {
      c3 = malloc(5);
      free(c3);
      H2LEAK_BLOCK() {
         c4 = malloc(7);
      }
   }

   free(c4);
   free(c1);
}

/*
 * h2unit can modify the total memory resource to make malloc() failed.
 */

H2CASE(memory_leak, "test memory faulty injection")
{
   H2_FAULTY_INJECT_MEMORY(10);
   H2EQUAL_TRUE(NULL == malloc(11));
}



/*
 * h2unit can detect out-of-bound access.
 *
 * OverFlow : access out-of-bound at end.
 *
 * UnderFlow : access out-of-bound at before.
 *
 */

H2UNIT(memory_out_of_bound)
{
   void setup() { }
   void teardown() { }
};

H2CASE(memory_out_of_bound, "test memory overflow")
{
   char * c1 = (char *) malloc(6);
   memcpy(c1, "1234567", 7);
   free(c1);
}

H2CASE(memory_out_of_bound, "test memory underflow")
{
   char * c2 = (char *) malloc(6);
   memcpy(c2 - 2, "123456", 6);
   free(c2);
}

