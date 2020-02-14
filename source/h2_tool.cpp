/* https://www.boost.org/doc/libs/1_65_0/libs/preprocessor/doc/index.html */

#define H2PP_CAT2(_1, _2) _H2PP_CAT2(_1, _2)
#define _H2PP_CAT2(_1, _2) _1##_2
#define H2PP_CAT5(_1, _2, _3, _4, _5) _H2PP_CAT5(_1, _2, _3, _4, _5)
#define _H2PP_CAT5(_1, _2, _3, _4, _5) _1##_2##_3##_4##_5

#define H2PP_STRINGIZE(...) _H2PP_STRINGIZE(__VA_ARGS__)
#define _H2PP_STRINGIZE(...) #__VA_ARGS__

#define H2PP_REMOVE_PARENTHESES(...) _H2PP_REMOVE_PARENTHESES __VA_ARGS__
#define _H2PP_REMOVE_PARENTHESES(...) __VA_ARGS__

/* clang-format off */
#define H2PP_IF(_Cond, _Then, _Else) H2PP_CAT2(_H2PP_IF_, _Cond) (_Then, _Else)
#define _H2PP_IF_1(_Then, _Else) _Then
#define _H2PP_IF_0(_Then, _Else) _Else
/* clang-format on */

#define _H2PP_NARGS(...) _H2PP_16TH(__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define _H2PP_HAS_COMMA(...) _H2PP_16TH(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define _H2PP_16TH(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16
#define _H2PP_COMMA(...) ,

#define H2PP_0ARGS(...) _H2PP_0ARGS(_H2PP_HAS_COMMA(__VA_ARGS__),             \
                                    _H2PP_HAS_COMMA(_H2PP_COMMA __VA_ARGS__), \
                                    _H2PP_HAS_COMMA(__VA_ARGS__()),           \
                                    _H2PP_HAS_COMMA(_H2PP_COMMA __VA_ARGS__()))

#define _H2PP_0ARGS(_1, _2, _3, _4) _H2PP_HAS_COMMA(_H2PP_CAT5(_H2PP_0ARGS_CASE_, _1, _2, _3, _4))

#define _H2PP_0ARGS_CASE_0001 ,

#define H2PP_NARGS(...) H2PP_IF(H2PP_0ARGS(__VA_ARGS__), 0, _H2PP_NARGS(__VA_ARGS__))

/* clang-format off */
#define H2PP_VARIADIC_CALL(_Macro, ...) H2PP_CAT2(_Macro, H2PP_NARGS(__VA_ARGS__)) (__VA_ARGS__)
/* clang-format on */

#define H2Q(Prefix) H2PP_CAT5(Prefix, _, __COUNTER__, _, __LINE__)

// H2_ALIGN_UP(15, 8) == 16
#define H2_ALIGN_UP(n, s) (((n) + (s)-1) / (s) * (s))
// H2_ALIGN_DOWN(15, 8) == 8
#define H2_ALIGN_DOWN(n, s) ((n) / (s) * (s))
// H2_DIV_ROUND_UP(15, 8) == 2
#define H2_DIV_ROUND_UP(n, s) (((n) + (s)-1) / (s))

struct h2_with {
   FILE* f;
   int (*c)(FILE*);
   h2_with(FILE* file, int (*close)(FILE*) = ::fclose) : f(file), c(close) {}
   ~h2_with() { f&& c&& c(f); }
};

static inline bool h2_wildcard_match(const char* pattern, const char* subject) {
   const char *scur = subject, *pcur = pattern;
   const char *sstar = nullptr, *pstar = nullptr;
   while (*scur) {
      if (*scur == *pcur || *pcur == '?') {
         ++scur;
         ++pcur;
      } else if (*pcur == '*') {
         pstar = pcur++;
         sstar = scur;
      } else if (pstar) {
         pcur = pstar + 1;
         scur = ++sstar;
      } else
         return false;
   }
   while (*pcur == '*') ++pcur;
   return !*pcur;
}

static inline long long h2_milliseconds() {
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

static inline const char* h2_style(const char* style_str, char* style_abi) {
   static struct {
      const char* name;
      const char* value;
   } K[] = {
     {"reset", "0;"},
     {"bold", "1;"},
     {"italics", "3;"},
     {"underline", "4;"},
     {"inverse", "7;"},
     {"strikethrough", "9;"},
     {"black", "30;"},
     {"red", "31;"},
     {"green", "32;"},
     {"yellow", "33;"},
     {"blue", "34;"},
     {"purple", "35;"},
     {"cyan", "36;"},
     {"gray", "37;"},
     {"default", "39;"},
     {"dark gray", "90;"},
     {"light red", "91;"},
     {"light green", "92;"},
     {"light yellow", "93;"},
     {"light blue", "94;"},
     {"light purple", "95;"},
     {"light cyan", "96;"},
     {"white", "97;"},
     {"bg_black", "40;"},
     {"bg_red", "41;"},
     {"bg_green", "42;"},
     {"bg_yellow", "43;"},
     {"bg_blue", "44;"},
     {"bg_purple", "45;"},
     {"bg_cyan", "46;"},
     {"bg_white", "47;"},
     {"bg_default", "49;"}};

   char __style_str[1024];
   strcpy(__style_str, style_str);

   strcpy(style_abi, "\033[");

   for (char* p = strtok(__style_str, ","); p; p = strtok(NULL, ","))
      for (size_t i = 0; i < sizeof(K) / sizeof(K[0]); i++)
         if (strcmp(K[i].name, p) == 0)
            strcat(style_abi, K[i].value), i = sizeof(K);

   style_abi[strlen(style_abi) - 1] = 'm';

   return style_abi;
}

static inline const char* h2_acronym_string(const char* full, int atmost = 10) {
   static char st[32];
   strncpy(st, full, atmost);
   strcpy(st + atmost, "...");
   return st;
}

static inline const char* h2_center_string(const char* str, int width, char* out) {
   int z = strlen(str), l = (width - z) / 2, r = width - l - z;
   char f[32];
   sprintf(f, "%%%ds%%s%%%ds", l, r);
   sprintf(out, f, "", str, "");
   return out;
}

static inline bool h2_endswith_string(const char* haystack, const char* needle) {
   int haystack_length = strlen(haystack), needle_length = strlen(needle);
   return haystack_length < needle_length ? false : strncmp(haystack + haystack_length - needle_length, needle, needle_length) == 0;
}

#if defined _WIN32
#   define h2_selectany __declspec(selectany)
#else
#   define h2_selectany __attribute__((weak))
#endif
