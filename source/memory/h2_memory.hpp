
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

#define __H2BLOCK(Attributes, Qb) for (h2::h2_memory::stack::block Qb(Attributes, __FILE__, __LINE__); Qb;)
#define H2BLOCK(...) __H2BLOCK(#__VA_ARGS__, H2PP_UNIQUE(t_block))
