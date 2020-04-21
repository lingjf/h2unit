
static inline void h2_fail_g(h2_fail* fail);

struct h2_heap {
   static void initialize();
   static void dohook();
   static void unhook();

   static void stack_push_block(const char* file, int line, const char* where, long long limited, const char* fill);
   static h2_fail* stack_pop_block();

   struct stack {
      static void root() { stack_push_block(__FILE__, __LINE__, "root", LLONG_MAX >> 9, nullptr); }
      static void push(const char* file, int line, long long limited = LLONG_MAX >> 9, const char* fill = nullptr) { stack_push_block(file, line, "case", limited, fill); }
      static h2_fail* pop() { return stack_pop_block(); }

      struct block : h2_once {
         block(const char* file, int line, long long limited = LLONG_MAX >> 9, const char* fill = nullptr) { stack_push_block(file, line, "block", limited, fill); }
         ~block() { h2_fail_g(stack_pop_block()); }
      };
   };
};
