
struct h2_debugger {
   static void trap();
};

#define h2_debug(...)                                                                               \
   do {                                                                                             \
      if (!O.debug) {                                                                               \
         h2_color::prints("", "%s %s : %d = %s\n", #__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__); \
         h2_backtrace bt(0);                                                                        \
         bt.print(3);                                                                               \
      }                                                                                             \
   } while (0)
