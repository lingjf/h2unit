
struct h2_exempt_stub {  // allocate memory inside asymmetrically
   static char* asctime(const struct tm* timeptr)
   {
      static char st[256];
      asctime_r(timeptr, st);
      return st;
   }

   static char* asctime_r(const struct tm* timeptr, char* buf)
   {
      char* ret;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::asctime_r); t;) ret = ::asctime_r(timeptr, buf);
      h2_memory::overrides();
      return ret;
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
      struct tm* ret;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::gmtime_r); t;) ret = ::gmtime_r(clock, result);
      h2_memory::overrides();
      return ret;
   }

   static struct tm* gmtime(const time_t* clock)
   {
      static struct tm st;
      gmtime_r(clock, &st);
      return &st;
   }

   static time_t mktime(struct tm* timeptr)
   {
      time_t ret;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::mktime); t;) ret = ::mktime(timeptr);
      h2_memory::overrides();
      return ret;
   }

   static double strtod(const char* nptr, char** endptr)
   {
      double ret;
      h2_memory::restores();
      for (h2::h2_stub_temporary_restore t((void*)::strtod); t;) ret = ::strtod(nptr, endptr);
      h2_memory::overrides();
      return ret;
   }

   static long double strtold(const char* nptr, char** endptr)
   {
      double ret;
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
      if (!size) size = function_size((unsigned char*)base);
   }

   int function_size(unsigned char* func)
   {
#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64
      // e8/e9/ff15/ff25   call/jmp PLT
      if (*func == 0xE8 || *func == 0xE9) return 1;
      if (*func == 0xFF && (*(func + 1) == 0x15 || *(func + 1) == 0x25)) return 2;
      for (unsigned char* p = func + 1;; p++) {
         // 5d c3   pop %ebp; ret;
         // 5b c3   pop %ebx; ret;
         // c9 c3   leave; ret;
         if (*p == 0xC3 && ((*(p - 1) == 0x5D) || (*(p - 1) == 0x5B) || (*(p - 1) == 0xC9))) {
            return p - func;
         }
      }
#endif
      return 300;
   }
};

h2_inline void h2_exempt::setup()
{
   static h2_stubs stubs;

   stubs.add((void*)::gmtime, (void*)h2_exempt_stub::gmtime, "gmtime", __FILE__, __LINE__);
   stubs.add((void*)::gmtime_r, (void*)h2_exempt_stub::gmtime_r, "gmtime_r", __FILE__, __LINE__);
   stubs.add((void*)::ctime, (void*)h2_exempt_stub::ctime, "ctime", __FILE__, __LINE__);
   stubs.add((void*)::ctime_r, (void*)h2_exempt_stub::ctime_r, "ctime_r", __FILE__, __LINE__);
   stubs.add((void*)::asctime, (void*)h2_exempt_stub::asctime, "asctime", __FILE__, __LINE__);
   stubs.add((void*)::asctime_r, (void*)h2_exempt_stub::asctime_r, "asctime_r", __FILE__, __LINE__);
   stubs.add((void*)::localtime, (void*)h2_exempt_stub::localtime, "localtime", __FILE__, __LINE__);
   stubs.add((void*)::localtime_r, (void*)h2_exempt_stub::localtime_r, "localtime_r", __FILE__, __LINE__);
   if (O.os == Linux) stubs.add((void*)::mktime, (void*)h2_exempt_stub::mktime, "mktime", __FILE__, __LINE__);
   if (O.os == macOS) stubs.add((void*)::strtod, (void*)h2_exempt_stub::strtod, "strtod", __FILE__, __LINE__);
   if (O.os == macOS) stubs.add((void*)::strtold, (void*)h2_exempt_stub::strtold, "strtold", __FILE__, __LINE__);

   add((void*)::sscanf);
   add((void*)::sprintf);
   add((void*)::vsnprintf);
#ifdef __APPLE__
   add((void*)::vsnprintf_l);
   add((void*)abi::__cxa_throw);
#endif
   add((void*)h2_pattern::regex_match);  // linux is 0xcb size, MAC is 0x100 (gap to next symbol)
}

h2_inline void h2_exempt::add(void* func)
{
   I().exempts.push_back((new h2_exemption((void*)func))->x);
}

h2_inline bool h2_exempt::in(const h2_backtrace& bt)
{
   h2_list_for_each_entry (p, I().exempts, h2_exemption, x)
      if (bt.has(p->base, p->size)) return true;
   return false;
}
