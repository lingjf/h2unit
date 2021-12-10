#define H2_FILINE __FILE__ ":" H2PP_STR(__LINE__)

struct h2_once {
   unsigned long c = 0;
   operator bool() { return !c++; }
};

#define h2_array_append(Array, a)                                      \
   for (size_t i = 0; i < sizeof(Array) / sizeof(Array)[0] - 1; ++i) { \
      if (!(Array)[i]) {                                               \
         (Array)[i] = (a);                                             \
         break;                                                        \
      }                                                                \
   }

// #define M(...) func(#__VA_ARGS__, other)
// Unix M() ==> func("", other) stringify empty __VA_ARGS__ to "" string
// Windows M() ==> func(, other) stringify empty __VA_ARGS__ to empty
// #define M(...) func(ss(#__VA_ARGS__), other)
static inline const char* ss(const char* a = "") { return a ? a : ""; }
static inline const size_t sn(const size_t a = 0) { return a; }
static inline const char* comma_if(bool a, const char* t = ", ", const char* f = "") { return a ? t : f; }

#define H2Foreach(Callback_x, ...) H2PP_FOREACH(, _H2ForeachMacro, (Callback_x), H2PP_REMOVE_PARENTHESES_IF(__VA_ARGS__))
#define _H2ForeachMacro(_Args, i, x) H2PP_REMOVE_PARENTHESES(_Args)(x)

#define H2Fullmesh(Callback_x_y, ...) H2PP_FULLMESH(, _H2FullmeshMacro, (Callback_x_y), __VA_ARGS__)
#define _H2FullmeshMacro(_Args, i, j, x, y) H2PP_REMOVE_PARENTHESES(_Args)(x, y)

/* clang-format off */
#define h2_singleton(Class) static Class& I() { static Class i; return i; }
