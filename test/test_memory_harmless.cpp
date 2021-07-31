#include "../source/h2_unit.cpp"
#include "test_types.hpp"
#if !defined _WIN32
#   include <pthread.h>
#endif

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
   char* p;
   int ret;

   Case(time.h)
   {
      struct timeval tv;
      time_t t3 = time(NULL);
      struct tm* t4 = gmtime(&t3);
      struct tm t5;
      ctime(&t3);
      asctime(t4);  // deprecated
#if !defined _WIN32
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
      // String manipulation
      strcpy(t, "h2unit");
      strncpy(t, "h2unit", 6);
      strcat(t, "h2unit");
      strncat(t, "h2unit", 6);
      strxfrm(t, "h2unit", 6);

      // String examination
      strlen("h2unit");
      strcmp(t, "h2unit");
      strncmp(t, "h2unit", 6);
      strchr("h2unit", 'u');
      strrchr("h2unit", 'u');
      strcoll(t, "h2unit");
      strspn("h2unit", "h2");
      strcspn("h2unit", "h2");
      strstr("h2unit", "h2");
      strpbrk("h2unit", "h2");
      strtok(t, ",");

      // Miscellaneous
      strerror(ENOMEM);

      // Memory manipulation
      memset(t, 0, 10);
      memcpy(t, "h2unit", 6);
      memmove(t, "h2unit", 6);
      memcmp(t, "h2unit", 6);
      memchr(t, 'u', 10);

      // Numeric conversions
      atof("12345.67");
      atoi("12345");
      atol("12345");
      atoll("12345");
      strtof("12345678.12", NULL);
      strtod("1912000101600571", NULL);
      strtold("1912000101600571", NULL);
      strtol("12345678", NULL, 10);
      strtoll("1234567890", NULL, 10);
      strtoul("12345678", NULL, 10);
      strtoull("1234567890", NULL, 10);

      // Popular extensions
#if defined WIN32
      stricmp(t, "h2unit");
#else
      strcasecmp(t, "h2unit");
      strtok_r(t, ",", &p);
#endif
   }

   Case(stdlib.h)
   {
      getenv("LANG");
      system("pwd");

      srand(0);
      rand();
      // bsearch();
      // qsort();
   }

   Case(stdio.h)
   {
      /* Formatted input/output */
      printf("%g%5s%c", 1.0 / 3.0, " test ", ' ');
      fprintf(stderr, "%g%5s%c", 1.0 / 3.0, " test ", ' ');
      sprintf(t, "%g%5s%c", 1.0 / 3.0, " test ", ' ');
      snprintf(t, 100, "%g%5s%c", 1.0 / 3.0, " test ", ' ');
      my_printf("%g%5s%c", 1.0 / 3.0, " test ", ' ');           // vprintf
      my_fprintf(stderr, "%g%5s%c", 1.0 / 3.0, " test ", ' ');  // vfprintf
      my_sprintf(t, "%g%5s%c", 1.0 / 3.0, " test ", ' ');       // vsprintf
      my_snprintf(t, 10, "%g%5s%c", 1.0 / 3.0, " test ", ' ');  // vsnprintf
      perror("perror test");

      sscanf("3   a.out  0x00008a3c foobar + 45", "%*s%*s%*s%s + %d", t, &ret);

      /* Operations on files */
      fclose(fopen("./_this.dat", "w+"));
      rename("./_this.dat", "./_that.dat");
      remove("./_that.dat");
      // tmpnam("./_dir"); // deprecated
      fclose(tmpfile());
   }

#if !defined WIN32
   Case(iostream)
   {
      std::ostringstream toss;
      toss << std::boolalpha << 42;
   }
#endif

   Case(math.h)
   {
      auto abs_ret = abs(-42);  // auto labs_ret = labs(-42); auto llabs_ret = llabs(-42);
      auto fabs_ret = fabs(-42.0);

      auto div_ret = div(42, 42);                // auto ldiv_ret = ldiv(42, 42); auto lldiv_ret = lldiv(42, 42);
      auto fmod_ret = fmod(2.0, 2.0);            // auto fmodf_ret = fmodf(2.0, 2.0); auto fmodl_ret = fmodl(2.0, 2.0);
      auto remainder_ret = remainder(2.0, 2.0);  // auto remainderf_ret = remainderf(2.0, 2.0); auto remainderl_ret = remainderl(2.0, 2.0);

      auto nan_ret = nan("1");  // auto nanf_ret = nanf("1"); auto nanl_ret = nanl("1");

      /* Exponential functions */
      auto exp_ret = exp(2.0);      // auto expf_ret = expf(2.0); auto expl_ret = expl(2.0);
      auto exp2_ret = exp2(2.0);    // auto exp2f_ret = exp2f(2.0); auto exp2l_ret = exp2l(2.0);
      auto expm1_ret = expm1(2.0);  // auto expm1f_ret = expm1f(2.0); auto expm1l_ret = expm1l(2.0);
      auto log_ret = log(2.0);      // auto logf_ret = logf(2.0); auto logl_ret = logl(2.0);
      auto log2_ret = log2(2.0);    // auto log2f_ret = log2f(2.0); auto log2l_ret = log2l(2.0);
      auto log10_ret = log10(2.0);  // auto log10f_ret = log10f(2.0); auto log10l_ret = log10l(2.0);
      auto log1p_ret = log1p(2.0);  // auto log1pf_ret = log1pf(2.0); auto log1pl_ret = log1pl(2.0);
      auto logb_ret = logb(2.0);    // auto logbf_ret = logbf(2.0); auto logbl_ret = logbl(2.0);
      auto ilogb_ret = ilogb(2.0);  // auto ilogbf_ret = ilogbf(2.0); auto ilogbl_ret = ilogbl(2.0);

      /* Power functions */
      auto sqrt_ret = sqrt(42.0);         // auto sqrtf_ret = sqrtf(42.0); auto sqrtl_ret = sqrtl(42.0);
      auto cbrt_ret = cbrt(42.0);         // auto cbrtf_ret = cbrtf(42.0); auto cbrtl_ret = cbrtl(42.0);
      auto hypot_ret = hypot(42.0, 1.0);  // auto hypotf_ret = hypotf(42.0, 1.0); auto hypotl_ret = hypotl(42.0, 1.0);
      auto pow_ret = pow(42.0, 1.0);      // auto powf_ret = powf(42.0, 1.0); auto powl_ret = powl(42.0, 1.0);

      /* Trigonometric functions */
      auto sin_ret = sin(42.0);           // auto sinf_ret = sinf(42.0); auto sinl_ret = sinl(42.0);
      auto cos_ret = cos(42.0);           // auto cosf_ret = cosf(42.0); auto cosl_ret = cosl(42.0);
      auto tan_ret = tan(42.0);           // auto tanf_ret = tanf(42.0); auto tanl_ret = tanl(42.0);
      auto asin_ret = asin(42.0);         // auto asinf_ret = asinf(42.0); auto asinl_ret = asinl(42.0);
      auto acos_ret = acos(42.0);         // auto acosf_ret = acosf(42.0); auto acosl_ret = acosl(42.0);
      auto atan_ret = atan(42.0);         // auto atanf_ret = atanf(42.0); auto atanl_ret = atanl(42.0);
      auto atan2_ret = atan2(42.0, 2.0);  // auto atan2f_ret = atan2f(42.0, 2.0); auto atan2l_ret = atan2l(42.0, 2.0);

      /* Hyperbolic functions */
      auto sinh_ret = sinh(3.0);    // auto sinhf_ret = sinhf(3.0); auto sinhl_ret = sinhl(3.0);
      auto cosh_ret = cosh(3.0);    // auto coshf_ret = coshf(3.0); auto coshl_ret = coshl(3.0);
      auto tanh_ret = tanh(3.0);    // auto tanhf_ret = tanhf(3.0); auto tanhl_ret = tanhl(3.0);
      auto asinh_ret = asinh(3.0);  // auto asinhf_ret = asinhf(3.0); auto asinhl_ret = asinhl(3.0);
      auto acosh_ret = acosh(3.0);  // auto acoshf_ret = acoshf(3.0); auto acoshl_ret = acoshl(3.0);
      auto atanh_ret = atanh(3.0);  // auto atanhf_ret = atanhf(3.0); auto atanhl_ret = atanhl(3.0);

      /* Nearest integer floating-point operations */
      auto ceil_ret = ceil(3.0);            // auto ceilf_ret = ceilf(3.0); auto ceill_ret = ceill(3.0);
      auto floor_ret = floor(3.0);          // auto floorf_ret = floorf(3.0); auto floorl_ret = floorl(3.0);
      auto trunc_ret = trunc(3.0);          // auto truncf_ret = truncf(3.0); auto truncl_ret = truncl(3.0);
      auto round_ret = round(3.0);          // auto roundf_ret = roundf(3.0); auto roundl_ret = roundl(3.0);
      auto nearbyint_ret = nearbyint(3.0);  // auto nearbyintf_ret = nearbyintf(3.0); auto nearbyintl_ret = nearbyintl(3.0);
      auto rint_ret = rint(3.0);            // auto rintf_ret = rintf(3.0); auto rintl_ret = rintl(3.0);
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
