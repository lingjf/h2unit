struct h2_symbol {
   h2_list x;
   char symbol[128]{'\0'};
   unsigned long long addr;
   h2_symbol(const char* symbol_, unsigned long long addr_) : addr(addr_) { strncpy(symbol, symbol_, 127); }
   char* name();
};

struct h2_nm {
   h2_singleton(h2_nm);
   h2_list mangle_symbols, demangle_symbols;
   bool leading_underscore = false;
   static int get_by_name(const char* name, h2_symbol* res[], int n);
   static h2_symbol* get_by_addr(unsigned long long addr);
   static unsigned long long get_mangle(const char* name);
};
