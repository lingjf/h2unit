struct h2_memory {
   static void initialize();
   static void finalize();
   static void hook(bool overrides = true);
   static void try_free(void* ptr);

   struct stack {
      static void root();
      static void push(const char* filine);
      static h2_fail* pop();
      static long long footprint();

      struct block : h2_once {
         bool unmem = false;
         block(const char* attributes, const char* filine);
         ~block();
      };
   };
};

template <typename T> inline void h2_unmem(T f) { h2_exempt::add_by_fp(h2_numberfy<void*>(f)); }
template <> inline void h2_unmem(const char* f) { h2_exempt::add_by_name(f); }
template <> inline void h2_unmem(char* f) { h2_exempt::add_by_name((const char*)f); }

#define __H2BLOCK(Attributes, Qb) for (h2::h2_memory::stack::block Qb(h2::ss(Attributes), H2_FILINE); Qb;)
#define H2BLOCK(...) __H2BLOCK(#__VA_ARGS__, H2PP_UNIQUE())

#define H2UNMEM(...) H2PP_CAT(H2UNMEM_, H2PP_IS_EMPTY(__VA_ARGS__))(__VA_ARGS__)
#define H2UNMEM_1(...) __H2BLOCK("unmem", H2PP_UNIQUE())
#define H2UNMEM_0(...) h2::h2_unmem(__VA_ARGS__)
