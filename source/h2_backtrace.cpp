
struct h2_nm {
   h2_singleton(h2_nm);

   std::map<std::string, unsigned long long> symbols;
   unsigned long long main_addr;

   unsigned long long get(const char* name) const {
      if (strlen(name) == 0) return 0;
      auto it = symbols.find(name);
      return it != symbols.end() ? it->second : ULLONG_MAX;
   }

   bool in_main(unsigned long long addr) const { return main_addr == ULLONG_MAX ? false : main_addr < addr && addr < main_addr + 256; }

   h2_nm() {
      nm_parse();
      main_addr = get("main");
   }

   void nm_parse() {
      char nm[256], line[1024], addr[128], type[32], name[1024];
      sprintf(nm, "nm %s", O.path);
      h2_with f(::popen(nm, "r"), ::pclose);
      if (f.f)
         while (::fgets(line, sizeof(line) - 1, f.f))
            if (3 == sscanf(line, "%s%s%s", addr, type, name))
               if (type[0] == 't' || type[0] == 'T' || type[0] == 'w' || type[0] == 'W')
                  symbols.insert(std::make_pair(name + O.isMAC(), (unsigned long long)strtoull(addr, nullptr, 16)));
   }
};

static inline bool demangle(const char* mangled, char* demangled, size_t len) {
   int status = 0;
   abi::__cxa_demangle(mangled, demangled, &len, &status);
   return status == 0;
}

static inline bool addr2line(unsigned long long addr, char* output, size_t len) {
   char t[256];
#if defined __APPLE__
   sprintf(t, "atos -o %s 0x%llx", O.path, addr);
#else
   sprintf(t, "addr2line -C -a -s -p -f -e %s -i %llx", O.path, addr);
#endif
   h2_with f(::popen(t, "r"), ::pclose);
   if (!f.f || !::fgets(output, len, f.f)) return false;
   for (int i = strlen(output) - 1; 0 <= i && ::isspace(output[i]); --i) output[i] = '\0';  //strip tail
   return true;
}

static inline bool backtrace_extract(const char* backtrace_symbol_line, char* module, char* mangled, unsigned long long* offset) {
   //MAC: `3   a.out  0x000000010e777f3d _ZN2h24hook6mallocEm + 45
   if (3 == ::sscanf(backtrace_symbol_line, "%*s%s%*s%s + %llu", module, mangled, offset))
      return true;

   //Linux: with '-rdynamic' linker option
   //Linux: module_name(mangled_function_name+relative_offset_to_function)[absolute_address]
   //Linux: `./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]
   if (3 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^_a-zA-Z]%127[^)+]+0x%llx", module, mangled, offset))
      return true;

   mangled[0] = '\0';

   //Linux: Ubuntu without '-rdynamic' linker option
   //Linux: module_name(+relative_offset_to_function)[absolute_address]
   //Linux: `./a.out(+0xb1887)[0x560c5ed06887]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^+]+0x%llx", module, offset))
      return true;

   //Linux: Redhat/CentOS without '-rdynamic' linker option
   //Linux: module_name()[relative_offset_to_module]
   //Linux: `./a.out() [0x40b960]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^[][0x%llx", module, offset))
      return true;

   //Where?
   //Linux: module_name[relative_offset_to_module]
   //Linux: `./a.out[0x4060e7]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^[][0x%llx", module, offset))
      return true;

   return false;
}

h2_inline h2_backtrace::h2_backtrace(int shift_) : shift(shift_) {
   h2_heap::unhook();
   count = ::backtrace(array, sizeof(array) / sizeof(array[0]));
   h2_heap::dohook();
}

h2_inline bool h2_backtrace::operator==(h2_backtrace& bt) {
   if (count != bt.count) return false;
   for (int i = 0; i < count; ++i)
      if (array[i] != bt.array[i])
         return false;
   return true;
}

h2_inline bool h2_backtrace::has(void* func, int size) const {
   for (int i = 0; i < count; ++i)
      if (func <= array[i] && (unsigned char*)array[i] < ((unsigned char*)func) + size)
         return true;
   return false;
}

h2_inline void h2_backtrace::print(int pad) const {
   h2_heap::unhook();
   char** backtraces = backtrace_symbols(array, count);
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
         address = h2_nm::I().get(mangled);
         if (address != ULLONG_MAX)
            if (addr2line(address + offset, addr2lined, sizeof(addr2lined)))
               if (strlen(addr2lined))
                  p = addr2lined;
      }
      ::printf("%s%d. %s\n", PAD(pad), i - shift, p);

      if (streq("main", mangled) || streq("main", demangled) || h2_nm::I().in_main(address + offset))
         break;
   }
   free(backtraces);
   h2_heap::dohook();
}
