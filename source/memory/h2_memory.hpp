
struct h2_memory {
   static void initialize();
   static void finalize();
   static void overrides();
   static void restores();

   struct stack {
      static void root();
      static void push(const char* file, int line);
      static h2_fail* pop();
      static long long footprint();

      struct block : h2_once {
         block(const char* attributes, const char* file, int line);
         ~block();
      };
   };
};

#define __H2BLOCK(Attributes, Qb) for (h2::h2_memory::stack::block Qb(h2::ss(Attributes), __FILE__, __LINE__); Qb;)
#define H2BLOCK(...) __H2BLOCK(#__VA_ARGS__, H2PP_UNIQUE())

template <typename T>
inline void h2_unmem(T f) { h2_exempt::add_by_fp(h2_un(f)); }
template <>
inline void h2_unmem(const char* f) { h2_exempt::add_by_name(f); }

#define H2UNMEM(f) h2::h2_unmem(f)
