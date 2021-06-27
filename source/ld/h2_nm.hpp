
struct h2_symbol {
   h2_list x;
   char name[128];
   unsigned long long offset;
   int size = 0;
   h2_symbol(char* _name, unsigned long long _offset) : offset(_offset) { strncpy(name, _name, 127); }
};

struct h2_nm {
   h2_singleton(h2_nm);
   std::map<std::string, unsigned long long>* symbols_mangled;
   h2_list symbols_demangled;
   static int get_by_name(const char* name, h2_symbol* res[], int n);
   static h2_symbol* get_by_offset(unsigned long long offset);
   static unsigned long long get_mangled(const char* name);
};
