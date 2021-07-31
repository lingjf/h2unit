
struct h2_exempt_stub {
   static time_t mktime(struct tm* timeptr) { return 1325347200; }
   static char* asctime(const struct tm* timeptr) { return (char*)"SUN JAN 1 00:00:00 2012\n"; }
   static char* asctime_r(const struct tm* timeptr, char* buf) { return strcpy(buf, (char*)"SUN JAN 1 00:00:00 2012\n"); }
   static char* ctime(const time_t* clock) { return (char*)"SUN JAN 1 00:00:00 2012\n"; }
   static char* ctime_r(const time_t* clock, char* buf) { return strcpy(buf, (char*)"SUN JAN 1 00:00:00 2012\n"); }
   static struct tm* localtime(const time_t* clock) { return gmtime(clock); }
   static struct tm* localtime_r(const time_t* timep, struct tm* result) { return gmtime_r(timep, result); }
   static struct tm* gmtime(const time_t* clock) { return gmtime_r(clock, nullptr); }

   static struct tm* gmtime_r(const time_t* clock, struct tm* result)
   {
      static struct tm st;
      if (!result) result = &st;
      memset(result, 0, sizeof(struct tm));
      result->tm_sec = 0;
      result->tm_min = 0;
      result->tm_hour = 0;
      result->tm_mday = 1;
      result->tm_mon = 0;
      result->tm_year = 2012 - 1900;
      result->tm_wday = 0;
      result->tm_yday = 0;
      return result;
   }
};

h2_inline void h2_exempt::setup()
{
   static h2_stubs stubs;

   stubs.add((void*)::gmtime, (void*)h2_exempt_stub::gmtime, "gmtime", __FILE__, __LINE__);
   stubs.add((void*)::ctime, (void*)h2_exempt_stub::ctime, "ctime", __FILE__, __LINE__);
   stubs.add((void*)::asctime, (void*)h2_exempt_stub::asctime, "asctime", __FILE__, __LINE__);
   stubs.add((void*)::localtime, (void*)h2_exempt_stub::localtime, "localtime", __FILE__, __LINE__);
   stubs.add((void*)::mktime, (void*)h2_exempt_stub::mktime, "mktime", __FILE__, __LINE__);

#if defined _WIN32
#else
   stubs.add((void*)::gmtime_r, (void*)h2_exempt_stub::gmtime_r, "gmtime_r", __FILE__, __LINE__);
   stubs.add((void*)::ctime_r, (void*)h2_exempt_stub::ctime_r, "ctime_r", __FILE__, __LINE__);
   stubs.add((void*)::asctime_r, (void*)h2_exempt_stub::asctime_r, "asctime_r", __FILE__, __LINE__);
   stubs.add((void*)::localtime_r, (void*)h2_exempt_stub::localtime_r, "localtime_r", __FILE__, __LINE__);
   add_by_fp((void*)::sscanf);
   add_by_fp((void*)::sprintf);
   add_by_fp((void*)::vsnprintf);
#   ifdef __APPLE__
   add_by_fp((void*)::snprintf);
   add_by_fp((void*)::strftime_l);
   add_by_fp((void*)::strtod_l);
   add_by_fp((void*)::strtold);
   add_by_fp((void*)::strtof_l);
   add_by_fp((void*)abi::__cxa_throw);
#   endif
#endif
}

h2_inline void h2_exempt::add_by_name(const char* fn)
{
   h2_symbol* res[16];
   int n = h2_nm::get_by_name(fn, res, 16);
   if (n != 1) {
      h2_color::prints("yellow", n ? "\nFind multiple exempt %s :\n" : "\nDon't find exempt %s\n", fn);
      for (int i = 0; i < n; ++i)
         h2_color::prints("yellow", "  %d. %s \n", i + 1, res[i]->name);
   }

   for (int i = 0; i < n; ++i)
      add_by_fp(h2_load::addr_to_ptr(res[i]->addr));
}

h2_inline void h2_exempt::add_by_fp(void* fp)
{
   I().fps[I().nfp++] = follow_jmp(fp);
   I().fps[I().nfp] = nullptr;
}
