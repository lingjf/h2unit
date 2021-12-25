#if !defined _MSC_VER
static inline void __nm(h2_list& symbols, bool demangle)
{
   h2_memory::hook(false);
   char cmd[256], line[2048], addr[128], type, symbol[2048];
   sprintf(cmd, "nm --defined-only %s -n %s", demangle ? "-f bsd --demangle" : "", O.path);
   FILE* f = ::popen(cmd, "r");
   if (f) {
      while (::fgets(line, sizeof(line) - 1, f)) {
         if (3 != sscanf(line, "%s %c %[^\n]", addr, &type, symbol)) continue;
         if (strchr("bBcCdDiIuU", type)) continue;  // reject bBcCdDiIuU, accept tTwWsSvV, sS for vtable
         symbols.push_back((new h2_symbol(symbol, strtoull(addr, nullptr, 16)))->x);
      }
      ::pclose(f);
   }
   h2_memory::hook();
}
static inline bool __leading_underscore(h2_list& symbols)
{
   bool _main = false;
   h2_list_for_each_entry (p, symbols, h2_symbol, x) {
      if (!strcmp("main", p->symbol)) return false;
      if (!strcmp("_main", p->symbol)) _main = true;
   }
   return _main;
}
static inline void nm(bool demangle, h2_list& symbols, bool& leading_underscore)
{
   __nm(symbols, demangle);
   h2_once_if() leading_underscore = __leading_underscore(symbols);
}
#endif

h2_inline char* h2_symbol::name()
{
   return h2_nm::I().leading_underscore && symbol[0] == '_' ? symbol + 1 : symbol;  // remove leading underscore '_' in MacOS
}

static inline bool strncmp_reverse(const char* a, const char* ae, const char* b, const char* be, size_t n)  // [a, ae) [b, be)
{
   if (ae < a + n || be < b + n) return false;
   return !strncmp(ae - n, be - n, n);
}

h2_inline int h2_nm::get_by_name(const char* name, h2_symbol* res[], int n)
{
   if (!name) return 0;
   size_t len = strlen(name);
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
   if (I().demangle_symbols.empty()) nm(true, I().demangle_symbols, I().leading_underscore);

   h2_list_for_each_entry (p, I().demangle_symbols, h2_symbol, x) {
      if (!strcmp(p->name(), name)) {
         res[0] = p;
         return 1;
      }
   }
   int count = 0;
   h2_list_for_each_entry (p, I().demangle_symbols, h2_symbol, x) {
      char* parentheses = strchr(p->name(), '(');
      if (!parentheses) continue;
      if (!strncmp_reverse(p->name(), parentheses, name, name + len, len)) continue;  // compare function name
      char* func = parentheses - len;
      if (p->name() < func && (func[-1] != ':' || func[-2] != ':')) continue;  // strip namespace
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
   strcpy(s_symbol.symbol, symbol->Name);
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
   if (I().mangle_symbols.empty()) nm(false, I().mangle_symbols, I().leading_underscore);

   h2_list_for_each_entry (p, I().mangle_symbols, h2_symbol, x)
      if (!strcmp(p->name(), name)) return p->addr;

   return 0;
#endif
}
