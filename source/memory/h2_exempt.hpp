
struct h2_exempt {
   h2_singleton(h2_exempt);
   h2_list exempts;
   static void setup();
   static void add_by_addr(void* func, int size = 0);
   static void add_by_name(const char* func, int size = 0);
   static bool in(const h2_backtrace& bt);
};

template <typename T>
inline void h2_unmem(T func) { return h2_exempt::add_by_addr((void*)func); }
template <>
inline void h2_unmem(const char* func) { return h2_exempt::add_by_name(func); }

#define H2UNMEM(func) h2::h2_unmem(func)
