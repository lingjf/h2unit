
struct h2__patch {
   h2_singleton(h2__patch);
   h2_stubs stubs;

   static struct tm* localtime(const time_t* clock) { return ::gmtime(clock); }
   static struct tm* localtime_r(const time_t* timep, struct tm* result)
   {
      // memcpy(result, ::gmtime(timep), sizeof(struct tm));
      return result;
   }

   h2__patch()
   {
#if defined __GLIBC__
      stubs.add((void*)::localtime, (void*)localtime);
      stubs.add((void*)::localtime_r, (void*)localtime_r);
#endif
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
     {(void*)localtime, 300},
#ifndef _WIN32
     {(void*)tzset, 300},
#endif
   };

   for (auto& x : exempt_functions)
      if (bt.has(x.base, x.size))
         return true;

   return false;
}
