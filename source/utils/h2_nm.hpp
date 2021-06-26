
struct h2_symbol {
   h2_list x;
   std::string name;
   unsigned long long addr;
   h2_symbol(char* _name, unsigned long long _addr) : name(_name), addr(_addr) {}
};

struct h2_scope {
   char name[256];
   unsigned long long addr, size;
};

struct h2_nm {
   h2_singleton(h2_nm);
   std::map<std::string, unsigned long long>* symbols_mangled;
   h2_list symbols_demangled;
   h2_nm(){};
   static unsigned long long get(const char* name);             // by mangled name
   static int get(const char* name, h2_scope res[], int n);    // by demangled name
   static int find(const char* name, h2_symbol* res[], int n);  // by demangled name
   static long long text_offset();
   static long long vtable_offset();
   static bool in_main(unsigned long long addr);
};
