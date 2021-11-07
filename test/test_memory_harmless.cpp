#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#if defined(__GNUC__) && __GNUC__ >= 8 && !defined(__clang__)  // gcc >= 8
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif

#if defined(__GNUC__) && __GNUC__ == 7 && !defined(__clang__)  // gcc == 7
#pragma GCC diagnostic ignored "-Wnoexcept-type"
#endif

#include "../source/h2_unit.cpp"
#include "test_types.hpp"

#if !defined _WIN32
#include <sys/time.h>
#include <pthread.h>
#endif

#if defined _WIN32
GlobalSetup()
{
   WORD wVersionRequested;
   WSADATA wsaData;

   wVersionRequested = MAKEWORD(1, 1);
   int nErrCode = WSAStartup(wVersionRequested, &wsaData);
   if (nErrCode != 0) {
      ::printf("WSAStartup() failed\n");
   }
}

GlobalCleanup()
{
   WSACleanup();
}
#endif

SUITE(harmless)
{
   char t[1024];
   int ret;

#if !defined __MINGW32__
   Case(time.h)
   {
      time_t t3 = time(NULL);
      struct tm* t4 = gmtime(&t3);
      struct tm t5;
      ctime(&t3);

      // asctime(t4);  // deprecated, arm64 and openSUSE crash in memory check mode

#if !defined _WIN32
      struct timeval tv;
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
#endif

   Case(string.h)
   {
      // String manipulation
      auto H2PP_UNIQUE() = strcpy(t, "h2unit");
      auto H2PP_UNIQUE() = strncpy(t, "h2unit", 3);
      auto H2PP_UNIQUE() = strcat(t, "h2unit");
      auto H2PP_UNIQUE() = strncat(t, "h2unit", 3);
      auto H2PP_UNIQUE() = strxfrm(t, "h2unit", 3);

      // String examination
      auto H2PP_UNIQUE() = strlen("h2unit");
      auto H2PP_UNIQUE() = strcmp(t, "h2unit");
      auto H2PP_UNIQUE() = strncmp(t, "h2unit", 3);
      auto H2PP_UNIQUE() = strchr("h2unit", 'u');
      auto H2PP_UNIQUE() = strrchr("h2unit", 'u');
      auto H2PP_UNIQUE() = strcoll(t, "h2unit");
      auto H2PP_UNIQUE() = strspn("h2unit", "h2");
      auto H2PP_UNIQUE() = strcspn("h2unit", "h2");
      auto H2PP_UNIQUE() = strstr("h2unit", "h2");
      auto H2PP_UNIQUE() = strpbrk("h2unit", "h2");
      auto H2PP_UNIQUE() = strtok(t, ",");

      // Miscellaneous
      auto H2PP_UNIQUE() = strerror(ENOMEM);

      // Memory manipulation
      auto H2PP_UNIQUE() = memset(t, 0, 10);
      auto H2PP_UNIQUE() = memcpy(t, "h2unit", 3);
      auto H2PP_UNIQUE() = memmove(t, "h2unit", 3);
      auto H2PP_UNIQUE() = memcmp(t, "h2unit", 3);
      auto H2PP_UNIQUE() = memchr(t, 'u', 10);

      // Numeric conversions
      auto H2PP_UNIQUE() = atof("12345.67");
      auto H2PP_UNIQUE() = atoi("12345");
      auto H2PP_UNIQUE() = atol("12345");
      auto H2PP_UNIQUE() = atoll("12345");
      auto H2PP_UNIQUE() = strtof("12345678.12", NULL);
      auto H2PP_UNIQUE() = strtod("1912000101600571", NULL);
      auto H2PP_UNIQUE() = strtold("1912000101600571", NULL);
      auto H2PP_UNIQUE() = strtol("12345678", NULL, 10);
      auto H2PP_UNIQUE() = strtoll("1234567890", NULL, 10);
      auto H2PP_UNIQUE() = strtoul("12345678", NULL, 10);
      auto H2PP_UNIQUE() = strtoull("1234567890", NULL, 10);

      // Popular extensions
#if defined _WIN32
      auto H2PP_UNIQUE() = _stricmp(t, "h2unit");
#else
      auto H2PP_UNIQUE() = strcasecmp(t, "h2unit");
      char* p;
      auto H2PP_UNIQUE() = strtok_r(t, ",", &p);
#endif
   }

   Case(stdlib.h)
   {
#if !(defined __MINGW32__ || defined __MINGW64__)
      getenv("LANG");
#ifdef _WIN32
      system("pwd");
#else
      system("echo 'a' | grep b");
#endif
      srand(0);
      rand();
      // bsearch();
      // qsort();
#endif
   }

   Case(stdio.h)
   {
      /* Formatted input/output */
      printf("%g%5s%c", 1.0 / 3.0, " test ", ' ');
      fprintf(stderr, "%g%5s%c", 1.0 / 3.0, " test ", ' ');
      sprintf(t, "%g%5s%c", 1.0 / 3.0, " test ", ' ');
      snprintf(t, 100, "%g%5s%c", 1.0 / 3.0, " test ", ' ');
      my_printf("%g%5s%c", 1.0 / 3.0, " test ", ' ');  // vprintf
#if 0
      my_fprintf(stderr, "%g%5s%c", 1.0 / 3.0, " test ", ' ');  // vfprintf
#endif
      my_sprintf(t, "%g%5s%c", 1.0 / 3.0, " test ", ' ');       // vsprintf
      my_snprintf(t, 10, "%g%5s%c", 1.0 / 3.0, " test ", ' ');  // vsnprintf

#if 0
      perror("perror");  // passed.
#endif

      sscanf("3   a.out  0x00008a3c foobar + 45", "%*s%*s%*s%s + %d", t, &ret);

      /* Operations on files */

      char* b = (char*)malloc(1024 * 1024);
      FILE* f1 = fopen(__FILE__, "r");
      auto n = fread(b, 1, 1024 * 1024, f1);
      fclose(f1);
      FILE* f2 = fopen("./_this.dat", "w+");
      fwrite(b, 1, n, f2);
      fclose(f2);
      free(b);

      rename("./_this.dat", "./_that.dat");
      remove("./_that.dat");
      // tmpnam("./_dir"); // deprecated
#if !(defined __MINGW32__ || defined __MINGW64__)
      fclose(tmpfile());
#endif
   }

   Case(iostream)
   {
      std::ostringstream toss;
      toss << std::boolalpha << 42;
   }

   Case(math.h)
   {
      auto H2PP_UNIQUE() = abs(-42);  // auto H2PP_UNIQUE() = labs(-42); auto H2PP_UNIQUE() = llabs(-42);
      auto H2PP_UNIQUE() = fabs(-42.0);

      auto H2PP_UNIQUE() = div(42, 42);          // auto H2PP_UNIQUE() = ldiv(42, 42); auto H2PP_UNIQUE() = lldiv(42, 42);
      auto H2PP_UNIQUE() = fmod(2.0, 2.0);       // auto H2PP_UNIQUE() = fmodf(2.0, 2.0); auto H2PP_UNIQUE() = fmodl(2.0, 2.0);
      auto H2PP_UNIQUE() = remainder(2.0, 2.0);  // auto H2PP_UNIQUE() = remainderf(2.0, 2.0); auto H2PP_UNIQUE() = remainderl(2.0, 2.0);

      auto H2PP_UNIQUE() = nan("1");  // auto H2PP_UNIQUE() = nanf("1"); auto H2PP_UNIQUE() = nanl("1");

      /* Exponential functions */
      auto H2PP_UNIQUE() = exp(2.0);    // auto H2PP_UNIQUE() = expf(2.0); auto H2PP_UNIQUE() = expl(2.0);
      auto H2PP_UNIQUE() = exp2(2.0);   // auto H2PP_UNIQUE() = exp2f(2.0); auto H2PP_UNIQUE() = exp2l(2.0);
      auto H2PP_UNIQUE() = expm1(2.0);  // auto H2PP_UNIQUE() = expm1f(2.0); auto H2PP_UNIQUE() = expm1l(2.0);
      auto H2PP_UNIQUE() = log(2.0);    // auto H2PP_UNIQUE() = logf(2.0); auto H2PP_UNIQUE() = logl(2.0);
      auto H2PP_UNIQUE() = log2(2.0);   // auto H2PP_UNIQUE() = log2f(2.0); auto H2PP_UNIQUE() = log2l(2.0);
      auto H2PP_UNIQUE() = log10(2.0);  // auto H2PP_UNIQUE() = log10f(2.0); auto H2PP_UNIQUE() = log10l(2.0);
      auto H2PP_UNIQUE() = log1p(2.0);  // auto H2PP_UNIQUE() = log1pf(2.0); auto H2PP_UNIQUE() = log1pl(2.0);
      auto H2PP_UNIQUE() = logb(2.0);   // auto H2PP_UNIQUE() = logbf(2.0); auto H2PP_UNIQUE() = logbl(2.0);
      auto H2PP_UNIQUE() = ilogb(2.0);  // auto H2PP_UNIQUE() = ilogbf(2.0); auto H2PP_UNIQUE() = ilogbl(2.0);

      /* Power functions */
      auto H2PP_UNIQUE() = sqrt(42.0);        // auto H2PP_UNIQUE() = sqrtf(42.0); auto H2PP_UNIQUE() = sqrtl(42.0);
      auto H2PP_UNIQUE() = cbrt(42.0);        // auto H2PP_UNIQUE() = cbrtf(42.0); auto H2PP_UNIQUE() = cbrtl(42.0);
      auto H2PP_UNIQUE() = hypot(42.0, 1.0);  // auto H2PP_UNIQUE() = hypotf(42.0, 1.0); auto H2PP_UNIQUE() = hypotl(42.0, 1.0);
      auto H2PP_UNIQUE() = pow(42.0, 1.0);    // auto H2PP_UNIQUE() = powf(42.0, 1.0); auto H2PP_UNIQUE() = powl(42.0, 1.0);

      /* Trigonometric functions */
      auto H2PP_UNIQUE() = sin(42.0);         // auto H2PP_UNIQUE() = sinf(42.0); auto H2PP_UNIQUE() = sinl(42.0);
      auto H2PP_UNIQUE() = cos(42.0);         // auto H2PP_UNIQUE() = cosf(42.0); auto H2PP_UNIQUE() = cosl(42.0);
      auto H2PP_UNIQUE() = tan(42.0);         // auto H2PP_UNIQUE() = tanf(42.0); auto H2PP_UNIQUE() = tanl(42.0);
      auto H2PP_UNIQUE() = asin(42.0);        // auto H2PP_UNIQUE() = asinf(42.0); auto H2PP_UNIQUE() = asinl(42.0);
      auto H2PP_UNIQUE() = acos(42.0);        // auto H2PP_UNIQUE() = acosf(42.0); auto H2PP_UNIQUE() = acosl(42.0);
      auto H2PP_UNIQUE() = atan(42.0);        // auto H2PP_UNIQUE() = atanf(42.0); auto H2PP_UNIQUE() = atanl(42.0);
      auto H2PP_UNIQUE() = atan2(42.0, 2.0);  // auto H2PP_UNIQUE() = atan2f(42.0, 2.0); auto H2PP_UNIQUE() = atan2l(42.0, 2.0);

      /* Hyperbolic functions */
      auto H2PP_UNIQUE() = sinh(3.0);   // auto H2PP_UNIQUE() = sinhf(3.0); auto H2PP_UNIQUE() = sinhl(3.0);
      auto H2PP_UNIQUE() = cosh(3.0);   // auto H2PP_UNIQUE() = coshf(3.0); auto H2PP_UNIQUE() = coshl(3.0);
      auto H2PP_UNIQUE() = tanh(3.0);   // auto H2PP_UNIQUE() = tanhf(3.0); auto H2PP_UNIQUE() = tanhl(3.0);
      auto H2PP_UNIQUE() = asinh(3.0);  // auto H2PP_UNIQUE() = asinhf(3.0); auto H2PP_UNIQUE() = asinhl(3.0);
      auto H2PP_UNIQUE() = acosh(3.0);  // auto H2PP_UNIQUE() = acoshf(3.0); auto H2PP_UNIQUE() = acoshl(3.0);
      auto H2PP_UNIQUE() = atanh(3.0);  // auto H2PP_UNIQUE() = atanhf(3.0); auto H2PP_UNIQUE() = atanhl(3.0);

      /* Nearest integer floating-point operations */
      auto H2PP_UNIQUE() = ceil(3.0);       // auto H2PP_UNIQUE() = ceilf(3.0); auto H2PP_UNIQUE() = ceill(3.0);
      auto H2PP_UNIQUE() = floor(3.0);      // auto H2PP_UNIQUE() = floorf(3.0); auto H2PP_UNIQUE() = floorl(3.0);
      auto H2PP_UNIQUE() = trunc(3.0);      // auto H2PP_UNIQUE() = truncf(3.0); auto H2PP_UNIQUE() = truncl(3.0);
      auto H2PP_UNIQUE() = round(3.0);      // auto H2PP_UNIQUE() = roundf(3.0); auto H2PP_UNIQUE() = roundl(3.0);
      auto H2PP_UNIQUE() = nearbyint(3.0);  // auto H2PP_UNIQUE() = nearbyintf(3.0); auto H2PP_UNIQUE() = nearbyintl(3.0);
      auto H2PP_UNIQUE() = rint(3.0);       // auto H2PP_UNIQUE() = rintf(3.0); auto H2PP_UNIQUE() = rintl(3.0);
   }

#if !defined _WIN32
   Case(syslog.h)
   {
      syslog(LOG_DEBUG, "This is test %d", 42);
   }
#endif

   Case(socket.h)
   {
      CloseSocket(socket(AF_INET, SOCK_DGRAM, 0));
   }

   Case(netdb.h)
   {
      struct addrinfo hints, *res;
      memset(&hints, 0, sizeof(hints));
      hints.ai_family = PF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      getaddrinfo("localhost", NULL, &hints, &res);
      freeaddrinfo(res);
   }

#if !defined _WIN32
   Case(pthread.h)
   {
      UNMEM(pthread_create);
      pthread_t tid;
      int ret = pthread_create(&tid, NULL, my_pthread, NULL);
      pthread_join(tid, NULL);

      pthread_rwlock_t rwlock;
      pthread_rwlock_init(&rwlock, NULL);
      pthread_rwlock_tryrdlock(&rwlock);
      pthread_rwlock_unlock(&rwlock);
      pthread_rwlock_trywrlock(&rwlock);
      pthread_rwlock_unlock(&rwlock);
      pthread_rwlock_rdlock(&rwlock);
      pthread_rwlock_unlock(&rwlock);
      pthread_rwlock_wrlock(&rwlock);
      pthread_rwlock_unlock(&rwlock);
      pthread_rwlock_destroy(&rwlock);

      pthread_cond_t cond;
      pthread_mutex_t mutex;
      pthread_mutex_init(&mutex, NULL);
      pthread_mutex_trylock(&mutex);
      pthread_mutex_unlock(&mutex);
      pthread_mutex_lock(&mutex);
      pthread_mutex_unlock(&mutex);
      pthread_cond_init(&cond, NULL);
      pthread_cond_broadcast(&cond);
      pthread_cond_signal(&cond);
      // pthread_cond_wait(&cond, &mutex);
      struct timespec ts = {1, 1};
      pthread_cond_timedwait(&cond, &mutex, &ts);
      pthread_cond_destroy(&cond);
      pthread_mutex_destroy(&mutex);
   }
#endif
}

CASE(dbg)
{
#if defined NDEBUG
   ::printf("NDEBUG is defined\n");
#else
   ::printf("NDEBUG is undefined\n");
#endif

#if defined _MSC_VER

#ifndef NDEBUG
#if defined _CRTDBG_MAP_ALLOC
   ::printf("_CRTDBG_MAP_ALLOC is defined\n");
   ::printf("when _CRTDBG_MAP_ALLOC is defined : &free=%p &_free_dbg=%p\n", free, _free_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is defined : &malloc=%p &_malloc_dbg=%p\n", malloc, _malloc_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is defined : &realloc=%p &_realloc_dbg=%p\n", realloc, _realloc_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is defined : &calloc=%p &_calloc_dbg=%p\n", calloc, _calloc_dbg);

#undef _CRTDBG_MAP_ALLOC
   ::printf("when _CRTDBG_MAP_ALLOC is undefined : &free=%p &_free_dbg=%p\n", free, _free_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is undefined : &malloc=%p &_malloc_dbg=%p\n", malloc, _malloc_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is undefined : &realloc=%p &_realloc_dbg=%p\n", realloc, _realloc_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is undefined : &calloc=%p &_calloc_dbg=%p\n", calloc, _calloc_dbg);

#else
   ::printf("_CRTDBG_MAP_ALLOC is undefined\n");
   ::printf("when _CRTDBG_MAP_ALLOC is undefined : &free=%p &_free_dbg=%p\n", free, _free_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is undefined : &malloc=%p &_malloc_dbg=%p\n", malloc, _malloc_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is undefined : &realloc=%p &_realloc_dbg=%p\n", realloc, _realloc_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is undefined : &calloc=%p &_calloc_dbg=%p\n", calloc, _calloc_dbg);

#define _CRTDBG_MAP_ALLOC
   ::printf("when _CRTDBG_MAP_ALLOC is defined : &free=%p &_free_dbg=%p\n", free, _free_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is defined : &malloc=%p &_malloc_dbg=%p\n", malloc, _malloc_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is defined : &realloc=%p &_realloc_dbg=%p\n", realloc, _realloc_dbg);
   ::printf("when _CRTDBG_MAP_ALLOC is defined : &calloc=%p &_calloc_dbg=%p\n", calloc, _calloc_dbg);
#endif
#endif
#endif
}

CASE(cpp std)
{
   // g++ -x c++ -std=c++11 -dM -E - </dev/null | grep __cplusplus
   // #define __cplusplus 201103L

   // g++ -x c++ -std=c++14 -dM -E - </dev/null | grep __cplusplus
   // #define __cplusplus 201402L

   // g++ -x c++ -std=c++17 -dM -E - </dev/null | grep __cplusplus
   // #define __cplusplus 201703L

   // g++ -x c++ -std=c++2a -dM -E - </dev/null | grep __cplusplus
   // #define __cplusplus 202002L

   // g++ -x c++ -std=c++2b -dM -E - </dev/null | grep __cplusplus
   // #define __cplusplus 202102L

   // https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros
   ::printf("__cplusplus = %s\n", H2PP_STR(__cplusplus));
#if defined _MSC_VER
   ::printf("__cplusplus = %s\n", H2PP_STR(_MSVC_LANG));
#endif
}
