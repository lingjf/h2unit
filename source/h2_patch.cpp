
struct h2__patch {
   h2_singleton(h2__patch);
   h2_stubs stubs;

   static struct tm* localtime(const time_t* clock) { return ::gmtime(clock); }

   h2__patch() {
#if defined __GLIBC__
      stubs.add((void*)::localtime, (void*)localtime);
#endif
   }
};

h2_inline void h2_patch::initialize() {
   h2__patch::I();
}
