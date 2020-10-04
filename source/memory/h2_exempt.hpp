
struct h2_exempt {
   h2_list exempts;
   h2_singleton(h2_exempt);
   static void setup();
   static void add(void* func);
   static bool in(const h2_backtrace& bt);
};

#define H2UNMEM(func) h2::h2_exempt::add((void*)func)
