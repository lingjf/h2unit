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
      if (strstr(t, h2_fs(path).basefile()) || strstr(t, attach_pid)) {
         ret = true;
         break;
      }
   }
   ::pclose(f);
   return false;
}
#endif

h2_inline void h2_debugger::trap()
{
#if defined __linux || defined __APPLE__
   int pid = (int)getpid();
   if (!under_debug(pid, O.path)) {
      static h2_once only_one_time;
      if (only_one_time) {
         char cmd[512];
#if defined __linux
         sprintf(cmd, "sudo gdb --pid=%d", pid);
#elif defined __APPLE__
         sprintf(cmd, "sudo lldb --attach-pid %d", pid);
#endif
         if (fork() == 0)
            exit(system(cmd));
         else
            while (!under_debug(pid, O.path)) h2_sleep(100);
      }
   }
#endif
}
