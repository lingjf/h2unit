struct h2_debugger {
   static void trap();
};

#define h2_debug(shift, ...)                                                          \
   do {                                                                               \
      if (!O.debugger_trap) {                                                         \
         h2_console::prints("", __VA_ARGS__);                                         \
         h2_console::prints("", " %s : %d = %s\n", __FILE__, __LINE__, __FUNCTION__); \
         h2_backtrace::dump(shift).print(3);                                          \
      }                                                                               \
   } while (0)
