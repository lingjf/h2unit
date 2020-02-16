
static inline void h2_dohook_g();
static inline void h2_unhook_g();

struct h2_nm {
   /* clang-format off */
   static h2_nm& I() { static h2_nm __; return __; }
   /* clang-format on */

   unsigned long get(const char* name) const {
      auto it = symbols.find(name);
      return it != symbols.end() ? it->second : ULONG_MAX;
   }

   bool in_main(unsigned long addr) const { return main_addr == ULONG_MAX ? false : main_addr < addr && addr < main_addr + 64; }

   h2_nm() {
      char nm[256], line[1024], addr[32], type[32], name[1024];
      sprintf(nm, "nm %s", h2cfg().path);
      h2_with f(::popen(nm, "r"), ::pclose);
      if (f.f)
         while (::fgets(line, sizeof(line) - 1, f.f))
            if (3 == sscanf(line, "%s%s%s", addr, type, name))
               if (::tolower((int)type[0]) == 't' || ::tolower((int)type[0]) == 'w')
                  symbols.insert(std::make_pair(name + h2cfg().isMAC(), strtol(addr, nullptr, 16)));

      main_addr = get("main");
   }

   std::map<std::string, unsigned long> symbols;
   unsigned long main_addr;
};

struct h2_backtrace {
   int count, shift;
   void* array[100];

   h2_backtrace() : count(0), shift(0) {}

   h2_backtrace(int shift_) : shift(shift_) {
      h2_unhook_g();
      count = ::backtrace(array, sizeof(array) / sizeof(array[0]));
      h2_dohook_g();
   }

   h2_backtrace(const h2_backtrace&) = default;
   h2_backtrace& operator=(const h2_backtrace&) = default;

   bool operator==(h2_backtrace& bt) {
      if (count != bt.count) return false;
      for (int i = 0; i < count; ++i)
         if (array[i] != bt.array[i])
            return false;
      return true;
   }

   bool has(void* func, int size) {
      for (int i = 0; i < count; ++i)
         if (func <= array[i] && (unsigned char*)array[i] < ((unsigned char*)func) + size)
            return true;
      return false;
   }

   void print() const {
      h2_unhook_g();
      char** backtraces = backtrace_symbols(array, count);
      for (int i = shift; i < count; ++i) {
         char *p = backtraces[i], module[256], mangled[256], demangled[256], addr2lined[512];
         unsigned long address = 0, offset = 0;
         if (extract(backtraces[i], module, mangled, &offset)) {
            if (strlen(mangled)) p = mangled;
            if (demangle(mangled, demangled, sizeof(demangled)) && strlen(demangled)) p = demangled;
            address = strlen(mangled) ? h2_nm::I().get(mangled) : 0;
            if (address != ULONG_MAX && h2_endswith_string(h2cfg().path, module))
               if (addr2line(address + offset, addr2lined, sizeof(addr2lined)))
                  p = addr2lined;
         }
         ::printf("   %d. %s\n", i - shift, p);

         if (!strcmp("main", mangled) || !strcmp("main", demangled) || h2_nm::I().in_main(address + offset))
            break;
      }
      free(backtraces);
      h2_dohook_g();
   }

   bool addr2line(unsigned long addr, char* output, size_t len) const {
      char t[256];
#if defined __APPLE__
      sprintf(t, "atos -o %s 0x%lx", h2cfg().path, addr);
#else
      sprintf(t, "addr2line -C -a -s -p -f -e %s -i %lx", h2cfg().path, addr);
#endif
      h2_with f(::popen(t, "r"), ::pclose);
      if (!f.f || !::fgets(output, len, f.f)) return false;
      for (int i = strlen(output) - 1; 0 <= i && ::isspace(output[i]); --i) output[i] = '\0';  //strip tail
      return 0 < strlen(output);
   }

   bool extract(const char* backtrace_symbol_line, char* module, char* mangled, unsigned long* offset) const {
      //MAC: `3   a.out  0x000000010e777f3d _ZN2h24hook6mallocEm + 45
      if (3 == ::sscanf(backtrace_symbol_line, "%*s%s%*s%s + %lu", module, mangled, offset))
         return true;

      //Linux: with '-rdynamic' linker option
      //Linux: module_name(mangled_function_name+relative_offset_to_function)[absolute_address]
      //Linux: `./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]
      if (3 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^_a-zA-Z]%127[^)+]+0x%lx", module, mangled, offset))
         return true;

      mangled[0] = '\0';

      //Linux: Ubuntu without '-rdynamic' linker option
      //Linux: module_name(+relative_offset_to_function)[absolute_address]
      //Linux: `./a.out(+0xb1887)[0x560c5ed06887]
      if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^+]+0x%lx", module, offset))
         return true;

      //Linux: Redhat/CentOS without '-rdynamic' linker option
      //Linux: module_name()[relative_offset_to_module]
      //Linux: `./a.out() [0x40b960]
      if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^[][0x%lx", module, offset))
         return true;

      //Where?
      //Linux: module_name[relative_offset_to_module]
      //Linux: `./a.out[0x4060e7]
      if (2 == ::sscanf(backtrace_symbol_line, "%[^[][0x%lx", module, offset))
         return true;

      return false;
   }

   bool demangle(const char* mangled, char* demangled, size_t len) const {
      int status = 0;
      strcpy(demangled, mangled);
      abi::__cxa_demangle(mangled, demangled, &len, &status);
      return status == 0;
   }
};

#define h2_debug(...)                                                              \
   do {                                                                            \
      printf("%s %s : %d = %s\n", #__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__); \
      h2_backtrace bt(0);                                                          \
      bt.print();                                                                  \
   } while (0)
