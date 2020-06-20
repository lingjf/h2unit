#include "../source/h2_unit.cpp"

namespace {

class Foobar {
 public:
   char data[100];
   Foobar(){};
};

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
      free(realloc(NULL, 100));  // act as malloc
      BLOCK(limit = 10)
      {
         OK(IsNull, realloc(NULL, 100));
      }
   }
   Case(new)
   {
      delete new Foobar;
      delete new (std::nothrow) Foobar;
      BLOCK(limit = 10)
      {
         OK(IsNull, new Foobar);
         OK(IsNull, new (std::nothrow) Foobar);
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
   Case(strndup)
   {
      free(strndup("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", 100));
      BLOCK(limit = 10)
      {
         OK(IsNull, strndup("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", 100));
      }
   }
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

#if (_XOPEN_SOURCE >= 500) && !(_POSIX_C_SOURCE >= 200112L) || _DEFAULT_SOURCE || _SVID_SOURCE || _BSD_SOURCE
   Case(valloc)
   {
      free(valloc(100));
      BLOCK(limit = 10)
      {
         OK(IsNull, valloc(100));
      }
   }
#endif
}

CASE(harmless)
{
   char t[1024];
   sprintf(t, "%g%g%g", 1.0 / 3.0, 1.0 / 7.0, 1.0 / 13.0);
   srand(0);
   rand();
   close(socket(AF_INET, SOCK_DGRAM, 0));
   fclose(fopen("/dev/null", "r"));
   struct timeval tv;
   struct timezone tz;
   gettimeofday(&tv, &tz);
   time_t t3 = time(NULL);
   struct tm* t4 = gmtime(&t3);
   struct tm t5;
   gmtime_r(&t3, &t5);
   ctime(&t3);
   ctime_r(&t3, t);
   asctime(t4);
   asctime_r(t4, t);
   mktime(&t5);
   localtime(&t3);
   localtime_r(&t3, &t5);

   strerror(ENOMEM);
   double t9 = sqrt(42.0);
   sprintf(t, "%g", t9);

   syslog(LOG_DEBUG, "This is test %d", 42);

   struct addrinfo hints, *res;
   memset(&hints, 0, sizeof(hints));
   hints.ai_family = PF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   getaddrinfo("localhost", NULL, &hints, &res);
   freeaddrinfo(res);
}

}  // namespace
