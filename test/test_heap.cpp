#include "../source/h2_unit.cpp"
#include <sys/time.h>

class C100 {
 public:
   char data[100];
   C100(){};
};

SUITE(piece)
{
   Case(new delete)
   {
      h2::h2_backtrace bt;
      h2::h2_piece* m = new h2::h2_piece(100, 0, "malloc", bt);
      delete m;
   }
}

SUITE(Heap Hook)
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
      delete new C100;
      delete new (std::nothrow) C100;
      BLOCK(limit = 10)
      {
         OK(IsNull, new C100);
         OK(IsNull, new (std::nothrow) C100);
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

SUITE(parse_block_attributes)
{
   long long n_limit;
   int n_align;
   unsigned char s_fill[32];
   int n_fill;

   Case("limit=1000, fill=1, align=3")
   {
      const char* x = "limit=1000, fill=1, align=3";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill);
      OK(1000, n_limit);
      OK(3, n_align);
      OK(1, n_fill);
      OK(Me("\01", 1), s_fill);
   }
   Case("limit=0x1000, fill=0x55")
   {
      const char* x = "limit =0x1000, fill = 0x55";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill);
      OK(0x1000, n_limit);
      OK(8, n_align);
      OK(1, n_fill);
      OK(Me("\x55", 1), s_fill);
   }
   Case("fill=0x5566")
   {
      const char* x = "fill=0x5566";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill);
      OK(Ge(0xFFFFFFFFU), n_limit);
      OK(8, n_align);
      OK(2, n_fill);
      OK(Me("\x55\x66", 2), s_fill);
   }
   Case("fill=60000")
   {
      const char* x = "fill=60000";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill);
      OK(2, n_fill);
      OK(Me("\x60\xEA", 2), s_fill);
   }
}

SUITE(BLOCK)
{
   void *c1, *c2, *c3, *c4;

   Case(block)
   {
      c1 = malloc(1);
      BLOCK()
      {
         c2 = malloc(3);
         free(c2);
      }
      free(c1);
   }

   Case(nested)
   {
      BLOCK()
      {
         c3 = malloc(5);
         BLOCK()
         {
            c4 = malloc(7);
            free(c4);
         }
         free(c3);
      }
   }

   Case(limited)
   {
      BLOCK(limit = 10)
      {
         c2 = malloc(8);
         OK(NotNull, c2);
         c3 = malloc(8);
         OK(IsNull, c3);
         free(c2);
         c4 = malloc(8);
         OK(NotNull, c4);
         free(c4);
      }
   }
}

CASE(sys api calls)
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

SUITE(leak)
{
   void* p = malloc(32 * 864);

   Cleanup() { free(p); }

   Case(malloc in setup and free in teardown)
   {
      strcpy((char*)p, "hello world");
      OK("hello world", (char*)p);
   }
}
