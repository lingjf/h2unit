#include "../source/h2_unit.cpp"

class User {
 public:
   char data[100];
   User(){};
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

#if !(defined __arm__ || defined __arm64__ || defined __aarch64__)
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
#endif

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
#   if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L
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
#   endif

#   if defined _ISOC11_SOURCE
   Case(aligned_alloc)
   {
      free(aligned_alloc(10, 10));
      BLOCK(limit = 10)
      {
         OK(IsNull, aligned_alloc(10, 100));
      }
   }
#   endif

#   if (_XOPEN_SOURCE >= 500) && !(_POSIX_C_SOURCE >= 200112L) || _DEFAULT_SOURCE || _SVID_SOURCE || _BSD_SOURCE
   Case(valloc)
   {
      free(valloc(100));
      BLOCK(limit = 10)
      {
         OK(IsNull, valloc(100));
      }
   }
#   endif
#endif
}

#ifdef _WIN32
GlobalSetup()
{
   WORD wVersionRequested;
   WSADATA wsaData;

   int err;
   wVersionRequested = MAKEWORD(1, 1);
   err = WSAStartup(wVersionRequested, &wsaData);
}

GlobalCleanup()
{
   WSACleanup();
}
#endif

SUITE(harmless)
{
   char t[1024];

   Case(time.h)
   {
      struct timeval tv;
      time_t t3 = time(NULL);
      struct tm* t4 = gmtime(&t3);
      struct tm t5;
      ctime(&t3);
#if !(defined __arm__ || defined __arm64__ || defined __aarch64__)
      asctime(t4);
#endif
#if !defined WIN32
      struct timezone tz;
      gettimeofday(&tv, &tz);
      ctime_r(&t3, t);
      gmtime_r(&t3, &t5);
      asctime_r(t4, t);
      localtime_r(&t3, &t5);
#endif
      mktime(&t5);
      localtime(&t3);
      strftime(t, sizeof(t), "%a, %d %b %Y %T %z", t4);
   }

   Case(string.h)
   {
      memchr("abcdefghijklmnopqrstuvwxyz", 'k', 26);
      memcmp("abc", "def", 3);

      strcoll("abc", "def");
      strcpy(t, "abc,def");
      strtok(t, ",");
      strxfrm(t, "xyz", 3);
      strerror(ENOMEM);
   }

   Case(stdlib.h)
   {
      atoi("12345");
      atol("12345");
      atof("12345.67");
      strtof("12345678.12", NULL);
      strtod("1912000101600571", NULL);
      strtold("1912000101600571", NULL);
      strtol("12345678", NULL, 10);
      strtoll("1234567890", NULL, 10);
      strtoul("12345678", NULL, 10);
      strtoull("1234567890", NULL, 10);

      getenv("LANG");
      system("pwd");

      srand(0);
      rand();
      // bsearch();
      // qsort();
   }

   Case(stdio.h)
   {
      sprintf(t, "%g%g%g", 1.0 / 3.0, 1.0 / 7.0, 1.0 / 13.0);
      fclose(fopen("./CMakeLists.txt", "r"));
   }

   Case(math.h)
   {
      auto ret1 = sqrt(42.0);
   }

#if !defined WIN32
   Case(syslog.h)
   {
      syslog(LOG_DEBUG, "This is test %d", 42);
   }
#endif

#if !defined WIN32
   Case(socket.h)
   {
      close(socket(AF_INET, SOCK_DGRAM, 0));
   }
#endif

   Case(netdb.h)
   {
      struct addrinfo hints, *res;
      memset(&hints, 0, sizeof(hints));
      hints.ai_family = PF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      getaddrinfo("localhost", NULL, &hints, &res);
      freeaddrinfo(res);
   }
}
