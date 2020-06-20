
struct h2__patch {
   h2_singleton(h2__patch);
   h2_stubs stubs;

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

   h2__patch()
   {
      stubs.add((void*)::gmtime, (void*)gmtime, "gmtime", __FILE__, __LINE__);
      stubs.add((void*)::gmtime_r, (void*)gmtime_r, "gmtime_r", __FILE__, __LINE__);
      stubs.add((void*)::ctime, (void*)ctime, "ctime", __FILE__, __LINE__);
      stubs.add((void*)::ctime_r, (void*)ctime_r, "ctime_r", __FILE__, __LINE__);
      stubs.add((void*)::asctime, (void*)asctime, "asctime", __FILE__, __LINE__);
      stubs.add((void*)::asctime_r, (void*)asctime_r, "asctime_r", __FILE__, __LINE__);
      stubs.add((void*)::localtime, (void*)localtime, "localtime", __FILE__, __LINE__);
      stubs.add((void*)::localtime_r, (void*)localtime_r, "localtime_r", __FILE__, __LINE__);
      stubs.add((void*)::mktime, (void*)mktime, "mktime", __FILE__, __LINE__);
   }
};

h2_inline void h2_patch::initialize()
{
   h2__patch::I();
}

h2_inline bool h2_patch::exempt(h2_backtrace& bt)
{
   static struct {
      void* base;
      int size;
   } exempt_functions[] = {
     {(void*)sscanf, 300},
     {(void*)sprintf, 300},
     {(void*)vsnprintf, 300},
#ifdef __APPLE__
     {(void*)vsnprintf_l, 300},
#endif
   };

   for (auto& x : exempt_functions)
      if (bt.has(x.base, x.size))
         return true;

   return false;
}
