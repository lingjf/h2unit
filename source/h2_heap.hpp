
struct h2_heap {
   static void initialize();
   static void dohook();
   static void unhook();

   struct stack {
      static void root();
      static void push(const char* file, int line);
      static h2_fail* pop();

      struct block : h2_once {
         block(const char* attributes, const char* file, int line);
         ~block();
      };
   };
};
