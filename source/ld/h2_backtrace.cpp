
static inline bool demangle(const char* mangled, char* demangled, size_t len)
{
   int status = 0;
#if !defined _WIN32
   abi::__cxa_demangle(mangled, demangled, &len, &status);
#endif
   return status == 0;
}

#if !defined _WIN32
static inline bool addr2line(unsigned long long addr, char* output, size_t len)
{
   char t[256];
#   if defined __APPLE__
   sprintf(t, "atos -o %s 0x%llx", O.path, addr);
#   else
   sprintf(t, "addr2line -C -a -s -p -f -e %s -i %llx", O.path, addr);
#   endif
   FILE* f = ::popen(t, "r");
   if (!f) return false;
   output = ::fgets(output, len, f);
   ::pclose(f);
   if (!output) return false;
   for (int i = strlen(output) - 1; 0 <= i && ::isspace(output[i]); --i) output[i] = '\0';  //strip tail
   return true;
}
#endif

static inline bool backtrace_extract(const char* backtrace_symbol_line, char* module, char* mangled, unsigned long long* offset)
{
   //MAC: `3   a.out  0x000000010e777f3d _ZN2h24hook6mallocEm + 45
   if (3 == ::sscanf(backtrace_symbol_line, "%*s%s%*s%s + %llu", module, mangled, offset)) return true;

   //Linux: with '-rdynamic' linker option
   //Linux: module_name(mangled_function_name+relative_offset_to_function)[absolute_address]
   //Linux: `./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]
   if (3 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^_a-zA-Z]%127[^)+]+0x%llx", module, mangled, offset)) return true;

   mangled[0] = '\0';

   //Linux: Ubuntu without '-rdynamic' linker option
   //Linux: module_name(+relative_offset_to_function)[absolute_address]
   //Linux: `./a.out(+0xb1887)[0x560c5ed06887]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^+]+0x%llx", module, offset)) return true;

   //Linux: Redhat/CentOS without '-rdynamic' linker option
   //Linux: module_name()[relative_offset_to_module]
   //Linux: `./a.out() [0x40b960]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^[][0x%llx", module, offset)) return true;

   //Where?
   //Linux: module_name[relative_offset_to_module]
   //Linux: `./a.out[0x4060e7]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^[][0x%llx", module, offset)) return true;

   return false;
}

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

h2_inline bool h2_backtrace::has(void* func, int size) const
{
   for (int i = 0; i < count; ++i)
      if (func <= frames[i] && (unsigned char*)frames[i] < ((unsigned char*)func) + size)
         return true;
   return false;
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
      if (!strcmp("main", symbol->Name) || h2_load::in_main(frames[i]))
         break;
   }
#else
   h2_memory::restores();
   char** backtraces = backtrace_symbols(frames, count);
   h2_memory::overrides();

   for (int i = shift; i < count; ++i) {
      char *p = backtraces[i], module[256] = "", mangled[256] = "", demangled[256] = "", addr2lined[512] = "";
      unsigned long long address = 0, offset = 0;
      if (backtrace_extract(backtraces[i], module, mangled, &offset)) {
         if (strlen(mangled)) {
            p = mangled;
            if (demangle(mangled, demangled, sizeof(demangled)))
               if (strlen(demangled))
                  p = demangled;
         }
         if (O.verbose || O.os != macOS /* for speed atos is slow */)
            if (addr2line(h2_load::addr_to_symbol(frames[i]), addr2lined, sizeof(addr2lined)))
               if (strlen(addr2lined))
                  p = addr2lined;
      }
      stacks.push_back(p);

      if (!strcmp("main", mangled) || !strcmp("main", demangled) || h2_load::in_main(frames[i]))
         break;
   }

   h2_memory::restores();
   free(backtraces);
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
