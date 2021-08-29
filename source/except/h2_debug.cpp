#if defined __linux
#   if defined(__GNUC__) && (defined(__i386) || defined(__x86_64))
#      define h2_raise_trap() asm volatile("int $3")
#   else
#      define h2_raise_trap() raise(SIGTRAP)
#   endif
#elif defined __APPLE__
/* clang-format off */
#   define h2_raise_trap() __asm__("int $3\n" : :)
/* clang-format on */
#endif

#if defined __linux
static inline bool under_debug(int, const char*)
{
   char t[1024];
   FILE* f = ::fopen("/proc/self/status", "r");
   if (!f) return false;
   bool ret = false;
   while (::fgets(t, sizeof(t) - 1, f)) {
      if (strncmp(t, "TracerPid:\t", 11) == 0) {
         ret = t[11] != '\0' && t[11] != '0';
         break;
      }
   }
   ::fclose(f);
   return ret;
}
#elif defined __APPLE__
static inline bool under_debug(int pid, const char* path)
{
   char t[1024], attach_pid[64];
   sprintf(attach_pid, "%d", pid);
   FILE* f = ::popen("ps -ef | grep lldb | grep -v sudo | grep -v grep", "r");
   if (!f) return false;
   bool ret = false;
   while (::fgets(t, sizeof(t) - 1, f)) {
      if (strstr(t, h2_basename(path)) || strstr(t, attach_pid)) {
         ret = true;
         break;
      }
   }
   ::pclose(f);
   return false;
}
#endif

static inline char* get_gdb1(char* s)
{
#if defined __linux
   sprintf(s, "gdb --quiet --args %s %s", O.path, O.args);
#elif defined __APPLE__
   sprintf(s, "lldb %s -- %s", O.path, O.args);
#endif
   return s;
}

static inline char* get_gdb2(char* s, int pid)
{
#if defined __linux
   sprintf(s, "sudo gdb --pid=%d", pid);
#elif defined __APPLE__
   sprintf(s, "sudo lldb --attach-pid %d", pid);
#endif
   return s;
}

h2_inline void h2_debugger::trap()
{
#if defined __linux || defined __APPLE__
   int pid = (int)getpid();
   if (!under_debug(pid, O.path)) {
      static h2_once only_one_time;
      if (only_one_time) {
         if (!strcmp("gdb attach", O.debug)) {
            if (fork() == 0)
               exit(system(get_gdb2((char*)alloca(512), pid)));
            else
               while (!under_debug(pid, O.path)) h2_sleep(100);  // wait for password
         } else {
            exit(system(get_gdb1((char*)alloca(512))));
         }
      }
   }

   h2_raise_trap();
#endif
}
