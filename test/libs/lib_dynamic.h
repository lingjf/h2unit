
#ifndef __LIB_DYNAMIC_H__
#define __LIB_DYNAMIC_H__

#if defined WIN32
#   if defined(MyLib_EXPORT)
#      define LIB_DYNAMIC_EXPORT __declspec(dllexport)
#   else
#      define LIB_DYNAMIC_EXPORT __declspec(dllimport)
#   endif
#else
#   define LIB_DYNAMIC_EXPORT
#endif

LIB_DYNAMIC_EXPORT int lib_dynamic_add(int a, int b);
LIB_DYNAMIC_EXPORT int lib_dynamic_plus(int a);

LIB_DYNAMIC_EXPORT void* lib_dynamic_create(int n);
LIB_DYNAMIC_EXPORT void lib_dynamic_destroy(void* o);

#endif
