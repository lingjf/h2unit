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
#include "test_cplusplus.hpp"
#include "test_types.hpp"

#if !defined _WIN32
#include <sys/time.h>
#include <pthread.h>
#endif

#include <functional>
#include <atomic>
#include <chrono>
#include <random>
#include <bitset>
#include <thread>
#include <mutex>
#include <condition_variable>

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
      fprintf(stderr, "%g%5s%c\n", 1.0 / 3.0, " test ", ' ');
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

   Case(atomic)
   {
      std::atomic<int> a1(0);
      a1++;
      --a1;
      a1 += 1;
      a1 -= 1;
   }

   Case(bitset)
   {
      std::bitset<8> bs;

      bs[0] = 1;
      bs[3] = true;
      bs[7] = 0;
      bs.to_ulong();
      bs.to_string();
   }

   Case(hash)
   {
      std::hash<int>{}(42);
      std::hash<std::string>{}("abc");
   }

   Case(type_info)
   {
      typeid(char).name();
   }

   Case(Pseudo-random number generation)
   {
      std::random_device r;

      std::default_random_engine e1(r());
      std::uniform_int_distribution<int> uniform_dist(1, 6);
      int mean = uniform_dist(e1);

      std::seed_seq seed2{r(), r(), r(), r(), r(), r(), r(), r()};
      std::mt19937 e2(seed2);
      std::normal_distribution<> normal_dist(mean, 2);
      normal_dist(e2);
   }

   Case(chrono library)
   {
      auto start = std::chrono::steady_clock::now();
      auto end = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsed_seconds = end - start;
      elapsed_seconds.count();
   }
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

#include <locale>
SUITE(locale)
{
   Case(locale)
   {
      // std::locale("");
      // std::locale();
   }
}

void functional_f1(int i)
{
}
struct functional_foo {
   functional_foo(int m) : m2(m) {}
   void f2(int i) const {}
   int m2;
};
struct functional_bar {
   void operator()(int i) const
   {
   }
};

SUITE(harmless functional)
{
   Case(normal)
   {
      std::function<void(int)> f1 = functional_f1;
      f1(-9);
   }

   Case(function object)
   {
      std::function<void(int)> f1 = functional_bar();
      f1(18);
   }

   Case(lambda)
   {
      [](int a) -> void {
         a = 42;
      }(24);
   }

   Case(lambda)
   {
      std::function<void()> f1 = []() { functional_f1(42); };
      f1();
   }

   Case(std::bind)
   {
      std::function<void()> f1 = std::bind(functional_f1, 31337);
      f1();
   }

   Case(member function)
   {
      const functional_foo foo(314159);
      std::function<void(const functional_foo&, int)> f2 = &functional_foo::f2;
      f2(foo, 1);
      f2(314159, 1);
   }

   Case(data member)
   {
      const functional_foo foo(314159);
      std::function<int(functional_foo const&)> m2 = &functional_foo::m2;
      m2(foo);
   }

   Case(member)
   {
      const functional_foo foo(314159);
      using std::placeholders::_1;
      std::function<void(int)> f2 = std::bind(&functional_foo::f2, foo, _1);
      f2(2);
      std::function<void(int)> f3 = std::bind(&functional_foo::f2, &foo, _1);
      f3(3);
   }

   Case(recursive lambda)
   {
      auto factorial = [](int n) {
         // store a lambda object to emulate "recursive lambda"; aware of extra overhead
         std::function<int(int)> fac = [&](int n) { return (n < 2) ? 1 : n * fac(n - 1); };
         // note that "auto fac = [&](int n){...};" does not work in recursive calls
         return fac(n);
      };
      for (int i{5}; i != 8; ++i) { factorial(i); }
   }
}

static void std_thread_f1(int n)
{
   for (int i = 0; i < 5; ++i) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
   }

   std::mutex std_thread_mutex;
   std::condition_variable std_condition_variable;

   std::unique_lock<std::mutex> locker(std_thread_mutex);
   //  std_condition_variable.wait(locker);
   locker.unlock();
}

SUITE(harmless thread)
{
   Case(std::thread)
   {
      BLOCK(unmem)
      {
         int n = 0;
         std::thread t1;                        // t1 is not a thread
         std::thread t2(std_thread_f1, n + 1);  // pass by value
         std::thread t3(std_thread_f1, n + 2);  // pass by value

         t2.join();
         t3.join();
      }
   }

#if !defined _WIN32
   Case(pthread.h [openSUSE])
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

#if __cplusplus >= 201703L || (defined _MSVC_LANG && _MSVC_LANG >= 201703L)

#include <charconv>
SUITE(harmless Elementary string conversions)
{
   Todo(to_chars)
   {
      std::array<char, 10> str;
      std::to_chars(str.data(), str.data() + str.size(), 42);
   }

   Case(from_chars)
   {
      int result{};
      std::string str = "15 foo bar  42 5000000000";
      std::from_chars(str.data(), str.data() + str.size(), result);
   }
}

#endif

#if __cplusplus >= 202002L || (defined _MSVC_LANG && _MSVC_LANG >= 202002L)
#include <span>
template <class T, std::size_t N>
constexpr auto slide_span(std::span<T, N> s, std::size_t offset, std::size_t width)
{
   return s.subspan(offset, offset + width <= s.size() ? width : 0U);
}

CASE(harmless span)
{
   constexpr int a[]{0, 1, 2, 3, 4, 5, 6, 7, 8};

   auto s = slide_span(std::span{a}, 1, 3);
   auto a1 = s.empty();
}

#if 0
// gcc11 not supported
// AppleClang13 not supported
// https://en.cppreference.com/w/cpp/compiler_support/20

#include <format>
#include <iterator>
SUITE(harmless Formatting library (C++20))
{
   Case(std::format)
   {
      std::format("Hello {}!\n", "world");
   }

   Case(std::format_to)
   {
      std::string buffer;

      std::format_to(
        std::back_inserter(buffer),  //< OutputIt
        "Hello, C++{}!\n",           //< fmt
        "20");                       //< arg
      buffer.clear();

      std::format_to(
        std::back_inserter(buffer),  //< OutputIt
        "Hello, {0}::{1}!{2}",       //< fmt
        "std",                       //< arg {0}
        "format_to()",               //< arg {1}
        "\n",                        //< arg {2}
        "extra param(s)...");        //< unused

      std::wstring wbuffer;
      std::format_to(
        std::back_inserter(wbuffer),  //< OutputIt
        L"Hello, {2}::{1}!{0}",       //< fmt
        L"\n",                        //< arg {0}
        L"format_to()",               //< arg {1}
        L"std",                       //< arg {2}
        L"...is not..."               //< unused
        L"...an error!");             //< unused
   }

   Case(std::format_to_n)
   {
      char buffer[64];
      std::format_to_n(buffer, std::size(buffer) - 1,
                       "Hubble's H{2} {3} {0}{4}{1} km/sec/Mpc.",
                       71,        // {0}, occupies 2 bytes
                       8,         // {1}, occupies 1 byte
                       "\u2080",  // {2}, occupies 3 bytes
                       "\u2245",  // {3}, occupies 3 bytes
                       "\u00B1"   // {4}, occupies 2 bytes
      );
   }

   Case(std::vformat)
   {
      std::vformat("Int 1 is %d, Int 2 is %d, Int 3 is %d", 11, 22, 33);
      std::vformat("{:02} │ {} │ {} │ {}", std::make_format_args(1, 2.0, '3', "4"));
   }

   Case(std::vformat_to)
   {
      std::string buffer;
      std::vformat_to(std::back_inserter(buffer), "{:02} │ {} │ {} │ {}", std::make_format_args(1, 2.0, '3', "4"));
   }
}
#endif
#endif
