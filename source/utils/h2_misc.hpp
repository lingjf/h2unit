#define H2_FILINE __FILE__ ":" H2PP_STR(__LINE__)

struct h2_once {
   unsigned long c = 0;
   operator bool() { return !c++; }
};

// #define M(...) func(#__VA_ARGS__, other)
// Unix M() ==> func("", other) stringify empty __VA_ARGS__ to "" string
// Windows M() ==> func(, other) stringify empty __VA_ARGS__ to empty
// #define M(...) func(ss(#__VA_ARGS__), other)
static inline const char* ss(const char* a = "") { return a ? a : ""; }
static inline const size_t sn(const size_t a = 0) { return a; }

#define H2Foreach(Callback_x, ...) H2PP_FOREACH(, _H2ForeachMacro, (Callback_x), H2PP_REMOVE_PARENTHESES_IF(__VA_ARGS__))
#define _H2ForeachMacro(_Args, i, x) H2PP_REMOVE_PARENTHESES(_Args)(x)

#define H2Fullmesh(Callback_x_y, ...) H2PP_FULLMESH(, _H2FullmeshMacro, (Callback_x_y), __VA_ARGS__)
#define _H2FullmeshMacro(_Args, i, j, x, y) H2PP_REMOVE_PARENTHESES(_Args)(x, y)

/* clang-format off */
#define h2_singleton(Class) static Class& I() { static Class i; return i; }
