
#if defined __linux__
#   if defined(__GNUC__) && (defined(__i386) || defined(__x86_64))
#      define h2_raise_trap() asm volatile("int $3")
#   else
#      define h2_raise_trap() raise(SIGTRAP)
#   endif
#elif defined __APPLE__
#   define h2_raise_trap() __asm__("int $3\n" \
                                   :          \
                                   :)
#endif

#if defined __linux__
static inline bool under_debug(int, const char*) {
   char t[1024];
   h2_with f(::fopen("/proc/self/status", "r"));
   if (f.f)
      while (::fgets(t, sizeof(t) - 1, f.f))
         if (strncmp(t, "TracerPid:\t", 11) == 0)
            return t[11] != '\0' && t[11] != '0';
   return false;
}
#elif defined __APPLE__
static inline bool under_debug(int pid, const char* path) {
   char t[1024], attach_pid[64];
   sprintf(attach_pid, "%d", pid);
   h2_with f(::popen("ps -ef | grep lldb | grep -v sudo | grep -v grep", "r"), ::pclose);
   if (f.f)
      while (::fgets(t, sizeof(t) - 1, f.f))
         if (strstr(t, basename((char*)path)) || strstr(t, attach_pid))
            return true;
   return false;
}
#endif

static inline char* get_gdb1(char* s) {
#if defined __linux__
   sprintf(s, "gdb --quiet --args %s %s", O.path, O.args);
#elif defined __APPLE__
   sprintf(s, "lldb %s -- %s", O.path, O.args);
#endif
   return s;
}

static inline char* get_gdb2(char* s, int pid) {
#if defined __linux__
   sprintf(s, "sudo gdb --pid=%d", pid);
#elif defined __APPLE__
   sprintf(s, "sudo lldb --attach-pid %d", pid);
#endif
   return s;
}

h2_inline void h2_debugger::trap() {
   int pid = (int)getpid();
   if (!under_debug(pid, O.path)) {
      static h2_once only_one_time;
      if (only_one_time) {
         if (streq("gdb attach", O.debug)) {
            if (fork() == 0) {
               system(get_gdb2((char*)alloca(256), pid));
               exit(0);
            } else
               while (!under_debug(pid, O.path))
                  h2_sleep(100);  // wait for password
         } else {
            system(get_gdb1((char*)alloca(256)));
            exit(0);
         }
      }
   }

   h2_raise_trap();
}
