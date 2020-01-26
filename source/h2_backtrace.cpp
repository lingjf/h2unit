
static inline void h2_dohook_g();
static inline void h2_unhook_g();

struct h2_nm {
   /* clang-format off */
   static h2_nm& I() { static h2_nm I; return I; }
   /* clang-format on */

   unsigned long get(const char* name) const {
      auto it = symbols.find(name);
      return it != symbols.end() ? it->second : ULONG_MAX;
   }

   bool in_main(unsigned long ptr) const { return main_ptr == ULONG_MAX ? false : main_ptr < ptr && ptr < main_ptr + 64; }

   h2_nm() {
      char buf[256], addr[32], type[32], name[1024];
      snprintf(buf, sizeof(buf), "nm %s", h2_cfg().path);
      h2_file pfp(::popen(buf, "r"), ::pclose);
      if (pfp.fp)
         while (::fgets(buf, sizeof(buf) - 1, pfp.fp))
            if (3 == sscanf(buf, "%s%s%s", addr, type, name))
               if (::tolower((int)type[0]) == 't' || ::tolower((int)type[0]) == 'w')
                  symbols.insert(std::make_pair(name + (!strcmp(h2_cfg().platform, "MAC") ? 1 : 0), strtol(addr, nullptr, 16)));

      main_ptr = get("main");
   }

   std::map<std::string, unsigned long> symbols;
   unsigned long main_ptr;
};

struct h2_backtrace {
   int count;
   int offset;
   void* array[100];

   h2_backtrace() : count(0), offset(0) {}

   h2_backtrace(int offset) {
      h2_unhook_g();
      count = ::backtrace(array, sizeof(array) / sizeof(array[0]));
      this->offset = offset;
      h2_dohook_g();
   }

   h2_backtrace(h2_backtrace* bt) {
      if (!bt)
         h2_backtrace();
      else
         offset = bt->offset, count = bt->count, memcpy(array, bt->array, bt->count * sizeof(void*));
   }

   h2_backtrace(const h2_backtrace&) = default;
   h2_backtrace& operator=(const h2_backtrace&) = default;

   bool operator==(h2_backtrace& bt) {
      if (count != bt.count) return false;
      for (int i = 0; i < count; ++i)
         if (array[i] != bt.array[i]) return false;
      return true;
   }

   bool has(void* func, int size) {
      for (int i = 0; i < count; ++i)
         if (func <= array[i] && (unsigned char*)array[i] < ((unsigned char*)func) + size) return true;
      return false;
   }

   void print() const {
      h2_file tfp(::tmpfile(), ::fclose);
      if (tfp.fp) {
         backtrace_symbols_fd(array, count, fileno(tfp.fp));
         ::fseek(tfp.fp, 0, SEEK_SET);
         h2_unhook_g();
         char backtrace_symbol_line[512], module[256], mangled[256], demangled[256], addr2lined[512];
         for (int i = 0; i < count && ::fgets(backtrace_symbol_line, 512, tfp.fp); ++i) {
            if (i < offset) continue;
            for (int j = strlen(backtrace_symbol_line) - 1; 0 <= j && isspace(backtrace_symbol_line[j]); --j) backtrace_symbol_line[j] = '\0';
            char* p = backtrace_symbol_line;
            unsigned long addr = 0, delta = 0;
            if (extract(backtrace_symbol_line, module, mangled, &delta)) {
               if (strlen(mangled)) p = mangled;
               if (demangle(mangled, demangled, sizeof(demangled)))
                  if (strlen(demangled)) p = demangled;
               addr = strlen(mangled) ? h2_nm::I().get(mangled) : 0;
               if (addr != ULONG_MAX && h2_endswith_string(h2_cfg().path, module))
                  if (addr2line(addr + delta, h2_cfg().path, addr2lined, sizeof(addr2lined)))
                     p = addr2lined;
            }
            ::printf("   %d. %s\n", i - offset, p);
            if (!strcmp("main", mangled) || !strcmp("main", demangled) || h2_nm::I().in_main(addr + delta)) break;
         }
         h2_dohook_g();
      }
   }

   // void print() const {
   //    h2_unhook_g();
   //    char** backtraces = backtrace_symbols(array, count);
   //    for (int i = offset; i < count; ++i) {
   //       char *p = backtraces[i], module[256], mangled[256], demangled[256], addr2lined[512];
   //       unsigned long addr = 0, delta = 0;
   //       if (extract(backtraces[i], module, mangled, &delta)) {
   //          if (strlen(mangled)) p = mangled;
   //          if (demangle(mangled, demangled, sizeof(demangled)))
   //             if (strlen(demangled)) p = demangled;
   //          addr = strlen(mangled) ? h2_nm::I().get(mangled) : 0;
   //          if (addr != ULONG_MAX && h2_endswith_string(h2_cfg().path, module))
   //             if (addr2line(addr + delta, h2_cfg().path, addr2lined, sizeof(addr2lined)))
   //                p = addr2lined;
   //       }
   //       ::printf("   %d. %s\n", i - offset, p);
   //       if (!strcmp("main", mangled) || !strcmp("main", demangled) || h2_nm::I().in_main(addr + delta)) break;
   //    }
   //    free(backtraces);
   //    h2_dohook_g();
   // }

   bool addr2line(unsigned long addr, const char* path, char* output, size_t len) const {
      char buf[256];
#if defined(__APPLE__)
      snprintf(buf, sizeof(buf), "atos -o %s 0x%lx", path, addr);
#else
      snprintf(buf, sizeof(buf), "addr2line -C -a -s -p -f -e %s -i %lx", path, addr);
#endif
      h2_file pfp(::popen(buf, "r"), ::pclose);
      if (pfp.fp)
         if (::fgets(output, len, pfp.fp))
            for (int i = strlen(output) - 1; 0 <= i && isspace(output[i]); --i) output[i] = '\0';
      return !!pfp.fp;
   }

   bool extract(const char* backtrace_symbol_line, char* module, char* mangled, unsigned long* offset) const {
      //MAC: `3   a.out  0x000000010e777f3d _ZN2h24hook6mallocEm + 45
      if (3 == ::sscanf(backtrace_symbol_line, "%*s%s%*s%s + %lu", module, mangled, offset))
         return true;

      //Linux: `./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]
      if (3 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^_a-zA-Z]%127[^)+]+0x%lx", module, mangled, offset))
         return true;

      //Linux: `./a.out(+0xb1887)[0x560c5ed06887]
      if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^+]+0x%lx", module, offset)) {
         mangled[0] = '\0';
         return true;
      }

      //Linux: `./a.out[0x4060e7]
      if (2 == ::sscanf(backtrace_symbol_line, "%[^[][0x%lx", module, offset)) {
         mangled[0] = '\0';
         return true;
      }

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
