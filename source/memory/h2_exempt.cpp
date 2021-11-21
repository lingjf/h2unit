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
      //// result->tm_sec = 0; result->tm_min = 0; result->tm_hour = 0; result->tm_mon = 0; result->tm_wday = 0; result->tm_yday = 0;
      result->tm_mday = 1;
      result->tm_year = 2012 - 1900;
      return result;
   }
};

h2_inline void h2_exempt::setup()
{
   static h2_list stubs;

   h2_stubs::add(stubs, (void*)::gmtime, (void*)h2_exempt_stub::gmtime, "gmtime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::ctime, (void*)h2_exempt_stub::ctime, "ctime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::asctime, (void*)h2_exempt_stub::asctime, "asctime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::localtime, (void*)h2_exempt_stub::localtime, "localtime", H2_FILINE);
   h2_stubs::add(stubs, (void*)::mktime, (void*)h2_exempt_stub::mktime, "mktime", H2_FILINE);

#if defined _WIN32
   add_by_fp((void*)::_wchdir);
   add_by_fp((void*)::fopen);
   add_by_fp((void*)::fclose);
   add_by_fp((void*)::strftime);
   add_by_fp((void*)::gmtime_s);
   add_by_fp((void*)::_gmtime32);
   add_by_fp((void*)::_gmtime64);
   add_by_fp((void*)::_gmtime32_s);
   add_by_fp((void*)::_gmtime64_s);
   add_by_fp(h2_numberfy<void*>(&std::type_info::name));
#else
   h2_stubs::add(stubs, (void*)::gmtime_r, (void*)h2_exempt_stub::gmtime_r, "gmtime_r", H2_FILINE);
   h2_stubs::add(stubs, (void*)::ctime_r, (void*)h2_exempt_stub::ctime_r, "ctime_r", H2_FILINE);
   h2_stubs::add(stubs, (void*)::asctime_r, (void*)h2_exempt_stub::asctime_r, "asctime_r", H2_FILINE);
   h2_stubs::add(stubs, (void*)::localtime_r, (void*)h2_exempt_stub::localtime_r, "localtime_r", H2_FILINE);
   add_by_fp((void*)::sscanf);
   add_by_fp((void*)::sprintf);
   add_by_fp((void*)::vsnprintf);

#if defined __APPLE__ && defined __clang__
   add_by_fp((void*)::snprintf);
   add_by_fp((void*)::snprintf_l);
   add_by_fp((void*)::strftime_l);
   add_by_fp((void*)::strtod_l);
   add_by_fp((void*)::strtold);
   add_by_fp((void*)::strtof_l);
#endif
#endif

#if defined __GNUC__
   add_by_fp((void*)abi::__cxa_demangle);
   add_by_fp((void*)abi::__cxa_throw);
#if !defined __clang__
   add_by_fp((void*)::__cxa_allocate_exception);
#endif
#endif

   add_by_fp((void*)h2_pattern::regex_match);
}

h2_inline void h2_exempt::add_by_name(const char* fn)
{
   h2_symbol* res[16];
   int n = h2_nm::get_by_name(fn, res, 16);
   for (int i = 0; i < n; ++i)
      add_by_fp(h2_load::addr_to_ptr(res[i]->addr));
}

h2_inline void h2_exempt::add_by_fp(void* fp)
{
   I().fps[I().nfp++] = h2_cxa::follow_jmp(fp);
   I().fps[I().nfp] = nullptr;
}
