static inline bool in_debugging()
{
   bool ret = false;
#if defined __linux
   char t[1024];
   FILE* f = ::fopen("/proc/self/status", "r");
   if (f) {
      while (::fgets(t, sizeof(t) - 1, f)) {
         if (strncmp(t, "TracerPid:\t", 11) == 0) {
            ret = t[11] != '\0' && t[11] != '0';
            break;
         }
      }
      ::fclose(f);
   }
#elif defined __APPLE__
   char t[1024], attach_pid[64];
   sprintf(attach_pid, "%d", (int)getpid());
   FILE* f = ::popen("ps -ef | grep lldb | grep -v sudo | grep -v grep", "r");
   if (f) {
      while (::fgets(t, sizeof(t) - 1, f)) {
         if (strstr(t, h2_basefile(O.path)) || strstr(t, attach_pid)) {
            ret = true;
            break;
         }
      }
      ::pclose(f);
   }
#endif
   return ret;
}

static inline bool h2_attach_debugger()
{
#if defined __linux || defined __APPLE__
   while (!in_debugging()) {
      h2_once_if()
      {
         char cmd[512];
         ::printf("\nEnter \033[33mpassword\033[0m for connecting \033[33m%s\033[0m. \n", O.os == OsMacOS ? "GDB" : "LLDB");
#if defined __linux
         sprintf(cmd, "sudo gdb --silent -ex cont --pid=%d", (int)getpid());
#elif defined __APPLE__
         sprintf(cmd, "sudo lldb -o 'continue' --attach-pid %d", (int)getpid());
#endif
         if (fork() == 0) exit(system(cmd));
      }
      h2_sleep(100);
   }
#endif
   return true;
}

#if defined __linux
#if defined(__GNUC__) && (defined(__i386) || defined(__x86_64))
#define h2_raise_trap() asm volatile("int $3")
#else
#define h2_raise_trap() raise(SIGTRAP)
#endif
#elif defined __APPLE__
#if defined(__i386__) || defined(__x86_64__)
#define h2_raise_trap() __asm__("int $3\n" : :)
#elif defined(__aarch64__)
#define h2_raise_trap() __asm__(".inst 0xd4200000")
#endif
#else
#define h2_raise_trap() in_debugging()
#endif
