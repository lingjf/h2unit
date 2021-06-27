
struct h2_exempt_stub {  // allocate memory inside asymmetrically
   static char* asctime(const struct tm* timeptr)
   {
      static char st[256];
      asctime_r(timeptr, st);
      return st;
   }

   static char* asctime_r(const struct tm* timeptr, char* buf)
   {
      h2_memory::restores();
#if defined _WIN32
      asctime_s(buf, 256, timeptr);
#else
      for (h2::h2_stub_temporary_restore t((void*)::asctime_r); t;) buf = ::asctime_r(timeptr, buf);
#endif
      h2_memory::overrides();
      return buf;
   }

   static char* ctime(const time_t* clock)
   {
      static char st[256];
      ctime_r(clock, st);
      return st;
   }

   static char* ctime_r(const time_t* clock, char* buf)
   {
      struct tm t;
      return asctime_r(gmtime_r(clock, &t), buf);
   }

   static struct tm* localtime(const time_t* clock)
   {
      return gmtime(clock);
   }

   static struct tm* localtime_r(const time_t* timep, struct tm* result)
   {
      return gmtime_r(timep, result);
   }

   static struct tm* gmtime_r(const time_t* clock, struct tm* result)
   {
      h2_memory::restores();
#if defined _WIN32
      ::gmtime_s(result, clock);
#else
      for (h2::h2_stub_temporary_restore t((void*)::gmtime_r); t;) result = ::gmtime_r(clock, result);
#endif
      h2_memory::overrides();
      return result;
   }

   static struct tm* gmtime(const time_t* clock)
   {
      static struct tm st;
      gmtime_r(clock, &st);
      return &st;
   }

   static time_t mktime(struct tm* timeptr)
   {
      time_t ret = 0;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::mktime); t;) ret = ::mktime(timeptr);
      h2_memory::overrides();
      return ret;
   }

   static double strtod(const char* nptr, char** endptr)
   {
      double ret = 0;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::strtod); t;) ret = ::strtod(nptr, endptr);
      h2_memory::overrides();
      return ret;
   }

   static long double strtold(const char* nptr, char** endptr)
   {
      double ret = 0;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::strtold); t;) ret = ::strtold(nptr, endptr);
      h2_memory::overrides();
      return ret;
   }
};

struct h2_exemption : h2_libc {
   h2_list x;
   void* base;
   int size;

   h2_exemption(void* _base, int _size = 0) : base(_base), size(_size)
   {
      h2_load::backtrace_scope(base, size);
   }
};

h2_inline void h2_exempt::setup()
{
   static h2_stubs stubs;

   stubs.add((void*)::gmtime, (void*)h2_exempt_stub::gmtime, "gmtime", __FILE__, __LINE__);
   stubs.add((void*)::ctime, (void*)h2_exempt_stub::ctime, "ctime", __FILE__, __LINE__);
   stubs.add((void*)::asctime, (void*)h2_exempt_stub::asctime, "asctime", __FILE__, __LINE__);
   stubs.add((void*)::localtime, (void*)h2_exempt_stub::localtime, "localtime", __FILE__, __LINE__);
#if !defined _WIN32
   stubs.add((void*)::gmtime_r, (void*)h2_exempt_stub::gmtime_r, "gmtime_r", __FILE__, __LINE__);
   stubs.add((void*)::ctime_r, (void*)h2_exempt_stub::ctime_r, "ctime_r", __FILE__, __LINE__);
   stubs.add((void*)::asctime_r, (void*)h2_exempt_stub::asctime_r, "asctime_r", __FILE__, __LINE__);
   stubs.add((void*)::localtime_r, (void*)h2_exempt_stub::localtime_r, "localtime_r", __FILE__, __LINE__);
#endif
   if (O.os == Linux) stubs.add((void*)::mktime, (void*)h2_exempt_stub::mktime, "mktime", __FILE__, __LINE__);
   if (O.os == macOS) stubs.add((void*)::strtod, (void*)h2_exempt_stub::strtod, "strtod", __FILE__, __LINE__);
   if (O.os == macOS) stubs.add((void*)::strtold, (void*)h2_exempt_stub::strtold, "strtold", __FILE__, __LINE__);

#if defined _WIN32
   add_by_name("h2::h2_defer_failure::~h2_defer_failure");
#else
#   ifdef __APPLE__
   add_by_addr((void*)::vsnprintf_l);
   add_by_addr((void*)abi::__cxa_throw);
#   endif
   add_by_addr((void*)::sscanf);
   add_by_addr((void*)::sprintf);
   add_by_addr((void*)::vsnprintf);
   add_by_addr((void*)h2_pattern::regex_match);  // linux is 0xcb size, MAC is 0x100 (gap to next symbol)
#endif
}

h2_inline void h2_exempt::add_by_name(const char* func, int size)
{
   h2_symbol* res[16];
   int n = h2_nm::get_by_name(func, res, 16);
   if (n == 0) {
      h2_color::prints("yellow", "\nDon't find %s\n", func);
   } else if (n > 1) {
      h2_color::prints("yellow", "\nFind multiple %s :\n", func);
      for (int i = 0; i < n; ++i)
         h2_color::prints("yellow", "  %d. %s \n", i + 1, res[i]->name);
   }

   for (int i = 0; i < n; ++i) {
      I().exempts.push_back((new h2_exemption(h2_load::symbol_to_addr(res[i]->offset), res[i]->size))->x);
   }
}

h2_inline void h2_exempt::add_by_addr(void* func, int size)
{
   unsigned long long symbol_offset = h2_load::addr_to_symbol(func);
   h2_symbol* res = h2_nm::get_by_offset(symbol_offset);
   if (res) {
      func = h2_load::symbol_to_addr(res->offset);
      size = res->size;
   }
   I().exempts.push_back((new h2_exemption((void*)func, size))->x);
}

h2_inline bool h2_exempt::in(const h2_backtrace& bt)
{
   h2_list_for_each_entry (p, I().exempts, h2_exemption, x)
      if (bt.has(p->base, p->size)) return true;
   return false;
}
