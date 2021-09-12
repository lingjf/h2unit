struct h2_once {
   unsigned long c = 0;
   operator bool() { return !c++; }
};

struct h2_extract {
   static const char* has(const char* attributes, const char* key);
   static bool numeric(const char* attributes, const char* key, double &value);
   static bool iport(const char* attributes, const char* key, char* str);
   static int fill(const char* attributes, const char* key, unsigned char bytes[]);
};

// #define M(...) func(#__VA_ARGS__, other)
// Unix M() ==> func("", other) stringify empty __VA_ARGS__ to "" string
// Windows M() ==> func(, other) stringify empty __VA_ARGS__ to empty
// #define M(...) func(ss(#__VA_ARGS__), other)
static inline const char* ss(const char* a = "") { return a ? a : ""; }

/* clang-format off */
#define h2_singleton(Class_) static Class_& I() { static Class_ i; return i; }

#define H2Foreach(_Macro_x, ...) H2PP_FOREACH(, _H2ForeachMacro, (_Macro_x), __VA_ARGS__)
#define _H2ForeachMacro(_Args, i, x) H2PP_REMOVE_PARENTHESES(_Args)(x)

#define H2Fullmesh(_Macro_x_y, ...) H2PP_FULLMESH((, _H2FullmeshMacro, (_Macro_x_y), __VA_ARGS__))
#define _H2FullmeshMacro(_Args, i, j, x, y) H2PP_REMOVE_PARENTHESES(_Args)(x, y)
