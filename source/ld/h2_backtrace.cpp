
#if !defined _WIN32
static inline bool demangle(const char* mangle_name, char* demangle_name, size_t len)
{
   int status = 0;
   abi::__cxa_demangle(mangle_name, demangle_name, &len, &status);
   return status == 0;
}

static inline bool addr2line(unsigned long long addr, char* output, size_t len)
{
   char t[256];
   sprintf(t, O.os == macOS ? "atos -o %s 0x%llx" : "addr2line -C -a -s -p -f -e %s -i %llx", O.path, addr);
   FILE* f = ::popen(t, "r");
   if (!f) return false;
   output = ::fgets(output, len, f);
   ::pclose(f);
   if (!output) return false;
   for (int i = strlen(output) - 1; 0 <= i && ::isspace(output[i]); --i) output[i] = '\0';  //strip tail
   return true;
}

static inline bool backtrace_extract(const char* backtrace_line, char* mangle_name, unsigned long long* displacement)
{
#   if defined __APPLE__
   //MAC: `3   a.out  0x000000010e777f3d _ZN2h24hook6mallocEm + 45
   if (2 == ::sscanf(backtrace_line, "%*s%*s%*s%s + %llu", mangle_name, displacement)) return true;
#   else
   static unsigned long long v1 = 0, v2 = 0, once = 0;
   //Linux: `./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]
   if (2 == ::sscanf(backtrace_line, "%*[^(]%*[^_a-zA-Z]%1023[^)+]+0x%llx", mangle_name, displacement)) return (bool)++v2;

   //Linux: `./a.out(+0xb1887)[0x560c5ed06887]
   mangle_name[0] = '\0';
   if (1 == ::sscanf(backtrace_line, "%*[^(]%*[^+]+0x%llx", displacement)) return (bool)++v1;

   if (!v2 && !once++) h2_color::prints("yellow", "\nAdd -rdynamic to linker options\n");
#   endif
   return false;
}
#endif

h2_inline h2_backtrace::h2_backtrace(int shift_) : shift(shift_)
{
   h2_memory::restores();
#ifdef _WIN32
   count = CaptureStackBackTrace(0, sizeof(frames) / sizeof(frames[0]), frames, NULL);
#else
   count = ::backtrace(frames, sizeof(frames) / sizeof(frames[0]));
#endif
   h2_memory::overrides();
}

h2_inline bool h2_backtrace::operator==(const h2_backtrace& bt)
{
   if (count != bt.count) return false;
   for (int i = 0; i < count; ++i)
      if (frames[i] != bt.frames[i])
         return false;
   return true;
}

h2_inline bool h2_backtrace::in(void* fps[]) const
{
   bool ret = false;
#ifdef _WIN32
   for (int i = shift; !ret && i < count; ++i) {
      char buffer[sizeof(SYMBOL_INFO) + 256];
      SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
      symbol->MaxNameLen = 256;
      if (SymFromAddr(O.hProcess, (DWORD64)(frames[i]), 0, symbol)) {
         for (int j = 0; !ret && fps[j]; ++j)
            if ((unsigned long long)symbol->Address == (unsigned long long)fps[j])
               ret = true;
         if (!strcmp("main", symbol->Name)) break;
      }
   }
#else
   h2_memory::restores();
   char** backtrace_lines = backtrace_symbols(frames, count);
   for (int i = shift; !ret && i < count; ++i) {
      char mangle_name[1024] = "";
      unsigned long long displacement = 0;
      if (backtrace_extract(backtrace_lines[i], mangle_name, &displacement)) {
         for (int j = 0; !ret && fps[j]; ++j)
            if ((unsigned long long)frames[i] - displacement == (unsigned long long)fps[j])
               ret = true;
         if (!strcmp("main", mangle_name)) break;
      }
   }
   free(backtrace_lines);
   h2_memory::overrides();
#endif
   return ret;
}

h2_inline void h2_backtrace::print(h2_vector<h2_string>& stacks) const
{
#ifdef _WIN32
   for (int i = shift; i < count; ++i) {
      char buffer[sizeof(SYMBOL_INFO) + 256];
      SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
      symbol->MaxNameLen = 256;
      h2_string frame;
      if (SymFromAddr(O.hProcess, (DWORD64)(frames[i]), 0, symbol))
         frame.sprintf("%s %p+%lld ", symbol->Name, symbol->Address, (DWORD64)(frames[i]) - (DWORD64)symbol->Address);
      DWORD dwDisplacement;
      IMAGEHLP_LINE64 fileline;
      SymSetOptions(SYMOPT_LOAD_LINES);
      fileline.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
      if (SymGetLineFromAddr64(O.hProcess, (DWORD64)(frames[i]), &dwDisplacement, &fileline))
         frame.sprintf("%s:%d", fileline.FileName, fileline.LineNumber);
      stacks.push_back(frame);
      if (!strcmp("main", symbol->Name)) break;
   }
#else
   h2_memory::restores();
   char** backtrace_lines = backtrace_symbols(frames, count);
   h2_memory::overrides();

   for (int i = shift; i < count; ++i) {
      char* p = backtrace_lines[i];
      char mangle_name[1024] = "", demangle_name[1024] = "", symbolic[1024] = "";
      unsigned long long displacement = 0;
      if (backtrace_extract(backtrace_lines[i], mangle_name, &displacement))
         if (strlen(mangle_name))
            if (demangle(p = mangle_name, demangle_name, sizeof(demangle_name)))
               if (strlen(demangle_name))
                  p = demangle_name;
      if (O.verbose || O.os != macOS /* atos is slow */)
         if (addr2line(h2_load::ptr_to_addr(frames[i]), symbolic, sizeof(symbolic)))
            if (strlen(symbolic))
               p = symbolic;
      stacks.push_back(p);
      if (!strcmp("main", mangle_name)) break;
   }

   h2_memory::restores();
   free(backtrace_lines);
   h2_memory::overrides();
#endif
}

h2_inline void h2_backtrace::print(int pad) const
{
   h2_vector<h2_string> stacks;
   print(stacks);
   h2_rows rows;
   for (auto& c : stacks)
      rows.push_back(c.startswith("h2::") ? gray(c) : h2_row(c));
   rows.sequence(pad);
   h2_color::printl(rows);
}
