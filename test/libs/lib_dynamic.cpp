#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef MyLib_EXPORT
#   define MyLib_EXPORT // CMake define it if target is SHARED library
#endif

#include "lib_dynamic.h"

LIB_DYNAMIC_EXPORT int lib_dynamic_add(int a, int b)
{
   return a + b;
}
LIB_DYNAMIC_EXPORT int lib_dynamic_plus(int a)
{
   return lib_dynamic_add(a, 1);
}

LIB_DYNAMIC_EXPORT void* lib_dynamic_create(int n)
{
   return malloc(n);
}
LIB_DYNAMIC_EXPORT void lib_dynamic_destroy(void* o)
{
   free(o);
}
