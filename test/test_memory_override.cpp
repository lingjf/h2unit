#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(override [memory_check])
{
   Case(malloc)
   {
      free(malloc(100));
      BLOCK(limit = 10)
      {
         OK(nullptr, malloc(100));
      }
   }

   Case(calloc)
   {
      free(calloc(10, 10));
      BLOCK(limit = 10)
      {
         OK(nullptr, calloc(10, 10));
      }
   }

   Case(realloc)
   {
      auto p = realloc(NULL, 100);  // act as malloc
      OK(Not(NULL), p);
      free(p);

      BLOCK(limit = 10)
      {
         OK(nullptr, realloc(NULL, 100));
      }
   }

   Case(new)
   {
      delete new A_PlainStruct;
      delete new (std::nothrow) A_PlainStruct;
      BLOCK(limit = 10)
      {
         OK(nullptr, new A_PlainStruct);
         OK(nullptr, new (std::nothrow) A_PlainStruct);
      }
   }

   Case("new[]")
   {
      delete[] new char[100];
      delete[] new (std::nothrow) char[100];
      BLOCK(limit = 10)
      {
         OK(nullptr, new char[100]);
         OK(nullptr, new (std::nothrow) char[100]);
      }
   }

   Case(strdup)
   {
      free(strdup("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"));
      BLOCK(limit = 10)
      {
         OK(nullptr, strdup("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"));
      }
   }

#if !defined _WIN32
   Case(strndup)
   {
      free(strndup("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", 100));
      BLOCK(limit = 10)
      {
         OK(nullptr, strndup("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", 100));
      }
   }
#endif

#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L
   Case(posix_memalign)
   {
      void* ptr = NULL;
      OK(0, posix_memalign(&ptr, 8, 100));
      OK(Not(NULL), ptr);
      free(ptr);
      BLOCK(limit = 10)
      {
         OK(Not(0), posix_memalign(&ptr, 8, 100));
      }
   }
#endif

#if defined _ISOC11_SOURCE
   Case(aligned_alloc)
   {
      free(aligned_alloc(10, 10));
      BLOCK(limit = 10)
      {
         OK(nullptr, aligned_alloc(10, 100));
      }
   }
#endif
}
