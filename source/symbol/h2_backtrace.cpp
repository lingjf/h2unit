#if !defined _WIN32  // -MinGW
static inline char* addr2line(unsigned long long addr)
{
   static char buf[1024];
   char cmd[256], *ret = nullptr;
#if defined __APPLE__
   sprintf(cmd, "atos -o %s 0x%llx", O.path, addr);
#else
   sprintf(cmd, "addr2line -C -a -s -p -f -e %s -i %llx", O.path, addr);
#endif
#if !defined _MSC_VER
   FILE* f = ::popen(cmd, "r");
   if (f) {
      ret = ::fgets(buf, sizeof(buf), f);
      ::pclose(f);
   }
   if (!ret) return nullptr;
   if (O.os == 'm' ? !memcmp(buf, "0x", 2) : !!strstr(buf, "??:")) return nullptr;
   *(char*)strip_right(buf) = '\0';
#endif
   return buf;
}
#endif

#if !(defined _MSC_VER || defined __CYGWIN__ || defined __MINGW32__ || defined __MINGW64__)
static inline bool backtrace_extract(const char* line, char* mangle_name, unsigned long long* displacement = nullptr)
{
   unsigned long long _t;
#if defined __APPLE__
   // MAC: `3   a.out  0x000000010e777f3d _ZN2h24unit6mallocEm + 45
   if (2 == ::sscanf(line, "%*s%*s%*s%s + %llu", mangle_name, displacement ? displacement : &_t)) return true;
#else
   // Linux: `./a.out(_ZN2h24unit7executeEv+0x131)[0x55aa6bb840ef]
   if (2 == ::sscanf(line, "%*[^(]%*[^_a-zA-Z]%1023[^)+]+0x%llx", mangle_name, displacement ? displacement : &_t)) return true;
   // Linux: `./a.out(+0xb1887)[0x560c5ed06887]
   mangle_name[0] = '\0';
   if (1 == ::sscanf(line, "%*[^(]%*[^+]+0x%llx", displacement ? displacement : &_t)) return true;
#endif
   return false;
}
#endif

h2_inline bool h2_backtrace::operator==(const h2_backtrace& bt) const
{
   if (count != bt.count) return false;
   for (int i = 0; i < count; ++i)
      if (frames[i] != bt.frames[i])
         return false;
   return true;
}

h2_inline h2_backtrace& h2_backtrace::dump(int shift_)
{
   static h2_backtrace s;
   s.shift = shift_;
#if defined _MSC_VER || defined __CYGWIN__ || defined __MINGW32__ || defined __MINGW64__
   s.count = CaptureStackBackTrace(0, sizeof(s.frames) / sizeof(s.frames[0]), s.frames, NULL);
#else
   h2_memory::hook(false);
   s.count = ::backtrace(s.frames, sizeof(s.frames) / sizeof(s.frames[0]));
   h2_memory::hook();
#endif
   return s;
}

h2_inline bool h2_backtrace::in(void* fps[]) const
{
   bool ret = false;
#if defined _MSC_VER
   for (int i = shift; !ret && i < count; ++i) {
      char buffer[sizeof(SYMBOL_INFO) + 256];
      SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
      symbol->MaxNameLen = 256;
      if (SymFromAddr(GetCurrentProcess(), (DWORD64)(frames[i]), 0, symbol))
         for (int j = 0; !ret && fps[j]; ++j)
            if ((unsigned long long)symbol->Address == (unsigned long long)fps[j])
               ret = true;
   }
#elif defined __CYGWIN__ || defined __MINGW32__ || defined __MINGW64__
   for (int i = shift; !ret && i < count; ++i)
      for (int j = 0; !ret && fps[j]; ++j)
         if ((unsigned long long)fps[j] <= (unsigned long long)frames[i] && (unsigned long long)frames[i] < 100 + (unsigned long long)fps[j])
            ret = true;
#else
   h2_memory::hook(false);
   char** symbols = backtrace_symbols(frames, count);
   for (int i = shift; !ret && i < count; ++i) {
      char _t[1024];
      unsigned long long displacement = 0;
      if (backtrace_extract(symbols[i], _t, &displacement))
         for (int j = 0; !ret && fps[j]; ++j)
            if ((unsigned long long)frames[i] - displacement == (unsigned long long)fps[j])
               ret = true;
   }
   free(symbols);
   h2_memory::hook();
#endif
   return ret;
}

h2_inline void h2_backtrace::print(h2_vector<h2_string>& stacks) const
{
#if defined _WIN32  // +MinGW
   for (int i = shift; i < count; ++i) {
      char buffer[sizeof(SYMBOL_INFO) + 256];
      SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
      symbol->MaxNameLen = 256;
      h2_string frame;
      if (SymFromAddr(GetCurrentProcess(), (DWORD64)(frames[i]), 0, symbol))
         frame.sprintf("%s %p+%lld ", symbol->Name, symbol->Address, (DWORD64)(frames[i]) - (DWORD64)symbol->Address);
      DWORD dwDisplacement;
      IMAGEHLP_LINE64 fileline;
      SymSetOptions(SYMOPT_LOAD_LINES);
      fileline.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
      if (SymGetLineFromAddr64(GetCurrentProcess(), (DWORD64)(frames[i]), &dwDisplacement, &fileline))
         frame.sprintf("%s:%d", fileline.FileName, fileline.LineNumber);
      stacks.push_back(frame);
      if (!strcmp("main", symbol->Name)) break;
   }
#elif defined __CYGWIN__
   for (int i = shift; i < count; ++i) {
      char* p = nullptr;
      p = addr2line(h2_load::ptr_to_addr(frames[i]));
      if (p) stacks.push_back(p);
   }
#else
   h2_memory::hook(false);
   char** symbols = backtrace_symbols(frames, count);
   h2_memory::hook();
   for (int i = shift; i < count; ++i) {
      char *p = nullptr, mangle_name[1024] = "", demangle_name[1024] = "";
      backtrace_extract(symbols[i], mangle_name);
      if (O.verbose >= VerboseDetail || O.os != 'm') p = addr2line(h2_load::ptr_to_addr(frames[i])); /* atos is slow */
      if (!p) p = h2_cxa::demangle(mangle_name, demangle_name);
      if (!p || !strlen(p)) {
         p = symbols[i];
         h2_once_if() h2_console::prints("yellow", "\nAdd -g to compiler options, -rdynamic to linker options\n");
      }
      stacks.push_back(p);
      if (!strcmp("main", mangle_name) || !strcmp("__libc_start_main", mangle_name)) break;
   }
   h2_memory::hook(false);
   free(symbols);
   h2_memory::hook();
#endif
}

h2_inline void h2_backtrace::print(size_t pad) const
{
   h2_vector<h2_string> stacks;
   print(stacks);
   h2_lines lines;
   for (auto& c : stacks) lines.push_back(c.startswith("h2::") || c.contains(": h2::") ? color(c, "dark gray") : h2_line(c));
   lines.sequence(pad);
   h2_console::printl(lines);
}
