
struct h2_debugger {
   static void trap();
};

#define h2_debug(...)                                                               \
   do {                                                                             \
      if (!O.debug) {                                                               \
         h2_color::prints("", __VA_ARGS__);                                         \
         h2_color::prints("", " %s : %d = %s\n", __FILE__, __LINE__, __FUNCTION__); \
         h2_backtrace bt____(0);                                                    \
         bt____.print(3);                                                           \
      }                                                                             \
   } while (0)
