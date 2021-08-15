
struct h2_symbol {
   h2_list x;
   char name[128];
   unsigned long long addr;
   h2_symbol(char* _name, unsigned long long _addr) : addr(_addr) { strncpy(name, _name, 127); }
};

struct h2_nm {
   h2_singleton(h2_nm);
   std::map<std::string, unsigned long long>* mangle_symbols;
   h2_list demangle_symbols;
   static int get_by_name(const char* name, h2_symbol* res[], int n);
   static h2_symbol* get_by_addr(unsigned long long addr);
   static unsigned long long get_mangle(const char* name);
   static char* demangle(const char* mangle_name);
};

template <typename T>
const char* h2_type_name(char* name)
{
   strcpy(name, "");
   typedef typename std::remove_reference<T>::type U;
   if (std::is_const<U>::value) strcat(name, "const ");
   strcat(name, h2_nm::demangle(typeid(U).name()));
   if (std::is_lvalue_reference<T>::value)
      strcat(name, "&");
   else if (std::is_rvalue_reference<T>::value)
      strcat(name, "&&");
   return name;
}
