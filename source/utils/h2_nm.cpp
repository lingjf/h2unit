
static inline void nm1(std::map<std::string, unsigned long long>*& symbols)
{
#if !defined _WIN32
   h2_memory::restores();
   char nm[256], line[2048], addr[128], type, name[2048];
   symbols = new std::map<std::string, unsigned long long>();
   sprintf(nm, "nm %s", O.path);
#   if defined __APPLE__
   sprintf(nm, "nm -U %s", O.path);
#   else
   sprintf(nm, "nm --defined-only %s", O.path);
#   endif
   FILE* f = ::popen(nm, "r");
   if (!f) return;
   while (::fgets(line, sizeof(line) - 1, f)) {
      if (3 != sscanf(line, "%s %c %s", addr, &type, name)) continue;
      if (strchr("bBcCdDiIuU", type)) continue;  // reject bBcCdDiIuU, accept tTwWsSvV, sS for vtable
      int _ = 0;
      if (O.os == macOS) _ = 1;  // remove prefix '_' in MacOS
      (*symbols)[name + _] = (unsigned long long)strtoull(addr, nullptr, 16);
   }
   ::pclose(f);
   h2_memory::overrides();
#endif
}

static inline void nm2(h2_list& symbols)
{
#if !defined _WIN32
   h2_memory::restores();
   char nm[256], line[2048], addr[128], type, name[2048];
#   if defined __APPLE__
   sprintf(nm, "nm --demangle -U %s", O.path);
#   else
   sprintf(nm, "nm --demangle --defined-only %s", O.path);
#   endif
   FILE* f = ::popen(nm, "r");
   if (!f) return;
   while (::fgets(line, sizeof(line) - 1, f)) {
      if (3 != sscanf(line, "%s %c %[^\n]", addr, &type, name)) continue;
      if (strchr("bBcCdDiIuU", type)) continue;
      int _ = 0;
      if (O.os == macOS && !strchr(name, '(')) _ = 1;
      symbols.push_back((new h2_symbol(name + _, (unsigned long long)strtoull(addr, nullptr, 16)))->x);
   }
   ::pclose(f);
   h2_memory::overrides();
#endif
}

h2_inline int h2_nm::get(const char* name, h2_scope res[], int n)
{
#if defined _WIN32
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (!SymFromName(O.hProcess, name, symbol))
      return 0;
   res[0].addr = (unsigned long long)symbol->Address;
   res[0].size = (unsigned long long)symbol->Size;
   return 1;
#else
   return 0;
#endif
}

h2_inline unsigned long long h2_nm::get(const char* name)
{
#if defined _WIN32
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (SymFromName(O.hProcess, name, symbol))
      return (unsigned long long)symbol->Address;
   return 0;
#else
   if (!name || strlen(name) == 0) return 0;
   if (!I().symbols_mangled) nm1(I().symbols_mangled);

   auto it = I().symbols_mangled->find(name);
   return it != I().symbols_mangled->end() ? it->second : 0ULL;
#endif
}

static inline bool strncmp_reverse(const char* a, const char* ae, const char* b, const char* be, int n)  // [a, ae) [b, be)
{
   if (ae < a + n || be < b + n) return false;
   return !strncmp(ae - n, be - n, n);
}

h2_inline int h2_nm::find(const char* name, h2_symbol* res[], int n)
{
   if (!name) return 0;
   int len = strlen(name);
   if (len == 0) return 0;
   if (I().symbols_demangled.empty()) nm2(I().symbols_demangled);

   h2_list_for_each_entry (p, I().symbols_demangled, h2_symbol, x) {
      if (!strcmp(p->name.c_str(), name)) {
         res[0] = p;
         return 1;
      }
   }

   int count = 0;
   h2_list_for_each_entry (p, I().symbols_demangled, h2_symbol, x) {
      char* parentheses = strchr((char*)p->name.c_str(), '(');
      if (!parentheses) continue;
      if (!strncmp_reverse(p->name.c_str(), parentheses, name, name + len, len)) continue;  // compare function name
      char* func = parentheses - len;
      if (p->name.c_str() < func && func[-1] != ':' && func[-2] != ':') continue;  // strip namespace
      if (count < n) res[count++] = p;
   }
   return count;
}

struct h2_offset {
   h2_offset() {}
   virtual ~h2_offset() {}
   virtual void dummy() {}

   static long long vtable_offset()
   {
      static long long s = get_vtable_offset();
      return s;
   }

   static long long get_vtable_offset()
   {
      char vtable_symbol[256];
      h2_offset t;
      long long absolute_vtable = (long long)*(void***)&t;
      sprintf(vtable_symbol, "_ZTV%s", typeid(h2_offset).name());  // mangled for "vtable for h2_offset"
      long long relative_vtable = (long long)h2_nm::get(vtable_symbol);
      return absolute_vtable - relative_vtable;
   }

   static long long text_offset()
   {
      static long long s = get_text_offset();
      return s;
   }

   static long long get_text_offset()
   {
      long long relative_vtable = (long long)h2_nm::get("main");
      return (long long)&main - relative_vtable;
   }
};

h2_inline long long h2_nm::text_offset()
{
   return h2_offset::text_offset();
}

h2_inline long long h2_nm::vtable_offset()
{
   return h2_offset::vtable_offset();
}

h2_inline bool h2_nm::in_main(unsigned long long addr)
{
   static unsigned long long main_addr = get("main");
   if (main_addr == 0) return false;
   /* main() 52~60 bytes code in linux MAC */
   return main_addr < addr && addr < main_addr + 128;
}
