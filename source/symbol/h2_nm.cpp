#if !defined _MSC_VER
static inline void nm_mangle(std::map<std::string, unsigned long long>*& symbols)
{
   h2_memory::hook(false);
   char nm[256], line[2048], addr[128], type, name[2048];
   symbols = new std::map<std::string, unsigned long long>();
#if defined __APPLE__
   sprintf(nm, "nm -U %s", O.path);
#else
   sprintf(nm, "nm --defined-only %s", O.path);
#endif
   FILE* f = ::popen(nm, "r");
   if (f) {
      while (::fgets(line, sizeof(line) - 1, f)) {
         if (3 != sscanf(line, "%s %c %s", addr, &type, name)) continue;
         if (strchr("bBcCdDiIuU", type)) continue;  // reject bBcCdDiIuU, accept tTwWsSvV, sS for vtable
         int underscore = 0;
         if (O.os == 'm') underscore = 1;  // remove prefix '_' in MacOS
         (*symbols)[name + underscore] = (unsigned long long)strtoull(addr, nullptr, 16);
      }
      ::pclose(f);
   }
   h2_memory::hook();
}

static inline void nm_demangle(h2_list& symbols)
{
   h2_memory::hook(false);
   char nm[256], line[2048], addr[128], type, name[2048];
#if defined __APPLE__
   sprintf(nm, "nm -f bsd --demangle -U -n %s", O.path);
#else
   sprintf(nm, "nm -f bsd --demangle --defined-only -n %s", O.path);
#endif
   FILE* f = ::popen(nm, "r");
   if (f) {
      while (::fgets(line, sizeof(line) - 1, f)) {
         if (3 != sscanf(line, "%s %c %[^\n]", addr, &type, name)) continue;
         if (strchr("bBcCdDiIuU", type)) continue;
         int underscore = 0;
         if (O.os == 'm' && !strchr(name, '(')) underscore = 1;
         h2_symbol* symbol = new h2_symbol(name + underscore, (unsigned long long)strtoull(addr, nullptr, 16));
         if (symbol) symbols.push_back(symbol->x);
      }
      ::pclose(f);
   }
   h2_memory::hook();
}
#endif

static inline bool strncmp_reverse(const char* a, const char* ae, const char* b, const char* be, int n)  // [a, ae) [b, be)
{
   if (ae < a + n || be < b + n) return false;
   return !strncmp(ae - n, be - n, n);
}

h2_inline void h2_nm::initialize()
{
#if defined _WIN32
   SymInitialize(GetCurrentProcess(), NULL, TRUE);
#endif
}
h2_inline int h2_nm::get_by_name(const char* name, h2_symbol* res[], int n)
{
   if (!name) return 0;
   int len = strlen(name);
   if (len == 0) return 0;
#if defined _MSC_VER
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (!SymFromName(GetCurrentProcess(), name, symbol)) return 0;
   static h2_symbol s_symbol("", 0);
   s_symbol.addr = (unsigned long long)symbol->Address;
   res[0] = &s_symbol;
   return 1;
#else
   if (I().demangle_symbols.empty()) nm_demangle(I().demangle_symbols);

   h2_list_for_each_entry (p, I().demangle_symbols, h2_symbol, x) {
      if (!strcmp(p->name, name)) {
         res[0] = p;
         return 1;
      }
   }
   int count = 0;
   h2_list_for_each_entry (p, I().demangle_symbols, h2_symbol, x) {
      char* parentheses = strchr(p->name, '(');
      if (!parentheses) continue;
      if (!strncmp_reverse(p->name, parentheses, name, name + len, len)) continue;  // compare function name
      char* func = parentheses - len;
      if (p->name < func && (func[-1] != ':' || func[-2] != ':')) continue;  // strip namespace
      if (count < n) res[count++] = p;
   }
   return count;
#endif
}

h2_inline h2_symbol* h2_nm::get_by_addr(unsigned long long addr)
{
#if defined _MSC_VER
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (!SymFromAddr(GetCurrentProcess(), (DWORD64)(addr), 0, symbol)) return nullptr;
   static h2_symbol s_symbol("", 0);
   strcpy(s_symbol.name, symbol->Name);
   s_symbol.addr = (unsigned long long)symbol->Address;
   return &s_symbol;
#else
   return nullptr;
#endif
}

h2_inline unsigned long long h2_nm::get_mangle(const char* name)
{
#if defined _MSC_VER
   char buffer[sizeof(SYMBOL_INFO) + 256];
   SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
   symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
   symbol->MaxNameLen = 256;
   if (!SymFromName(GetCurrentProcess(), name, symbol)) return 0;
   return (unsigned long long)symbol->Address;
#else
   if (!name || strlen(name) == 0) return 0;
   if (!I().mangle_symbols) nm_mangle(I().mangle_symbols);

   auto it = I().mangle_symbols->find(name);
   return it != I().mangle_symbols->end() ? it->second : 0ULL;
#endif
}
