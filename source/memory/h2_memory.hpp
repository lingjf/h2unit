struct h2_memory {
   static void initialize();
   static void finalize();
   static void hook(bool overrides = true);

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

#define __H2BLOCK(Attributes, Qb) for (h2::h2_memory::stack::block Qb(h2::ss(Attributes), H2_FILINE); Qb;)
#define H2BLOCK(...) __H2BLOCK(#__VA_ARGS__, H2PP_UNIQUE())

template <typename T>
inline void h2_unmem(T f) { h2_exempt::add_by_fp(h2_un<void*>(f)); }
template <>
inline void h2_unmem(const char* f) { h2_exempt::add_by_name(f); }

#define H2UNMEM(f) h2::h2_unmem(f)
