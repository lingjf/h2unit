#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(override)
{
   Case(malloc)
   {
      free(malloc(100));
      BLOCK(limit = 10)
      {
         OK(IsNull, malloc(100));
      }
   }

   Case(calloc)
   {
      free(calloc(10, 10));
      BLOCK(limit = 10)
      {
         OK(IsNull, calloc(10, 10));
      }
   }

   Case(realloc)
   {
      auto p = realloc(NULL, 100);  // act as malloc
      OK(NotNull, p);
      free(p);

      BLOCK(limit = 10)
      {
         OK(IsNull, realloc(NULL, 100));
      }
   }

   Case(new)
   {
      delete new User;
      delete new (std::nothrow) User;
      BLOCK(limit = 10)
      {
         OK(IsNull, new User);
         OK(IsNull, new (std::nothrow) User);
      }
   }

   Case(new[])
   {
      delete[] new char[100];
      delete[] new (std::nothrow) char[100];
      BLOCK(limit = 10)
      {
         OK(IsNull, new char[100]);
         OK(IsNull, new (std::nothrow) char[100]);
      }
   }

   Case(strdup)
   {
      free(strdup("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"));
      BLOCK(limit = 10)
      {
         OK(IsNull, strdup("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890"));
      }
   }

#if !defined WIN32
   Case(strndup)
   {
      free(strndup("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", 100));
      BLOCK(limit = 10)
      {
         OK(IsNull, strndup("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", 100));
      }
   }
#endif

#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L
   Case(posix_memalign)
   {
      void* ptr = NULL;
      OK(0, posix_memalign(&ptr, 8, 100));
      OK(NotNull, ptr);
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
         OK(IsNull, aligned_alloc(10, 100));
      }
   }
#endif
}
