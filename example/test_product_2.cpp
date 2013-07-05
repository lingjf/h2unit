#include "h2unit.h"
#include <stdlib.h>
#include <string.h>

#include "product_2.h"

H2UNIT(wstring)
{
   /*
    * setup() and teardown() can be omitted.
    */
};

/*
 * h2unit H2EQUAL_STRCMP can be used to verify String.
 */

H2CASE(wstring, "Constructor")
{
   wstring w;
   H2EQUAL_STRCMP("wstring(len=1,cap=256,dat=)", w.tuString());
}

/*
 * h2unit H2EQUAL_REGEX can be used to verify WildCard String.
 * Regular Express is not supported yet ,
 * due to import complex PCRE will break the h2unit design principle :
 *       Light-Weight just like hydrogen !
 */
H2CASE(wstring, "fromInt(123)")
{
   wstring w;
   w.fromInt(123);
   H2EQUAL_REGEX("wstring(len=[0-9],cap=??,dat=*)", w.tuString());
}

/*
 * h2unit H2EQUAL_MEMCMP can be used to verify memory block.
 */
H2CASE(wstring, "fromInt(12345)")
{
   wstring w;
   w.fromInt(12345);
   unsigned char exp_buf[] = { 0x32, '\0', 0x32, '\0', 0x33, '\0', 0x34, '\0', 0x35, '\0' };
   H2EQUAL_MEMCMP(exp_buf, w.getData(), sizeof(exp_buf));
}

H2CASE_IGNORE(wstring, "todo feature")
{

}

H2CASE(wstring, "show compare format")
{
   H2EQUAL_STRCMP_NOCASE(
               "abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ",
               "abcdefghijklmnopqrstuvwXYZ1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ");

   H2EQUAL_STRCMP(
               "abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ",
               "abcdefghijklmnopqrstuvwXYZ1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

/*
 * h2unit can check the C++ exception throw out.
 */

H2CASE(wstring, "test catch none")
{
   wstring w;
   H2CATCH_NONE(w.fromInt(8));
   H2CATCH_NONE(w.fromInt(-8));
}

H2CASE(wstring, "test catch something")
{
   wstring w;
   H2CATCH_THROW(w.fromInt(-8), int);
   H2CATCH_THROW(w.fromInt(-8), float);
}

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

H2CASE(memory_leak, "test memory leak")
{
   allocation();
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

