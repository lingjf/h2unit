/* v3.1 */

#ifndef ___H2UNIT_H_3_0__
#define ___H2UNIT_H_3_0__

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <cstring>
#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <regex>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <functional>
#include <utility>
#include <memory>
#include <sstream>
#include <type_traits>
#include <typeinfo>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <execinfo.h>
#include <time.h>
#include <errno.h>

#ifndef ___H2_TOOL__H___
#define ___H2_TOOL__H___

/*
 *  https://github.com/lingjf/variadic/blob/master/macro.h
 */

/* This was inspired by BOOST's PP_CAT macro. Using such a thing avoids
   to define multiple levels of expansion for each macro. */

#define H2_PP_X0()
#define H2_PP_X1(_1) _1

#define H2_PP_CAT2(_1, _2) _H2_PP_CAT2(_1, _2)
#define _H2_PP_CAT2(_1, _2) _1##_2

#define H2_PP_STRINGIZE(...) _H2_PP_STRINGIZE(__VA_ARGS__)
#define _H2_PP_STRINGIZE(...) #__VA_ARGS__

#define H2_PP_REMOVE_PARENTHESES(...) _H2_PP_REMOVE_PARENTHESES __VA_ARGS__
#define _H2_PP_REMOVE_PARENTHESES(...) __VA_ARGS__

#define H2_PP_IF(_Cond, _Then, _Else) H2_PP_CAT2(_H2_PP_IF_, _Cond) \
(_Then, _Else)
#define _H2_PP_IF_1(_Then, _Else) _Then
#define _H2_PP_IF_0(_Then, _Else) _Else

#define _H2_PP_NARGS(...) _H2_PP_INTERNAL_16TH(__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define _H2_PP_HAS_COMMA(...) _H2_PP_INTERNAL_16TH(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0)
#define _H2_PP_INTERNAL_16TH(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16
#define _H2_PP_COMMA(...) ,

#define H2_PP_0ARGS(...) _H2_PP_0ARGS(_H2_PP_HAS_COMMA(__VA_ARGS__),              \
                                      _H2_PP_HAS_COMMA(_H2_PP_COMMA __VA_ARGS__), \
                                      _H2_PP_HAS_COMMA(__VA_ARGS__()),            \
                                      _H2_PP_HAS_COMMA(_H2_PP_COMMA __VA_ARGS__()))

#define _H2_PP_INTERNAL_CAT5(_1, _2, _3, _4, _5) _1##_2##_3##_4##_5
#define _H2_PP_0ARGS(_1, _2, _3, _4) _H2_PP_HAS_COMMA(_H2_PP_INTERNAL_CAT5(_H2_PP_0ARGS_CASE_, _1, _2, _3, _4))

#define _H2_PP_0ARGS_CASE_0000 _H2_PP_0ARGS_CASE_0000
#define _H2_PP_0ARGS_CASE_0001 ,
#define _H2_PP_0ARGS_CASE_0010 _H2_PP_0ARGS_CASE_0010
#define _H2_PP_0ARGS_CASE_0011 _H2_PP_0ARGS_CASE_0011
#define _H2_PP_0ARGS_CASE_0100 _H2_PP_0ARGS_CASE_0100
#define _H2_PP_0ARGS_CASE_0101 _H2_PP_0ARGS_CASE_0101
#define _H2_PP_0ARGS_CASE_0110 _H2_PP_0ARGS_CASE_0110
#define _H2_PP_0ARGS_CASE_0111 _H2_PP_0ARGS_CASE_0111
#define _H2_PP_0ARGS_CASE_1000 _H2_PP_0ARGS_CASE_1000
#define _H2_PP_0ARGS_CASE_1001 _H2_PP_0ARGS_CASE_1001
#define _H2_PP_0ARGS_CASE_1010 _H2_PP_0ARGS_CASE_1010
#define _H2_PP_0ARGS_CASE_1011 _H2_PP_0ARGS_CASE_1011
#define _H2_PP_0ARGS_CASE_1100 _H2_PP_0ARGS_CASE_1100
#define _H2_PP_0ARGS_CASE_1101 _H2_PP_0ARGS_CASE_1101
#define _H2_PP_0ARGS_CASE_1110 _H2_PP_0ARGS_CASE_1110
#define _H2_PP_0ARGS_CASE_1111 _H2_PP_0ARGS_CASE_1111

#define H2_PP_NARGS(...) H2_PP_IF(H2_PP_0ARGS(__VA_ARGS__), 0, _H2_PP_NARGS(__VA_ARGS__))

#define H2_PP_VARIADIC_CALL(_Macro, ...) H2_PP_X1(H2_PP_CAT2(_Macro, H2_PP_NARGS(__VA_ARGS__))(__VA_ARGS__))

#define H2_PP_CAT(...) H2_PP_VARIADIC_CALL(_H2_PP_CAT_, __VA_ARGS__)
#define _H2_PP_CAT_2(_1, _2) H2_PP_CAT2(_1, _2)
#define _H2_PP_CAT_3(_1, _2, _3) H2_PP_CAT2(_H2_PP_CAT_2(_1, _2), _3)
#define _H2_PP_CAT_4(_1, _2, _3, _4) H2_PP_CAT2(_H2_PP_CAT_3(_1, _2, _3), _4)
#define _H2_PP_CAT_5(_1, _2, _3, _4, _5) H2_PP_CAT2(_H2_PP_CAT_4(_1, _2, _3, _4), _5)
#define _H2_PP_CAT_6(_1, _2, _3, _4, _5, _6) H2_PP_CAT2(_H2_PP_CAT_5(_1, _2, _3, _4, _5), _6)
#define _H2_PP_CAT_7(_1, _2, _3, _4, _5, _6, _7) H2_PP_CAT2(_H2_PP_CAT_6(_1, _2, _3, _4, _5, _6), _7)
#define _H2_PP_CAT_8(_1, _2, _3, _4, _5, _6, _7, _8) H2_PP_CAT2(_H2_PP_CAT_7(_1, _2, _3, _4, _5, _6, _7), _8)
#define _H2_PP_CAT_9(_1, _2, _3, _4, _5, _6, _7, _8, _9) H2_PP_CAT2(_H2_PP_CAT_8(_1, _2, _3, _4, _5, _6, _7, _8), _9)

// H2_ALIGN_UP(15, 8) == 16
#define H2_ALIGN_UP(n, s) (((n) + (s)-1) / (s) * (s))
// H2_DIV_ROUND_UP(15, 8) == 2
#define H2_DIV_ROUND_UP(n, s) (((n) + (s)-1) / (s))
#define H2_ARRAY_COUNTOF(a) ((int)(sizeof(a) / sizeof((a)[0])))

static inline bool h2_wildcard_match(const char* pattern, const char* subject)
{
   const char *scur = subject, *pcur = pattern;
   const char *sstar = nullptr, *pstar = nullptr;
   while (*scur) {
      if (*scur == *pcur || *pcur == '?') {
         ++scur;
         ++pcur;
      }
      else if (*pcur == '*') {
         pstar = pcur++;
         sstar = scur;
      }
      else if (pstar) {
         pcur = pstar + 1;
         scur = ++sstar;
      }
      else
         return false;
   }
   while (*pcur == '*') ++pcur;
   return !*pcur;
}

#ifdef _WIN32
#   include <windows.h>
static inline long h2_milliseconds()
{
   return timeGetTime() / 1000;
}

#   pragma warning(disable : 4267)
#   pragma warning(disable : 4311)
#   pragma warning(disable : 4800)

#else
static inline long h2_milliseconds()
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
#endif

static inline const char* h2_style(const char* style_str, char* style_abi)
{
   static struct
   {
      const char* name;
      const char* value;
   } K[] = {
     // normal style
     {"reset", "0;"},
     {"bold", "1;"},
     {"italics", "3;"},
     {"underline", "4;"},
     {"inverse", "7;"},
     {"strikethrough", "9;"},
     // foreground color
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
     // background color
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

   for (char* opt = strtok(__style_str, ","); opt; opt = strtok(NULL, ",")) {
      for (int i = 0; i < H2_ARRAY_COUNTOF(K); i++) {
         if (strcmp(K[i].name, opt) == 0) {
            strcat(style_abi, K[i].value);
            break;
         }
      }
   }

   style_abi[strlen(style_abi) - 1] = 'm';

   return style_abi;
}

static inline const char* h2_acronym_string(const char* full, int atmost = 10)
{
   static char buffer[32];
   strncpy(buffer, full, atmost);
   strcpy(buffer + atmost, "...");
   return buffer;
}

static inline const char* h2_center_string(const char* str, int width, char* t)
{
   int slen = strlen(str);
   char* p = t;
   for (int i = 0; i < (width - slen) / 2; i++) {
      *p++ = ' ';
   }
   strcpy(p, str);
   p += slen;

   for (; p < t + width; p++) {
      *p = ' ';
   }
   *p = '\0';
   return t;
}

#endif

#ifndef ___H2_CFG__H___
#define ___H2_CFG__H___

#include <getopt.h>
#include <sys/ioctl.h>

#ifdef _WIN32

static inline int getopt(int argc, char** argv, char* opts)
{
   static int sp = 1;
   register int c;
   register char* cp;

   int opterr = 1;
   int optind = 1;
   int optopt;
   char* optarg;

   if (sp == 1)
      if (optind >= argc ||
          argv[optind][0] != '-' || argv[optind][1] == '\0') {
         return (-1);
      }
      else if (strcmp(argv[optind], "--") == NULL) {
         optind++;
         return (-1);
      }
   optopt = c = argv[optind][sp];
   if (c == ':' || (cp = strchr(opts, c)) == NULL) {
      if (argv[optind][++sp] == '\0') {
         optind++;
         sp = 1;
      }
      return ('?');
   }
   if (*++cp == ':') {
      if (argv[optind][sp + 1] != '\0') {
         optarg = &argv[optind++][sp + 1];
      }
      else if (++optind >= argc) {
         sp = 1;
         return ('?');
      }
      else {
         optarg = argv[optind++];
      }
      sp = 1;
   }
   else {
      if (argv[optind][++sp] == '\0') {
         sp = 1;
         optind++;
      }
      optarg = NULL;
   }
   return (c);
}

#endif

struct h2_cfg
{
   char* path;
   bool verbose;
   bool colorable;
   bool randomize;
   bool memory_check;
   char* junit;

   std::vector<const char*> include_patterns;
   std::vector<const char*> exclude_patterns;

   h2_cfg()
     : path(nullptr), verbose(false), colorable(true), randomize(false), memory_check(true), junit(nullptr) {}
   ~h2_cfg() {}

   static h2_cfg& I()
   {
      static h2_cfg I;
      return I;
   }

   void configure(int argc, char** argv)
   {
      path = argv[0];

      int c;
      while (-1 != (c = getopt(argc, argv, "vcrmj:i:x:h?"))) {
         switch (c) {
         case 'v': verbose = true; break;
         case 'c': colorable = !colorable; break;
         case 'r': randomize = true; break;
         case 'm': memory_check = !memory_check; break;
         case 'j': junit = optarg; break;
         case 'i': include_patterns.push_back(optarg); break;
         case 'x': exclude_patterns.push_back(optarg); break;
         case 'h':
         case '?':
         default:
            printf("Usage: \n"
                   "-v                  Make the operation more talkative\n"
                   "-c                  Output in black-white color mode\n"
                   "-r                  Run cases in random order\n"
                   "-m                  Run cases without memory check\n"
                   "-j {path}           Generate junit report\n"
                   "-i {pattern}        Run cases which case name or unit name matches\n"
                   "-x {pattern}        Run cases which case name and unit name not matches\n");
            exit(0);
         }
      }
   }

   int filter(std::vector<const char*>& patterns, const char* subject)
   {
      if (patterns.empty()) {
         return -1;
      }
      for (auto it = patterns.begin(); it != patterns.end(); it++) {
         const char* pattern = *it;
         if (strchr(pattern, '?') || strchr(pattern, '*')) {
            if (h2_wildcard_match(pattern, subject)) return 1;
         }
         else {
            if (strstr(subject, pattern)) return 1;
         }
      }
      return 0;
   }

   bool filter(const char* suitename, const char* casename)
   {
      if (0 == filter(include_patterns, suitename) && 0 == filter(include_patterns, casename)) {
         return true;
      }
      if (1 == filter(exclude_patterns, suitename) || 1 == filter(exclude_patterns, casename)) {
         return true;
      }
      return false;
   }

   static const char* style(const char* style_str)
   {
      if (!I().colorable) { return ""; }

      static char shift_buffer[8][128];
      static long shift_index = 0;

      shift_index = (shift_index + 1) % 8;

#if defined(_WIN32)

#else

      return h2_style(style_str, shift_buffer[shift_index]);
#endif
   }

   static int get_term_columns()
   {
      struct winsize w;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
      return w.ws_col;
   }
};

#endif

#ifndef ___H2_ALLOC__H___
#define ___H2_ALLOC__H___

struct h2_alloc
{
   static h2_alloc& I()
   {
      static h2_alloc I;
      return I;
   }
   static h2_alloc& U()
   {
      static h2_alloc U;
      return U;
   }

   h2_alloc() {}

   struct X
   {
      uint32_t size;
   };

   void* malloc(size_t size)
   {
      if (h2_cfg::I().memory_check) {
         void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

         if (ptr == MAP_FAILED) {
            return NULL;
         }

         X* p = (X*)ptr;
         p->size = size;
         return (void*)(p + 1);
      }
      else {
         return ::malloc(size);
      }
   }

   void free(void* ptr)
   {
      if (h2_cfg::I().memory_check) {
         X* p = (X*)ptr;
         p = p - 1;
         munmap(p, p->size);
      }
      else {
         ::free(ptr);
      }
   }
};

template <typename T>
class h2_allocator
{
 public:
   typedef size_t size_type;
   typedef ptrdiff_t difference_type;
   typedef T* pointer;
   typedef const T* const_pointer;
   typedef T& reference;
   typedef const T& const_reference;
   typedef T value_type;

   h2_allocator() {}
   h2_allocator(const h2_allocator&) {}

   pointer allocate(size_type n, const void* = 0)
   {
      T* t = (T*)h2_alloc::I().malloc(n * sizeof(T));
      return t;
   }

   void deallocate(void* p, size_type)
   {
      if (p) {
         h2_alloc::I().free(p);
      }
   }

   pointer address(reference x) const
   {
      return &x;
   }
   const_pointer address(const_reference x) const
   {
      return &x;
   }
   h2_allocator<T>& operator=(const h2_allocator&)
   {
      return *this;
   }
   void construct(pointer p, const T& val)
   {
      new ((T*)p) T(val);
   }
   void destroy(pointer p)
   {
      p->~T();
   }

   size_type max_size() const
   {
      return size_t(-1);
   }

   template <typename U>
   struct rebind
   {
      typedef h2_allocator<U> other;
   };

   template <typename U>
   h2_allocator(const h2_allocator<U>&) {}

   template <typename U>
   h2_allocator& operator=(const h2_allocator<U>&)
   {
      return *this;
   }
};

template <typename T>
inline bool
operator==(const h2_allocator<T>&, const h2_allocator<T>&)
{
   return true;
}

template <typename T>
inline bool
operator!=(const h2_allocator<T>&, const h2_allocator<T>&)
{
   return false;
}

typedef std::basic_string<char, std::char_traits<char>, h2_allocator<char>> h2_string;
typedef std::basic_ostringstream<char, std::char_traits<char>, h2_allocator<char>> h2_ostringstream;

#endif
/*
 * TINYEXPR - Tiny recursive descent parser and evaluation engine in C
 *
 * Copyright (c) 2015-2018 Lewis Van Winkle
 *
 * http://CodePlea.com
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgement in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef __TINYEXPR_H__
#define __TINYEXPR_H__

/* clang-format off */


#ifndef NAN
#define NAN (0.0/0.0)
#endif

#ifndef INFINITY
#define INFINITY (1.0/0.0)
#endif


#define TYPE_MASK(TYPE) ((TYPE)&0x0000001F)

#define IS_PURE(TYPE) (((TYPE) & TE_FLAG_PURE) != 0)
#define IS_FUNCTION(TYPE) (((TYPE) & TE_FUNCTION0) != 0)
#define IS_CLOSURE(TYPE) (((TYPE) & TE_CLOSURE0) != 0)
#define ARITY(TYPE) ( ((TYPE) & (TE_FUNCTION0 | TE_CLOSURE0)) ? ((TYPE) & 0x00000007) : 0 )


class h2_tinyexpr
{
public:    
    /* Parses the input expression, evaluates it, and frees it. */
    /* Returns NaN on error. */
    static double te_interp(const char *expression, int *error) {
        te_expr *n = te_compile(expression, 0, 0, error);
        double ret;
        if (n) {
            ret = te_eval(n);
            te_free(n);
        } else {
            ret = NAN;
        }
        return ret;
    }

private:

    struct te_expr {
        int type;
        union {double value; const double *bound; const void *function;};
        void *parameters[1];
    };


    enum {
        TE_VARIABLE = 0,

        TE_FUNCTION0 = 8, TE_FUNCTION1, TE_FUNCTION2, TE_FUNCTION3,
        TE_FUNCTION4, TE_FUNCTION5, TE_FUNCTION6, TE_FUNCTION7,

        TE_CLOSURE0 = 16, TE_CLOSURE1, TE_CLOSURE2, TE_CLOSURE3,
        TE_CLOSURE4, TE_CLOSURE5, TE_CLOSURE6, TE_CLOSURE7,

        TE_FLAG_PURE = 32
    };

    struct te_variable {
        const char *name;
        const void *address;
        int type;
        void *context;
    };


    typedef double (*te_fun2)(double, double);

    enum {
        TOK_NULL = TE_CLOSURE7+1, TOK_ERROR, TOK_END, TOK_SEP,
        TOK_OPEN, TOK_CLOSE, TOK_NUMBER, TOK_VARIABLE, TOK_INFIX
    };


    enum {TE_CONSTANT = 1};


    struct state {
        const char *start;
        const char *next;
        int type;
        union {double value; const double *bound; const void *function;};
        void *context;

        const te_variable *lookup;
        int lookup_len;
    };

    static te_expr *new_expr(const int type, const te_expr *parameters[]) {
        const int arity = ARITY(type);
        const int psize = sizeof(void*) * arity;
        const int size = (sizeof(te_expr) - sizeof(void*)) + psize + (IS_CLOSURE(type) ? sizeof(void*) : 0);
        te_expr *ret = (te_expr *)h2_alloc::I().malloc(size);
        memset(ret, 0, size);
        if (arity && parameters) {
            memcpy(ret->parameters, parameters, psize);
        }
        ret->type = type;
        ret->bound = 0;
        return ret;
    }

    static void te_free_parameters(te_expr *n) {
        if (!n) return;
        switch (TYPE_MASK(n->type)) {
            case TE_FUNCTION7: case TE_CLOSURE7: te_free((te_expr *)n->parameters[6]);     /* Falls through. */
            case TE_FUNCTION6: case TE_CLOSURE6: te_free((te_expr *)n->parameters[5]);     /* Falls through. */
            case TE_FUNCTION5: case TE_CLOSURE5: te_free((te_expr *)n->parameters[4]);     /* Falls through. */
            case TE_FUNCTION4: case TE_CLOSURE4: te_free((te_expr *)n->parameters[3]);     /* Falls through. */
            case TE_FUNCTION3: case TE_CLOSURE3: te_free((te_expr *)n->parameters[2]);     /* Falls through. */
            case TE_FUNCTION2: case TE_CLOSURE2: te_free((te_expr *)n->parameters[1]);     /* Falls through. */
            case TE_FUNCTION1: case TE_CLOSURE1: te_free((te_expr *)n->parameters[0]);
        }
    }

    static void te_free(te_expr *n) {
        if (!n) return;
        te_free_parameters(n);
        h2_alloc::I().free((void *)n);
    }

    static double _fabs(double x) {return fabs(x);}
    static double _cos(double x) {return cos(x);}
    static double _acos(double x) {return acos(x);}
    static double _sin(double x) {return sin(x);}
    static double _asin(double x) {return asin(x);}
    static double _tan(double x) {return tan(x);}
    static double _atan(double x) {return atan(x);}
    static double _sqrt(double x) {return sqrt(x);}
    static double _ln(double x) {return log(x);}
    static double _log10(double x) {return log10(x);}
    static double _log2(double x) {return log2(x);}
    static double _floor(double x) {return floor(x);}
    static double _ceil(double x) {return ceil(x);}
    static double _pow(double x, double y) {return pow(x, y);}
    static double _exp(double x) {return exp(x);}
    static double _fmod(double x, double y) {return fmod(x, y);}

    static double pi(void) {return 3.14159265358979323846;}
    static double e(void) {return 2.71828182845904523536;}
    static double fac(double a) {/* simplest version of fac */
        if (a < 0.0)
            return NAN;
        if (a > UINT_MAX)
            return INFINITY;
        unsigned int ua = (unsigned int)(a);
        unsigned long int result = 1, i;
        for (i = 1; i <= ua; i++) {
            if (i > ULONG_MAX / result)
                return INFINITY;
            result *= i;
        }
        return (double)result;
    }
    static double ncr(double n, double r) {
        if (n < 0.0 || r < 0.0 || n < r) return NAN;
        if (n > UINT_MAX || r > UINT_MAX) return INFINITY;
        unsigned long int un = (unsigned int)(n), ur = (unsigned int)(r), i;
        unsigned long int result = 1;
        if (ur > un / 2) ur = un - ur;
        for (i = 1; i <= ur; i++) {
            if (result > ULONG_MAX / (un - ur + i))
                return INFINITY;
            result *= un - ur + i;
            result /= i;
        }
        return result;
    }
    static double npr(double n, double r) {return ncr(n, r) * fac(r);}


    static const te_variable *find_builtin(const char *name, int len) {
        static const te_variable functions[] = {
            /* must be in alphabetical order */
            {"abs", (const void *)_fabs,     TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"acos", (const void *)_acos,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"asin", (const void *)_asin,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"atan", (const void *)_atan,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
        //     {"atan2", (const void *)atan2,  TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"ceil", (const void *)_ceil,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"cos", (const void *)_cos,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
        //     {"cosh", (const void *)cosh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"e", (const void *)e,          TE_FUNCTION0 | TE_FLAG_PURE, 0},
            {"exp", (const void *)_exp,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"fac", (const void *)fac,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"floor", (const void *)_floor,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"ln", (const void *)_ln,       TE_FUNCTION1 | TE_FLAG_PURE, 0},
        // #ifdef TE_NAT_LOG
        //     {"log", (const void *)log,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
        // #else
        //     {"log", (const void *)log10,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
        // #endif
            {"log10", (const void *)_log10,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"log2", (const void *)_log2,  TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"ncr", (const void *)ncr,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"npr", (const void *)npr,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"pi", (const void *)pi,        TE_FUNCTION0 | TE_FLAG_PURE, 0},
            {"pow", (const void *)_pow,      TE_FUNCTION2 | TE_FLAG_PURE, 0},
            {"sin", (const void *)_sin,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
        //     {"sinh", (const void *)sinh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"sqrt", (const void *)_sqrt,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {"tan", (const void *)_tan,      TE_FUNCTION1 | TE_FLAG_PURE, 0},
        //     {"tanh", (const void *)tanh,    TE_FUNCTION1 | TE_FLAG_PURE, 0},
            {0, 0, 0, 0}
        };
        int imin = 0;
        int imax = sizeof(functions) / sizeof(te_variable) - 2;

        /*Binary search.*/
        while (imax >= imin) {
            const int i = (imin + ((imax-imin)/2));
            int c = strncmp(name, functions[i].name, len);
            if (!c) c = '\0' - functions[i].name[len];
            if (c == 0) {
                return functions + i;
            } else if (c > 0) {
                imin = i + 1;
            } else {
                imax = i - 1;
            }
        }

        return 0;
    }

    static const te_variable *find_lookup(const state *s, const char *name, int len) {
        int iters;
        const te_variable *var;
        if (!s->lookup) return 0;

        for (var = s->lookup, iters = s->lookup_len; iters; ++var, --iters) {
            if (strncmp(name, var->name, len) == 0 && var->name[len] == '\0') {
                return var;
            }
        }
        return 0;
    }

    static double add(double a, double b) {return a + b;}
    static double sub(double a, double b) {return a - b;}
    static double mul(double a, double b) {return a * b;}
    static double divide(double a, double b) {return a / b;}
    static double negate(double a) {return -a;}
    static double comma(double a, double b) {(void)a; return b;}


    static void next_token(state *s) {
        s->type = TOK_NULL;

        do {

            if (!*s->next){
                s->type = TOK_END;
                return;
            }

            /* Try reading a number. */
            if ((s->next[0] >= '0' && s->next[0] <= '9') || s->next[0] == '.') {
                s->value = strtod(s->next, (char**)&s->next);
                s->type = TOK_NUMBER;
            } else {
                /* Look for a variable or builtin function call. */
                if (s->next[0] >= 'a' && s->next[0] <= 'z') {
                    const char *start;
                    start = s->next;
                    while ((s->next[0] >= 'a' && s->next[0] <= 'z') || (s->next[0] >= '0' && s->next[0] <= '9') || (s->next[0] == '_')) s->next++;

                    const te_variable *var = find_lookup(s, start, s->next - start);
                    if (!var) var = find_builtin(start, s->next - start);

                    if (!var) {
                        s->type = TOK_ERROR;
                    } else {
                        switch(TYPE_MASK(var->type))
                        {
                            case TE_VARIABLE:
                                s->type = TOK_VARIABLE;
                                s->bound = (const double *)var->address;
                                break;

                            case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:         /* Falls through. */
                            case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:         /* Falls through. */
                                s->context = var->context;                                                  /* Falls through. */

                            case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:     /* Falls through. */
                            case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:     /* Falls through. */
                                s->type = var->type;
                                s->function = var->address;
                                break;
                        }
                    }

                } else {
                    /* Look for an operator or special character. */
                    switch (s->next++[0]) {
                        case '+': s->type = TOK_INFIX; s->function = (const void *)add; break;
                        case '-': s->type = TOK_INFIX; s->function = (const void *)sub; break;
                        case '*': s->type = TOK_INFIX; s->function = (const void *)mul; break;
                        case '/': s->type = TOK_INFIX; s->function = (const void *)divide; break;
                        case '^': s->type = TOK_INFIX; s->function = (const void *)_pow; break;
                        // case '%': s->type = TOK_INFIX; s->function = (const void *)fmod; break;
                        case '(': s->type = TOK_OPEN; break;
                        case ')': s->type = TOK_CLOSE; break;
                        case ',': s->type = TOK_SEP; break;
                        case ' ': case '\t': case '\n': case '\r': break;
                        default: s->type = TOK_ERROR; break;
                    }
                }
            }
        } while (s->type == TOK_NULL);
    }

    static te_expr *base(state *s) {
        /* <base>      =    <constant> | <variable> | <function-0> {"(" ")"} | <function-1> <power> | <function-X> "(" <expr> {"," <expr>} ")" | "(" <list> ")" */
        te_expr *ret;
        int arity;

        switch (TYPE_MASK(s->type)) {
            case TOK_NUMBER:
                ret = new_expr(TE_CONSTANT, 0);
                ret->value = s->value;
                next_token(s);
                break;

            case TOK_VARIABLE:
                ret = new_expr(TE_VARIABLE, 0);
                ret->bound = s->bound;
                next_token(s);
                break;

            case TE_FUNCTION0:
            case TE_CLOSURE0:
                ret = new_expr(s->type, 0);
                ret->function = s->function;
                if (IS_CLOSURE(s->type)) ret->parameters[0] = s->context;
                next_token(s);
                if (s->type == TOK_OPEN) {
                    next_token(s);
                    if (s->type != TOK_CLOSE) {
                        s->type = TOK_ERROR;
                    } else {
                        next_token(s);
                    }
                }
                break;

            case TE_FUNCTION1:
            case TE_CLOSURE1:
                ret = new_expr(s->type, 0);
                ret->function = s->function;
                if (IS_CLOSURE(s->type)) ret->parameters[1] = s->context;
                next_token(s);
                ret->parameters[0] = power(s);
                break;

            case TE_FUNCTION2: case TE_FUNCTION3: case TE_FUNCTION4:
            case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
            case TE_CLOSURE2: case TE_CLOSURE3: case TE_CLOSURE4:
            case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
                arity = ARITY(s->type);

                ret = new_expr(s->type, 0);
                ret->function = s->function;
                if (IS_CLOSURE(s->type)) ret->parameters[arity] = s->context;
                next_token(s);

                if (s->type != TOK_OPEN) {
                    s->type = TOK_ERROR;
                } else {
                    int i;
                    for(i = 0; i < arity; i++) {
                        next_token(s);
                        ret->parameters[i] = expr(s);
                        if(s->type != TOK_SEP) {
                            break;
                        }
                    }
                    if(s->type != TOK_CLOSE || i != arity - 1) {
                        s->type = TOK_ERROR;
                    } else {
                        next_token(s);
                    }
                }

                break;

            case TOK_OPEN:
                next_token(s);
                ret = list(s);
                if (s->type != TOK_CLOSE) {
                    s->type = TOK_ERROR;
                } else {
                    next_token(s);
                }
                break;

            default:
                ret = new_expr(0, 0);
                s->type = TOK_ERROR;
                ret->value = NAN;
                break;
        }

        return ret;
    }


    static te_expr *power(state *s) {
        /* <power>     =    {("-" | "+")} <base> */
        int sign = 1;
        while (s->type == TOK_INFIX && (s->function == add || s->function == sub)) {
            if (s->function == sub) sign = -sign;
            next_token(s);
        }

        te_expr *ret;

        if (sign == 1) {
            ret = base(s);
        } else {
            const te_expr*_t[] = {base(s)};
            ret = new_expr(TE_FUNCTION1 | TE_FLAG_PURE, _t);
            ret->function = (const void *)negate;
        }

        return ret;
    }

    static te_expr *factor(state *s) {
        /* <factor>    =    <power> {"^" <power>} */
        te_expr *ret = power(s);

        while (s->type == TOK_INFIX && (s->function == _pow)) {
            te_fun2 t = (te_fun2)s->function;
            next_token(s);

            const te_expr*_t[] = {ret, power(s)};
            ret = new_expr(TE_FUNCTION2 | TE_FLAG_PURE, _t);
            ret->function = (const void *)t;
        }

        return ret;
    }

    static te_expr *term(state *s) {
        /* <term>      =    <factor> {("*" | "/" | "%") <factor>} */
        te_expr *ret = factor(s);

        while (s->type == TOK_INFIX && (s->function == mul || s->function == divide || s->function == (const void *)_fmod)) {
            te_fun2 t = (te_fun2)s->function;
            next_token(s);
            const te_expr*_t[] = {ret, factor(s)};
            ret = new_expr(TE_FUNCTION2 | TE_FLAG_PURE, _t);
            ret->function = (const void *)t;
        }

        return ret;
    }


    static te_expr *expr(state *s) {
        /* <expr>      =    <term> {("+" | "-") <term>} */
        te_expr *ret = term(s);

        while (s->type == TOK_INFIX && (s->function == add || s->function == sub)) {
            te_fun2 t = (te_fun2)s->function;
            next_token(s);
            const te_expr*_t[] = {ret, term(s)};
            ret = new_expr(TE_FUNCTION2 | TE_FLAG_PURE, _t);
            ret->function = (const void *)t;
        }

        return ret;
    }


    static te_expr *list(state *s) {
        /* <list>      =    <expr> {"," <expr>} */
        te_expr *ret = expr(s);

        while (s->type == TOK_SEP) {
            next_token(s);
            const te_expr*_t[] = {ret, expr(s)};
            ret = new_expr(TE_FUNCTION2 | TE_FLAG_PURE, _t);
            ret->function = (const void *)comma;
        }

        return ret;
    }


#define TE_FUN(...) ((double(*)(__VA_ARGS__))n->function)
#define M(e) te_eval((const te_expr *)n->parameters[e])


    static double te_eval(const te_expr *n) {
        if (!n) return NAN;

        switch(TYPE_MASK(n->type)) {
            case TE_CONSTANT: return n->value;
            case TE_VARIABLE: return *n->bound;

            case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:
            case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
                switch(ARITY(n->type)) {
                    case 0: return TE_FUN(void)();
                    case 1: return TE_FUN(double)(M(0));
                    case 2: return TE_FUN(double, double)(M(0), M(1));
                    case 3: return TE_FUN(double, double, double)(M(0), M(1), M(2));
                    case 4: return TE_FUN(double, double, double, double)(M(0), M(1), M(2), M(3));
                    case 5: return TE_FUN(double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4));
                    case 6: return TE_FUN(double, double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4), M(5));
                    case 7: return TE_FUN(double, double, double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4), M(5), M(6));
                    default: return NAN;
                }

            case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:
            case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
                switch(ARITY(n->type)) {
                    case 0: return TE_FUN(void*)(n->parameters[0]);
                    case 1: return TE_FUN(void*, double)(n->parameters[1], M(0));
                    case 2: return TE_FUN(void*, double, double)(n->parameters[2], M(0), M(1));
                    case 3: return TE_FUN(void*, double, double, double)(n->parameters[3], M(0), M(1), M(2));
                    case 4: return TE_FUN(void*, double, double, double, double)(n->parameters[4], M(0), M(1), M(2), M(3));
                    case 5: return TE_FUN(void*, double, double, double, double, double)(n->parameters[5], M(0), M(1), M(2), M(3), M(4));
                    case 6: return TE_FUN(void*, double, double, double, double, double, double)(n->parameters[6], M(0), M(1), M(2), M(3), M(4), M(5));
                    case 7: return TE_FUN(void*, double, double, double, double, double, double, double)(n->parameters[7], M(0), M(1), M(2), M(3), M(4), M(5), M(6));
                    default: return NAN;
                }

            default: return NAN;
        }

    }

#undef TE_FUN
#undef M

    static void optimize(te_expr *n) {
        /* Evaluates as much as possible. */
        if (n->type == TE_CONSTANT) return;
        if (n->type == TE_VARIABLE) return;

        /* Only optimize out functions flagged as pure. */
        if (IS_PURE(n->type)) {
            const int arity = ARITY(n->type);
            int known = 1;
            int i;
            for (i = 0; i < arity; ++i) {
                optimize((te_expr*)n->parameters[i]);
                if (((te_expr*)(n->parameters[i]))->type != TE_CONSTANT) {
                    known = 0;
                }
            }
            if (known) {
                const double value = te_eval(n);
                te_free_parameters(n);
                n->type = TE_CONSTANT;
                n->value = value;
            }
        }
    }


    static te_expr *te_compile(const char *expression, const te_variable *variables, int var_count, int *error) {
        state s;
        s.start = s.next = expression;
        s.lookup = variables;
        s.lookup_len = var_count;

        next_token(&s);
        te_expr *root = list(&s);

        if (s.type != TOK_END) {
            te_free(root);
            if (error) {
                *error = (s.next - s.start);
                if (*error == 0) *error = 1;
            }
            return 0;
        } else {
            optimize(root);
            if (error) *error = 0;
            return root;
        }
    }


    static void pn (const te_expr *n, int depth) {
        int i, arity;
        printf("%*s", depth, "");

        switch(TYPE_MASK(n->type)) {
        case TE_CONSTANT: printf("%f\n", n->value); break;
        case TE_VARIABLE: printf("bound %p\n", n->bound); break;

        case TE_FUNCTION0: case TE_FUNCTION1: case TE_FUNCTION2: case TE_FUNCTION3:
        case TE_FUNCTION4: case TE_FUNCTION5: case TE_FUNCTION6: case TE_FUNCTION7:
        case TE_CLOSURE0: case TE_CLOSURE1: case TE_CLOSURE2: case TE_CLOSURE3:
        case TE_CLOSURE4: case TE_CLOSURE5: case TE_CLOSURE6: case TE_CLOSURE7:
            arity = ARITY(n->type);
            printf("f%d", arity);
            for(i = 0; i < arity; i++) {
                printf(" %p", n->parameters[i]);
            }
            printf("\n");
            for(i = 0; i < arity; i++) {
                pn((const te_expr*)n->parameters[i], depth + 1);
            }
            break;
        }
    }


    static void te_print(const te_expr *n) {
        pn(n, 0);
    }
};


/* clang-format on */


#endif /*__TINYEXPR_H__*/

#ifndef ___H2_JSON__H___
#define ___H2_JSON__H___

struct h2json
{
   static const int absent = 0;

   static const int null = 1;
   static const int boolean = 2;
   static const int number = 3;
   static const int string = 4;
   static const int regexp = 5;
   static const int array = 6;
   static const int object = 7;

   static constexpr char indent_char = ' ';
   static constexpr char samelength_char = ' ';
   static constexpr char occupy_char = ' ';
   static constexpr char columns_char = ' ';

   struct X
   {
      const char* text;
      int length;
      int offset;

      struct X& strip()
      {
         while (offset < length && ::isspace(text[offset])) offset++;
         return *this;
      }
      bool startwith(const char* s, int n)
      {
         if (length - offset < n) return false;
         return ::strncmp(text + offset, s, n) == 0;
      }
      bool startwith(char from, char to = '\0')
      {
         if (length - offset < 1) return false;
         if (to == '\0') to = from;
         return from <= text[offset] && text[offset] <= to;
      }
   };

   struct node
   {
      int type;

      h2_string key_string;
      h2_string value_string;
      double value_double;
      bool value_boolean;
      std::vector<node*, h2_allocator<node*>> child_array_or_object;

      node()
        : type(null), value_double(0), value_boolean(false) {}

      int size()
      {
         return child_array_or_object.size();
      }

      node* get(int index)
      {
         if (index < 0 || (int)child_array_or_object.size() <= index) {
            return nullptr;
         }

         return child_array_or_object[index];
      }

      node* get(const char* name)
      {
         if (!name) {
            return nullptr;
         }

         for (auto it = child_array_or_object.begin(); it != child_array_or_object.end(); it++) {
            if (!(*it)->key_string.compare(name)) {
               return *it;
            }
         }

         return nullptr;
      }

      void del(node* child)
      {
         for (auto it = child_array_or_object.begin(); it != child_array_or_object.end(); it++) {
            if (child == *it) {
               child_array_or_object.erase(it);
               delete child;
               return;
            }
         }
      }

      bool is_null()
      {
         return null == type;
      }
      bool is_bool()
      {
         return boolean == type;
      }
      bool is_number()
      {
         return number == type;
      }
      bool is_string()
      {
         return string == type;
      }
      bool is_regexp()
      {
         return regexp == type;
      }
      bool is_array()
      {
         return array == type;
      }
      bool is_object()
      {
         return object == type;
      }

      bool parse_number(X& x)
      {
         int i;
         for (i = 0; x.offset + i < x.length; ++i) {
            const char c = x.text[x.offset + i];
            if (c == ',' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == '\0') {
               break;
            }
         }

         this->value_string.assign(x.text + x.offset, i);

         int err = 0;
         this->value_double = h2_tinyexpr::te_interp(this->value_string.c_str(), &err);
         type = number;
         x.offset += i;

         return 0 == err;
      }

      bool parse_string(X& x)
      {
         const char bound = x.text[x.offset];
         x.offset++;

         if (x.length <= x.offset) {
            return false;
         }

         const char* src = x.text + x.offset;
         int len = 0;
         for (; x.text[x.offset] != bound; ++len) {
            if (x.text[x.offset++] == '\\') {
               x.offset++;
            }
            if (x.length <= x.offset) {
               return false;
            }
         }

         for (; len > 0; ++src, --len) {
            if (*src != '\\') {
               this->value_string.push_back(*src);
            }
            else {
               switch (*++src) {
               case 'b': this->value_string.push_back('\b'); break;
               case 'f': this->value_string.push_back('\f'); break;
               case 'n': this->value_string.push_back('\n'); break;
               case 'r': this->value_string.push_back('\r'); break;
               case 't': this->value_string.push_back('\t'); break;
               case '\"': this->value_string.push_back('\"'); break;
               case '\\': this->value_string.push_back('\\'); break;
               case '/': this->value_string.push_back('/'); break;
               default: return false;
               }
            }
         }

         this->type = string;
         x.offset++;

         return true;
      }

      bool parse_regexp(X& x)
      {
         bool ret = parse_string(x);
         this->type = regexp;
         return ret;
      }

      bool parse_value(X& x)
      {
         /* null */
         if (x.startwith("null", 4)) {
            this->type = null;
            x.offset += 4;
            return true;
         }
         /* false */
         if (x.startwith("false", 5)) {
            this->type = boolean;
            this->value_boolean = false;
            x.offset += 5;
            return true;
         }
         /* true */
         if (x.startwith("true", 4)) {
            this->type = boolean;
            this->value_boolean = true;
            x.offset += 4;
            return true;
         }
         /* string */
         if (x.startwith('\"') || x.startwith('\'')) {
            return parse_string(x);
         }
         /* regexp */
         if (x.startwith('/')) {
            return parse_regexp(x);
         }

         /* array */
         if (x.startwith('[')) {
            return parse_array(x);
         }
         /* object */
         if (x.startwith('{')) {
            return parse_object(x);
         }

         /* number */
         if (1 /* x.startwith('-') || x.startwith('0', '9') */) {
            return parse_number(x);
         }

         return false;
      }

      bool parse_array(X& x)
      {
         x.offset++;  //pass [

         while (!x.strip().startwith(']')) {
            node* new_node = new node();
            if (!new_node) {
               return false;
            }

            child_array_or_object.push_back(new_node);

            if (!new_node->parse_value(x)) {
               return false;
            }

            if (x.strip().startwith(',')) {
               x.offset++;
            }
         }

         this->type = array;
         x.offset++;

         return true;
      }

      bool parse_object(X& x)
      {
         x.offset++;  //pass {

         while (!x.strip().startwith('}')) {
            node* new_node = new node();
            if (!new_node) {
               return false;
            }

            child_array_or_object.push_back(new_node);

            if (!new_node->parse_string(x)) {
               return false;
            }

            new_node->key_string = new_node->value_string;
            new_node->value_string = "";

            if (!x.strip().startwith(':')) {
               return false;
            }
            x.offset++;

            if (!new_node->parse_value(x.strip())) {
               return false;
            }

            if (x.strip().startwith(',')) {
               x.offset++;
            }
         }

         this->type = object;
         x.offset++;

         return true;
      }

      static void* operator new(std::size_t sz)
      {
         return h2_alloc::I().malloc(sz);
      }
      static void operator delete(void* ptr)
      {
         h2_alloc::I().free(ptr);
      }
   };

   static node* parse(const char* json_string, int length = 0)
   {
      if (length == 0) {
         length = strlen(json_string);
      }

      if (!json_string || length == 0) {
         return nullptr;
      }

      X x;

      x.text = json_string;
      x.length = length;
      x.offset = 0;

      node* root = new node();
      if (!root->parse_value(x.strip())) {
         return nullptr;
      }

      return root;
   }

   static void frees(node* root)
   {
      for (auto it = root->child_array_or_object.begin(); it != root->child_array_or_object.end(); it++) {
         frees(*it);
      }
      delete root;
   }

   static bool match_array(node* e, node* a)
   {
      if (!e || !a) {
         return false;
      }
      if (e->child_array_or_object.size() != a->child_array_or_object.size()) {
         return false;
      }

      for (size_t i = 0; i < e->child_array_or_object.size(); ++i) {
         if (!match(e->child_array_or_object[i], a->child_array_or_object[i])) return false;
      }
      return true;
   }

   static bool match_object(node* e, node* a)
   {
      if (!e || !a) {
         return false;
      }
      if (e->child_array_or_object.size() > a->child_array_or_object.size()) {
         return false;
      }
      for (size_t i = 0; i < e->child_array_or_object.size(); ++i) {
         if (!match(e->child_array_or_object[i], a->get(e->child_array_or_object[i]->key_string.c_str()))) return false;
      }

      return true;
   }

   static bool match(node* e, node* a)
   {
      if (!e || !a) {
         return false;
      }

      switch (e->type) {
      case null:
         if (a->is_null()) return true;
         break;
      case boolean:
         if (a->is_bool() && e->value_boolean == a->value_boolean) return true;
         break;
      case number:
         if (a->is_number() && fabs(e->value_double - a->value_double) < 0.00001) return true;
         break;
      case string:
         if (a->is_string() && e->value_string == a->value_string) return true;
         break;
      case regexp:
         if (a->is_string()) {
            bool result;
            try {
               std::regex re(e->value_string.c_str());
               result = std::regex_match(a->value_string.c_str(), re);
            }
            catch (const std::regex_error& e) {
               result = false;
            }
            return result;
         }
         break;
      case array:
         if (a->is_array() && match_array(e, a)) return true;
         break;
      case object:
         if (a->is_object() && match_object(e, a)) return true;
         break;
      };

      return false;
   }

   static bool match(const char* expect, const char* actual)
   {
      node* e = parse(expect);
      node* a = parse(actual);

      bool result = match(e, a);

      frees(e);
      frees(a);

      return result;
   }

   struct dual
   {
      int depth;
      int e_type, a_type;
      h2_string e_key, a_key;
      h2_string e_value, a_value;
      std::vector<dual*, h2_allocator<dual*>> child;
      dual* perent;

      dual(int depth_, dual* perent_)
        : depth(depth_), e_type(absent), a_type(absent), perent(perent_) {}

      static void* operator new(std::size_t sz)
      {
         return h2_alloc::I().malloc(sz);
      }
      static void operator delete(void* ptr)
      {
         h2_alloc::I().free(ptr);
      }
   };

   static void frees(dual* root)
   {
      for (size_t i = 0; i < root->child.size(); ++i) {
         frees(root->child[i]);
      }
      delete root;
   }

   static void node2dual(node* n, int& type, h2_string& key, h2_string& value)
   {
      if (!n) return;

      char t[128];
      type = string;

      if (n->key_string.size()) {
         key = "\"" + n->key_string + "\"";
      }

      switch (n->type) {
      case null:
         type = string;
         value = "null";
         return;
      case boolean:
         type = string;
         value = n->value_boolean ? "true" : "false";
         return;
      case number:
         type = string;
         sprintf(t, "%1.15g", n->value_double);
         value = t;
         return;
      case string:
         type = string;
         value = "\"" + n->value_string + "\"";
         return;
      case regexp:
         type = string;
         value = n->value_string;
         return;
      case array:
         type = array;
         return;
      case object:
         type = object;
         return;
      }
      return;
   }

   static void samelengthify(h2_string& e, h2_string& a)
   {
      int e_l = e.length();
      int a_l = a.length();
      int m_l = std::max(e_l, a_l);

      e.append(m_l - e_l, samelength_char);
      a.append(m_l - a_l, samelength_char);
   }

   static void __dual(node* e, node* a, dual* d)
   {
      node2dual(e, d->e_type, d->e_key, d->e_value);
      node2dual(a, d->a_type, d->a_key, d->a_value);
      samelengthify(d->e_key, d->a_key);
      samelengthify(d->e_value, d->a_value);

      if (d->e_type != d->a_type) {
         if (d->e_type == object) {
            d->e_type = string;
            d->e_value = "{ ... }";
         }
         if (d->e_type == array) {
            d->e_type = string;
            d->e_value = "[ ... ]";
         }
         if (d->a_type == object) {
            d->a_type = string;
            d->a_value = "{ ... }";
         }
         if (d->a_type == array) {
            d->a_type = string;
            d->a_value = "[ ... ]";
         }
         samelengthify(d->e_value, d->a_value);
         return;
      }

      if (d->e_type == object) {
         for (auto i = e->child_array_or_object.begin(); i != e->child_array_or_object.end();) {
            node* e1 = *i;
            node* a1 = a->get(e1->key_string.c_str());
            if (!a1) {
               for (auto j = a->child_array_or_object.begin(); j != a->child_array_or_object.end(); j++) {
                  if (match(e1, *j)) {
                     a1 = *j;
                     break;
                  }
               }
            }
            if (a1) {
               dual* d1 = new dual(d->depth + 1, d);
               d->child.push_back(d1);
               __dual(e1, a1, d1);
               a->del(a1);
               i = e->child_array_or_object.erase(i);
               delete e1;
            }
            else {
               i++;
            }
         }

         size_t K = std::max(e->child_array_or_object.size(), a->child_array_or_object.size());
         for (size_t i = 0; i < K; ++i) {
            dual* d1 = new dual(d->depth + 1, d);
            d->child.push_back(d1);
            node* e1 = e->get(i);
            node* a1 = a->get(i);
            __dual(e1, a1, d1);
         }
      }

      if (d->e_type == array) {
         size_t K = std::max(e->child_array_or_object.size(), a->child_array_or_object.size());
         for (size_t i = 0; i < K; ++i) {
            dual* d1 = new dual(d->depth + 1, d);
            d->child.push_back(d1);
            node* e1 = e->get(i);
            node* a1 = a->get(i);
            __dual(e1, a1, d1);
         }
      }
   }

   static h2_string indent(int depth)
   {
      return h2_string(depth * 2, indent_char);
   }
   static h2_string occupy(h2_string p)
   {
      return h2_string(p.length(), occupy_char);
   }

   typedef std::vector<h2_string, h2_allocator<h2_string>> list;

   static void diff(dual* d, list& e, list& a)
   {
      if (!d) return;
      e.push_back("\n");
      e.push_back(indent(d->depth));
      a.push_back("\n");
      a.push_back(indent(d->depth));

      if (d->e_type != absent) {
         if (d->a_type == absent) {  // only e-side exist
            if (d->e_key.size()) {
               e.push_back("#cyan");
               e.push_back(d->e_key + ": ");
               e.push_back("#reset");
            }
            if (d->e_value.size()) {
               e.push_back("#cyan");
               e.push_back(d->e_value);
               e.push_back("#reset");
            }
         }
         else {
            if (d->e_key.size()) {
               if (d->e_key != d->a_key) e.push_back("#green");
               e.push_back(d->e_key);
               if (d->e_key != d->a_key) e.push_back("#reset");
               e.push_back(": ");
            }
            if (d->e_value.size()) {
               if (d->e_value != d->a_value) e.push_back("#green");
               e.push_back(d->e_value);
               if (d->e_value != d->a_value) e.push_back("#reset");
            }
         }
      }
      else {
         if (d->a_key.size()) {
            e.push_back(occupy(d->a_key + ": "));
         }
         if (d->a_value.size()) {
            e.push_back(occupy(d->a_value));
         }
      }

      if (d->a_type != absent) {
         if (d->e_type == absent) {  // only a-side exist
            const char* style = "#red,bold";
            if (d->perent && d->perent->a_type == object) {
               style = "#yellow";
            }
            if (d->a_key.size()) {
               a.push_back(style);
               a.push_back(d->a_key + ": ");
               a.push_back("#reset");
            }
            if (d->a_value.size()) {
               a.push_back(style);
               a.push_back(d->a_value);
               a.push_back("#reset");
            }
         }
         else {
            if (d->a_key.size()) {
               if (d->a_key != d->e_key) a.push_back("#red,bold");
               a.push_back(d->a_key);
               if (d->a_key != d->e_key) a.push_back("#reset");
               a.push_back(": ");
            }
            if (d->a_value.size()) {
               if (d->a_value != d->e_value) a.push_back("#red,bold");
               a.push_back(d->a_value);
               if (d->a_value != d->e_value) a.push_back("#reset");
            }
         }
      }
      else {
         if (d->e_key.size()) {
            a.push_back(occupy(d->e_key + ": "));
         }
         if (d->e_value.size()) {
            a.push_back(occupy(d->e_value));
         }
      }

      /* e/a type shoud be same */

      if (d->e_type == object && d->a_type == object) {
         e.push_back("{");
         a.push_back("{");
      }
      if (d->e_type == array && d->a_type == array) {
         e.push_back("[");
         a.push_back("[");
      }

      if ((d->e_type == object && d->a_type == object) ||
          (d->e_type == array && d->a_type == array)) {
         for (size_t i = 0; i < d->child.size(); i++) {
            diff(d->child[i], e, a);

            bool e_not_last = false, a_not_last = false;
            for (size_t j = i + 1; j < d->child.size(); j++) {
               e_not_last = e_not_last || (d->child[j]->e_type != absent);
               a_not_last = a_not_last || (d->child[j]->a_type != absent);
            }
            if (e_not_last) e.push_back(",");
            if (a_not_last) a.push_back(",");
         }
         if (d->child.size()) {
            e.push_back("\n");
            e.push_back(indent(d->depth));
            a.push_back("\n");
            a.push_back(indent(d->depth));
         }
      }

      if (d->e_type == object && d->a_type == object) {
         e.push_back("}");
         a.push_back("}");
      }
      if (d->e_type == array && d->a_type == array) {
         e.push_back("]");
         a.push_back("]");
      }
   }

   typedef std::vector<h2_string, h2_allocator<h2_string>> line;
   typedef std::vector<line, h2_allocator<line>> lines;

   static void merge_line(list& x_list, lines& x_lines)
   {
      line x_line;
      for (auto it = x_list.begin(); it != x_list.end(); it++) {
         if ((*it) == "\n") {
            x_lines.push_back(x_line);
            x_line.clear();
            continue;
         }
         x_line.push_back(*it);
      }
      x_lines.push_back(x_line);
      x_line.clear();
   }

   static int lines_most(lines& x_lines)
   {
      int most = 0;
      for (auto i = x_lines.begin(); i != x_lines.end(); i++) {
         auto x_line = *i;
         int curr = 0;
         for (auto j = x_line.begin(); j != x_line.end(); j++) {
            auto word = *j;
            if (word[0] == '#') {
               continue;
            }
            else {
               curr += word.length();
            }
         }
         most = std::max(most, curr);
      }
      return most;
   }

   static int line_wrap(line& x_line, int columns)
   {
      int char_count = 0;
      for (auto i = x_line.begin(); i != x_line.end(); i++) {
         auto word = *i;
         if (word[0] == '#') {
            continue;
         }
         else {
            char_count += word.length();
         }
      }

      int num_of_line = H2_DIV_ROUND_UP(char_count, columns);
      return num_of_line;
   }

   static h2_string line_wrap(line& x_line, int index, int columns, h2_string& current_style)
   {
      int s = 0, u = 0;
      h2_string wrap;
      for (auto i = x_line.begin(); i != x_line.end(); i++) {
         auto word = *i;
         if (word[0] == '#') {
            if (index * columns <= s && s < (index + 1) * columns) {
               const char* style = h2_cfg::style(word.c_str() + 1);
               wrap.append(style);
               current_style = style;
            }
         }
         else {
            for (auto j = word.begin(); j != word.end(); j++) {
               if (index * columns <= s && s < (index + 1) * columns) {
                  wrap.append(1, *j);
                  ++u;
               }
               ++s;
            }
         }
      }

      wrap.append(columns - u, columns_char);
      return wrap;
   }

   static void print(lines& e_lines, lines& a_lines, int side_columns)
   {
      h2_string e_last_style, a_last_style;
      assert(e_lines.size() == a_lines.size());
      for (size_t i = 0; i < std::max(e_lines.size(), a_lines.size()); ++i) {
         auto e_line = e_lines[i];
         auto a_line = a_lines[i];
         int e_wraps = line_wrap(e_line, side_columns - 2);
         int a_wraps = line_wrap(a_line, side_columns - 2);
         assert(e_wraps == a_wraps);
         int K = std::max(e_wraps, a_wraps);
         for (int j = 0; j < K; ++j) {
            h2_string e_current_style, a_current_style;
            auto e_wrap = line_wrap(e_line, j, side_columns - 2, e_current_style);
            auto a_wrap = line_wrap(a_line, j, side_columns - 2, a_current_style);
            ::printf("%s%s %s%s%s│%s %s%s %s%s%s%s\n",
                     e_last_style.c_str(), e_wrap.c_str(), h2_cfg::style("reset"), 
                     h2_cfg::style("dark gray"), j == K - 1 ? " " : "\\", h2_cfg::style("reset"),
                     a_last_style.c_str(), a_wrap.c_str(), h2_cfg::style("reset"), 
                     h2_cfg::style("dark gray"), j == K - 1 ? " " : "\\", h2_cfg::style("reset"));

            e_last_style = e_current_style;
            a_last_style = a_current_style;
         }
      }
   }

   static void diff_print(const char* expect, const char* actual, int terminal_columns)
   {
      node* e_node = parse(expect);
      node* a_node = parse(actual);

      dual* d = new dual(0, nullptr);
      __dual(e_node, a_node, d);

      frees(e_node);
      frees(a_node);

      list e_list, a_list;
      diff(d, e_list, a_list);
      frees(d);

      lines e_lines, a_lines;
      merge_line(e_list, e_lines);
      merge_line(a_list, a_lines);

      int e_most = lines_most(e_lines);
      int a_most = lines_most(a_lines);

      int fav_columns = std::max(std::max(e_most, a_most) + 3, 30);

      int side_columns = std::min(terminal_columns / 2 - 4, fav_columns);

      char t1[256], t2[256];
      ::printf("%s%s%s%s│%s%s%s%s\n",
               h2_cfg::style("dark gray"),
               h2_center_string("expect", side_columns, t1),
               h2_cfg::style("reset"),
               h2_cfg::style("dark gray"), h2_cfg::style("reset"),
               h2_cfg::style("dark gray"),
               h2_center_string("actual", side_columns, t2),
               h2_cfg::style("reset"));

      print(e_lines, a_lines, side_columns);
   }
};

#endif

#ifndef ___H2_BT__H___
#define ___H2_BT__H___

#include <cxxabi.h>

class h2_backtrace
{
 private:
   int count;
   int offset;
   void* array[100];

 public:
   h2_backtrace()
     : count(0), offset(0) {}

   h2_backtrace& operator=(h2_backtrace& bt)
   {
      offset = bt.offset;
      count = bt.count;
      memcpy(array, bt.array, bt.count * sizeof(void*));

      return *this;
   }

   void backtrace(int offset = 0)
   {
      count = ::backtrace(array, H2_ARRAY_COUNTOF(array));
      this->offset = offset;
   }

   bool operator==(h2_backtrace& bt)
   {
      if (count != bt.count) return false;
      for (int i = 0; i < count; ++i) {
         if (array[i] != bt.array[i]) {
            return false;
         }
      }
      return true;
   }

   bool has(void* func, int size)
   {
      for (int i = 0; i < count; ++i) {
         if (func <= array[i] &&
             (unsigned char*)array[i] < ((unsigned char*)func) + size) {
            return true;
         }
      }
      return false;
   }

   void print()
   {
      char** s = backtrace_symbols(array, count);
      for (int i = offset; i < count; ++i) {
         const char* mangled_name = parse(s[i]);
         const char* demangled_name = demangle(mangled_name);

         int line = 0;
#if __linux__
         char func[1024];
         char file[1024];
         int ret = read_by_addr2line(array[i], h2_cfg::I().path, func, file, line);
         if (0 <= ret) {
            demangled_name = func;
         }
#endif
         ::printf("   %d. %s", i, demangled_name);
         if (0 < line) {
            ::printf(" : %d", line);
         }
         ::printf("\n");

         if (strcmp("main", demangled_name) == 0) {
            break;
         }
      }
      free(s);
   }

   int read_by_addr2line(void* addr, const char* path, char* func, char* file, int& line)
   {
      char buf[1024 * 2] = {0};
      FILE* fp;

      snprintf(buf, sizeof(buf), "addr2line -C -e %s -f -i %p", path, addr);
      // snprintf(buf, sizeof(buf), "atos -o %s %p", path, addr);

      fp = ::popen(buf, "r");
      if (fp == NULL) {
         printf("run addr2line error: %s\n", strerror(errno));
         return -1;
      }

      //1st line function name
      ::fgets(buf, sizeof(buf) - 1, fp);
      if (buf[strlen(buf) - 1] == '\n') {
         buf[strlen(buf) - 1] = '\0';
      }

      strcpy(func, buf);
      if (buf[0] == '?') {
         sprintf(func, "unknown %p", addr);
      }

      //2nd line file and line
      ::fgets(buf, sizeof(buf) - 1, fp);

      char* p = ::strchr(buf, ':');
      if (p) {
         *p = '\0';
         strcpy(file, buf);
         line = atoi(p + 1);
      }

      ::pclose(fp);
      return 0;
   }

   const char* parse(const char* symbol)
   {
      static char temp[128];

      if (1 == sscanf(symbol, "%*[^(]%*[^_]%127[^)+]", temp)) {
         return temp;
      }

      if (1 == sscanf(symbol, "%*s%*s%*s %127[^ )+]", temp)) {
         return temp;
      }

      if (1 == sscanf(symbol, "%127s", temp)) {
         return temp;
      }

      return symbol;
   }

   const char* demangle(const char* mangled_name)
   {
      static char temp[1024];
      int status = 0;
      const char* realname = abi::__cxa_demangle(mangled_name, 0, 0, &status);
      switch (status) {
      case 0:
         strcpy(temp, realname);
         break;
      case -1:
         // printf("FAIL: failed to allocate memory while demangling %s\n", mangled_name);
      case -2:
         // printf("FAIL: %s is not a valid name under the C++ ABI mangling rules\n", mangled_name);
      default:
         // printf("FAIL: some other unexpected error: %d\n", status);
         strcpy(temp, mangled_name);
         break;
      }
      if (realname) free((void*)realname);
      return temp;
   }
};

#endif

#ifndef ___H2_FAIL__H___
#define ___H2_FAIL__H___

#define H2_FAIL_FOREACH(f, First)                                 \
   for (h2_fail* x_fail = First; x_fail; x_fail = x_fail->x_next) \
      for (h2_fail* f = x_fail; f; f = f->y_next)

class h2_fail
{
 public:
   h2_fail *x_next, *y_next;

 protected:
   const char* file;
   int line;
   const char* func;

   int _argth;

   h2_string _k;

 public:
   h2_fail(const char* file_, int line_)
     : x_next(nullptr), y_next(nullptr), file(file_), line(line_), func(nullptr), _argth(-1) {}

   virtual ~h2_fail()
   {
      if (y_next) {
         delete y_next;
      }
      if (x_next) {
         delete x_next;
      }
   };

   void appendx(h2_fail* f)
   {
      if (!x_next) {
         x_next = f;
         return;
      }
      h2_fail* p = x_next;
      while (p->x_next) {
         p = p->x_next;
      }
      p->x_next = f;
   }

   void appendy(h2_fail* f)
   {
      if (!y_next) {
         y_next = f;
         return;
      }
      h2_fail* p = y_next;
      while (p->y_next) {
         p = p->y_next;
      }
      p->y_next = f;
   }

   void locate(const char* file_, int line_, const char* func_)
   {
      file = file_;
      line = line_;
      func = func_;

      if (y_next) {
         y_next->locate(file_, line_, func_);
      }
      if (x_next) {
         x_next->locate(file_, line_, func_);
      }
   }

   void argth(int argth_)
   {
      _argth = argth_;
      if (y_next) {
         y_next->argth(argth_);
      }
   }

   void kprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _k = _k + t;
   }

   virtual void print()
   {
      if (0 < _k.size()) {
         ::printf(" %s", _k.c_str());
         print_locate();
         ::printf("\n");
      }
   }

   void print_locate()
   {
      if (func && strlen(func)) {
         ::printf(", in %s(", func);
         if (0 <= _argth) {
            static const char* argz[] = {"1st", "2nd", "3rd", "4th"};
            if (_argth < 4)
               ::printf("%s", argz[_argth]);
            else
               ::printf("%dth", _argth - 1);
         }
         ::printf(")");
      }
      if (file && strlen(file) && 0 < line) {
         ::printf(", at %s:%d", file, line);
      }
   }

   virtual void print(FILE* fp) {}

   static void* operator new(std::size_t sz)
   {
      return h2_alloc::I().malloc(sz);
   }
   static void operator delete(void* ptr)
   {
      h2_alloc::I().free(ptr);
   }
};

class h2_fail_normal : public h2_fail
{
 public:
   h2_fail_normal(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_normal() {}

   void k(h2_string key)
   {
      _k = key;
   }

   void print(FILE* fp)
   {
      fprintf(fp, "normal");
   }
};

class h2_fail_unexpect : public h2_fail
{
 private:
   h2_string _e, _a, _h, _m, _t;

 public:
   h2_fail_unexpect(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_unexpect() {}

   void hamet(h2_string head, h2_string actual, h2_string middle, h2_string expect, h2_string tail)
   {
      _h = head;
      _a = actual;
      _m = middle;
      _e = expect;
      _t = tail;
   }

   void hprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _h = t;
   }

   void eprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _e = t;
   }

   void mprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _m = t;
   }

   void aprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _a = t;
   }

   void tprintf(const char* format, ...)
   {
      char t[1024 * 8];
      va_list args;

      va_start(args, format);
      vsnprintf(t, sizeof(t), format, args);
      va_end(args);

      _t = t;
   }

   void print()
   {
      h2_fail::print();
      ::printf(" %s%s%s%s %s %s%s%s%s",
               _h.c_str(),
               h2_cfg::style("bold,red"), _a.c_str(), h2_cfg::style("reset"),
               _m.c_str(),
               h2_cfg::style("green"), _e.c_str(), h2_cfg::style("reset"),
               _t.c_str());
      print_locate();
      ::printf("\n");
   }

   void print(FILE* fp)
   {
      fprintf(fp, " %s %s %s %s %s ", _h.c_str(), _a.c_str(), _m.c_str(), _e.c_str(), _t.c_str());
   }
};

class h2_fail_strcmp : public h2_fail
{
 private:
   h2_string e, a;
   bool caseless;

 public:
   h2_fail_strcmp(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_strcmp() {}

   void add(h2_string expect, h2_string actual, bool caseless)
   {
      e = expect;
      a = actual;
      this->caseless = caseless;
      kprintf("String not %sequals", caseless ? "case-insensitive " : "");
   }

   void get(int i, char& e0, char& e1, char& a0, char& a1)
   {
      if (i < (int)e.length())
         e0 = e[i];
      else
         e0 = ' ';
      e1 = ::tolower(e0);

      if (i < (int)a.length())
         a0 = a[i];
      else
         a0 = ' ';
      a1 = ::tolower(a0);
   }

   void print()
   {
      h2_fail::print();

      int terminal_columns = h2_cfg::get_term_columns();
      int columns = terminal_columns - 12;
      int rows = H2_DIV_ROUND_UP((int)std::max(e.length(), a.length()), columns);

      for (int i = 0; i < rows; ++i) {
         ::printf("%sexpect%s> ", h2_cfg::style("dark gray"), h2_cfg::style("reset"));
         for (int j = 0; j < columns; ++j) {
            char e0, e1, a0, a1;
            get(i * columns + j, e0, e1, a0, a1);
            bool eq = caseless ? e1 == a1 : e0 == a0;
            if (!eq) ::printf("%s", h2_cfg::style("green"));
            ::printf("%c", e0);
            if (!eq) ::printf("%s", h2_cfg::style("reset"));
         }
         ::printf("\n");
         ::printf("%sactual%s> ", h2_cfg::style("dark gray"), h2_cfg::style("reset"));
         for (int j = 0; j < columns; ++j) {
            char e0, e1, a0, a1;
            get(i * columns + j, e0, e1, a0, a1);
            bool eq = caseless ? e1 == a1 : e0 == a0;
            if (!eq) ::printf("%s", h2_cfg::style("red,bold"));
            ::printf("%c", a0);
            if (!eq) ::printf("%s", h2_cfg::style("reset"));
         }
         ::printf("\n");
      }
   }

   void print(FILE* fp)
   {
      fprintf(fp, "strcmp failed");
   }
};

class h2_fail_memcmp : public h2_fail
{
 private:
   const void* _actual;
   int _size;
   std::vector<unsigned char, h2_allocator<unsigned char>> e;
   std::vector<unsigned char, h2_allocator<unsigned char>> a;

 public:
   h2_fail_memcmp(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_memcmp() {}
   void add(const void* expect, const void* actual, const int len)
   {
      _actual = actual;
      _size = len;
      e.assign((unsigned char*)expect, ((unsigned char*)expect) + len);
      a.assign((unsigned char*)actual, ((unsigned char*)actual) + len);
      kprintf("Memory %p binary %d bytes not equal", actual, len);
   }

   void print()
   {
      h2_fail::print();

      ::printf("                     %sexpect%s                       %s│%s                       %sactual%s \n",
               h2_cfg::style("dark gray"), h2_cfg::style("reset"),
               h2_cfg::style("dark gray"), h2_cfg::style("reset"),
               h2_cfg::style("dark gray"), h2_cfg::style("reset"));

      int size = (int)e.size();
      int rows = (size + 15) / 16;  //H2_DIV_ROUND_UP(size, 16);

      for (int i = 0; i < rows; ++i) {
         for (int j = 0; j < 16; ++j) {
            if (size <= i * 16 + j) {
               ::printf("   ");
               continue;
            }
            if (e[i * 16 + j] != a[i * 16 + j]) {
               ::printf("%s", h2_cfg::style("green"));
            }
            ::printf(j < 8 ? "%02X " : " %02X", e[i * 16 + j]);
            ::printf("%s", h2_cfg::style("reset"));
         }
         ::printf("  %s│%s  ", h2_cfg::style("dark gray"), h2_cfg::style("reset"));
         for (int j = 0; j < 16; ++j) {
            if (size <= i * 16 + j) {
               ::printf("   ");
               continue;
            }
            if (e[i * 16 + j] != a[i * 16 + j]) {
               ::printf("%s", h2_cfg::style("bold,red"));
            }
            ::printf(j < 8 ? "%02X " : " %02X", a[i * 16 + j]);
            ::printf("%s", h2_cfg::style("reset"));
         }
         ::printf("\n");
      }
   }
   void print(FILE* fp)
   {
      fprintf(fp, "Memory %p compare %d bytes failed", _actual, _size);
   }
};

class h2_fail_memover : public h2_fail
{
 private:
   const void* _ptr;
   int _offset;
   std::vector<unsigned char, h2_allocator<unsigned char>> _a;
   const unsigned char* _magic;
   h2_backtrace bt;

 public:
   h2_fail_memover(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_memover() {}

   void add(void* ptr, int offset, const unsigned char* magic, int size, h2_backtrace& bt)
   {
      _ptr = ptr;
      _offset = offset;
      _magic = magic;
      _a.assign(((unsigned char*)ptr) + offset, ((unsigned char*)ptr) + offset + size);
      this->bt = bt;
   }

   void print()
   {
      h2_fail::print();
      ::printf(" Memory overflow malloc %p %+d  ", _ptr, _offset);

      for (int i = 0; i < (int)_a.size(); ++i) {
         if (_magic[i] == _a[i]) {
            ::printf("%s", h2_cfg::style("green"));
         }
         else {
            ::printf("%s", h2_cfg::style("bold,red"));
         }
         ::printf("%02X ", _a[i]);
         ::printf("%s", h2_cfg::style("reset"));
      }
      print_locate();
      ::printf("\n");
      bt.print();
   }

   void print(FILE* fp)
   {
      fprintf(fp, "Memory overflow malloc %p %+d  ", _ptr, _offset);
   }
};

class h2_fail_memleak : public h2_fail
{
 private:
   const char* _where;
   struct V
   {
      void* ptr;
      int size;
      int bytes;
      int times;
      h2_backtrace bt;
      V(void* _ptr, int _size, h2_backtrace& _bt)
        : ptr(_ptr), size(_size), bytes(_size), times(1), bt(_bt)
      {
      }
   };
   std::vector<V, h2_allocator<V>> _leaks;
   long long _bytes;
   int _places;

 public:
   h2_fail_memleak(const char* file = "", int line = -1, const char* where = "")
     : h2_fail(file, line), _where(where), _bytes(0), _places(0) {}
   virtual ~h2_fail_memleak() {}

   void add(void* ptr, int size, h2_backtrace& bt)
   {
      _bytes += size;
      _places += 1;
      for (auto i = _leaks.begin(); i != _leaks.end(); i++) {
         if ((*i).bt == bt) {
            (*i).bytes += size;
            (*i).times += 1;
            return;
         }
      }
      _leaks.push_back(V(ptr, size, bt));
   }

   void print()
   {
      kprintf("Memory Leaked %s%lld bytes in %s totally", str_places(_places), _bytes, _where);
      h2_fail::print();
      for (auto i = _leaks.begin(); i != _leaks.end(); i++) {
         auto v = *i;
         ::printf("  %p", v.ptr);
         if (v.times <= 1)
            ::printf(" ");
         else
            ::printf("... ");
         ::printf("Leaked ");
         if (v.times <= 1)
            /* ::printf("") */;
         else
            ::printf("%d times ", v.times);
         ::printf("%d", v.bytes);
         if (v.times <= 1)
            ::printf(" ");
         else
            ::printf("(%d+...)", v.size);
         ::printf("bytes, at backtrace\n");

         v.bt.print();
      }
   }

   const char* str_places(int places)
   {
      if (places <= 1) return "";
      static char t[32];
      sprintf(t, "%d places ", places);
      return t;
   }

   void print(FILE* fp)
   {
      fprintf(fp, "Memory Leaked %s%lld bytes in %s totally", str_places(_places), _bytes, _where);
   }
};

class h2_fail_json : public h2_fail
{
 private:
   h2_string e, a;

 public:
   h2_fail_json(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_json() {}

   void add(const char* expect, const char* actual)
   {
      e = expect;
      a = actual;
   }

   void print()
   {
      h2_fail::print();

      int terminal_columns = h2_cfg::get_term_columns();

      h2json::diff_print(e.c_str(), a.c_str(), terminal_columns);
   }

   void print(FILE* fp)
   {
      fprintf(fp, "JSON compare failed");
   }
};

class h2_fail_instantiate : public h2_fail
{
 private:
   const char* action_type;
   const char* return_type;
   const char* class_type;
   const char* method_name;
   const char* return_args;
   int why;

 public:
   h2_fail_instantiate(const char* file = "", int line = -1)
     : h2_fail(file, line) {}
   virtual ~h2_fail_instantiate() {}

   void add(
     const char* action_type,
     const char* return_type,
     const char* class_type,
     const char* method_name,
     const char* return_args,
     int why)
   {
      this->action_type = action_type;
      this->return_type = return_type;
      this->class_type = class_type;
      this->method_name = method_name;
      this->return_args = return_args;
      this->why = why;

      if (why == 1)
         kprintf("Instantiate 'class %s' don't know initialize arguments", class_type);
      else if (why == 2)
         kprintf("Instantiate 'class %s' is a abstract class", class_type);
   }

   void print()
   {
      h2_fail::print();

      ::printf("You may take following solutions to fix it: \n");
      if (why == 1) {
         ::printf("1. Define default constructor in class %s, or \n", class_type);
         ::printf("2. Add parameterized construction in %s(%s%s%s, %s, %s%s, %s(...)%s) \n",
                  action_type,
                  strlen(return_type) ? return_type : "",
                  strlen(return_type) ? ", " : "",
                  class_type, method_name, return_args,
                  h2_cfg::style("bold,yellow"),
                  class_type,
                  h2_cfg::style("reset"));
      }
      else if (why == 2) {
         ::printf("1. Add non-abstract Derived Class instance in %s(%s%s%s, %s, %s%s, Derived_%s(...)%s) \n",
                  action_type,
                  strlen(return_type) ? return_type : "",
                  strlen(return_type) ? ", " : "",
                  class_type, method_name, return_args,
                  h2_cfg::style("bold,yellow"),
                  class_type,
                  h2_cfg::style("reset"));
      }
   }

   void print(FILE* fp)
   {
      fprintf(fp, "Instantiate 'class %s' don't know initialize arguments", class_type);
   }
};

#endif

#ifndef ___H2_LEAK__H___
#define ___H2_LEAK__H___

static unsigned char h2_over_magic[] = {0xbe, 0xaf, 0xca, 0xfe, 0xc0, 0xde, 0xfa, 0xce};

struct h2_mm
{
   void* ptr;
   int size;
   bool escape;
   h2_backtrace bt;
   signed char data[0];

   static void __gen_aligned_ptr(h2_mm* mm, int alignment)
   {
      if (alignment == 0) {
         alignment = 8;
      }
      signed char* p1 = &mm->data[1 + sizeof(h2_over_magic)];
      signed char* p2 = (signed char*)H2_ALIGN_UP((uint64_t)p1, alignment);
      p2[-1 - sizeof(h2_over_magic)] = (signed char)((int64_t)p2 - (int64_t)mm->data);
      mm->ptr = (void*)p2;
   }

   static h2_mm* container_of(void* ptr)
   {
      return (h2_mm*)(((signed char*)ptr) - sizeof(h2_mm) - ((signed char*)ptr)[-1 - sizeof(h2_over_magic)]);
   }

   static h2_mm* allocate(int size, int alignment, h2_backtrace& bt)
   {
      h2_mm* mm = (h2_mm*)h2_alloc::U().malloc(sizeof(h2_mm) + size + alignment + 32 + sizeof(h2_over_magic) * 2);
      if (!mm) {
         return nullptr;
      }

      mm->size = size;
      mm->bt = bt;

      __gen_aligned_ptr(mm, alignment);

      memcpy((unsigned char*)mm->ptr + mm->size, h2_over_magic, sizeof(h2_over_magic));
      memcpy((unsigned char*)mm->ptr - sizeof(h2_over_magic), h2_over_magic, sizeof(h2_over_magic));

      return mm;
   }

   static h2_fail* release(h2_mm* mm)
   {
      h2_fail_memover* fail1 = nullptr;
      h2_fail_memover* fail2 = nullptr;

      /* overflow and under-flow checking */
      if (memcmp((unsigned char*)mm->ptr + mm->size, h2_over_magic, sizeof(h2_over_magic))) {
         fail1 = new h2_fail_memover();
         fail1->add(mm->ptr, mm->size, h2_over_magic, sizeof(h2_over_magic), mm->bt);
      }

      if (memcmp((unsigned char*)mm->ptr - sizeof(h2_over_magic), h2_over_magic, sizeof(h2_over_magic))) {
         fail2 = new h2_fail_memover();
         fail2->add(mm->ptr, -(int)sizeof(h2_over_magic), h2_over_magic, sizeof(h2_over_magic), mm->bt);
      }

      if (fail1) {
         fail1->appendx(fail2);
      }

      h2_alloc::U().free(mm);

      return fail1 ? fail1 : fail2;
   }
};

class h2_leak_stack
{
 private:
   struct block
   {
      std::vector<void*, h2_allocator<void*>> mm_list;

      const char* file;
      int line;
      const char* where;
      long long limited;
      const char* fill;

      block(const char* _file, int _line, const char* _where, long long _limited, const char* _fill)
        : file(_file), line(_line), where(_where), limited(_limited), fill(_fill) {}

      bool escape(h2_backtrace& bt)
      {
         struct
         {
            unsigned char* base;
            int size;
         } exclude_functions[] = {
           {(unsigned char*)sprintf, 300},
           {(unsigned char*)sscanf, 300},
           {(unsigned char*)localtime, 300}};

         for (int i = 0; i < H2_ARRAY_COUNTOF(exclude_functions); ++i) {
            if (bt.has(exclude_functions[i].base, exclude_functions[i].size)) {
               return true;
            }
         }
         return false;
      }

      h2_mm* new_mm(int size, int alignment, const char* fill, h2_backtrace& bt)
      {
         if (limited < size) {
            return nullptr;
         }
         h2_mm* mm = h2_mm::allocate(size, alignment, bt);
         if (!fill) {
            fill = this->fill;
         }
         if (fill) {
            int length = strlen(fill);
            for (int i = 0, j = 0; i < size; ++i, ++j) {
               if (j >= length) j = 0;
               ((char*)mm->ptr)[i] = fill[j];
            }
         }

         mm->escape = escape(bt);
         if (!mm->escape) {
            mm_list.push_back(mm->ptr);
         }
         return mm;
      }

      h2_mm* get_mm(void* ptr)
      {
         for (auto it = mm_list.begin(); it != mm_list.end(); it++) {
            if (*it == ptr) {
               return h2_mm::container_of(ptr);
            }
         }
         return nullptr;
      }

      h2_fail* rel_mm(h2_mm* mm)
      {
         limited += mm->size;
         for (auto it = mm_list.begin(); it != mm_list.end(); it++) {
            if (*it == mm->ptr) {
               mm_list.erase(it);
               return h2_mm::release(mm);
            }
         }

         return nullptr;
      }

      h2_fail* check()
      {
         h2_fail_memleak* fail = nullptr;

         if (!mm_list.empty()) {
            fail = new h2_fail_memleak(file, line, where);
            for (auto it = mm_list.begin(); it != mm_list.end(); it++) {
               h2_mm* mm = h2_mm::container_of(*it);
               fail->add(mm->ptr, mm->size, mm->bt);
            }
         }
         return fail;
      }

      static void* operator new(std::size_t sz)
      {
         return h2_alloc::I().malloc(sz);
      }
      static void operator delete(void* ptr)
      {
         h2_alloc::I().free(ptr);
      }
   };

   block* stack[100];
   int top;

 public:
   h2_leak_stack()
     : top(0) {}

   bool push(const char* file, int line, const char* where, long long limited = 0x7fffffffffffLL, const char* fill = NULL)
   {
      if (top > 64) {
         printf("\nWarning: nested leak block too many! %s:%d\n", file, line);
      }
      stack[top++] = new block(file, line, where, limited, fill);
      return true;
   }

   h2_fail* pop()
   {
      block* b = stack[--top];
      auto fail = b->check();
      delete b;
      return fail;
   }

   h2_mm* new_mm(size_t size, size_t alignment, const char* fill, h2_backtrace& bt)
   {
      if (top == 0) {
         printf("\nWarning: malloc in idle state! %s:%d\n", __FUNCTION__, __LINE__);
         return nullptr;
      }
      return stack[top - 1]->new_mm(size, alignment, fill, bt);
   }

   h2_mm* get_mm(void* ptr)
   {
      for (int i = top - 1; i >= 0; --i) {
         h2_mm* mm = stack[i]->get_mm(ptr);
         if (mm) {
            return mm;
         }
      }

      return nullptr;
   }

   h2_fail* rel_mm(h2_mm* mm)
   {
      if (mm->escape) {
         return nullptr;
      }

      if (top == 0) {
         printf("\nWarning: free in idle state! \n");
         return nullptr;
      }

      for (int i = top - 1; i >= 0; --i) {
         if (stack[i]->get_mm(mm->ptr)) {
            return stack[i]->rel_mm(mm);
         }
      }

      printf("\nWarning: free not found! \n");

      return nullptr;
   }
};

static inline h2_mm* h2_getm_g(void* ptr);
static inline h2_mm* h2_newm_g(size_t size, size_t alignment, const char* fill, h2_backtrace& bt);
static inline void h2_relm_g(h2_mm* mm);

struct h2_leak_inspector
{
   static void free(void* ptr)
   {
      if (ptr) {
         h2_mm* mm = h2_getm_g(ptr);
         if (mm) {
            h2_relm_g(mm);
         }
      }
   }

   static void* malloc(size_t size)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      h2_mm* mm = h2_newm_g(size, 0, nullptr, bt);
      return mm ? mm->ptr : nullptr;
   }

   static void* calloc(size_t count, size_t size)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      h2_mm* mm = h2_newm_g(size * count, 0, "\0", bt);
      return mm ? mm->ptr : nullptr;
   }

   static void* realloc(void* ptr, size_t size)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      if (size == 0) {
         if (ptr) {
            h2_mm* old_mm = h2_getm_g(ptr);
            if (old_mm) {
               h2_relm_g(old_mm);
            }
         }
         return nullptr;
      }

      h2_mm* old_mm = h2_getm_g(ptr);
      if (!old_mm) {
         return nullptr;
      }

      h2_mm* new_mm = h2_newm_g(size, 0, nullptr, bt);
      if (!new_mm) {
         return nullptr;
      }

      memcpy(new_mm->ptr, old_mm->ptr, old_mm->size);
      h2_relm_g(old_mm);

      return new_mm->ptr;
   }

   static int posix_memalign(void** memptr, size_t alignment, size_t size)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      h2_mm* mm = h2_newm_g(size, alignment, nullptr, bt);
      if (mm) {
         *memptr = mm->ptr;
         return 0;
      }
      // #ifndef ENOMEM
      // #define ENOMEM -1
      // #endif
      return ENOMEM;
   }

   static void* aligned_alloc(size_t alignment, size_t size)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      h2_mm* mm = h2_newm_g(size, alignment, nullptr, bt);
      return mm ? mm->ptr : nullptr;
   }

   static char* strdup(const char* s)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      size_t size = strlen(s) + 1;
      h2_mm* mm = h2_newm_g(size, 0, nullptr, bt);
      if (mm) {
         memcpy(mm->ptr, s, size);
      }
      return mm ? (char*)mm->ptr : nullptr;
   }

   static char* strndup(const char* s, size_t n)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      size_t size = strlen(s);
      size = (size > n ? n : size) + 1;
      h2_mm* mm = h2_newm_g(size, 0, nullptr, bt);
      if (mm) {
         memcpy(mm->ptr, s, size - 1);
         ((char*)mm->ptr)[size - 1] = '\0';
      }
      return mm ? (char*)mm->ptr : nullptr;
   }
};

#endif

#ifndef ___H2_STUB__H___
#define ___H2_STUB__H___

// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_stub
{
   void* be_fp;
#if defined(__x86_64__) || defined(_M_X64)
   unsigned char saved_code[sizeof(void*) + 4];
#elif defined(__i386__) || defined(_M_IX86)
   unsigned char saved_code[sizeof(void*) + 1];
#else
   unsigned char saved_code[1];
#endif

   static h2_stub* acq()
   {
      h2_stub* stub = (h2_stub*)h2_alloc::I().malloc(sizeof(h2_stub));
      return stub;
   }
   static void rel(h2_stub* stub)
   {
      h2_alloc::I().free(stub);
   }

   bool save(void* be_fp)
   {
#ifdef _WIN32
      DWORD saved;
      if (!VirtualProtect(be_fp, sizeof(void*) + 4, PAGE_EXECUTE_READWRITE, &saved)) {  // PAGE_EXECUTE_WRITECOPY OR PAGE_WRITECOPY
         return false;
      }
#else
      long long pagesize = (long long)sysconf(_SC_PAGE_SIZE);
      if (mprotect((void*)((unsigned long long)be_fp & (~(pagesize - 1))), pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         printf("STUB failed %s\n", strerror(errno));
         return false;
      }
      if (mprotect((void*)(((unsigned long long)be_fp + sizeof(saved_code)) & (~(pagesize - 1))), pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         printf("STUB failed %s\n", strerror(errno));
         return false;
      }
#endif

      this->be_fp = be_fp;
      memcpy(saved_code, be_fp, sizeof(saved_code));
      return true;
   }
   void set(void* to_fp)
   {
      unsigned char* I = (unsigned char*)be_fp;

      //x86 __asm("jmp $to_fp") : 0xE9 {offset=to_fp-be_fp-5}
      //x86 __asm("movl $to_fp, %eax; jmpl %eax") : 0xB8 {to_fp} 0xFF 0xE0
      //x86_64 __asm("movq $to_fp, %rax; jmpq %rax") : 0x48 0xB8 {to_fp} 0xFF 0xE0
#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)
      long long delta = (long long)to_fp - (long long)be_fp;
#   if defined(__x86_64__) || defined(_M_X64)
      if (delta < (int)(-2147483647 - 1) || (int)2147483647 < delta) {
         *I++ = 0x48;
         *I++ = 0xB8;
         memcpy(I, &to_fp, sizeof(void*));
         I += sizeof(void*);
         *I++ = 0xFF;
         *I++ = 0xE0;
         return;
      }
#   endif

      int offset = delta - 5;
      *I++ = 0xE9;
      memcpy(I, (void*)&offset, sizeof(offset));

#elif defined(__powerpc__)
#else
#endif
   }
   void restore()
   {
      memcpy(be_fp, saved_code, sizeof(saved_code));
   }
};

#endif

#ifndef ___H2_MFP__H___
#define ___H2_MFP__H___

/* clang-format off */

template <typename...>
using h2_void_t = void;

template <typename T, int I>
struct h2_constructible_error
{ static T* O(void *o) { throw I; return (T*)o; } };

template <typename, typename T>
struct h2_constructible0_impl : std::false_type { };
template <typename T>
struct h2_constructible0_impl<h2_void_t<decltype(T())>, T> : std::true_type
{ static T* O(void* o) { return new (o) T(); } };

template <typename, typename T>
struct h2_constructible1_impl : std::false_type { };
template <typename T>
struct h2_constructible1_impl<h2_void_t<decltype(T({}))>, T> : std::true_type
{ static T* O(void* o) { return new (o) T({}); } };

template <typename, typename T>
struct h2_constructible2_impl : std::false_type { };
template <typename T>
struct h2_constructible2_impl<h2_void_t<decltype(T({}, {}))>, T> : std::true_type
{ static T* O(void* o) { return new (o) T({}, {}); } };

template <typename, typename T>
struct h2_constructible3_impl : std::false_type { };
template <typename T>
struct h2_constructible3_impl<h2_void_t<decltype(T({}, {}, {}))>, T> : std::true_type
{ static T* O(void* o) { return new (o) T({}, {}, {}); } };

template <typename, typename T>
struct h2_constructible4_impl : std::false_type { };
template <typename T>
struct h2_constructible4_impl<h2_void_t<decltype(T({}, {}, {}, {}))>, T> : std::true_type
{ static T* O(void* o) { return new (o) T({}, {}, {}, {}); } };

template <typename, typename T>
struct h2_constructible5_impl : std::false_type { };
template <typename T>
struct h2_constructible5_impl<h2_void_t<decltype(T({}, {}, {}, {}, {}))>, T> : std::true_type
{ static T* O(void* o) { return new (o) T({}, {}, {}, {}, {}); } };


template <typename T>
using h2_constructible0 = h2_constructible0_impl<h2_void_t<>, T>;
template <typename T>
using h2_constructible1 = h2_constructible1_impl<h2_void_t<>, T>;
template <typename T>
using h2_constructible2 = h2_constructible2_impl<h2_void_t<>, T>;
template <typename T>
using h2_constructible3 = h2_constructible3_impl<h2_void_t<>, T>;
template <typename T>
using h2_constructible4 = h2_constructible4_impl<h2_void_t<>, T>;
template <typename T>
using h2_constructible5 = h2_constructible5_impl<h2_void_t<>, T>;


template <typename T>
using h2_constructible = 
  typename std::conditional<
    h2_constructible0<T>::value,
    h2_constructible0<T>,
    typename std::conditional<
      h2_constructible1<T>::value,
      h2_constructible1<T>,
      typename std::conditional<
        h2_constructible2<T>::value,
        h2_constructible2<T>,
        typename std::conditional<
          h2_constructible3<T>::value,
          h2_constructible3<T>,
          typename std::conditional<
            h2_constructible4<T>::value,
            h2_constructible4<T>,
            typename std::conditional<
              h2_constructible5<T>::value,
              h2_constructible5<T>,
              typename std::conditional<
                std::is_abstract<T>::value,
                h2_constructible_error<T, 2>,
                h2_constructible_error<T, 1>
              >::type
            >::type
          >::type
        >::type
      >::type
    >::type
  >::type;

/* clang-format on */

/*    
    https://itanium-cxx-abi.github.io/cxx-abi/

    &Class::Method has separate representations for non-virtual and virtual functions.
    For non-virtual functions, it is the address of the function.  
    For virtual functions, it is 1 plus the virtual table offset (in bytes) of the function.
    The least-significant bit therefore discriminates between virtual and non-virtual functions.
*/

static inline void h2_fail_g(h2_fail* fail);

template <typename Class, typename F>
struct h2_mfp;

template <typename Class, typename Return, typename... Args>
struct h2_mfp<Class, Return(Args...)>
{
   typedef Return (Class::*mfp_type)(Args...);
   typedef union
   {
      mfp_type f;
      void* p;
      intptr_t v;
   } cast_type;

   static inline bool is_virtual(cast_type& u)
   {
      return (u.v & 1) &&
             (u.v - 1) % sizeof(void*) == 0 &&
             (u.v - 1) / sizeof(void*) < 1000;
   }

   static inline void* get_vmfp(cast_type& u, Class* obj)
   {
      void** vtable = *(void***)obj;
      return vtable[(u.v - 1) / sizeof(void*)];
   }

   static void* A(mfp_type f,
                  const char* action_type,
                  const char* return_type,
                  const char* class_type,
                  const char* method_name,
                  const char* return_args,
                  const char* file,
                  int line)
   {
      cast_type u{f};

      if (is_virtual(u)) {
         void* p = nullptr;
         void* o = h2_alloc::I().malloc(sizeof(Class));
         try {
            p = get_vmfp(u, h2_constructible<Class>::O(o));
         }
         catch (int e) {
            h2_alloc::I().free(o);
            h2_fail_instantiate* fail = new h2_fail_instantiate(file, line);
            fail->add(action_type, return_type, class_type, method_name, return_args, e);
            h2_fail_g(fail);
         }
         h2_alloc::I().free(o);
         return p;
      }
      else {
         return u.p;
      }
   }

   template <typename Derived>
   static void* A(mfp_type f, Derived obj)
   {
      cast_type u{f};

      if (is_virtual(u)) {
         return get_vmfp(u, dynamic_cast<Class*>(&obj));
      }
      else {
         return u.p;
      }
   }
};

#endif

#ifndef ___H2_MATCHER__H___
#define ___H2_MATCHER__H___

template <typename T>
class h2_matcher_mpl
{
 public:
   virtual h2_fail* matches(T a, bool caseless = false, bool dont = false) const = 0;

   virtual ~h2_matcher_mpl() {}
};

template <typename T>
class h2_matcher
{
 private:
   const h2_matcher_mpl<const T&>* impl_;

 public:
   explicit h2_matcher() {}

   h2_matcher(const h2_matcher_mpl<const T&>* _impl, const int placeholder)
   {
      impl_ = _impl;
   }

   h2_matcher(const h2_matcher&) = default;
   h2_matcher& operator=(const h2_matcher&) = default;
   h2_matcher(h2_matcher&&) = default;
   h2_matcher& operator=(h2_matcher&&) = default;

   virtual ~h2_matcher() {}

 public:
   h2_fail* matches(const T& a, bool caseless = false, bool dont = false) const
   {
      return impl_->matches(a, caseless, dont);
   }

   h2_matcher(T value);
   h2_matcher(std::function<bool(T)> f);
};

template <typename T, typename F>
class h2_lambda_matcher : public h2_matcher_mpl<T>
{
 private:
   F f_;

 public:
   h2_lambda_matcher(F f)
     : f_(f) {}
   h2_fail* matches(T a, bool caseless = false, bool dont = false) const
   {
      bool result = f_(a);
      if (result) {
         return nullptr;
      }
      h2_fail_normal* fail = new h2_fail_normal();
      fail->kprintf("lambda failed");
      return fail;
   }

   static void* operator new(std::size_t sz)
   {
      return h2_alloc::I().malloc(sz);
   }
   static void operator delete(void* ptr)
   {
      h2_alloc::I().free(ptr);
   }
};

template <typename T>
inline h2_matcher<T>::h2_matcher(std::function<bool(T)> f)
{
   *this = h2_matcher<T>(new h2_lambda_matcher<const T&, std::function<bool(T)>>(f), 0);
}

template <typename Matches>
class h2_polymorphic_matcher
{
 public:
   explicit h2_polymorphic_matcher(const Matches& _matches)
     : matches_(_matches) {}

   template <typename T>
   operator h2_matcher<T>() const
   {
      return h2_matcher<T>(new internal_impl<const T&>(matches_), 0);
   }

 private:
   template <typename T>
   class internal_impl : public h2_matcher_mpl<T>
   {
    public:
      explicit internal_impl(const Matches& _matches)
        : matches_(_matches) {}

      h2_fail* matches(T a, bool caseless = false, bool dont = false) const override
      {
         return matches_.matches(a, caseless, dont);
      }

      static void* operator new(std::size_t sz)
      {
         return h2_alloc::I().malloc(sz);
      }
      static void operator delete(void* ptr)
      {
         h2_alloc::I().free(ptr);
      }

    private:
      const Matches matches_;
   };

   Matches matches_;
};

template <typename Matches>
inline h2_polymorphic_matcher<Matches> MakePolymorphicMatcher(const Matches& matches)
{
   return h2_polymorphic_matcher<Matches>(matches);
}

template <typename E>
class h2_eq_matches
{
 public:
   explicit h2_eq_matches(const E& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = a == e_;
      if (result == !dont)
         return nullptr;
      else
         return desc(a, dont);
   }
   // template <>
   h2_fail* matches(const float a, bool caseless = false, bool dont = false) const
   {
      bool result = std::fabs(a - e_) < 0.00001;
      if (result == !dont)
         return nullptr;
      else
         return desc(a, dont);
   }
   // template <>
   h2_fail* matches(const double a, bool caseless = false, bool dont = false) const
   {
      bool result = std::fabs(a - e_) < 0.00001;
      if (result == !dont)
         return nullptr;
      else
         return desc(a, dont);
   }
   // template <>
   h2_fail* matches(const long double a, bool caseless = false, bool dont = false) const
   {
      bool result = std::fabs(a - e_) < 0.00001;
      if (result == !dont)
         return nullptr;
      else
         return desc(a, dont);
   }
   // template <>
   h2_fail* matches(char* a, bool caseless = false, bool dont = false) const
   {
      return matches(h2_string(a), caseless, dont);
   }
   // template <>
   h2_fail* matches(const char* a, bool caseless = false, bool dont = false) const
   {
      return matches(h2_string(a), caseless, dont);
   }
   // template <>
   h2_fail* matches(const h2_string a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a), e2(e_);
      if (caseless) {
         for (auto& c : a2) { c = tolower(c); }
         for (auto& c : e2) { c = tolower(c); }
      }
      bool result = e2 == a2;
      if (result == !dont) return nullptr;
      if (dont) {
         h2_fail_unexpect* fail = new h2_fail_unexpect();
         fail->eprintf("\"%s\"", h2_acronym_string(h2_string(e_).c_str()));
         fail->aprintf("\"%s\"", h2_acronym_string(a.c_str()));
         fail->mprintf("should not %sequal to", caseless ? "caseless " : "");
         return fail;
      }
      else {
         if (h2_string(e_).length() < 35 && a.length() < 35) {
            h2_fail_unexpect* fail = new h2_fail_unexpect();
            fail->eprintf("\"%s\"", h2_string(e_).c_str());
            fail->aprintf("\"%s\"", a.c_str());
            fail->mprintf("not %sequal to", caseless ? "caseless " : "");
            return fail;
         }
         else {
            h2_fail_strcmp* fail = new h2_fail_strcmp();
            fail->add(e_, a, caseless);
            return fail;
         }
      }
   }

   template <typename A>
   h2_fail* desc(A a, bool dont) const
   {
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      h2_ostringstream ose, osa;
      ose << std::boolalpha << e_;
      osa << std::boolalpha << a;
      fail->eprintf("%s", ose.str().c_str());
      fail->aprintf("%s", osa.str().c_str());
      if (dont) {
         fail->mprintf("should not equal to");
      }
      else {
         fail->mprintf("not equal to");
      }
      return fail;
   }

 private:
   E e_;
};

template <typename T>
inline h2_matcher<T>::h2_matcher(T value)
{
   *this = MakePolymorphicMatcher(h2_eq_matches<T>(value));
}

// This general version is used when MatcherCast()'s argument is a
// polymorphic matcher (i.e. something that can be converted to a
// Matcher but is not one yet; for example, Eq(value)) or a value (for
// example, "hello").
template <typename T, typename M>
class MatcherCastImpl
{
 public:
   static h2_matcher<T> Cast(const M& polymorphic_matcher_or_value)
   {
      // M can be a polymorphic matcher, in which case we want to use
      // its conversion operator to create Matcher<T>.  Or it can be a value
      // that should be passed to the Matcher<T>'s constructor.
      //
      // We can't call Matcher<T>(polymorphic_matcher_or_value) when M is a
      // polymorphic matcher because it'll be ambiguous if T has an implicit
      // constructor from M (this usually happens when T has an implicit
      // constructor from any type).
      //
      // It won't work to unconditionally implict_cast
      // polymorphic_matcher_or_value to Matcher<T> because it won't trigger
      // a user-defined conversion from M to T if one exists (assuming M is
      // a value).
      return CastImpl(polymorphic_matcher_or_value,
                      std::is_convertible<M, h2_matcher<T>>{},
                      std::is_convertible<M, T>{});
   }

 private:
   template <bool Ignore>
   static h2_matcher<T> CastImpl(const M& polymorphic_matcher_or_value,
                                 std::true_type /* convertible_to_matcher */,
                                 std::integral_constant<bool, Ignore>)
   {
      // M is implicitly convertible to Matcher<T>, which means that either
      // M is a polymorphic matcher or Matcher<T> has an implicit constructor
      // from M.  In both cases using the implicit conversion will produce a
      // matcher.
      //
      // Even if T has an implicit constructor from M, it won't be called because
      // creating Matcher<T> would require a chain of two user-defined conversions
      // (first to create T from M and then to create Matcher<T> from T).
      return polymorphic_matcher_or_value;
   }

   template <typename To>
   static To ImplicitCast_(To x)
   {
      return x;
   }

   // M can't be implicitly converted to Matcher<T>, so M isn't a polymorphic
   // matcher. It's a value of a type implicitly convertible to T. Use direct
   // initialization to create a matcher.
   static h2_matcher<T> CastImpl(const M& value,
                                 std::false_type /* convertible_to_matcher */,
                                 std::true_type /* convertible_to_T */)
   {
      return h2_matcher<T>(ImplicitCast_<T>(value));
   }

   // M can't be implicitly converted to either Matcher<T> or T. Attempt to use
   // polymorphic matcher Eq(value) in this case.
   //
   // Note that we first attempt to perform an implicit cast on the value and
   // only fall back to the polymorphic Eq() matcher afterwards because the
   // latter calls bool operator==(const Lhs& lhs, const Rhs& rhs) in the end
   // which might be undefined even when Rhs is implicitly convertible to Lhs
   // (e.g. std::pair<const int, int> vs. std::pair<int, int>).
   //
   static h2_matcher<T> CastImpl(const M& value,
                                 std::false_type /* convertible_to_matcher */,
                                 std::false_type /* convertible_to_T */)
   {
      return MakePolymorphicMatcher(h2_eq_matches<M>(value));
   }
};

// This more specialized version is used when MatcherCast()'s argument
// is already a Matcher.  This only compiles when type T can be
// statically converted to type U.
template <typename T, typename U>
class MatcherCastImpl<T, h2_matcher<U>>
{
 public:
   static h2_matcher<T> Cast(const h2_matcher<U>& source_matcher)
   {
      return h2_matcher<T>(new Impl(source_matcher));
   }

 private:
   class Impl : public h2_matcher_mpl<T>
   {
    public:
      explicit Impl(const h2_matcher<U>& source_matcher)
        : source_matcher_(source_matcher) {}

      // We delegate the matching logic to the source matcher.
      h2_fail* matches(T x, bool caseless, bool dont) const override
      {
         using FromType = typename std::remove_cv<typename std::remove_pointer<
           typename std::remove_reference<T>::type>::type>::type;
         using ToType = typename std::remove_cv<typename std::remove_pointer<
           typename std::remove_reference<U>::type>::type>::type;
         // Do not allow implicitly converting base*/& to derived*/&.
         static_assert(
           // Do not trigger if only one of them is a pointer. That implies a
           // regular conversion and not a down_cast.
           (std::is_pointer<typename std::remove_reference<T>::type>::value !=
            std::is_pointer<typename std::remove_reference<U>::type>::value) ||
             std::is_same<FromType, ToType>::value ||
             !std::is_base_of<FromType, ToType>::value,
           "Can't implicitly convert from <base> to <derived>");

         return source_matcher_.matches(static_cast<U>(x), caseless, dont);
      }

      static void* operator new(std::size_t sz)
      {
         return h2_alloc::I().malloc(sz);
      }
      static void operator delete(void* ptr)
      {
         h2_alloc::I().free(ptr);
      }

    private:
      const h2_matcher<U> source_matcher_;

      void operator=(Impl const&) = delete;
   };
};

// This even more specialized version is used for efficiently casting
// a matcher to its own type.
template <typename T>
class MatcherCastImpl<T, h2_matcher<T>>
{
 public:
   static h2_matcher<T> Cast(const h2_matcher<T>& matcher)
   {
      return matcher;
   }
};

// In order to be safe and clear, casting between different matcher
// types is done explicitly via MatcherCast<T>(m), which takes a
// matcher m and returns a Matcher<T>.  It compiles only when T can be
// statically converted to the argument type of m.
template <typename T, typename M>
inline h2_matcher<T> MatcherCast(const M& matcher)
{
   return MatcherCastImpl<T, M>::Cast(matcher);
}

struct h2_any_matches
{
   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      return nullptr;
   }
};

struct h2_null_matches
{
   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = nullptr == reinterpret_cast<const void*>(a);
      if (result == !dont) return nullptr;
      h2_fail_normal* fail = new h2_fail_normal();
      if (dont) {
         fail->kprintf("shoud not be null");
      }
      else {
         fail->kprintf("is not null %p", reinterpret_cast<const void*>(a));
      }
      return fail;
   }
};

template <typename E>
class h2_ge_matches
{
 public:
   explicit h2_ge_matches(const E& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = a >= e_;
      if (result == !dont) return nullptr;
      h2_ostringstream ose, osa;
      ose << e_;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("%s", ose.str().c_str());
      fail->aprintf("%s", osa.str().c_str());
      if (dont) {
         fail->mprintf("shoud >=");
      }
      else {
         fail->mprintf("not >=");
      }
      return fail;
   }

 private:
   E e_;
};

template <typename E>
class h2_gt_matches
{
 public:
   explicit h2_gt_matches(const E& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = a > e_;
      if (result == !dont) return nullptr;
      h2_ostringstream ose, osa;
      ose << e_;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("%s", ose.str().c_str());
      fail->aprintf("%s", osa.str().c_str());
      if (dont) {
         fail->mprintf("shoud >");
      }
      else {
         fail->mprintf("not >");
      }
      return fail;
   }

 private:
   E e_;
};

template <typename E>
class h2_le_matches
{
 public:
   explicit h2_le_matches(const E& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = a <= e_;
      if (result == !dont) return nullptr;
      h2_ostringstream ose, osa;
      ose << e_;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("%s", ose.str().c_str());
      fail->aprintf("%s", osa.str().c_str());
      if (dont) {
         fail->mprintf("shoud <=");
      }
      else {
         fail->mprintf("not <=");
      }
      return fail;
   }

 private:
   E e_;
};

template <typename E>
class h2_lt_matches
{
 public:
   explicit h2_lt_matches(const E& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = a < e_;
      if (result == !dont) return nullptr;
      h2_ostringstream ose, osa;
      ose << e_;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("%s", ose.str().c_str());
      fail->aprintf("%s", osa.str().c_str());
      if (dont) {
         fail->mprintf("shoud <");
      }
      else {
         fail->mprintf("not <");
      }
      return fail;
   }

 private:
   E e_;
};

class h2_me_matches
{
 public:
   explicit h2_me_matches(const void* e, const int size)
     : e_(e), size_(size)
   {
      if (size == 0) {
         size_ = strlen((const char*)e);
      }
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = memcmp(e_, (const void*)a, size_) == 0;
      if (result == !dont) return nullptr;
      h2_fail_memcmp* fail = new h2_fail_memcmp();
      fail->add(e_, (const void*)a, size_);
      return fail;
   }

 private:
   const void* e_;
   int size_;
};

template <typename M>
class h2_pe_matches
{
 public:
   explicit h2_pe_matches(M matcher)
     : inner_matcher_(matcher) {}

   // PointeeOf<Pointer>::type is the type of a value pointed to by a
   // Pointer, which can be either a smart pointer or a raw pointer.
   template <typename Pointer>
   struct PointeeOf
   {
      // Smart pointer classes define type element_type as the type of
      // their pointees.
      typedef typename Pointer::element_type type;
   };
   // This specialization is for the raw pointer case.
   template <typename T>
   struct PointeeOf<T*>
   {
      typedef T type;
   };

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      typedef typename std::remove_const<typename std::remove_reference<A>::type>::type Pointer;
      typedef typename PointeeOf<Pointer>::type Pointee;

      return MatcherCast<Pointee>(inner_matcher_).matches(*a, caseless, dont);
   }

 private:
   M inner_matcher_;
};

class h2_regex_matches
{
 public:
   explicit h2_regex_matches(const h2_string& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(const A& a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a);
      bool result;
      try {
         std::regex re(e_);
         result = std::regex_match(a2, re);
      }
      catch (const std::regex_error& e) {
         result = false;
      }
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("/%s/", e_.c_str());
      fail->aprintf("\"%s\"", a2.c_str());
      if (dont) {
         fail->mprintf("shoud matches Regex");
      }
      else {
         fail->mprintf("not matches Regex");
      }
      return fail;
   }

 private:
   const h2_string e_;
};

class h2_wildcard_matches
{
 public:
   explicit h2_wildcard_matches(const h2_string& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a);
      bool result = h2_wildcard_match(e_.c_str(), a2.c_str());
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("/%s/", e_.c_str());
      fail->aprintf("\"%s\"", a2.c_str());
      if (dont) {
         fail->mprintf("shoud matches Wildcard");
      }
      else {
         fail->mprintf("not matches Wildcard");
      }
      return fail;
   }

 private:
   const h2_string e_;
};

class h2_hassubstr_matches
{
 public:
   explicit h2_hassubstr_matches(const h2_string& substring)
     : substring_(substring) {}

   // Accepts pointer types, particularly:
   //   const char*
   //   char*
   //   const wchar_t*
   //   wchar_t*

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a), e2(substring_);
      if (caseless) {
         for (auto& c : a2) { c = tolower(c); }
         for (auto& c : e2) { c = tolower(c); }
      }
      bool result = a2.find(e2) != h2_string::npos;
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", substring_.c_str());
      fail->aprintf("\"%s\"", h2_string(a).c_str());
      if (dont) {
         fail->mprintf("shoud not %shas substr", caseless ? "caseless " : "");
      }
      else {
         fail->mprintf("not %shas substr", caseless ? "caseless " : "");
      }
      return fail;
   }

 private:
   const h2_string substring_;
};

class h2_startswith_matches
{
 public:
   explicit h2_startswith_matches(const h2_string& prefix_string)
     : prefix_string_(prefix_string) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a), e2(prefix_string_);
      if (caseless) {
         for (auto& c : a2) { c = tolower(c); }
         for (auto& c : e2) { c = tolower(c); }
      }

      bool result = a2.length() >= e2.length() && a2.substr(0, e2.length()) == e2;
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", prefix_string_.c_str());
      fail->aprintf("\"%s\"", h2_string(a).c_str());
      if (dont) {
         fail->mprintf("shoud not %sstarts with", caseless ? "caseless " : "");
      }
      else {
         fail->mprintf("not %sstarts with", caseless ? "caseless " : "");
      }
      return fail;
   }

 private:
   const h2_string prefix_string_;
};

class h2_endswith_matches
{
 public:
   explicit h2_endswith_matches(const h2_string& suffix_string)
     : suffix_string_(suffix_string) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      h2_string a2(a), e2(suffix_string_);
      if (caseless) {
         for (auto& c : a2) { c = tolower(c); }
         for (auto& c : e2) { c = tolower(c); }
      }
      bool result = a2.length() >= e2.length() && a2.substr(a2.length() - e2.length()) == e2;
      if (result == !dont) return nullptr;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->eprintf("\"%s\"", suffix_string_.c_str());
      fail->aprintf("\"%s\"", h2_string(a).c_str());
      if (dont) {
         fail->mprintf("shoud not %sends with", caseless ? "caseless " : "");
      }
      else {
         fail->mprintf("not %sends with", caseless ? "caseless " : "");
      }
      return fail;
   }

 private:
   const h2_string suffix_string_;
};

class h2_je_matches
{
 public:
   explicit h2_je_matches(const h2_string& e)
     : e_(e) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      bool result = h2json::match(e_.c_str(), (const char*)a);
      if (result == !dont) return nullptr;
      h2_fail_json* fail = new h2_fail_json();
      if (dont) {
         fail->kprintf("JSON should not equals");
      }
      else {
         fail->kprintf("JSON not equals");
      }
      fail->add(e_.c_str(), (const char*)a);
      return fail;
   }

 private:
   const h2_string e_;
};

template <typename M>
class h2_caseless_matches
{
 public:
   explicit h2_caseless_matches(M matcher)
     : inner_matcher_(matcher) {}

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      return MatcherCast<const h2_string>(inner_matcher_).matches(a, true, dont);
   }

 private:
   M inner_matcher_;
};

template <typename M>
class h2_not_matches
{
 public:
   explicit h2_not_matches(M matcher)
     : inner_matcher_(matcher) {}
   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      return MatcherCast<const A&>(inner_matcher_).matches(a, caseless, !dont);
   }

 private:
   M inner_matcher_;
};

template <typename... Matchers>
class h2_allof_matches
{
 public:
   explicit h2_allof_matches(const Matchers&... matchers)
     : matchers_(matchers...)
   {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      std::vector<h2_matcher<A>, h2_allocator<h2_matcher<A>>> matchers;
      tuple_to_vector_matcher<A>(&matchers, std::integral_constant<size_t, 0>());

      h2_fail* fail = nullptr;
      for (auto i = matchers.begin(); i != matchers.end(); i++) {
         auto m = *i;
         auto f = m.matches(a, caseless, false);
         if (f) {
            if (!fail) {
               fail = f;
            }
            else {
               fail->appendy(f);
            }
         }
      }
      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>* matchers, std::integral_constant<size_t, I>) const
   {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

template <typename... Matchers>
class h2_anyof_matches
{
 public:
   explicit h2_anyof_matches(const Matchers&... matchers)
     : matchers_(matchers...)
   {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      std::vector<h2_matcher<A>, h2_allocator<h2_matcher<A>>> matchers;
      tuple_to_vector_matcher<A>(&matchers, std::integral_constant<size_t, 0>());

      for (auto i = matchers.begin(); i != matchers.end(); i++) {
         auto m = *i;
         auto f = m.matches(a, caseless, false);
         if (!f) {
            return nullptr;
         }
      }
      h2_ostringstream osa;
      osa << a;
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf("\"%s\"", osa.str().c_str());
      if (dont) {
         //TODO
      }
      else {
         fail->mprintf("not matches any of");
      }
      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>* matchers, std::integral_constant<size_t, I>) const
   {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

template <typename... Matchers>
class h2_noneof_matches
{
 public:
   explicit h2_noneof_matches(const Matchers&... matchers)
     : matchers_(matchers...)
   {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      std::vector<h2_matcher<A>, h2_allocator<h2_matcher<A>>> matchers;
      tuple_to_vector_matcher<A>(&matchers, std::integral_constant<size_t, 0>());

      h2_fail* fail = nullptr;
      for (auto i = matchers.begin(); i != matchers.end(); i++) {
         auto m = *i;
         auto f = m.matches(a, caseless, true);
         if (f) {
            if (!fail) {
               fail = f;
            }
            else {
               fail->appendy(f);
            }
         }
      }
      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>* matchers, std::integral_constant<size_t, I>) const
   {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

template <typename... Matchers>
class h2_listof_matches
{
 public:
   explicit h2_listof_matches(const Matchers&... matchers)
     : matchers_(matchers...)
   {
      static_assert(sizeof...(Matchers) > 0, "Must have at least one matcher.");
   }

   template <typename A>
   h2_fail* matches(A a, bool caseless = false, bool dont = false) const
   {
      typedef decltype(a[0]) B;
      std::vector<h2_matcher<B>, h2_allocator<h2_matcher<B>>> matchers;
      tuple_to_vector_matcher<B>(&matchers, std::integral_constant<size_t, 0>());

      h2_fail* fail = nullptr;
      for (int i = 0; i < (int)matchers.size(); ++i) {
         auto f = matchers[i].matches(a[i], caseless, false);
         if (f) {
            if (!fail) {
               fail = f;
            }
            else {
               fail->appendy(f);
            }
         }
      }
      return fail;
   }

   template <typename T, size_t I>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>* matchers, std::integral_constant<size_t, I>) const
   {
      matchers->push_back(MatcherCast<T>(std::get<I>(matchers_)));
      tuple_to_vector_matcher<T>(matchers, std::integral_constant<size_t, I + 1>());
   }

   template <typename T>
   void tuple_to_vector_matcher(std::vector<h2_matcher<T>, h2_allocator<h2_matcher<T>>>*, std::integral_constant<size_t, sizeof...(Matchers)>) const {}

 private:
   std::tuple<Matchers...> matchers_;
};

/* clang-format off */

            const h2_polymorphic_matcher<h2_any_matches> 
_                                   { h2_any_matches() };
            inline h2_polymorphic_matcher<h2_any_matches> 
Any()                               { return MakePolymorphicMatcher(h2_any_matches()); }
            inline h2_polymorphic_matcher<h2_null_matches> 
Null()                              { return MakePolymorphicMatcher(h2_null_matches()); }
            template <typename E> inline h2_polymorphic_matcher<h2_eq_matches<E>> 
Eq(const E expect)                  { return MakePolymorphicMatcher(h2_eq_matches<E>(expect)); }
            template <typename E> inline h2_polymorphic_matcher<h2_ge_matches<E>> 
Ge(const E expect)                  { return MakePolymorphicMatcher(h2_ge_matches<E>(expect)); }
            template <typename E> inline h2_polymorphic_matcher<h2_gt_matches<E>> 
Gt(const E expect)                  { return MakePolymorphicMatcher(h2_gt_matches<E>(expect)); }
            template <typename E> inline h2_polymorphic_matcher<h2_le_matches<E>> 
Le(const E expect)                  { return MakePolymorphicMatcher(h2_le_matches<E>(expect)); }
            template <typename E> inline h2_polymorphic_matcher<h2_lt_matches<E>> 
Lt(const E expect)                  { return MakePolymorphicMatcher(h2_lt_matches<E>(expect)); }
            inline h2_polymorphic_matcher<h2_me_matches> 
Me(const void *buf, const int size = 0)   { return MakePolymorphicMatcher(h2_me_matches(buf, size)); }
            template <typename Matcher_or_Value> inline h2_polymorphic_matcher<h2_pe_matches<Matcher_or_Value>> 
Pe(Matcher_or_Value expect)               { return MakePolymorphicMatcher(h2_pe_matches<Matcher_or_Value>(expect)); }
            template <typename Matcher_or_Value> inline h2_polymorphic_matcher<h2_pe_matches<Matcher_or_Value>> 
PointeeEq(Matcher_or_Value expect)        { return MakePolymorphicMatcher(h2_pe_matches<Matcher_or_Value>(expect)); }
            inline h2_polymorphic_matcher<h2_regex_matches> 
Re(const h2_string& regex_pattern)        { return MakePolymorphicMatcher(h2_regex_matches(regex_pattern)); }
            inline h2_polymorphic_matcher<h2_wildcard_matches> 
We(const h2_string& wildcard_pattern)     { return MakePolymorphicMatcher(h2_wildcard_matches(wildcard_pattern)); }
            inline h2_polymorphic_matcher<h2_hassubstr_matches> 
HasSubstr(const h2_string& substring)     { return MakePolymorphicMatcher(h2_hassubstr_matches(substring)); }
            inline h2_polymorphic_matcher<h2_startswith_matches> 
StartsWith(const h2_string& prefix_string)  { return MakePolymorphicMatcher(h2_startswith_matches(prefix_string)); }
            inline h2_polymorphic_matcher<h2_endswith_matches> 
EndsWith(const h2_string& suffix_string)    { return MakePolymorphicMatcher(h2_endswith_matches(suffix_string)); }
            template <typename Matcher_or_String> inline h2_polymorphic_matcher<h2_caseless_matches<Matcher_or_String>> 
CaseLess(Matcher_or_String expect)          { return MakePolymorphicMatcher(h2_caseless_matches<Matcher_or_String>(expect)); }
            inline h2_polymorphic_matcher<h2_je_matches> 
JsonEq(const h2_string& expect)   { return MakePolymorphicMatcher(h2_je_matches(expect)); }
            inline h2_polymorphic_matcher<h2_je_matches> 
Je(const h2_string& expect)       { return MakePolymorphicMatcher(h2_je_matches(expect)); }
            template <typename Matcher_or_Value> inline h2_polymorphic_matcher<h2_not_matches<Matcher_or_Value>> 
Not(Matcher_or_Value matcher)     { return MakePolymorphicMatcher(h2_not_matches<Matcher_or_Value>(matcher)); }
            template <typename... Matchers> inline h2_polymorphic_matcher<h2_allof_matches<typename std::decay<const Matchers&>::type...>> 
AllOf(const Matchers&... matchers)    { return MakePolymorphicMatcher(h2_allof_matches<typename std::decay<const Matchers&>::type...>(matchers...)); }
            template <typename... Matchers> inline h2_polymorphic_matcher<h2_anyof_matches<typename std::decay<const Matchers&>::type...>> 
AnyOf(const Matchers&... matchers)    { return MakePolymorphicMatcher(h2_anyof_matches<typename std::decay<const Matchers&>::type...>(matchers...)); }
            template <typename... Matchers> inline h2_polymorphic_matcher<h2_noneof_matches<typename std::decay<const Matchers&>::type...>> 
NoneOf(const Matchers&... matchers)   { return MakePolymorphicMatcher(h2_noneof_matches<typename std::decay<const Matchers&>::type...>(matchers...)); }
            template <typename... Matchers> inline h2_polymorphic_matcher<h2_listof_matches<typename std::decay<const Matchers&>::type...>> 
ListOf(const Matchers&... matchers)   { return MakePolymorphicMatcher(h2_listof_matches<typename std::decay<const Matchers&>::type...>(matchers...)); }

/* clang-format on */

#endif

#ifndef ___H2_COUNT__H___
#define ___H2_COUNT__H___

#define h2_count_between(_min, _max) h2_count(_min, _max)
#define h2_count_atleast(n) h2_count(n, INT_MAX)
#define h2_count_atmost(n) h2_count(0, n)
#define h2_count_any() h2_count(0, INT_MAX)
#define h2_count_exactly(n) h2_count(n, n)

class h2_count
{
 private:
   int call_;
   int min_, max_;

 public:
   h2_count(int min, int max)
     : call_(0), min_(min), max_(max) {}

   void inc()
   {
      call_ += 1;
   }

   bool is_shortage(/*不够*/)
   {
      return call_ < min_;
   }
   bool is_satisfied(/*满足*/)
   {
      return min_ <= call_ && call_ <= max_;
   }
   bool is_saturated(/*饱和*/)
   {
      return call_ == max_;
   }
   bool is_overfill(/*过多*/)
   {
      return max_ < call_;
   }

   h2_fail* double_check()
   {
      if (is_satisfied() || is_saturated()) {
         return nullptr;
      }
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->hamet("", actual(), "called but expect", expect(), "");
      return fail;
   }

   const char* actual()
   {
      static char t[64];
      if (call_ > 0) {
         sprintf(t, "%d times", call_);
      }
      else {
         sprintf(t, "never");
      }
      return t;
   }

   const char* expect()
   {
      static char t[128];
      if (min_ == 0) {
         if (max_ == 0) {
            sprintf(t, "never called");
         }
         else if (max_ == INT_MAX) {
            sprintf(t, "any number of times");
         }
         else {
            sprintf(t, "at most %d times", max_);
         }
      }
      else if (min_ == max_) {
         sprintf(t, "exactly %d times", min_);
      }
      else if (max_ == INT_MAX) {
         sprintf(t, "at least %d times", min_);
      }
      else {
         // 0 < min_ < max_ < INT_MAX
         sprintf(t, "between %d and %d times", min_, max_);
      }

      return t;
   }
};

#endif

#ifndef ___H2_RETURN__H___
#define ___H2_RETURN__H___

template <typename Class, typename F>
class h2_return;

template <typename Class, typename Return, typename... Args>
class h2_return<Class, Return(Args...)>
{
 private:
   Return _r;
   std::function<Return(Args...)> _f1;
   std::function<Return(Class*, Args...)> _f2;

 public:
   h2_return()
     : _r(), _f1(), _f2() {}
   h2_return(Return r)
     : _r(r), _f1(), _f2() {}
   h2_return(std::function<Return(Args...)> f)
     : _f1(f) {}
   h2_return(std::function<Return(Class*, Args...)> f)
     : _f2(f) {}

   Return operator()(Class* that, Args... args)
   {
      if (_f2)
         return _f2(that, args...);
      else if (_f1)
         return _f1(args...);
      else
         return _r;
   }
};

template <typename Class, typename... Args>
class h2_return<Class, void(Args...)>
{
 private:
   std::function<void(Args...)> _f1;
   std::function<void(Class*, Args...)> _f2;

 public:
   h2_return()
     : _f1(), _f2() {}
   h2_return(std::function<void(Args...)> f)
     : _f1(f) {}
   h2_return(std::function<void(Class*, Args...)> f)
     : _f2(f) {}

   void operator()(Class* that, Args... args)
   {
      if (_f2)
         _f2(that, args...);
      else if (_f1)
         _f1(args...);
      else
         ;
      return;
   }
};

#endif

#ifndef ___H2_MOCK__H___
#define ___H2_MOCK__H___

template <size_t I, typename T, typename... Args>
struct nth_type_impl
{
   using type = typename nth_type_impl<I - 1, Args...>::type;
};

template <typename T, typename... Args>
struct nth_type_impl<0, T, Args...>
{
   using type = T;
};

template <size_t Index, typename... Args>
using nth_type = typename nth_type_impl<Index, Args..., int, int, int, int, int, int, int, int, int, int, int, int, int, int>::type;

template <size_t Index, typename... Args>
using nth_type_decay = typename std::decay<nth_type<Index, Args...>>::type;

struct h2_mock
{
   void* _be_fp;
   const char* _be_fn;
   void* _to_fp;
   const char* _file;
   int _line;

   std::vector<h2_count, h2_allocator<h2_count>> c_array;
   int c_index;

   h2_mock(void* be_fp, const char* be_fn, const char* file, int line)
     : _be_fp(be_fp), _be_fn(be_fn), _file(file), _line(line), c_index(0) {}

   h2_fail* double_check()
   {
      h2_fail* fail = nullptr;
      for (auto it = c_array.begin(); it != c_array.end(); it++) {
         auto f = (*it).double_check();
         if (f) {
            f->locate(_file, _line, _be_fn);
            if (!fail) {
               fail = f;
            }
            else {
               fail->appendy(f);
            }
         }
      }
      return fail;
   }
};

static inline void h2_mock_g(h2_mock* mock);
static inline void h2_fail_g(h2_fail* fail);

template <size_t N>
struct TupleMatch
{
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& args)
   {
      h2_fail* fail1 = TupleMatch<N - 1>::matches(matchers, args);
      h2_fail* fail2 = std::get<N - 1>(matchers).matches(std::get<N - 1>(args));
      if (fail2) {
         fail2->argth(N - 1);
      }

      if (!fail1) {
         return fail2;
      }
      else {
         fail1->appendx(fail2);
         return fail1;
      }
   }
};

template <>
struct TupleMatch<0>
{
   template <typename MatcherTuple, typename ArgumentTuple>
   static h2_fail* matches(MatcherTuple& matchers, ArgumentTuple& args)
   {
      return nullptr;
   }
};

template <int Counter, int Lineno, typename Class, typename F>
class h2_mocker;

template <int Counter, int Lineno, typename Class, typename Return, typename... Args>
class h2_mocker<Counter, Lineno, Class, Return(Args...)> : h2_mock
{
 private:
   h2_mocker(void* be_fp, const char* be_fn, const char* file)
     : h2_mock(be_fp, be_fn, file, Lineno)
   {
      if (std::is_same<std::false_type, Class>::value) {
         _to_fp = (void*)&normal_function_stub;
      }
      else {
         _to_fp = (void*)&member_function_stub;
      }

      h2_mock_g(this);
   }

/* clang-format off */
#define __H2_MATCHER_TYPE_LIST             \
   h2_matcher<nth_type_decay<0, Args...>>, \
   h2_matcher<nth_type_decay<1, Args...>>, \
   h2_matcher<nth_type_decay<2, Args...>>, \
   h2_matcher<nth_type_decay<3, Args...>>, \
   h2_matcher<nth_type_decay<4, Args...>>, \
   h2_matcher<nth_type_decay<5, Args...>>, \
   h2_matcher<nth_type_decay<6, Args...>>, \
   h2_matcher<nth_type_decay<7, Args...>>, \
   h2_matcher<nth_type_decay<8, Args...>>, \
   h2_matcher<nth_type_decay<9, Args...>>

#define __H2_MATCHER_VARIABLE_LIST             \
   h2_matcher<nth_type_decay<0, Args...>> a_0; \
   h2_matcher<nth_type_decay<1, Args...>> a_1; \
   h2_matcher<nth_type_decay<2, Args...>> a_2; \
   h2_matcher<nth_type_decay<3, Args...>> a_3; \
   h2_matcher<nth_type_decay<4, Args...>> a_4; \
   h2_matcher<nth_type_decay<5, Args...>> a_5; \
   h2_matcher<nth_type_decay<6, Args...>> a_6; \
   h2_matcher<nth_type_decay<7, Args...>> a_7; \
   h2_matcher<nth_type_decay<8, Args...>> a_8; \
   h2_matcher<nth_type_decay<9, Args...>> a_9

#define __H2_MATCHER_PARAMETER_DEFAULT_LIST        \
   h2_matcher<nth_type_decay<0, Args...>> a_0 = _, \
   h2_matcher<nth_type_decay<1, Args...>> a_1 = _, \
   h2_matcher<nth_type_decay<2, Args...>> a_2 = _, \
   h2_matcher<nth_type_decay<3, Args...>> a_3 = _, \
   h2_matcher<nth_type_decay<4, Args...>> a_4 = _, \
   h2_matcher<nth_type_decay<5, Args...>> a_5 = _, \
   h2_matcher<nth_type_decay<6, Args...>> a_6 = _, \
   h2_matcher<nth_type_decay<7, Args...>> a_7 = _, \
   h2_matcher<nth_type_decay<8, Args...>> a_8 = _, \
   h2_matcher<nth_type_decay<9, Args...>> a_9 = _

#define __H2_MATCHER_ARGUMENT_LIST \
   a_0, a_1, a_2, a_3, a_4, a_5, a_6, a_7, a_8, a_9

   /* clang-format on */

   typedef std::tuple<Args..., int> h2_argument_tuple;
   typedef std::tuple<__H2_MATCHER_TYPE_LIST> h2_matcher_tuple;

   std::vector<h2_matcher_tuple, h2_allocator<h2_matcher_tuple>> m_array;
   std::vector<h2_return<Class, Return(Args...)>, h2_allocator<h2_return<Class, Return(Args...)>>> r_array;

   static Return normal_function_stub(Args... args)
   {
      int r_index = I().matches(std::move(args)...);
      return I().r_array[r_index](nullptr, std::move(args)...);
   }

   static Return member_function_stub(Class* that, Args... args)
   {
      int r_index = I().matches(std::move(args)...);
      return I().r_array[r_index](that, std::move(args)...);
   }

   h2_fail* matches(h2_matcher_tuple& matchers, h2_argument_tuple& args)
   {
      if (1 == std::tuple_size<h2_argument_tuple>::value) return nullptr;
      h2_fail* fail = TupleMatch<std::tuple_size<h2_argument_tuple>::value>::matches(matchers, args);
      if (fail) {
         fail->locate(_file, _line, _be_fn);
      }
      return fail;
   }

   int matches(Args... args)
   {
      h2_argument_tuple a_tuple = std::make_tuple(args..., 0);
      int offset = -1;
      for (int i = c_index; i < (int)c_array.size(); ++i) {
         h2_fail* fail = matches(m_array[i], a_tuple);
         if (fail) {
            if (c_array[i].is_shortage()) {
               h2_fail_g(fail);
            }
            if (c_array[i].is_satisfied()) {
               delete fail;
               continue; /* try next */
            }
         }
         else {
            offset = i;
            c_array[i].inc();
            if (c_array[i].is_saturated()) {
               c_index += 1;
            }
            break;
         }
      }
      if (-1 == offset) {
         h2_fail_normal* fail = new h2_fail_normal();
         fail->kprintf("unexpect call");
         fail->locate(_file, _line, _be_fn);
         h2_fail_g(fail);
      }
      return offset;
   }

 public:
   static h2_mocker& I(void* be_fp = nullptr, const char* be_fn = nullptr, const char* file = nullptr)
   {
      static h2_mocker* I = nullptr;
      if (!I) {
         I = new h2_mocker(be_fp, be_fn, file);
      }
      return *I;
   }

   h2_mocker& once(__H2_MATCHER_PARAMETER_DEFAULT_LIST)
   {
      c_array.push_back(h2_count_exactly(1));
      m_array.push_back(std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST));
      r_array.push_back(h2_return<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& twice(__H2_MATCHER_PARAMETER_DEFAULT_LIST)
   {
      c_array.push_back(h2_count_exactly(2));
      m_array.push_back(std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST));
      r_array.push_back(h2_return<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& times(int count)
   {
      c_array.push_back(h2_count_exactly(count));
      m_array.push_back(h2_matcher_tuple());
      r_array.push_back(h2_return<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& any(__H2_MATCHER_PARAMETER_DEFAULT_LIST)
   {
      c_array.push_back(h2_count_any());
      m_array.push_back(std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST));
      r_array.push_back(h2_return<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& atleast(int count)
   {
      c_array.push_back(h2_count_atleast(count));
      m_array.push_back(h2_matcher_tuple());
      r_array.push_back(h2_return<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& atmost(int count)
   {
      c_array.push_back(h2_count_atmost(count));
      m_array.push_back(h2_matcher_tuple());
      r_array.push_back(h2_return<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& between(int left, int right)
   {
      c_array.push_back(h2_count_between(left, right));
      m_array.push_back(h2_matcher_tuple());
      r_array.push_back(h2_return<Class, Return(Args...)>());
      return *this;
   }

   h2_mocker& with(__H2_MATCHER_PARAMETER_DEFAULT_LIST)
   {
      if (!m_array.empty()) {
         m_array.back() = std::forward_as_tuple(__H2_MATCHER_ARGUMENT_LIST);
      }
      return *this;
   }

   /* clang-format off */
   h2_mocker& th0(h2_matcher<nth_type_decay<0, Args...>> a_=_) { if (!m_array.empty()) std::get<0>(m_array.back()) = a_; return *this; }
   h2_mocker& th1(h2_matcher<nth_type_decay<1, Args...>> a_=_) { if (!m_array.empty()) std::get<1>(m_array.back()) = a_; return *this; }
   h2_mocker& th2(h2_matcher<nth_type_decay<2, Args...>> a_=_) { if (!m_array.empty()) std::get<2>(m_array.back()) = a_; return *this; }
   h2_mocker& th3(h2_matcher<nth_type_decay<3, Args...>> a_=_) { if (!m_array.empty()) std::get<3>(m_array.back()) = a_; return *this; }
   h2_mocker& th4(h2_matcher<nth_type_decay<4, Args...>> a_=_) { if (!m_array.empty()) std::get<4>(m_array.back()) = a_; return *this; }
   h2_mocker& th5(h2_matcher<nth_type_decay<5, Args...>> a_=_) { if (!m_array.empty()) std::get<5>(m_array.back()) = a_; return *this; }
   h2_mocker& th6(h2_matcher<nth_type_decay<6, Args...>> a_=_) { if (!m_array.empty()) std::get<6>(m_array.back()) = a_; return *this; }
   h2_mocker& th7(h2_matcher<nth_type_decay<7, Args...>> a_=_) { if (!m_array.empty()) std::get<7>(m_array.back()) = a_; return *this; }
   h2_mocker& th8(h2_matcher<nth_type_decay<8, Args...>> a_=_) { if (!m_array.empty()) std::get<8>(m_array.back()) = a_; return *this; }
   h2_mocker& th9(h2_matcher<nth_type_decay<9, Args...>> a_=_) { if (!m_array.empty()) std::get<9>(m_array.back()) = a_; return *this; }
   /* clang-format on */

   h2_mocker& returns(h2_return<Class, Return(Args...)> r)
   {
      if (!r_array.empty()) {
         r_array.back() = r;
      }
      return *this;
   }

   h2_mocker& operator=(std::function<Return(Args...)> f)
   {
      if (!r_array.empty()) {
         r_array.back() = h2_return<Class, Return(Args...)>(f);
      }
      return *this;
   }

   h2_mocker& operator=(std::function<Return(Class*, Args...)> f)
   {
      if (!r_array.empty()) {
         r_array.back() = h2_return<Class, Return(Args...)>(f);
      }
      return *this;
   }

   static void* operator new(std::size_t sz)
   {
      return h2_alloc::I().malloc(sz);
   }
   static void operator delete(void* ptr)
   {
      h2_alloc::I().free(ptr);
   }
};

#endif

#ifndef ___H2_SUITE__H___
#define ___H2_SUITE__H___

class h2_case;

struct h2_suite
{
   const char* name;
   const char* file;
   const int line;

   int status_count[10];

   std::vector<h2_case*> case_list;

   h2_suite(const char* _name, const char* _file, const int _line)
     : name(_name), file(_file), line(_line)
   {
      memset(status_count, 0, sizeof(status_count));

      G()->push_back(this);
   }

   ~h2_suite() {}

   void install(h2_case* case_)
   {
      case_list.push_back(case_);
   }

   static h2_suite* A()
   {
      static h2_suite A("Anonymous", "", 0);
      return &A;
   }

   static std::vector<h2_suite*>* G()
   {
      static std::vector<h2_suite*> G;
      return &G;
   }
};

#endif

#ifndef ___H2_CASE__H___
#define ___H2_CASE__H___

#include <csetjmp>

class h2_case
{
 public:
   enum {
      _INITED_ = 0,
      _PASSED_ = 1,
      _FAILED_ = 2,
      _TODOED_ = 3,
      _FILTED_ = 4,
   };

   const char* status()
   {
      switch (_status_) {
      case _TODOED_: return "TODO";
      case _FILTED_: return "Filtered";
      case _PASSED_: return "Passed";
      case _FAILED_: return "Failed";
      }
      return "";
   }

   jmp_buf _jb;
   int _jc;
   int stash()
   {
      _jc = 0;
      return ::setjmp(_jb);
   }
   void stash_pop()
   {
      if (_jc++ == 0) ::longjmp(_jb, 1);
   }

   int _status_;
   const char* _casename_;
   const char* _casefile_;
   int _caseline_;

   h2_suite* _suite_;

   void _prev_setup_()
   {
      _leak_push_(_casefile_, _caseline_, "case");

      // _stub_((void *)free, (void *)h2_leak_inspector::free, "", "", "", 0);
      // _stub_((void *)malloc, (void *)h2_leak_inspector::malloc, "", "", "", 0);
      // _stub_((void *)calloc, (void *)h2_leak_inspector::calloc, "", "", "", 0);
      // _stub_((void *)realloc, (void *)h2_leak_inspector::realloc, "", "", "", 0);
      // _stub_((void *)strdup, (void *)h2_leak_inspector::strdup, "", "", "", 0);
      // _stub_((void *)strndup, (void *)h2_leak_inspector::strndup, "", "", "", 0);
   }
   virtual void setup() {}
   void _post_setup_()
   {
   }
   void _prev_teardown_()
   {
   }
   virtual void teardown() {}
   void _post_teardown_()
   {
      _mock_double_check_();
      _stub_restore_();
      _leak_pop_();
   }
   void execute(h2_leak_stack* leak_stack)
   {
      _leak_stack_ = leak_stack;

      _start_ = h2_milliseconds();

      _status_ = _PASSED_;
      _prev_setup_();
      setup();
      _post_setup_();
      if (!stash()) {
         testcase();
      }
      else {
         _status_ = _FAILED_;
      }

      _prev_teardown_();
      teardown();
      _post_teardown_();

      _endup_ = h2_milliseconds();
   }

 public:
   long _start_, _endup_;
   std::vector<h2_fail*, h2_allocator<h2_fail*>> _fail_list_;

 public:
   h2_leak_stack* _leak_stack_;
   bool _leak_push_(const char* file, int line, const char* where, long long limited = 0x7fffffffffffLL, const char* fill = NULL)
   {
      _leak_stack_->push(file, line, where, limited, fill);
      return true;
   }
   void _leak_pop_()
   {
      if (_status_ == _FAILED_) { /* other failure already happen, ignore memory leak failure */
         return;
      }

      auto fail = _leak_stack_->pop();
      if (fail) {
         _status_ = _FAILED_;
         _fail_list_.push_back(fail);
         stash_pop();
      }
   }

 public:
   std::vector<h2_stub*, h2_allocator<h2_stub*>> _stub_list_;
   void _stub_(void* be_fp, void* to_fp, const char* be_fn, const char* to_fn, const char* file, int line)
   {
      h2_stub* stub = NULL;
      for (auto it = _stub_list_.begin(); it != _stub_list_.end(); it++) {
         if ((*it)->be_fp == be_fp) {
            stub = NULL;
         }
      }

      if (!stub) {
         stub = h2_stub::acq();
         _stub_list_.push_back(stub);
         stub->save(be_fp);
      }
      stub->set(to_fp);
   }

   void _stub_restore_()
   {
      auto it = _stub_list_.begin();
      while (it != _stub_list_.end()) {
         h2_stub* stub = *it;
         it = _stub_list_.erase(it);
         stub->restore();
         h2_stub::rel(stub);
      }
   }

   std::vector<h2_mock*, h2_allocator<h2_mock*>> _mock_list_;
   void _mock_(h2_mock* mock)
   {
      _stub_(mock->_be_fp, mock->_to_fp, mock->_be_fn, "", mock->_file, mock->_line);
      _mock_list_.push_back(mock);
   }

   void _mock_double_check_()
   {
      if (_status_ == _FAILED_) { /* other failure already happen, ignore mock call count failure */
         return;
      }

      h2_fail* fail = nullptr;
      auto it = _mock_list_.begin();
      while (it != _mock_list_.end()) {
         h2_mock* mock = *it;
         it = _mock_list_.erase(it);
         auto f = mock->double_check();
         if (!fail) {
            fail = f;
         }
         else {
            fail->appendx(f);
         }
      }

      if (fail) {
         _status_ = _FAILED_;
         _fail_list_.push_back(fail);
         return;
      }
   }

 public:
   h2_case() {}
   virtual ~h2_case() {}
   void _init_(h2_suite* suite, const char* casename, bool todo, const char* file, int line)
   {
      _status_ = _INITED_;
      _suite_ = suite;
      _casename_ = casename;
      _casefile_ = file;
      _caseline_ = line;

      if (todo) { _status_ = _TODOED_; }

      _suite_->install(this);
   }

   virtual void testcase() {}
};

#endif

#ifndef ___H2_LOG__H___
#define ___H2_LOG__H___

class h2_log
{
 public:
   h2_log()
   {
   }
   virtual ~h2_log()
   {
   }

   virtual void on_task_start() = 0;
   virtual void on_task_endup(int failed, int passed, int todo, int filtered, int cases, long duration) = 0;
   virtual void on_case_start(h2_case* c) = 0;
   virtual void on_case_endup(h2_case* c) = 0;
};

class h2_logs : public h2_log
{
 public:
   std::vector<h2_log*> logs;

   h2_logs()
   {
   }

   virtual ~h2_logs()
   {
      auto it = logs.begin();
      while (it != logs.end()) {
         it = logs.erase(it);
      }
   }

   void add(h2_log* log)
   {
      logs.push_back(log);
   }

   void on_task_start()
   {
      for (auto it = logs.begin(); it != logs.end(); it++) {
         (*it)->on_task_start();
      }
   }

   void on_task_endup(int failed, int passed, int todo, int filtered, int cases, long duration)
   {
      for (auto it = logs.begin(); it != logs.end(); it++) {
         (*it)->on_task_endup(failed, passed, todo, filtered, cases, duration);
      }
   }

   void on_case_start(h2_case* c)
   {
      for (auto it = logs.begin(); it != logs.end(); it++) {
         (*it)->on_case_start(c);
      }
   }

   void on_case_endup(h2_case* c)
   {
      for (auto it = logs.begin(); it != logs.end(); it++) {
         (*it)->on_case_endup(c);
      }
   }
};

class h2_log_console : public h2_log
{
 public:
   h2_log_console() {}
   virtual ~h2_log_console() {}
   void on_task_start() {}
   void on_task_endup(int failed, int passed, int todo, int filtered, int cases, long duration)
   {
      if (failed > 0) {
         printf("%s", h2_cfg::style("bold,red"));
         printf("\nFailed <%d failed, %d passed, %d todo, %d filtered, %ld ms>\n", failed, passed, todo, filtered, duration);
      }
      else {
         printf("%s", h2_cfg::style("bold,green"));
         printf("\nPassed <%d passed, %d todo, %d filtered, %d cases, %ld ms>\n", passed, todo, filtered, cases, duration);
      }
      printf("%s", h2_cfg::style("reset"));
   }
   void on_case_start(h2_case* c) {}
   void on_case_endup(h2_case* c)
   {
      switch (c->_status_) {
      case h2_case::_TODOED_:
         if (!strlen(c->_suite_->name)) {
            printf("H2UNIT_CASE(%s): TODO at %s:%d\n", c->_casename_, c->_casefile_, c->_caseline_);
         }
         else {
            printf("H2CASE(%s, %s): TODO at %s:%d\n", c->_suite_->name, c->_casename_, c->_casefile_, c->_caseline_);
         }
         break;
      case h2_case::_FILTED_:
         break;
      case h2_case::_PASSED_:
         if (h2_cfg::I().verbose) {
            printf("%s", h2_cfg::style("blue"));
            if (!strlen(c->_suite_->name)) {
               printf("H2UNIT_CASE(%s): Passed - %ld ms\n", c->_casename_, c->_endup_ - c->_start_);
            }
            else {
               printf("H2CASE(%s, %s): Passed - %ld ms\n", c->_suite_->name, c->_casename_, c->_endup_ - c->_start_);
            }
            printf("%s", h2_cfg::style("reset"));
         }
         break;
      case h2_case::_FAILED_:
         printf("%s", h2_cfg::style("bold,purple"));
         if (!strlen(c->_suite_->name)) {
            printf("H2UNIT_CASE(%s): Failed at %s:%d\n", c->_casename_, c->_casefile_, c->_caseline_);
         }
         else {
            printf("H2CASE(%s, %s): Failed at %s:%d\n", c->_suite_->name, c->_casename_, c->_casefile_, c->_caseline_);
         }
         printf("%s", h2_cfg::style("reset"));

         for (auto it = c->_fail_list_.begin(); it != c->_fail_list_.end(); it++) {
            H2_FAIL_FOREACH(f, *it)
            {
               f->print();
            }
         }
         ::printf("\n");
         break;
      }

      printf("%s", h2_cfg::style("reset"));
   }
};

class h2_log_xml : public h2_log
{
 private:
   FILE* fp;

 public:
   h2_log_xml() {}
   virtual ~h2_log_xml() {}

   bool set_file(const char* filepath)
   {
      fp = fopen(filepath, "w");
      return fp != NULL;
   }

   void on_task_start() {}
   void on_task_endup(int failed, int passed, int todo, int filtered, int cases, long duration)
   {
      fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(fp, "<testsuites>\n");

      for (auto i = h2_suite::G()->begin(); i != h2_suite::G()->end(); i++) {
         auto p = *i;
         fprintf(fp, "  <testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n",
                 p->status_count[h2_case::_FAILED_],
                 p->name,
                 p->status_count[h2_case::_TODOED_] + p->status_count[h2_case::_FILTED_],
                 (int)p->case_list.size(),
                 0, "");

         for (auto j = p->case_list.begin(); j != p->case_list.end(); j++) {
            auto c = *j;
            fprintf(fp, "    <testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n",
                    c->_suite_->name, c->_casename_, c->status(), (c->_endup_ - c->_start_) / 1000.0);

            if (c->_status_ == h2_case::_FAILED_) {
               fprintf(fp, "      <failure message=\"%s:%d:", c->_casefile_, c->_caseline_);

               for (auto k = c->_fail_list_.begin(); k != c->_fail_list_.end(); k++) {
                  H2_FAIL_FOREACH(f, *k)
                  {
                     fprintf(fp, "{newline}");
                     f->print(fp);
                  }
               }
               fprintf(fp, "\" type=\"AssertionFailedError\"></failure>\n");
            }
            fprintf(fp, "      <system-out></system-out><system-err></system-err>\n");
            fprintf(fp, "    </testcase>\n");
         }
         fprintf(fp, "  </testsuite>\n");
      }
      fprintf(fp, "</testsuites>\n");

      fclose(fp);
   }

   void on_case_start(h2_case* c) {}
   void on_case_endup(h2_case* c) {}
};

#endif

#ifndef ___H2_EXTRA__H___
#define ___H2_EXTRA__H___

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

struct h2extra
{
   static struct sockaddr* get_sockaddrs()
   {
      static struct sockaddr sockaddrs[100] = {{0}};
      return sockaddrs;
   }

   static int getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res)
   {
      static struct addrinfo addrinfos[100];
      struct sockaddr* sockaddrs = get_sockaddrs();

      struct addrinfo** pp = res;

      for (int i = 0; i < 100; ++i) {
         struct sockaddr_in* b = (struct sockaddr_in*)&sockaddrs[i];
         if (0 == b->sin_addr.s_addr) {
            break;
         }

         struct addrinfo* a = &addrinfos[i];
         a->ai_addr = &sockaddrs[i];
         a->ai_addrlen = sizeof(struct sockaddr_in);
         a->ai_family = AF_INET;
         a->ai_socktype = SOCK_STREAM;
         a->ai_protocol = IPPROTO_TCP;
         a->ai_canonname = nullptr;
         a->ai_next = nullptr;
         if (hints) {
            a->ai_family = hints->ai_family;
            a->ai_socktype = hints->ai_socktype;
            a->ai_protocol = hints->ai_protocol;
         }
         *pp = a;
         pp = &a->ai_next;
      }
      *pp = nullptr;
      return 0;
   }

   static void freeaddrinfo(struct addrinfo* ai)
   {
   }

   static struct hostent* gethostbyname(const char* name)
   {
      static char* h_aliases[1] = {NULL};
      static char* h_addr_list[100];
      static struct hostent h;
      h.h_name = (char*)name;
      h.h_addrtype = AF_INET;
      h.h_aliases = h_aliases;
      h.h_addr_list = h_addr_list;
      // h.h_length

      struct sockaddr* sockaddrs = get_sockaddrs();
      for (int i = 0; i < 100; ++i) {
         struct sockaddr_in* b = (struct sockaddr_in*)&sockaddrs[i];
         if (0 == b->sin_addr.s_addr) {
            break;
         }
         // h_addr_list[i] = (char *)b->sin_addr.s_addr;
      }

      return &h;
   }
};

static inline void h2_set_addrinfo(int count, ...)
{
   struct sockaddr* sockaddrs = h2extra::get_sockaddrs();

   int i = 0;
   va_list a;
   va_start(a, count);
   for (; i < count; ++i) {
      const char* p = va_arg(a, const char*);
      struct sockaddr_in* b = (struct sockaddr_in*)&sockaddrs[i];
      //b->sin_len = sizeof(struct sockaddr_in);
      b->sin_family = AF_INET;
      b->sin_addr.s_addr = inet_addr(p);
   }
   va_end(a);

   memset(&sockaddrs[i], 0, sizeof(struct sockaddr));
}

#define H2DNS(...) h2_set_addrinfo(H2_PP_NARGS(__VA_ARGS__), __VA_ARGS__)

#endif

#ifndef ___H2_TASK__H___
#define ___H2_TASK__H___

class h2_task
{
 public:
   std::vector<h2_case*> case_list;

   h2_case* currcase;
   h2_leak_stack leak_stack;

   h2_logs logs;
   h2_log_console log_console;
   h2_log_xml log_xml;

   h2_task()
     : currcase(nullptr)
   {
   }

   ~h2_task()
   {
   }

   static h2_task& I()
   {
      static h2_task I;
      return I;
   }

   void configure(int argc, char** argv)
   {
      h2_cfg::I().configure(argc, argv);
      logs.add(&log_console);
   }

   h2_stub getaddrinfo_stub;
   h2_stub freeaddrinfo_stub;

   h2_stub free_stub;
   h2_stub malloc_stub;
   h2_stub calloc_stub;
   h2_stub realloc_stub;
   // #if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
   h2_stub posix_memalign_stub;
// #endif
#ifdef _ISOC11_SOURCE
   h2_stub aligned_alloc_stub;
#endif
   h2_stub strdup_stub;
   h2_stub strndup_stub;

   void prepare()
   {
      if (h2_cfg::I().junit) {
         if (!log_xml.set_file(h2_cfg::I().junit)) {
            printf("Can't open file %s\n", h2_cfg::I().junit);
            exit(1);
         }
         logs.add(&log_xml);
      }

      for (auto i = h2_suite::G()->begin(); i != h2_suite::G()->end(); i++) {
         for (auto j = (*i)->case_list.begin(); j != (*i)->case_list.end(); j++) {
            case_list.push_back(*j);
         }
      }

      if (h2_cfg::I().randomize) {
         random_shuffle(case_list.begin(), case_list.end());
      }

      getaddrinfo_stub.save((void*)::getaddrinfo);
      freeaddrinfo_stub.save((void*)::freeaddrinfo);

      getaddrinfo_stub.set((void*)h2extra::getaddrinfo);
      freeaddrinfo_stub.set((void*)h2extra::freeaddrinfo);

      h2_set_addrinfo(1, "127.0.0.1");

      leak_stack.push("", 0, "root");

      if (h2_cfg::I().memory_check) {
         free_stub.save((void*)::free);
         malloc_stub.save((void*)::malloc);
         calloc_stub.save((void*)::calloc);
         realloc_stub.save((void*)::realloc);
         // #if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
         posix_memalign_stub.save((void*)::posix_memalign);
// #endif
#ifdef _ISOC11_SOURCE
         aligned_alloc_stub.save((void*)::aligned_alloc);
#endif
         strdup_stub.save((void*)::strdup);
         strndup_stub.save((void*)::strndup);

         free_stub.set((void*)h2_leak_inspector::free);
         malloc_stub.set((void*)h2_leak_inspector::malloc);
         calloc_stub.set((void*)h2_leak_inspector::calloc);
         realloc_stub.set((void*)h2_leak_inspector::realloc);
         // #if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
         posix_memalign_stub.set((void*)h2_leak_inspector::posix_memalign);
// #endif
#ifdef _ISOC11_SOURCE
         aligned_alloc_stub.set((void*)h2_leak_inspector::aligned_alloc);
#endif
         strdup_stub.set((void*)h2_leak_inspector::strdup);
         strndup_stub.set((void*)h2_leak_inspector::strndup);
      }
   }

   void roundoff()
   {
      if (h2_cfg::I().memory_check) {
         free_stub.restore();
         malloc_stub.restore();
         calloc_stub.restore();
         realloc_stub.restore();
         // #if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
         posix_memalign_stub.restore();
// #endif
#ifdef _ISOC11_SOURCE
         aligned_alloc_stub.restore();
#endif
         strdup_stub.restore();
         strndup_stub.restore();
      }

      getaddrinfo_stub.restore();
      freeaddrinfo_stub.restore();
   }

   void run()
   {
      int status_count[10];
      memset(status_count, 0, sizeof(status_count));

      long start = h2_milliseconds();

      logs.on_task_start();

      for (auto it = case_list.begin(); it != case_list.end(); it++) {
         currcase = *it;
         logs.on_case_start(currcase);

         if (h2_cfg::I().filter(currcase->_suite_->name, currcase->_casename_)) {
            currcase->_status_ = h2_case::_FILTED_;
         }

         if (h2_case::_INITED_ == currcase->_status_) {
            currcase->execute(&leak_stack);
         }

         logs.on_case_endup(currcase);

         status_count[currcase->_status_] += 1;
         currcase->_suite_->status_count[currcase->_status_] += 1;
      }

      logs.on_task_endup(status_count[h2_case::_FAILED_],
                         status_count[h2_case::_PASSED_],
                         status_count[h2_case::_TODOED_],
                         status_count[h2_case::_FILTED_],
                         case_list.size(),
                         h2_milliseconds() - start);
   }
};

static inline void h2_stub_g(void* be_fp, void* to_fp, const char* be_fn, const char* to_fn, const char* file, int line)
{
   h2_task::I().currcase->_stub_(be_fp, to_fp, be_fn, to_fn, file, line);
}

static inline void h2_mock_g(h2_mock* mock)
{
   h2_task::I().currcase->_mock_(mock);
}

static inline void h2_fail_g(h2_fail* fail)
{
   if (fail) {
      h2_task::I().currcase->_status_ = h2_case::_FAILED_;
      h2_task::I().currcase->_fail_list_.push_back(fail);
      h2_task::I().currcase->stash_pop();
   }
}

static inline h2_mm* h2_newm_g(size_t size, size_t alignment, const char* fill, h2_backtrace &bt)
{
   return h2_task::I().leak_stack.new_mm(size, alignment, fill, bt);
}

static inline h2_mm* h2_getm_g(void* ptr)
{
   return h2_task::I().leak_stack.get_mm(ptr);
}

static inline void h2_relm_g(h2_mm* mm)
{
   auto fail = h2_task::I().leak_stack.rel_mm(mm);
   if (fail) {
      h2_fail_g(fail);
   }
}

#endif

#define __H2UNIT_UNIT_TYPE(name) H2_PP_CAT(h2_unit_, name, _type)
#define __H2UNIT_UNIT_INST(name) H2_PP_CAT(h2_unit_, name, _inst)

#define H2UNIT(_suite_)                                                              \
   static struct h2_suite __H2UNIT_UNIT_INST(_suite_)(#_suite_, __FILE__, __LINE__); \
   struct __H2UNIT_UNIT_TYPE(_suite_) : public h2_case

#define __H2UNIT_CASE_TYPE(name) H2_PP_CAT(h2_unit_, name, _case_, __COUNTER__, _, __LINE__, _type)
#define __H2UNIT_CASE_INST(name) H2_PP_CAT(h2_unit_, name, _case_, __COUNTER__, _, __LINE__, _inst)

#define __H2CASE(_suitetype_, _suiteinst_, _casetype_, _casename_, _todo_) \
   namespace {                                                             \
   class _casetype_ : public _suitetype_                                   \
   {                                                                       \
    public:                                                                \
      _casetype_()                                                         \
      {                                                                    \
         _init_(_suiteinst_, _casename_, _todo_, __FILE__, __LINE__);      \
      }                                                                    \
      void testcase();                                                     \
   } __H2UNIT_CASE_INST(any);                                              \
   }                                                                       \
   void _casetype_::testcase()

#define H2CASE(_suite_, ...) \
   __H2CASE(__H2UNIT_UNIT_TYPE(_suite_), &__H2UNIT_UNIT_INST(_suite_), __H2UNIT_CASE_TYPE(_suite_), #__VA_ARGS__, false)

#define H2TODO(_suite_, ...) \
   __H2CASE(__H2UNIT_UNIT_TYPE(_suite_), &__H2UNIT_UNIT_INST(_suite_), __H2UNIT_CASE_TYPE(_suite_), #__VA_ARGS__, true)

#define H2UNIT_CASE(...) \
   __H2CASE(h2_case, h2_suite::A(), __H2UNIT_CASE_TYPE(____), #__VA_ARGS__, false)

#define H2UNIT_TODO(...) \
   __H2CASE(h2_case, h2_suite::A(), __H2UNIT_CASE_TYPE(____), #__VA_ARGS__, true)

#define __H2EQ1(condition)                                       \
   do {                                                          \
      if (!(condition)) {                                        \
         h2_fail* fail = new h2_fail_normal(__FILE__, __LINE__); \
         fail->kprintf("%s is false", #condition);               \
         h2_fail_g(fail);                                        \
      }                                                          \
   } while (0)

#define __H2EQ2(expect, actual)                                                                              \
   do {                                                                                                      \
      auto t__a = actual;                                                                                    \
      typedef typename std::decay<decltype(t__a)>::type actualtype;                                          \
      typedef typename std::conditional<std::is_enum<actualtype>::value, int, actualtype>::type matchertype; \
      h2_matcher<matchertype> t__e(expect);                                                                  \
      h2_fail* fail = t__e.matches(t__a);                                                                    \
      if (fail) {                                                                                            \
         fail->locate(__FILE__, __LINE__, nullptr);                                                                   \
         h2_fail_g(fail);                                                                                    \
      }                                                                                                      \
   } while (0)

#define H2EQ(...) H2_PP_VARIADIC_CALL(__H2EQ, __VA_ARGS__)

#define H2JE(expect, actual) __H2EQ2(Je(expect), actual)

#define __H2STUB2(BeFunc, ToFunc)                                                    \
   do {                                                                              \
      h2_stub_g((void*)BeFunc, (void*)ToFunc, #BeFunc, #ToFunc, __FILE__, __LINE__); \
   } while (0)

#define __H2STUB3(Return, BeFunc, Args)                                                 \
   struct                                                                               \
   {                                                                                    \
      void operator=(Return(*toF) Args)                                                 \
      {                                                                                 \
         h2_stub_g((void*)BeFunc, (void*)(toF), #BeFunc, "lambda", __FILE__, __LINE__); \
      }                                                                                 \
   } H2_PP_CAT2(_stub3_, __LINE__);                                                     \
   H2_PP_CAT2(_stub3_, __LINE__) = [] Args -> Return

#define __H2STUB40(Return, Class, Method, Args)                                                                                                                                                    \
   struct                                                                                                                                                                                          \
   {                                                                                                                                                                                               \
      void operator=(Return (*toF)(Class * that))                                                                                                                                                  \
      {                                                                                                                                                                                            \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, "H2STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "lambda", __FILE__, __LINE__); \
      }                                                                                                                                                                                            \
   } H2_PP_CAT2(_stub40_, __LINE__);                                                                                                                                                               \
   H2_PP_CAT2(_stub40_, __LINE__) = [](Class * that) -> Return

#define __H2STUB41(Return, Class, Method, Args)                                                                                                                                                    \
   struct                                                                                                                                                                                          \
   {                                                                                                                                                                                               \
      void operator=(Return (*toF)(Class * that, H2_PP_REMOVE_PARENTHESES(Args)))                                                                                                                  \
      {                                                                                                                                                                                            \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, "H2STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "lambda", __FILE__, __LINE__); \
      }                                                                                                                                                                                            \
   } H2_PP_CAT2(_stub41_, __LINE__);                                                                                                                                                               \
   H2_PP_CAT2(_stub41_, __LINE__) = [](Class * that, H2_PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB4(Return, Class, Method, Args) H2_PP_IF(H2_PP_0ARGS Args, __H2STUB40(Return, Class, Method, Args), __H2STUB41(Return, Class, Method, Args))

#define __H2STUB50(Return, Class, Method, Args, Instance)                                                                                     \
   struct                                                                                                                                     \
   {                                                                                                                                          \
      void operator=(Return (*toF)(Class * that))                                                                                             \
      {                                                                                                                                       \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "lambda", __FILE__, __LINE__); \
      }                                                                                                                                       \
   } H2_PP_CAT2(_stub50_, __LINE__);                                                                                                          \
   H2_PP_CAT2(_stub50_, __LINE__) = [](Class * that) -> Return

#define __H2STUB51(Return, Class, Method, Args, Instance)                                                                                     \
   struct                                                                                                                                     \
   {                                                                                                                                          \
      void operator=(Return (*toF)(Class * that, H2_PP_REMOVE_PARENTHESES(Args)))                                                             \
      {                                                                                                                                       \
         h2_stub_g(h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "lambda", __FILE__, __LINE__); \
      }                                                                                                                                       \
   } H2_PP_CAT2(_stub51_, __LINE__);                                                                                                          \
   H2_PP_CAT2(_stub51_, __LINE__) = [](Class * that, H2_PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB5(Return, Class, Method, Args, Instance) H2_PP_IF(H2_PP_0ARGS Args, __H2STUB50(Return, Class, Method, Args, Instance), __H2STUB51(Return, Class, Method, Args, Instance))

#define H2STUB(...) H2_PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__)

#define __H2MOCK2(BeFunc, ReturnArgs) \
   h2_mocker<__COUNTER__, __LINE__, std::false_type, ReturnArgs>::I((void*)BeFunc, #BeFunc, __FILE__)

#define __H2MOCK3(Class, Method, ReturnArgs) \
   h2_mocker<__COUNTER__, __LINE__, Class, ReturnArgs>::I(h2_mfp<Class, ReturnArgs>::A(&Class::Method, "H2MOCK", "", #Class, #Method, #ReturnArgs, __FILE__, __LINE__), #Class "::" #Method, __FILE__)

#define __H2MOCK4(Class, Method, ReturnArgs, Instance) \
   h2_mocker<__COUNTER__, __LINE__, Class, ReturnArgs>::I(h2_mfp<Class, ReturnArgs>::A(&Class::Method, Instance), #Class "::" #Method, __FILE__)

#define H2MOCK(...) H2_PP_VARIADIC_CALL(__H2MOCK, __VA_ARGS__)

#define __H2BLOCK0() \
   for (bool t = _leak_push_(__FILE__, __LINE__, "block"); t; _leak_pop_(), t = false)

#define __H2BLOCK1(...) \
   for (bool t = _leak_push_(__FILE__, __LINE__, "block", __VA_ARGS__); t; _leak_pop_(), t = false)

#define H2BLOCK(...) H2_PP_IF(H2_PP_0ARGS(__VA_ARGS__), __H2BLOCK0(), __H2BLOCK1(__VA_ARGS__))
// #define H2BLOCK(...) for (bool t = _leak_push_(__FILE__, __LINE__, "block", ##__VA_ARGS__); t; _leak_pop_(), t = false)
// #define H2BLOCK(...) for (bool t = _leak_push_(__FILE__, __LINE__, "block" __VA_OPT__(,) __VA_ARGS__); t; _leak_pop_(), t = false)

#if defined(_WIN32)
__declspec(selectany)
#else
__attribute__((weak))
#endif

  int main(int argc, char** argv)
{
   h2_task::I().configure(argc, argv);
   h2_task::I().prepare();
   h2_task::I().run();
   h2_task::I().roundoff();
   return 0;
}

#endif
#ifndef ___H2UNIT_H_3_0__BACK_COMPATIBLE__
#define ___H2UNIT_H_3_0__BACK_COMPATIBLE__
#define H2EQ_TRUE H2EQ 
#define H2EQ_MATH H2EQ 
#define H2EQ_STRCMP H2EQ 
#define H2EQ_STRCMP_NOCASE(e,a) H2EQ(CaseLess(e),a) 
#define H2EQ_JSON(e,a) H2EQ(Je(e),a) 
#define H2EQ_WILDCARD(e,a) H2EQ(We(e),a) 
#define H2EQ_REGEX(e,a) H2EQ(Re(e),a) 
#define H2EQ_MEMCMP(e,a,l) H2EQ(Me(e,l),a) 
#define H2LEAK_BLOCK H2BLOCK 
#endif
