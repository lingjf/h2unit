/* v5.0  2020-03-27 01:07:00 */
/* https://github.com/lingjf/h2unit */
/* Apache Licence 2.0 */
#include "h2unit.hpp"
#ifdef H2_2FILES

#include <algorithm>    /* shuffle */
#include <arpa/inet.h>  /* inet_addr */
#include <cassert>      /* assert */
#include <cctype>       /* tolower, isspace */
#include <cmath>        /* fabs */
#include <cstdarg>      /* va_list */
#include <cstdint>      /* int32_t */
#include <cxxabi.h>     /* demangle */
#include <errno.h>      /* strerror */
#include <execinfo.h>   /* backtrace */
#include <fcntl.h>      /* fcntl */
#include <iostream>     /* cout */
#include <libgen.h>     /* basename */
#include <map>          /* std::map */
#include <memory>       /* allocator */
#include <netdb.h>      /* getaddrinfo, gethostbyname */
#include <random>       /* shuffle */
#include <regex>        /* std::regex */
#include <signal.h>     /* sigaction */
#include <sys/ioctl.h>  /* ioctl */
#include <sys/mman.h>   /* mprotect, mmap */
#include <sys/socket.h> /* sockaddr */
#include <sys/time.h>   /* gettimeofday */
#include <sys/types.h>  /* size_t */
#include <typeinfo>     /* typeid */
#include <unistd.h>     /* sysconf */

#if defined __GLIBC__
#   include <malloc.h> /* __malloc_hook */
#elif defined __APPLE__
#   include <AvailabilityMacros.h>
#   include <malloc/malloc.h> /* malloc_zone_t */
#endif

namespace h2 {

static inline bool streq(const char* s1, const char* s2) { return !strcmp(s1, s2); }

static inline bool h2_regex_match(const char* pattern, const char* subject, bool caseless = false) {
   bool result = false;
   try {
      std::regex re(pattern);
      result = std::regex_match(subject, caseless ? std::regex(pattern, std::regex::icase) : std::regex(pattern));
   } catch (const std::regex_error& e) {
      result = false;
   }
   return result;
}

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

static inline long long h2_now() {
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

static inline void h2_sleep(long long milliseconds) {
   ::usleep(milliseconds * 1000);
}

static inline int h2_winsz() {
   struct winsize w;
   if (-1 == ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)) return 80;
   return w.ws_col;
}

static inline const char* h2_style(const char* style, char* ascii_code) {
   static struct {
      const char *name, *value;
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

   char t[1024], *s = strcpy(t, style), *d = ascii_code + sprintf(ascii_code, "\033["), *q = d;
   for (char* p = strtok(s, ","); p; p = strtok(nullptr, ","))
      for (auto& k : K)
         if (streq(k.name, p)) {
            q += sprintf(q, "%s", k.value);
            break;
         }
   return d == q ? strcpy(ascii_code, "") : (*(q - 1) = 'm', ascii_code);
}


struct h2_nm {
   h2_singleton(h2_nm);

   std::map<std::string, unsigned long long> symbols;
   unsigned long long main_addr;

   unsigned long long get(const char* name) const {
      if (strlen(name) == 0) return 0;
      auto it = symbols.find(name);
      return it != symbols.end() ? it->second : ULLONG_MAX;
   }

   bool in_main(unsigned long long addr) const { return main_addr == ULLONG_MAX ? false : main_addr < addr && addr < main_addr + 256; }

   h2_nm() {
      nm_parse();
      main_addr = get("main");
   }

   void nm_parse() {
      char nm[256], line[1024], addr[128], type[32], name[1024];
      sprintf(nm, "nm %s", O.path);
      h2_with f(::popen(nm, "r"), ::pclose);
      if (f.f)
         while (::fgets(line, sizeof(line) - 1, f.f))
            if (3 == sscanf(line, "%s%s%s", addr, type, name))
               if (type[0] == 't' || type[0] == 'T' || type[0] == 'w' || type[0] == 'W')
                  symbols.insert(std::make_pair(name + O.isMAC(), (unsigned long long)strtoull(addr, nullptr, 16)));
   }
};

static inline bool demangle(const char* mangled, char* demangled, size_t len) {
   int status = 0;
   abi::__cxa_demangle(mangled, demangled, &len, &status);
   return status == 0;
}

static inline bool addr2line(unsigned long long addr, char* output, size_t len) {
   char t[256];
#if defined __APPLE__
   sprintf(t, "atos -o %s 0x%llx", O.path, addr);
#else
   sprintf(t, "addr2line -C -a -s -p -f -e %s -i %llx", O.path, addr);
#endif
   h2_with f(::popen(t, "r"), ::pclose);
   if (!f.f || !::fgets(output, len, f.f)) return false;
   for (int i = strlen(output) - 1; 0 <= i && ::isspace(output[i]); --i) output[i] = '\0';  //strip tail
   return true;
}

static inline bool backtrace_extract(const char* backtrace_symbol_line, char* module, char* mangled, unsigned long long* offset) {
   //MAC: `3   a.out  0x000000010e777f3d _ZN2h24hook6mallocEm + 45
   if (3 == ::sscanf(backtrace_symbol_line, "%*s%s%*s%s + %llu", module, mangled, offset))
      return true;

   //Linux: with '-rdynamic' linker option
   //Linux: module_name(mangled_function_name+relative_offset_to_function)[absolute_address]
   //Linux: `./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]
   if (3 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^_a-zA-Z]%127[^)+]+0x%llx", module, mangled, offset))
      return true;

   mangled[0] = '\0';

   //Linux: Ubuntu without '-rdynamic' linker option
   //Linux: module_name(+relative_offset_to_function)[absolute_address]
   //Linux: `./a.out(+0xb1887)[0x560c5ed06887]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^+]+0x%llx", module, offset))
      return true;

   //Linux: Redhat/CentOS without '-rdynamic' linker option
   //Linux: module_name()[relative_offset_to_module]
   //Linux: `./a.out() [0x40b960]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^(]%*[^[][0x%llx", module, offset))
      return true;

   //Where?
   //Linux: module_name[relative_offset_to_module]
   //Linux: `./a.out[0x4060e7]
   if (2 == ::sscanf(backtrace_symbol_line, "%[^[][0x%llx", module, offset))
      return true;

   return false;
}

h2_inline h2_backtrace::h2_backtrace(int shift_) : shift(shift_) {
   h2_heap::unhook();
   count = ::backtrace(array, sizeof(array) / sizeof(array[0]));
   h2_heap::dohook();
}

h2_inline bool h2_backtrace::operator==(h2_backtrace& bt) {
   if (count != bt.count) return false;
   for (int i = 0; i < count; ++i)
      if (array[i] != bt.array[i])
         return false;
   return true;
}

h2_inline bool h2_backtrace::has(void* func, int size) const {
   for (int i = 0; i < count; ++i)
      if (func <= array[i] && (unsigned char*)array[i] < ((unsigned char*)func) + size)
         return true;
   return false;
}

h2_inline void h2_backtrace::print() const {
   h2_heap::unhook();
   char** backtraces = backtrace_symbols(array, count);
   for (int i = shift; i < count; ++i) {
      char *p = backtraces[i], module[256] = "", mangled[256] = "", demangled[256] = "", addr2lined[512] = "";
      unsigned long long address = 0, offset = 0;
      if (backtrace_extract(backtraces[i], module, mangled, &offset)) {
         if (strlen(mangled)) {
            p = mangled;
            if (demangle(mangled, demangled, sizeof(demangled)))
               if (strlen(demangled))
                  p = demangled;
         }
         address = h2_nm::I().get(mangled);
         if (address != ULLONG_MAX)
            if (addr2line(address + offset, addr2lined, sizeof(addr2lined)))
               if (strlen(addr2lined))
                  p = addr2lined;
      }
      ::printf("   %d. %s\n", i - shift, p);

      if (streq("main", mangled) || streq("main", demangled) || h2_nm::I().in_main(address + offset))
         break;
   }
   free(backtraces);
   h2_heap::dohook();
}

h2_inline h2_fail* h2_callexp::check() {
   if (is_satisfied() || is_saturated()) return nullptr;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->aprintf("%s", actual());
   fail->mprintf("called but expect");
   fail->eprintf("%s", expect());
   return fail;
}

h2_inline const char* h2_callexp::actual() {
   static char st[64];
   if (call > 0)
      sprintf(st, "%d times", call);
   else
      sprintf(st, "never");
   return st;
}

h2_inline const char* h2_callexp::expect() {
   static char st[128];
   if (least == 0)
      if (most == 0)
         sprintf(st, "never called");
      else if (most == INT_MAX)
         sprintf(st, "any number of times");
      else
         sprintf(st, "at most %d times", most);
   else if (least == most)
      sprintf(st, "exactly %d times", least);
   else if (most == INT_MAX)
      sprintf(st, "at least %d times", least);
   else  // 0 < least < most < INT_MAX
      sprintf(st, "between %d and %d times", least, most);

   return st;
}

h2_inline h2_case::h2_case(const char* name_, int todo, const char* file_, int line_)
  : name(name_), file(file_), line(line_), status(todo ? TODOED : INITED), fails(nullptr), sock(nullptr) {}

h2_inline void h2_case::prev_setup() {
   status = PASSED;
   h2_heap::stack::push(file, line);
}

h2_inline void h2_case::post_cleanup() {
   if (sock) delete sock;
   dnses.clear();
   stubs.clear();
   h2_fail* fail = mocks.clear();
   h2_fail::append_x(fail, h2_heap::stack::pop());

   if (!fail) return;
   if (status != FAILED)
      h2_fail::append_x(fails, fail);
   else
      delete fail;
   status = FAILED;
}

h2_inline void h2_case::do_fail(h2_fail* fail) {
   status = FAILED;
   h2_fail::append_x(fails, fail);
   ::longjmp(jump, 1);
}

#if defined __linux__
#   if defined(__GNUC__) && (defined(__i386) || defined(__x86_64))
#      define h2_raise_trap() asm volatile("int $3")
#   else
#      define h2_raise_trap() raise(SIGTRAP)
#   endif
#elif defined __APPLE__
#   define h2_raise_trap() __asm__("int $3\n" \
                                   :          \
                                   :)
#endif

#if defined __linux__
static inline bool under_debug(int, const char*) {
   char t[1024];
   h2_with f(::fopen("/proc/self/status", "r"));
   if (f.f)
      while (::fgets(t, sizeof(t) - 1, f.f))
         if (strncmp(t, "TracerPid:\t", 11) == 0)
            return t[11] != '\0' && t[11] != '0';
   return false;
}
#elif defined __APPLE__
static inline bool under_debug(int pid, const char* path) {
   char t[1024], attach_pid[64];
   sprintf(attach_pid, "%d", pid);
   h2_with f(::popen("ps -ef | grep lldb | grep -v sudo | grep -v grep", "r"), ::pclose);
   if (f.f)
      while (::fgets(t, sizeof(t) - 1, f.f))
         if (strstr(t, basename((char*)path)) || strstr(t, attach_pid))
            return true;
   return false;
}
#endif

static inline char* get_gdb1(char* s) {
#if defined __linux__
   sprintf(s, "gdb --quiet --args %s %s", O.path, O.args);
#elif defined __APPLE__
   sprintf(s, "lldb %s -- %s", O.path, O.args);
#endif
   return s;
}

static inline char* get_gdb2(char* s, int pid) {
#if defined __linux__
   sprintf(s, "sudo gdb --pid=%d", pid);
#elif defined __APPLE__
   sprintf(s, "sudo lldb --attach-pid %d", pid);
#endif
   return s;
}

h2_inline void h2_debugger::trap() {
   int pid = (int)getpid();
   if (!under_debug(pid, O.path)) {
      static h2_once only_one_time;
      if (only_one_time) {
         if (streq("gdb attach", O.debug)) {
            if (fork() == 0) {
               system(get_gdb2((char*)alloca(256), pid));
               exit(0);
            } else
               while (!under_debug(pid, O.path))
                  h2_sleep(100);  // wait for password
         } else {
            system(get_gdb1((char*)alloca(256)));
            exit(0);
         }
      }
   }

   h2_raise_trap();
}

static constexpr const char* last_order_file_path = ".last_order";

static inline bool isfit(const char opt, const char e) { return opt == 'a' || opt == 'A' || ::tolower(opt) == ::tolower(e); }

h2_inline void h2_directory::drop_last_order() { ::remove(last_order_file_path); }

static inline void read_last_order(std::vector<std::pair<std::string, std::string>>& list) {
   char suitename[512], casename[512];
   h2_with f(fopen(last_order_file_path, "r"));
   if (f.f)
      while (1 == fscanf(f.f, "%[^\n]\n", suitename) && 1 == fscanf(f.f, "%[^\n]\n", casename))
         list.push_back(std::pair<std::string, std::string>(suitename, casename));
}

static inline void save_last_order() {
   h2_with f(fopen(last_order_file_path, "w"));
   if (f.f)
      for (auto& s : h2_directory::I().suites)
         for (auto& c : s->cases())
            fprintf(f.f, "%s\n%s\n", s->name, c->name);
}

static inline void find_and_sequent(std::pair<std::string, std::string>& suite_case) {
   static long long seq = INT_MIN;
   for (auto& s : h2_directory::I().suites)
      if (suite_case.first == s->name)
         for (auto& c : s->cases())
            if (suite_case.second == c->name)
               s->seq = c->seq = ++seq;
}

h2_inline int h2_directory::count() {
   int count = 0;
   for (auto& s : h2_directory::I().suites)
      count += s->cases().size();
   return count;
}

h2_inline int h2_directory::sort() {
   std::vector<std::pair<std::string, std::string>> last_list;
   read_last_order(last_list);
   if (0 < last_list.size()) {
      for (auto& k : last_list) find_and_sequent(k);
      for (auto& s : h2_directory::I().suites)
         std::sort(s->cases().begin(), s->cases().end(), [](h2_case* a, h2_case* b) { return a->seq < b->seq; });
      std::sort(h2_directory::I().suites.begin(), h2_directory::I().suites.end(), [](h2_suite* a, h2_suite* b) { return a->seq < b->seq; });
   } else {
      if (isfit(O.randomize, 'c'))
         for (auto& s : h2_directory::I().suites)
            shuffle(s->cases().begin(), s->cases().end(), std::default_random_engine{std::random_device()()});
      if (isfit(O.randomize, 's')) shuffle(h2_directory::I().suites.begin(), h2_directory::I().suites.end(), std::default_random_engine{std::random_device()()});
      if (O.randomize) save_last_order();
   }
   return count();
}

h2_inline void h2_directory::print_list() {
   int ss = 0, cs = 0, t;

   for (auto& s : h2_directory::I().suites) {
      if (t = 0, isfit(O.listing, 's')) {
         if (!O.filter(s->name, "", "")) t++;
         for (auto& c : s->cases())
            if (!O.filter(s->name, isfit(O.listing, 'c') ? c->name : "", "")) t++;
         if (t) printf("S%d. %s \\\\ %s:%d\n", ++ss, s->name, basename((char*)s->file), s->line);
      }
      if (t = 0, isfit(O.listing, 'c'))
         for (auto& c : s->cases())
            if (!O.filter(s->name, c->name, ""))
               isfit(O.listing, 's') ? printf("C%d/S%d-%d. %s // %s \\\\ %s:%d\n", ++cs, ss, ++t, s->name, c->name, basename((char*)c->file), c->line) : printf("C%d. %s // %s \\\\ %s:%d\n", ++cs, s->name, c->name, basename((char*)c->file), c->line);
   }
}

h2_inline void h2_directory::list_then_exit() {
   sort();
   print_list();
   exit(0);
}
// TINYEXPR - Tiny recursive descent parser and evaluation engine in C
//
// Copyright (c) 2015-2018 Lewis Van Winkle
//
// http://CodePlea.com

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

struct te
{
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

    enum { TOK_NULL = TE_CLOSURE7+1, TOK_ERROR, TOK_END, TOK_SEP, TOK_OPEN, TOK_CLOSE, TOK_NUMBER, TOK_VARIABLE, TOK_INFIX };

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
        te_expr *ret = (te_expr *)h2_libc::malloc(size);
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
        h2_libc::free((void *)n);
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
        if (a < 0.0) return NAN;
        if (a > UINT_MAX) return INFINITY;
        unsigned int ua = (unsigned int)(a);
        unsigned long int result = 1, i;
        for (i = 1; i <= ua; i++) {
            if (i > ULONG_MAX / result) return INFINITY;
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
            if (result > ULONG_MAX / (un - ur + i)) return INFINITY;
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
};

/* clang-format on */

h2_inline double tinyexpr::eval(const char* expression, int* error) {
   return te::te_interp(expression, error);
}
#define _H2_XPRINTF(value, format)           \
   do {                                      \
      char t[1024 * 8];                      \
      va_list args;                          \
      va_start(args, format);                \
      vsnprintf(t, sizeof(t), format, args); \
      va_end(args);                          \
      value = value + t;                     \
   } while (0)

h2_inline void h2_fail::append_x(h2_fail*& fail, h2_fail* n) {
   if (!fail)
      fail = n;
   else {
      h2_fail** pp = &fail->x_next;
      while (*pp) pp = &(*pp)->x_next;
      *pp = n;
   }
}

h2_inline void h2_fail::append_y(h2_fail*& fail, h2_fail* n) {
   if (!fail)
      fail = n;
   else {
      h2_fail** pp = &fail->y_next;
      while (*pp) pp = &(*pp)->y_next;
      *pp = n;
   }
}

h2_inline h2_fail::h2_fail(const char* file_, int line_, const char* func_, int argi_)
  : x_next(nullptr), y_next(nullptr), file(file_), line(line_), func(func_), argi(argi_), w_type(0) {}

h2_inline h2_fail::~h2_fail() {
   if (y_next) delete y_next;
   if (x_next) delete x_next;
};

h2_inline void h2_fail::locate(const char* file_, int line_, const char* func_, int argi_) {
   file = file_, line = line_, func = func_, argi = argi_;
   if (y_next) y_next->locate(file_, line_, func_, argi_);
}

h2_inline void h2_fail::set_w_type(int w_type_) {
   w_type = w_type_;
   if (y_next) y_next->set_w_type(w_type_);
}

h2_inline void h2_fail::set_e_expr(const char* e_expr_) {
   e_expr = e_expr_;
   if (y_next) y_next->set_e_expr(e_expr_);
}

h2_inline void h2_fail::set_a_expr(const char* a_expr_) {
   a_expr = a_expr_;
   if (y_next) y_next->set_a_expr(a_expr_);
}

h2_inline void h2_fail::kprintf(const char* format, ...) { _H2_XPRINTF(_k, format); }
h2_inline void h2_fail::hprintf(const char* format, ...) { _H2_XPRINTF(_h, format); }
h2_inline void h2_fail::eprintf(const char* format, ...) { _H2_XPRINTF(_e, format); }
h2_inline void h2_fail::aprintf(const char* format, ...) { _H2_XPRINTF(_a, format); }
h2_inline void h2_fail::mprintf(const char* format, ...) { _H2_XPRINTF(_m, format); }
h2_inline void h2_fail::uprintf(const char* format, ...) { _H2_XPRINTF(_u, format); }

h2_inline void h2_fail::print_locate() {
   static constexpr const char* a9 = "1st\0002nd\0003rd\0004th\0005th\0006th\0007th\0008th\0009th";
   if (func && strlen(func)) ::printf(", in %s(%s)", func, 0 <= argi && argi < 9 ? a9 + argi * 4 : "");
   if (file && strlen(file) && 0 < line) ::printf(", at %s:%d", file, line);
   ::printf("\n");
}

h2_inline void h2_fail::print() { _k.size() && ::printf(" %s", _k.c_str()); }
h2_inline void h2_fail::print(FILE* fp) { fprintf(fp, "%s", _k.c_str()); }

h2_inline h2_fail_normal::h2_fail_normal(const char* file_, int line_, const char* func_, const char* format, ...)
  : h2_fail(file_, line_, func_) { _H2_XPRINTF(_k, format); }

h2_inline void h2_fail_normal::print() { h2_fail::print(), print_locate(); }

h2_inline h2_fail_unexpect::h2_fail_unexpect(const char* file_, int line_) : h2_fail(file_, line_) {}

h2_inline void h2_fail_unexpect::print_OK1() {
   ::printf(" OK(%s) is %sfalse%s", a_expr.c_str(), S("bold,red"), S("reset"));
}

h2_inline void h2_fail_unexpect::print_OK2() {
   char t1[256] = {0}, t2[256] = {0};
   if (e_expr == _e) /* OK(1, ret) */
      sprintf(t1, "%s%s%s", S("green"), _e.acronym().c_str(), S("reset"));
   else { /* OK(Eq(1), ret) */
      sprintf(t1, "%s", e_expr.acronym().c_str());
      if (_e.length()) sprintf(t1 + strlen(t1), "%s==>%s%s%s%s", S("dark gray"), S("reset"), S("green"), _e.acronym().c_str(), S("reset"));
   }
   if (a_expr == _a)
      sprintf(t2, "%s%s%s", S("bold,red"), _a.acronym().c_str(), S("reset"));
   else {
      if (_a.length()) sprintf(t2, "%s%s%s%s<==%s", S("bold,red"), _a.acronym().c_str(), S("reset"), S("dark gray"), S("reset"));
      sprintf(t2 + strlen(t2), "%s", a_expr.acronym().c_str());
   }

   ::printf(" OK(%s, %s)", t1, t2);
   if (_m.length()) ::printf(" actual %s", _m.c_str());
}

h2_inline void h2_fail_unexpect::print_OK3() {
   ::printf(" JE(%s, %s)", e_expr.acronym().c_str(), a_expr.acronym().c_str());
   if (_m.length()) ::printf(" actual %s expect", _m.c_str());
}

h2_inline void h2_fail_unexpect::print_MOCK() {
   ::printf(" actual %s%s%s", S("green"), _a.acronym().c_str(), S("reset"));
   if (_m.length()) ::printf(" %s", _m.c_str());
   if (_e.length()) ::printf(" %s%s%s", S("bold,red"), _e.acronym().c_str(), S("reset"));
}

h2_inline void h2_fail_unexpect::print() {
   h2_fail::print(); /* nothing */
   if (w_type == 0) print_MOCK();
   if (w_type == 1) print_OK1();
   if (w_type == 2) print_OK2();
   if (w_type == 3) print_OK3();
   if (_u.length()) ::printf(", %s", _u.c_str());
   print_locate();
}

h2_inline h2_fail_strcmp::h2_fail_strcmp(const h2_string& expect_, const h2_string& actual_, bool caseless_, const char* file_, int line_)
  : h2_fail_unexpect(file_, line_), expect(expect_), actual(actual_), caseless(caseless_) {}

h2_inline void h2_fail_strcmp::print() {
   h2_fail_unexpect::print();

   int columns = h2_winsz() - 12;
   int rows = ::ceil(std::max(expect.length(), actual.length()) / (double)columns);
   for (int i = 0; i < rows; ++i) {
      char eline[1024], aline[1024], *ep = eline, *ap = aline;
      ep += sprintf(ep, "%sexpect%s>%s ", S("dark gray"), S("green"), S("reset"));
      ap += sprintf(ap, "%sactual%s>%s ", S("dark gray"), S("red"), S("reset"));
      for (int j = 0; j < columns; ++j) {
         char ec = i * columns + j <= expect.length() ? expect[i * columns + j] : ' ';
         char ac = i * columns + j <= actual.length() ? actual[i * columns + j] : ' ';

         bool eq = caseless ? ::tolower(ec) == ::tolower(ac) : ec == ac;
         ep += sprintf(ep, "%s%s%s", eq ? "" : S("green"), fmt_char(ec, eq), S("reset"));
         ap += sprintf(ap, "%s%s%s", eq ? "" : S("red,bold"), fmt_char(ac, eq), S("reset"));
      }
      ::printf("%s\n%s\n", eline, aline);
   }
}
h2_inline char* h2_fail_strcmp::fmt_char(char c, bool eq) {
   static char st[32];
   sprintf(st, "%c", c);

   if (c == '\n') sprintf(st, "%sn", S("inverse"));
   if (c == '\r') sprintf(st, "%sr", S("inverse"));
   if (c == '\t') sprintf(st, "%st", S("inverse"));
   if (c == '\0') sprintf(st, "%s ", eq ? "" : S("inverse"));

   return st;
}

h2_inline h2_fail_json::h2_fail_json(const h2_string& expect_, const h2_string& actual_, const char* file_, int line_)
  : h2_fail_unexpect(file_, line_), expect(expect_), actual(actual_) { _e = expect_, _a = actual_; }

h2_inline void h2_fail_json::print() {
   h2_fail_unexpect::print();
   h2_string str;
   int side_width = h2_json_exporter::diff(expect, actual, h2_winsz(), str);

   ::printf("%s%s│%s%s\n", S("dark gray"), h2_string("expect").center(side_width).c_str(), h2_string("actual").center(side_width).c_str(), S("reset"));
   ::printf("%s", str.c_str());
}

h2_inline h2_fail_memcmp::h2_fail_memcmp(const unsigned char* expect_, const unsigned char* actual_, int len, const char* file_, int line_)
  : h2_fail_unexpect(file_, line_), expect(expect_, expect_ + len), actual(actual_, actual_ + len) {
   eprintf("%p", expect_);
   aprintf("%p", actual_);
}

h2_inline void h2_fail_memcmp::print() {
   h2_fail_unexpect::print();
   ::printf("%s%s  │  %s%s \n", S("dark gray"), h2_string("expect").center(16 * 3).c_str(), h2_string("actual").center(16 * 3).c_str(), S("reset"));
   int bytes = expect.size(), rows = ::ceil(bytes / 16.0);
   for (int i = 0; i < rows; ++i) {
      char eline[256], aline[256], *ep = eline, *ap = aline;
      for (int j = 0; j < 16; ++j)
         if (bytes <= i * 16 + j)
            ep += sprintf(ep, "   "), ap += sprintf(ap, "   ");
         else
            ep = fmt_byte(expect[i * 16 + j], actual[i * 16 + j], j, "green", ep),
            ap = fmt_byte(actual[i * 16 + j], expect[i * 16 + j], j, "bold,red", ap);

      ::printf("%s  %s│%s  %s \n", eline, S("dark gray"), S("reset"), aline);
   }
}

h2_inline char* h2_fail_memcmp::fmt_byte(unsigned char c, unsigned char t, int j, const char* style, char* p) {
   if (c != t) p += sprintf(p, "%s", S(style));
   p += sprintf(p, j < 8 ? "%02X " : " %02X", c);
   if (c != t) p += sprintf(p, "%s", S("reset"));
   return p;
}

h2_inline h2_fail_memoverflow::h2_fail_memoverflow(void* ptr_, int offset_, const unsigned char* magic_, int size, h2_backtrace bt0_, h2_backtrace bt1_, const char* file_, int line_)
  : h2_fail(file_, line_), ptr((unsigned char*)ptr_), offset(offset_), magic(magic_), spot(((unsigned char*)ptr_) + offset_, ((unsigned char*)ptr_) + offset_ + size), bt0(bt0_), bt1(bt1_) {
   kprintf("Memory overflow malloc %p %+d (%p) ", ptr, offset, ptr + offset);
}

h2_inline void h2_fail_memoverflow::print() {
   h2_fail::print();

   for (int i = 0; i < spot.size(); ++i)
      ::printf("%s%02X %s", magic[i] == spot[i] ? S("green") : S("bold,red"), spot[i], S("reset"));

   print_locate();
   if (0 < bt1.count) ::printf("  %p trampled at backtrace:\n", ptr + offset), bt1.print();
   if (0 < bt0.count) ::printf("  which allocated at backtrace:\n"), bt0.print();
}

h2_inline h2_fail_memleak::h2_fail_memleak(const char* file_, int line_, const char* where_)
  : h2_fail(file_, line_), where(where_), bytes(0), times(0) {}

h2_inline void h2_fail_memleak::add(void* ptr, int size, h2_backtrace& bt) {
   bytes += size, times += 1;
   for (auto c : places)
      if (c.bt == bt) {
         c.ptr2 = ptr, c.size2 = size, c.bytes += size, c.times += 1;
         return;
      }
   places.push_back(P(ptr, size, bt));
}

h2_inline void h2_fail_memleak::print() {
   char t1[64] = "", t2[64] = "";
   if (1 < places.size()) sprintf(t1, "%d places ", (int)places.size());
   if (1 < times) sprintf(t2, "%lld times ", times);

   kprintf("Memory Leaked %s%s%lld bytes in %s totally", t1, t2, bytes, where);
   h2_fail::print(), print_locate();
   for (auto c : places) {
      c.times <= 1 ? ::printf("  %p Leaked %lld bytes, at backtrace\n", c.ptr, c.bytes) :
                     ::printf("  %p, %p ... Leaked %lld times %lld bytes (%lld, %lld ...), at backtrace\n", c.ptr, c.ptr2, c.times, c.bytes, c.size, c.size2);
      c.bt.print();
   }
}

h2_inline h2_fail_doublefree::h2_fail_doublefree(void* ptr_, h2_backtrace& bt0_, h2_backtrace bt1_, const char* file_, int line_)
  : h2_fail(file_, line_), bt0(bt0_), bt1(bt1_) { kprintf("%p double freed", ptr_); }

h2_inline void h2_fail_doublefree::print() {
   h2_fail::print(), ::printf(" at backtrace:\n");
   bt1.print();
   if (0 < bt0.count) ::printf("  which allocated at backtrace:\n"), bt0.print();
}

h2_inline h2_fail_instantiate::h2_fail_instantiate(const char* action_type_, const char* return_type_, const char* class_type_, const char* method_name_, const char* return_args_, int why_abstract_, const char* file_, int line_)
  : h2_fail(file_, line_), action_type(action_type_), return_type(return_type_), class_type(class_type_), method_name(method_name_), return_args(return_args_), why_abstract(why_abstract_) {
   why_abstract ? kprintf("Instantiate 'class %s' is a abstract class", class_type) :
                  kprintf("Instantiate 'class %s' don't know initialize arguments", class_type);
}

h2_inline void h2_fail_instantiate::print() {
   h2_fail::print(), print_locate();

   ::printf("You may take following solutions to fix it: \n");
   if (why_abstract)
      ::printf("1. Add non-abstract Derived Class instance in %s(%s%s%s, %s, %s%s, Derived_%s(...)%s) \n",
               action_type,
               strlen(return_type) ? return_type : "",
               strlen(return_type) ? ", " : "",
               class_type, method_name, return_args,
               S("bold,yellow"),
               class_type,
               S("reset"));
   else {
      ::printf("1. Define default constructor in class %s, or \n", class_type);
      ::printf("2. Add parameterized construction in %s(%s%s%s, %s, %s%s, %s(...)%s) \n",
               action_type,
               strlen(return_type) ? return_type : "",
               strlen(return_type) ? ", " : "",
               class_type, method_name, return_args,
               S("bold,yellow"),
               class_type,
               S("reset"));
   }
}
static const unsigned char snowfield[] = {0xbe, 0xaf, 0xca, 0xfe, 0xc0, 0xde, 0xfa, 0xce};

struct h2_piece : h2_libc {
   unsigned char *ptr, *page;
   h2_list x;
   int size, pagesize, pagecount, freed;
   h2_backtrace bt;

   h2_piece(int size_, int alignment, h2_backtrace& bt_) : size(size_), freed(0), bt(bt_) {
      pagesize = ::sysconf(_SC_PAGE_SIZE);
      if (alignment <= 0) alignment = 8;
      pagecount = ::ceil((size + alignment + sizeof(snowfield)) / (double)pagesize);

      page = (unsigned char*)::mmap(nullptr, pagesize * (pagecount + 1), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      assert(page != MAP_FAILED);

      ptr = page + pagesize * pagecount - size;
      ptr = (unsigned char*)(((intptr_t)ptr / alignment) * alignment);

      h2_piece** backward = (h2_piece**)(ptr - sizeof(snowfield) - sizeof(void*));
      *backward = this;

      mark_snowfield();
   }

   ~h2_piece() {
      ::munmap(page, pagesize * (pagecount + 1));
   }

   void mark_snowfield() {
      memcpy(ptr - sizeof(snowfield), snowfield, sizeof(snowfield));
      memcpy(ptr + size, snowfield, sizeof(snowfield));

      if (::mprotect(page + pagesize * pagecount, pagesize, PROT_READ) != 0)
         ::printf("mprotect PROT_READ failed %s\n", strerror(errno));
   }

   h2_fail* check_snowfield() {
      h2_fail* fail = nullptr;
      if (memcmp(ptr + size, snowfield, sizeof(snowfield)))
         h2_fail::append_x(fail, new h2_fail_memoverflow(ptr, size, snowfield, sizeof(snowfield), bt, h2_backtrace()));
      if (memcmp(ptr - sizeof(snowfield), snowfield, sizeof(snowfield)))
         h2_fail::append_x(fail, new h2_fail_memoverflow(ptr, -(int)sizeof(snowfield), snowfield, sizeof(snowfield), bt, h2_backtrace()));

      if (::mprotect(page, pagesize * (pagecount + 1), PROT_NONE) != 0)
         ::printf("mprotect PROT_NONE failed %s\n", strerror(errno));
      return fail;
   }

   h2_fail* free() {
      if (freed++) return new h2_fail_doublefree(ptr, bt, h2_backtrace(O.isMAC() ? 5 : 4));
      return check_snowfield();
   }

   bool in_range(const void* p) {
      const unsigned char* p0 = page;
      const unsigned char* p2 = p0 + pagesize * (pagecount + 1);
      return p0 <= (const unsigned char*)p && (const unsigned char*)p < p2;
   }
};

struct h2_block : h2_libc {
   h2_list x;
   h2_list using_list, freed_list;

   const char* file;
   int line;
   const char* where;
   long long limited;
   const char* fill;

   h2_block(const char* file_, int line_, const char* where_, long long limited_, const char* fill_)
     : file(file_), line(line_), where(where_), limited(limited_), fill(fill_) {}

   h2_fail* leak_check() {
      h2_fail_memleak* fail = nullptr;
      if (!using_list.empty()) {
         fail = new h2_fail_memleak(file, line, where);
         h2_list_for_each_entry(p, &using_list, h2_piece, x) fail->add(p->ptr, p->size, p->bt);
      }
      h2_list_for_each_entry(p, &freed_list, h2_piece, x) h2_out_delete(p);
      return fail;
   }

   h2_piece* new_piece(int size, int alignment, const char* fill_, h2_backtrace& bt) {
      if (limited < size) return nullptr;
      limited -= size;

      h2_piece* p = new h2_piece(size, alignment, bt);

      if (fill_ ? fill_ : (fill_ = fill))
         for (int i = 0, j = 0, l = strlen(fill_); i < size; ++i, ++j)
            ((char*)p->ptr)[i] = fill_[j % (l ? l : 1)];

      using_list.push(&p->x);
      return p;
   }

   h2_piece* get_piece(const void* ptr) {
      h2_list_for_each_entry(p, &using_list, h2_piece, x) if (p->ptr == ptr) return p;
      h2_list_for_each_entry(p, &freed_list, h2_piece, x) if (p->ptr == ptr) return p;
      return nullptr;
   }

   h2_fail* rel_piece(h2_piece* p) {
      limited += p->size;

      p->x.out();
      freed_list.push(&p->x);
      return p->free();
   }

   h2_piece* host_piece(const void* addr) {
      h2_list_for_each_entry(p, &using_list, h2_piece, x) if (p->in_range(addr)) return p;
      h2_list_for_each_entry(p, &freed_list, h2_piece, x) if (p->in_range(addr)) return p;
      return nullptr;
   }
};

struct h2_stack {
   h2_singleton(h2_stack);

   h2_list blocks;

   bool escape(h2_backtrace& bt) {
      static struct {
         void* base;
         int size;
      } exclude_functions[] = {
        {(void*)printf, 300},
        {(void*)sprintf, 300},
        {(void*)vsnprintf, 300},
        {(void*)sscanf, 300},
        {(void*)localtime, 300}};

      for (auto& x : exclude_functions)
         if (bt.has(x.base, x.size))
            return true;

      return false;
   }

   void push(const char* file, int line, const char* where, long long limited, const char* fill) {
      h2_block* b = new h2_block(file, line, where, limited, fill);
      blocks.push(&b->x);
   }

   h2_fail* pop() {
      h2_block* b = h2_list_pop_entry(&blocks, h2_block, x);
      h2_fail* fail = b->leak_check();
      delete b;
      return fail;
   }

   h2_piece* new_piece(size_t size, size_t alignment, const char* fill) {
      h2_backtrace bt(O.isMAC() ? 6 : 2);
      h2_block* b = escape(bt) ? h2_list_bottom_entry(&blocks, h2_block, x) : h2_list_top_entry(&blocks, h2_block, x);
      return b ? b->new_piece(size, alignment, fill, bt) : nullptr;
   }

   h2_piece* get_piece(const void* ptr) {
      h2_list_for_each_entry(p, &blocks, h2_block, x) h2_if_return(p->get_piece(ptr), );
      return nullptr;
   }

   h2_fail* rel_piece(void* ptr) {
      h2_list_for_each_entry(p, &blocks, h2_block, x) h2_if_return(p->get_piece(ptr), p->rel_piece);
      h2_debug("Warning: free not found!");
      return nullptr;
   }

   h2_piece* host_piece(const void* addr) {
      h2_list_for_each_entry(p, &blocks, h2_block, x) h2_if_return(p->host_piece(addr), );
      return nullptr;
   }
};

// https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html
// https://github.com/gperftools/gperftools/blob/master/src/libc_override.h

#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
extern "C" {
extern malloc_zone_t* malloc_default_purgeable_zone(void) WEAK_IMPORT_ATTRIBUTE;
}
#endif

struct h2_hook {
   h2_singleton(h2_hook);

   static void free(void* ptr) {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece(ptr)); /* overflow or double free */
   }

   static void* malloc(size_t size) {
      h2_piece* p = h2_stack::I().new_piece(size, 0, nullptr);
      return p ? p->ptr : nullptr;
   }

   static void* calloc(size_t count, size_t size) {
      h2_piece* p = h2_stack::I().new_piece(size * count, 0, "\0");
      return p ? p->ptr : nullptr;
   }

   static void* realloc(void* ptr, size_t size) {
      if (size == 0) {
         free(ptr);
         return nullptr;
      }

      h2_piece* old_p = h2_stack::I().get_piece(ptr);
      if (!old_p) return nullptr;

      h2_piece* new_p = h2_stack::I().new_piece(size, 0, nullptr);
      if (!new_p) return nullptr;

      memcpy(new_p->ptr, old_p->ptr, old_p->size);
      h2_fail_g(h2_stack::I().rel_piece(ptr));

      return new_p->ptr;
   }

   static int posix_memalign(void** memptr, size_t alignment, size_t size) {
      h2_piece* p = h2_stack::I().new_piece(size, alignment, nullptr);
      return p ? (*memptr = p->ptr, 0) : ENOMEM;
   }

   static void* aligned_alloc(size_t alignment, size_t size) {
      h2_piece* p = h2_stack::I().new_piece(size, alignment, nullptr);
      return p ? p->ptr : nullptr;
   }

#if defined __GLIBC__

   static void free_hook(void* __ptr, const void* caller) { free(__ptr); }
   static void* malloc_hook(size_t __size, const void* caller) { return malloc(__size); }
   static void* realloc_hook(void* __ptr, size_t __size, const void* caller) { return realloc(__ptr, __size); }
   static void* memalign_hook(size_t __alignment, size_t __size, const void* caller) { return aligned_alloc(__alignment, __size); }

   void (*sys__free_hook)(void*, const void*);
   void* (*sys__malloc_hook)(size_t, const void*);
   void* (*sys__realloc_hook)(void*, size_t, const void*);
   void* (*sys__memalign_hook)(size_t, size_t, const void*);

#elif defined __APPLE__
   static size_t mz_size(malloc_zone_t* zone, const void* ptr) {
      h2_piece* p = h2_stack::I().get_piece(ptr);
      return p ? p->size : 0;
   }

   static void* mz_malloc(malloc_zone_t* zone, size_t size) { return malloc(size); }
   static void* mz_calloc(malloc_zone_t* zone, size_t num_items, size_t size) { return calloc(num_items, size); }
   static void* mz_valloc(malloc_zone_t* zone, size_t size) { return malloc(size); }
   static void mz_free(malloc_zone_t* zone, void* ptr) { free(ptr); }
   static void mz_free_sized(malloc_zone_t* zone, void* ptr, size_t size) { free(ptr); }
   static void* mz_realloc(malloc_zone_t* zone, void* ptr, size_t size) { return realloc(ptr, size); }
   static void* mz_memalign(malloc_zone_t* zone, size_t align, size_t size) { return aligned_alloc(align, size); }
   static void mz_destroy(malloc_zone_t* zone) {}

   static kern_return_t mi_enumerator(task_t task, void*, unsigned type_mask, vm_address_t zone_address, memory_reader_t reader, vm_range_recorder_t recorder) { return KERN_FAILURE; }
   static size_t mi_good_size(malloc_zone_t* zone, size_t size) { return size; }
   static boolean_t mi_check(malloc_zone_t* zone) { return true; }
   static void mi_print(malloc_zone_t* zone, boolean_t verbose) {}
   static void mi_log(malloc_zone_t* zone, void* address) {}
   static void mi_force_lock(malloc_zone_t* zone) {}
   static void mi_force_unlock(malloc_zone_t* zone) {}

   static boolean_t mi_zone_locked(malloc_zone_t* zone) { return false; }  // Hopefully unneeded by us!

   static malloc_zone_t* get_default_zone() {
      malloc_zone_t** zones = nullptr;
      unsigned int num_zones = 0;

      if (KERN_SUCCESS != malloc_get_all_zones(0, nullptr, (vm_address_t**)&zones, &num_zones)) num_zones = 0;
      if (num_zones) return zones[0];
      return malloc_default_zone();
   }

   malloc_introspection_t mi;
   malloc_zone_t mz;

#else

#endif

   h2_stubs stubs;

   h2_hook() {
#if defined __GLIBC__
      sys__free_hook = __free_hook;
      sys__malloc_hook = __malloc_hook;
      sys__realloc_hook = __realloc_hook;
      sys__memalign_hook = __memalign_hook;

#elif defined __APPLE__

      memset(&mi, 0, sizeof(mi));
      mi.enumerator = &mi_enumerator;
      mi.good_size = &mi_good_size;
      mi.check = &mi_check;
      mi.print = &mi_print;
      mi.log = &mi_log;
      mi.force_lock = &mi_force_lock;
      mi.force_unlock = &mi_force_unlock;

      memset(&mz, 0, sizeof(malloc_zone_t));
      mz.version = 4;
      mz.zone_name = "h2unit";
      mz.size = &mz_size;
      mz.malloc = &mz_malloc;
      mz.calloc = &mz_calloc;
      mz.valloc = &mz_valloc;
      mz.free = &mz_free;
      mz.realloc = &mz_realloc;
      mz.destroy = &mz_destroy;
      mz.batch_malloc = nullptr;
      mz.batch_free = nullptr;
      mz.introspect = &mi;

#   if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
      mz.version = 6;
      mz.free_definite_size = &mz_free_sized;
      mz.memalign = &mz_memalign;
      mi.zone_locked = &mi_zone_locked;

      if (malloc_default_purgeable_zone) malloc_default_purgeable_zone();

#   endif

#else
#endif
   }

   void dohook() {
#if defined __GLIBC__
      __free_hook = free_hook;
      __malloc_hook = malloc_hook;
      __realloc_hook = realloc_hook;
      __memalign_hook = memalign_hook;
#elif defined __APPLE__
      malloc_zone_register(&mz);
      malloc_zone_t* default_zone = get_default_zone();
      malloc_zone_unregister(default_zone);
      malloc_zone_register(default_zone);
#else
      stubs.add((void*)::free, (void*)hook::free, "", "", __FILE__, __LINE__);
      stubs.add((void*)::malloc, (void*)hook::malloc, "", "", __FILE__, __LINE__);
      stubs.add((void*)::realloc, (void*)hook::realloc, "", "", __FILE__, __LINE__);
      stubs.add((void*)::posix_memalign, (void*)hook::posix_memalign, "", "", __FILE__, __LINE__);
#endif
   }

   void unhook() {
#if defined __GLIBC__
      __free_hook = sys__free_hook;
      __malloc_hook = sys__malloc_hook;
      __realloc_hook = sys__realloc_hook;
      __memalign_hook = sys__memalign_hook;
#elif defined __APPLE__
      malloc_zone_unregister(&mz);
#else
      stubs.clear();
#endif
   }

   static void segment_fault_handler(int sig, siginfo_t* si, void* unused) {
      h2_piece* m = h2_stack::I().host_piece(si->si_addr);
      if (m) h2_fail_g(new h2_fail_memoverflow(m->ptr, (intptr_t)si->si_addr - (intptr_t)m->ptr, nullptr, 0, m->bt, h2_backtrace(O.isMAC() ? 5 : 4)));
      h2_debug();
      exit(1);
   }

   void dosegv() {
      struct sigaction act;
      act.sa_sigaction = segment_fault_handler;
      act.sa_flags = SA_SIGINFO;
      sigemptyset(&act.sa_mask);
      if (sigaction(SIGSEGV, &act, nullptr) == -1) perror("Register SIGSEGV handler failed");
      if (sigaction(SIGBUS, &act, nullptr) == -1) perror("Register SIGBUS handler failed");
   }
};

h2_inline void h2_heap::dosegv() {
   if (O.memory_check && !O.debug) h2_hook::I().dosegv();
}
h2_inline void h2_heap::dohook() {
   if (O.memory_check) h2_hook::I().dohook();
}
h2_inline void h2_heap::unhook() {
   if (O.memory_check) h2_hook::I().unhook();
}
h2_inline void h2_heap::stack_push_block(const char* file, int line, const char* where, long long limited, const char* fill) {
   h2_stack::I().push(file, line, where, limited, fill);
}
h2_inline h2_fail* h2_heap::stack_pop_block() {
   return h2_stack::I().pop();
}
struct h2_json {
   static const int t_absent = 0;

   static const int t_null = 1;
   static const int t_boolean = 2;
   static const int t_number = 3;
   static const int t_string = 4;
   static const int t_regexp = 5;
   static const int t_array = 6;
   static const int t_object = 7;

   static constexpr char indent_char = ' ';
   static constexpr char samelength_char = ' ';
   static constexpr char occupy_char = ' ';
   static constexpr char columns_char = ' ';

   struct P {
      const char* text;
      int length;
      int offset;

      struct P& strip() {
         while (offset < length && ::isspace(text[offset])) offset++;
         return *this;
      }
      bool startswith(const char* s, int n) {
         if (length - offset < n) return false;
         return ::strncmp(text + offset, s, n) == 0;
      }
      bool startswith(char from, char to = '\0') {
         if (length - offset < 1) return false;
         if (to == '\0') to = from;
         return from <= text[offset] && text[offset] <= to;
      }
   };

   struct Node : h2_libc {
      int type;

      h2_string key_string;
      h2_string value_string;
      double value_double;
      bool value_boolean;
      h2_vector<Node*> children; /* array or object */

      Node() : type(t_null), value_double(0), value_boolean(false) {}

      int size() { return children.size(); }

      Node* get(int index) { return 0 <= index && index < children.size() ? children[index] : nullptr; }

      Node* get(const char* name) {
         if (name)
            for (auto node : children)
               if (!node->key_string.compare(name))
                  return node;

         return nullptr;
      }

      void del(Node* child) {
         for (auto it = children.begin(); it != children.end(); it++)
            if (child == *it) {
               children.erase(it);
               delete child;
               return;
            }
      }

      bool is_null() { return t_null == type; }
      bool is_bool() { return t_boolean == type; }
      bool is_number() { return t_number == type; }
      bool is_string() { return t_string == type; }
      bool is_regexp() { return t_regexp == type; }
      bool is_array() { return t_array == type; }
      bool is_object() { return t_object == type; }

      bool parse_number(P& x) {
         int i;
         for (i = 0; x.offset + i < x.length; ++i) {
            const char c = x.text[x.offset + i];
            if (c == ',' || c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == '\0')
               break;
         }

         value_string.assign(x.text + x.offset, i);

         int err = 0;
         value_double = tinyexpr::eval(value_string.c_str(), &err);
         type = t_number;
         x.offset += i;

         return 0 == err;
      }

      bool parse_string(P& x) {
         const char bound = x.text[x.offset];
         x.offset++;

         if (x.length <= x.offset) return false;

         const char* src = x.text + x.offset;
         int len = 0;
         for (; x.text[x.offset] != bound; ++len) {
            if (x.text[x.offset++] == '\\') x.offset++;
            if (x.length <= x.offset) return false;
         }

         for (; len > 0; ++src, --len)
            if (*src != '\\')
               value_string.push_back(*src);
            else
               switch (*++src) {
               case 'b': value_string.push_back('\b'); break;
               case 'f': value_string.push_back('\f'); break;
               case 'n': value_string.push_back('\n'); break;
               case 'r': value_string.push_back('\r'); break;
               case 't': value_string.push_back('\t'); break;
               case '\"': value_string.push_back('\"'); break;
               case '\\': value_string.push_back('\\'); break;
               case '/': value_string.push_back('/'); break;
               default: return false;
               }

         type = t_string;
         x.offset++;

         return true;
      }

      bool parse_regexp(P& x) {
         bool ret = parse_string(x);
         type = t_regexp;
         return ret;
      }

      bool parse_value(P& x) {
         /* t_null */
         if (x.startswith("null", 4)) {
            type = t_null;
            x.offset += 4;
            return true;
         }
         /* false */
         if (x.startswith("false", 5)) {
            type = t_boolean;
            value_boolean = false;
            x.offset += 5;
            return true;
         }
         /* true */
         if (x.startswith("true", 4)) {
            type = t_boolean;
            value_boolean = true;
            x.offset += 4;
            return true;
         }
         /* t_string */
         if (x.startswith('\"') || x.startswith('\'')) return parse_string(x);
         /* t_regexp */
         if (x.startswith('/')) return parse_regexp(x);

         /* t_array */
         if (x.startswith('[')) return parse_array(x);
         /* t_object */
         if (x.startswith('{')) return parse_object(x);

         /* t_number */
         if (1 /* x.startswith('-') || x.startswith('0', '9') */) return parse_number(x);

         return false;
      }

      bool parse_array(P& x) {
         x.offset++;  //pass [

         while (!x.strip().startswith(']')) {
            Node* new_node = new Node();
            if (!new_node) return false;

            children.push_back(new_node);

            if (!new_node->parse_value(x)) return false;

            if (x.strip().startswith(',')) x.offset++;
         }

         type = t_array;
         x.offset++;

         return true;
      }

      bool parse_object(P& x) {
         x.offset++;  //pass {

         while (!x.strip().startswith('}')) {
            Node* new_node = new Node();
            if (!new_node) return false;

            children.push_back(new_node);

            if (!new_node->parse_string(x)) return false;

            new_node->key_string = new_node->value_string;
            new_node->value_string = "";

            if (!x.strip().startswith(':')) return false;
            x.offset++;

            if (!new_node->parse_value(x.strip())) return false;

            if (x.strip().startswith(',')) x.offset++;
         }

         type = t_object;
         x.offset++;

         return true;
      }
   };

   static Node* parse(const char* json_string, int length = 0) {
      if (!json_string) return nullptr;
      if (length == 0) length = strlen(json_string);
      if (length == 0) return nullptr;

      P x;

      x.text = json_string;
      x.length = length;
      x.offset = 0;

      Node* root = new Node();
      if (!root->parse_value(x.strip())) return nullptr;

      return root;
   }

   static void frees(Node* root) {
      if (root) {
         for (auto node : root->children) frees(node);
         delete root;
      }
   }

   static bool match_array(Node* e, Node* a) {
      if (!e || !a) return false;
      if (e->children.size() != a->children.size()) return false;
      for (int i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->children[i]))
            return false;
      return true;
   }

   static bool match_object(Node* e, Node* a) {
      if (!e || !a) return false;
      if (e->children.size() > a->children.size()) return false;
      for (int i = 0; i < e->children.size(); ++i)
         if (!match(e->children[i], a->get(e->children[i]->key_string.c_str())))
            return false;
      return true;
   }

   static bool match(Node* e, Node* a) {
      if (!e || !a) return false;
      switch (e->type) {
      case t_null:
         return a->is_null();
      case t_boolean:
         return a->is_bool() && e->value_boolean == a->value_boolean;
      case t_number:
         return a->is_number() && ::fabs(e->value_double - a->value_double) < 0.00001;
      case t_string:
         return a->is_string() && e->value_string == a->value_string;
      case t_regexp:
         return a->is_string() && h2_regex_match(e->value_string.c_str(), a->value_string.c_str());
      case t_array:
         return a->is_array() && match_array(e, a);
      case t_object:
         return a->is_object() && match_object(e, a);
      default: return false;
      };
   }

   struct Dual : h2_libc {
      int depth;
      int e_type, a_type;
      h2_string e_key, a_key;
      h2_string e_value, a_value;
      h2_vector<Dual*> child;
      Dual* perent;

      Dual(int depth_, Dual* perent_) : depth(depth_), e_type(t_absent), a_type(t_absent), perent(perent_) {}
   };

   static void frees(Dual* root) {
      if (root) {
         for (auto dual : root->child) frees(dual);
         delete root;
      }
   }

   static void node2dual(Node* node, int& type, h2_string& key, h2_string& value) {
      if (!node) return;

      type = t_string;

      if (node->key_string.size()) key = "\"" + node->key_string + "\"";

      switch (node->type) {
      case t_null:
         type = t_string;
         value = "null";
         break;
      case t_boolean:
         type = t_string;
         value = node->value_boolean ? "true" : "false";
         break;
      case t_number:
         type = t_string;
         value.sprintf("%1.15g", node->value_double);
         break;
      case t_string:
         type = t_string;
         value = "\"" + node->value_string + "\"";
         break;
      case t_regexp:
         type = t_string;
         value = node->value_string;
         break;
      case t_array:
         type = t_array;
         break;
      case t_object:
         type = t_object;
         break;
      }
   }

   static void samelengthify(h2_string& e, h2_string& a) {
      int e_l = e.length(), a_l = a.length();

      e.append(std::max(e_l, a_l) - e_l, samelength_char);
      a.append(std::max(e_l, a_l) - a_l, samelength_char);
   }

   static void dual(Node* e, Node* a, Dual* d) {
      node2dual(e, d->e_type, d->e_key, d->e_value);
      node2dual(a, d->a_type, d->a_key, d->a_value);
      samelengthify(d->e_key, d->a_key);
      samelengthify(d->e_value, d->a_value);

      if (d->e_type != d->a_type) {
         if (d->e_type == t_object) d->e_type = t_string, d->e_value = "{ ... }";
         if (d->e_type == t_array) d->e_type = t_string, d->e_value = "[ ... ]";
         if (d->a_type == t_object) d->a_type = t_string, d->a_value = "{ ... }";
         if (d->a_type == t_array) d->a_type = t_string, d->a_value = "[ ... ]";
         samelengthify(d->e_value, d->a_value);
         return;
      }

      if (d->e_type == t_object) {
         for (auto i = e->children.begin(); i != e->children.end();) {
            Node *e1 = *i, *a1 = a->get(e1->key_string.c_str());
            if (!a1)
               for (auto& j : a->children)
                  if (match(e1, j)) {
                     a1 = j;
                     break;
                  }
            if (a1) {
               Dual* d1 = new Dual(d->depth + 1, d);
               d->child.push_back(d1);
               dual(e1, a1, d1);
               a->del(a1);
               i = e->children.erase(i);
               delete e1;
            } else
               i++;
         }

         for (int i = 0; i < std::max(e->children.size(), a->children.size()); ++i) {
            Dual* d1 = new Dual(d->depth + 1, d);
            d->child.push_back(d1);
            Node *e1 = e->get(i), *a1 = a->get(i);
            dual(e1, a1, d1);
         }
      }

      if (d->e_type == t_array) {
         for (int i = 0; i < std::max(e->children.size(), a->children.size()); ++i) {
            Dual* d1 = new Dual(d->depth + 1, d);
            d->child.push_back(d1);
            Node *e1 = e->get(i), *a1 = a->get(i);
            dual(e1, a1, d1);
         }
      }
   }

   static h2_string indent(int depth) { return h2_string(depth * 2, indent_char); }
   static h2_string occupy(h2_string p) { return h2_string(p.length(), occupy_char); }

   static void diff(Dual* d, h2_vector<h2_string>& e, h2_vector<h2_string>& a) {
      if (!d) return;
      e.push_back("\n");
      e.push_back(indent(d->depth));
      a.push_back("\n");
      a.push_back(indent(d->depth));

      if (d->e_type != t_absent) {
         if (d->a_type == t_absent) {  // only e-side exist
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
         } else {
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
      } else {
         if (d->a_key.size()) e.push_back(occupy(d->a_key + ": "));
         if (d->a_value.size()) e.push_back(occupy(d->a_value));
      }

      if (d->a_type != t_absent) {
         if (d->e_type == t_absent) {  // only a-side exist
            const char* style = "#red,bold";
            if (d->perent && d->perent->a_type == t_object) {
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
         } else {
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
      } else {
         if (d->e_key.size()) a.push_back(occupy(d->e_key + ": "));
         if (d->e_value.size()) a.push_back(occupy(d->e_value));
      }

      /* e/a type shoud be same */

      if (d->e_type == t_object && d->a_type == t_object) {
         e.push_back("{");
         a.push_back("{");
      }
      if (d->e_type == t_array && d->a_type == t_array) {
         e.push_back("[");
         a.push_back("[");
      }

      if ((d->e_type == t_object && d->a_type == t_object) || (d->e_type == t_array && d->a_type == t_array)) {
         for (int i = 0; i < d->child.size(); i++) {
            diff(d->child[i], e, a);

            bool e_not_last = false, a_not_last = false;
            for (int j = i + 1; j < d->child.size(); j++) {
               e_not_last = e_not_last || (d->child[j]->e_type != t_absent);
               a_not_last = a_not_last || (d->child[j]->a_type != t_absent);
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

      if (d->e_type == t_object && d->a_type == t_object) {
         e.push_back("}");
         a.push_back("}");
      }
      if (d->e_type == t_array && d->a_type == t_array) {
         e.push_back("]");
         a.push_back("]");
      }
   }

   typedef h2_vector<h2_string> Line;
   typedef h2_vector<Line> Lines;

   static void merge_line(h2_vector<h2_string>& list, Lines& lines) {
      Line line;
      for (auto& s : list) {
         if (s == "\n") {
            lines.push_back(line);
            line.clear();
            continue;
         }
         line.push_back(s);
      }
      lines.push_back(line);
      line.clear();
   }

   static int lines_most(Lines& lines) {
      int most = 0;
      for (auto& line : lines) {
         int curr = 0;
         for (auto& word : line)
            if (word[0] != '#') curr += word.length();
         most = std::max(most, curr);
      }
      return most;
   }

   static int line_wrap(Line& line, int columns) {
      int char_count = 0;
      for (auto& word : line)
         if (word[0] != '#') char_count += word.length();

      return ::ceil(char_count / (double)columns);  // num_of_line
   }

   static h2_string line_wrap(Line& line, int index, int columns, h2_string& current_style) {
      int s = 0, u = 0;
      h2_string wrap;
      for (auto& word : line)
         if (word[0] == '#') {
            if (index * columns <= s && s < (index + 1) * columns) {
               const char* style = S(word.c_str() + 1);
               wrap.append(style);
               current_style = style;
            }
         } else {
            for (auto& c : word) {
               if (index * columns <= s && s < (index + 1) * columns) {
                  wrap.append(1, c);
                  ++u;
               }
               ++s;
            }
         }

      wrap.append(columns - u, columns_char);
      return wrap;
   }

   static void print(Lines& e_lines, Lines& a_lines, int side_width, h2_string& str) {
      h2_string e_last_style, a_last_style;
      // assert(e_lines.size() == a_lines.size());
      for (int i = 0; i < std::max(e_lines.size(), a_lines.size()); ++i) {
         auto e_line = e_lines[i];
         auto a_line = a_lines[i];
         int e_wraps = line_wrap(e_line, side_width - 2);
         int a_wraps = line_wrap(a_line, side_width - 2);
         // assert(e_wraps == a_wraps);
         int K = std::max(e_wraps, a_wraps);
         for (int j = 0; j < K; ++j) {
            h2_string e_current_style, a_current_style;
            auto e_wrap = line_wrap(e_line, j, side_width - 2, e_current_style);
            auto a_wrap = line_wrap(a_line, j, side_width - 2, a_current_style);
            str.sprintf("%s%s %s%s%s│%s %s%s %s%s%s%s\n",
                        e_last_style.c_str(), e_wrap.c_str(), S("reset"),
                        S("dark gray"), j == K - 1 ? " " : "\\", S("reset"),
                        a_last_style.c_str(), a_wrap.c_str(), S("reset"),
                        S("dark gray"), j == K - 1 ? " " : "\\", S("reset"));

            e_last_style = e_current_style;
            a_last_style = a_current_style;
         }
      }
   }
};

h2_inline bool h2_json_exporter::match(const h2_string expect, const h2_string actual) {
   h2_json::Node *e = h2_json::parse(expect.c_str()), *a = h2_json::parse(actual.c_str());

   bool result = h2_json::match(e, a);

   h2_json::frees(e);
   h2_json::frees(a);

   return result;
}

h2_inline int h2_json_exporter::diff(const h2_string expect, const h2_string actual, int terminal_width, h2_string& str) {
   h2_json::Node *e_node = h2_json::parse(expect.c_str()), *a_node = h2_json::parse(actual.c_str());

   h2_json::Dual* d = new h2_json::Dual(0, nullptr);
   h2_json::dual(e_node, a_node, d);

   h2_json::frees(e_node);
   h2_json::frees(a_node);

   h2_vector<h2_string> e_list, a_list;
   h2_json::diff(d, e_list, a_list);
   h2_json::frees(d);

   h2_json::Lines e_lines, a_lines;
   h2_json::merge_line(e_list, e_lines);
   h2_json::merge_line(a_list, a_lines);

   int e_most = h2_json::lines_most(e_lines), a_most = h2_json::lines_most(a_lines);
   int fav_width = std::max(std::max(e_most, a_most) + 3, 30);
   int side_width = std::min(terminal_width / 2 - 4, fav_width);

   h2_json::print(e_lines, a_lines, side_width, str);
   return side_width;
}

h2_inline void* h2_libc::malloc(size_t sz) {
   if (!O.memory_check) {
      return ::malloc(sz);
   }
   void* ptr = mmap(nullptr, sz, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
   if (ptr == MAP_FAILED) return nullptr;

   uintptr_t* p = (uintptr_t*)ptr;
   *p = (uintptr_t)sz;
   return (void*)(p + 1);
}

h2_inline void h2_libc::free(void* ptr) {
   if (!O.memory_check) {
      ::free(ptr);
      return;
   }
   if (!ptr) return;
   uintptr_t* p = ((uintptr_t*)ptr) - 1;
   munmap((void*)p, (size_t)*p);
}

h2_inline int h2_libc::write(FILE* stream, const void* buf, size_t nbyte) {
   return ::write(fileno(stream), buf, nbyte);
}

h2_inline h2_log::h2_log() : total_cases(0), done_cases(0), percentage(0), tt(0), ts(0), tc(0) {}
h2_inline void h2_log::on_task_start(int cases) {
   total_cases = cases;
   tt = h2_now();
};
h2_inline void h2_log::on_task_endup(int status_stats[8]) { tt = h2_now() - tt; };
h2_inline void h2_log::on_suite_start(h2_suite* s) { ts = h2_now(); }
h2_inline void h2_log::on_suite_endup(h2_suite* s) { ts = h2_now() - ts; }
h2_inline void h2_log::on_case_start(h2_suite* s, h2_case* c) { tc = h2_now(); };
h2_inline void h2_log::on_case_endup(h2_suite* s, h2_case* c) {
   percentage = ++done_cases * 100 / total_cases;
   tc = h2_now() - tc;
};

struct h2_log_console : h2_log {
   void on_task_endup(int status_stats[8]) override {
      h2_log::on_task_endup(status_stats);
      printf("\n[%3d%%] ", percentage);
      if (0 < status_stats[h2_case::FAILED]) {
         printf("%s", S("bold,red"));
         printf("Failed <%d failed, %d passed, %d todo, %d filtered, %lld ms>\n", status_stats[h2_case::FAILED], status_stats[h2_case::PASSED], status_stats[h2_case::TODOED], status_stats[h2_case::FILTED], tt);
      } else {
         printf("%s", S("bold,green"));
         printf("Passed <%d passed, %d todo, %d filtered, %d cases, %lld ms>\n", status_stats[h2_case::PASSED], status_stats[h2_case::TODOED], status_stats[h2_case::FILTED], total_cases, tt);
      }
      printf("%s", S("reset"));
   }
   void on_case_endup(h2_suite* s, h2_case* c) override {
      h2_log::on_case_endup(s, c);
      switch (c->status) {
      case h2_case::INITED: break;
      case h2_case::TODOED:
         if (O.verbose)
            printf("[%3d%%] (%s // %s): %s at %s:%d\n", percentage, s->name, c->name, CSS[c->status], basename((char*)c->file), c->line);
         break;
      case h2_case::FILTED: break;
      case h2_case::PASSED:
         if (O.verbose) {
            printf("[%3d%%] ", percentage);
            printf("%s", S("light blue"));
            printf("(%s // %s): Passed - %lld ms\n", s->name, c->name, tc);
            printf("%s", S("reset"));
         } else if (!O.debug)
            printf("\r[%3d%%] (%d/%d)\r", percentage, done_cases, total_cases);
         break;
      case h2_case::FAILED:
         printf("[%3d%%] ", percentage);
         printf("%s", S("bold,purple"));
         printf("(%s // %s): Failed at %s:%d\n", s->name, c->name, basename((char*)c->file), c->line);
         printf("%s", S("reset"));
         for (h2_fail* x_fail = c->fails; x_fail; x_fail = x_fail->x_next)
            for (h2_fail* fail = x_fail; fail; fail = fail->y_next)
               fail->print();
         printf("\n");
         break;
      }
   }
};

struct h2_log_xml : h2_log {
   FILE* f;

   void on_task_start(int cases) override {
      h2_log::on_task_start(cases);
      f = fopen(O.junit, "w");
      if (!f) return;
      fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
      fprintf(f, "<testsuites>\n");
   };

   void on_suite_start(h2_suite* s) override {
      h2_log::on_suite_start(s);
      if (!f) return;
      fprintf(f, "  <testsuite errors=\"0\" failures=\"%d\" hostname=\"localhost\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%d\" timestamp=\"%s\">\n",
              s->status_stats[h2_case::FAILED], s->name, s->status_stats[h2_case::TODOED] + s->status_stats[h2_case::FILTED], (int)s->cases().size(), 0, "");
   }

   void on_case_endup(h2_suite* s, h2_case* c) override {
      h2_log::on_case_endup(s, c);
      if (!f) return;
      fprintf(f, "    <testcase classname=\"%s\" name=\"%s\" status=\"%s\" time=\"%.3f\">\n",
              s->name, c->name, CSS[c->status], tc / 1000.0);

      if (c->status == h2_case::FAILED) {
         fprintf(f, "      <failure message=\"%s:%d:", c->file, c->line);
         for (h2_fail* x_fail = c->fails; x_fail; x_fail = x_fail->x_next)
            for (h2_fail* fail = x_fail; fail; fail = fail->y_next) {
               fprintf(f, "{newline}");
               fail->print(f);
            }
         fprintf(f, "\" type=\"AssertionFailedError\"></failure>\n");
      }
      fprintf(f, "      <system-out></system-out><system-err></system-err>\n");
      fprintf(f, "    </testcase>\n");
   }
   void on_suite_endup(h2_suite* s) override {
      h2_log::on_suite_endup(s);
      if (!f) return;
      fprintf(f, "  </testsuite>\n");
   }

   void on_task_endup(int status_stats[8]) override {
      h2_log::on_task_endup(status_stats);
      if (!f) return;
      fprintf(f, "</testsuites>\n");
      fclose(f);
   }
};

struct h2_log_tap : h2_log {
   /* */
};

h2_inline void h2_logs::init() {
   static h2_log_console console_log;
   static h2_log_xml xml_log;
   static h2_log_tap tap_log;
   logs.push_back(&console_log);
   if (strlen(O.junit)) logs.push_back(&xml_log);
}

h2_inline h2_fail* h2_string_equal_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (r) dont = !dont;
   if (a.equals(e, caseless) == !dont) return nullptr;
   if (a.wildcard_match(e, caseless) == !dont) return nullptr;
   if (a.regex_match(e, caseless) == !dont) return nullptr;

   h2_fail* fail;
   if (dont) {
      fail = (h2_fail*)new h2_fail_unexpect();
      fail->mprintf("should not %sequals", caseless ? "caseless " : "");
   } else {
      fail = (h2_fail*)new h2_fail_strcmp(e, a, caseless);
      fail->mprintf("not %sequals", caseless ? "caseless " : "");
   }
   fail->eprintf("\"%s\"", e.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   return fail;
}

h2_inline h2_fail* h2_float_equal_matches::matches(const long double a, bool caseless, bool dont) const {
   if (r) dont = !dont;
   // the machine epsilon has to be scaled to the magnitude of the values used
   // and multiplied by the desired precision in ULPs (units in the last place)
   // bool result = std::fabs(a - e) < std::numeric_limits<double>::epsilon() * std::fabs(a + e) * 2
   //      || std::fabs(a - e) < std::numeric_limits<double>::min();  // unless the result is subnormal
   bool result = std::fabs(a - e) < 0.00001;
   if (result == !dont) return nullptr;
   return h2_common_unexpect(a, e, dont, "equals");
}

h2_inline h2_fail* h2_memcmp_matches::matches(const void* a, bool caseless, bool dont) const {
   int sz = size ? size : strlen((const char*)e);
   if ((memcmp(e, a, sz) == 0) == !dont) return nullptr;
   h2_fail_memcmp* fail = new h2_fail_memcmp((const unsigned char*)e, (const unsigned char*)a, sz);
   if (dont)
      fail->mprintf("shoud not %d bytes equals", sz);
   else
      fail->mprintf("not %d bytes equals", sz);
   return fail;
}

h2_inline h2_fail* h2_regex_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.regex_match(e, caseless) == !dont) return nullptr;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("/%s/", e.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   if (dont)
      fail->mprintf("shoud not Regex matches");
   else
      fail->mprintf("not Regex matches");

   return fail;
}

h2_inline h2_fail* h2_wildcard_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.wildcard_match(e, caseless) == !dont) return nullptr;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("/%s/", e.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   if (dont)
      fail->mprintf("shoud not Wildcard matches");
   else
      fail->mprintf("not Wildcard matches");

   return fail;
}

h2_inline h2_fail* h2_strcmp_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.equals(e, caseless) == !dont) return nullptr;
   h2_fail* fail;
   if (dont) {
      fail = (h2_fail*)new h2_fail_unexpect();
      fail->mprintf("should not %sequals", caseless ? "caseless " : "");
   } else {
      fail = (h2_fail*)new h2_fail_strcmp(e, a, caseless);
      fail->mprintf("not %sequals", caseless ? "caseless " : "");
   }
   fail->eprintf("\"%s\"", e.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   return fail;
}

h2_inline h2_fail* h2_contains_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.contains(substring, caseless) == !dont) return nullptr;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("\"%s\"", substring.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   if (dont)
      fail->mprintf("shoud not %shas substr", caseless ? "caseless " : "");
   else
      fail->mprintf("not %shas substr", caseless ? "caseless " : "");

   return fail;
}

h2_inline h2_fail* h2_startswith_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.startswith(prefix_string, caseless) == !dont) return nullptr;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("\"%s\"", prefix_string.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   if (dont)
      fail->mprintf("shoud not %sstarts with", caseless ? "caseless " : "");
   else
      fail->mprintf("not %sstarts with", caseless ? "caseless " : "");

   return fail;
}

h2_inline h2_fail* h2_endswith_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if (a.endswith(suffix_string, caseless) == !dont) return nullptr;
   h2_fail_unexpect* fail = new h2_fail_unexpect();
   fail->eprintf("\"%s\"", suffix_string.c_str());
   fail->aprintf("\"%s\"", a.c_str());
   if (dont)
      fail->mprintf("shoud not %sends with", caseless ? "caseless " : "");
   else
      fail->mprintf("not %sends with", caseless ? "caseless " : "");

   return fail;
}

h2_inline h2_fail* h2_json_matches::matches(const h2_string& a, bool caseless, bool dont) const {
   if ((h2_json_exporter::match(e, a)) == !dont) return nullptr;
   h2_fail_json* fail = new h2_fail_json(e, a);
   if (dont)
      fail->mprintf("should not equals");
   else
      fail->mprintf("not equals");

   return fail;
}

static inline bool is_ipv4(const char* str) {
   int s1, s2, s3, s4;
   return 4 == ::sscanf(str, "%d.%d.%d.%d", &s1, &s2, &s3, &s4);
}

struct h2_network {
   h2_singleton(h2_network);

   h2_list dnses, socks;

   static bool inet_addr(const char* str, struct sockaddr_in* addr) {
      int s1, s2, s3, s4;
      if (4 == ::sscanf(str, "%d.%d.%d.%d", &s1, &s2, &s3, &s4)) {
         addr->sin_family = AF_INET;
         addr->sin_addr.s_addr = ::inet_addr(str);
         return true;
      }
      return false;
   }

   static bool iport_parse(const char* str, struct sockaddr_in* addr) {  // todo for simplify
      char* colon = NULL;
      char temp[1024];
      strcpy(temp, str);

      addr->sin_family = AF_INET;
      addr->sin_port = 0;
      colon = strchr(temp, ':');
      if (colon) {
         *colon = '\0';
         addr->sin_port = htons(atoi(colon + 1));
      }
      addr->sin_addr.s_addr = ::inet_addr(temp);
      return true;
   }

   static const char* iport_tostring(struct sockaddr_in* addr, char* str) {
      sprintf(str, "%s:%d", inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
      return str;
   }

   static bool isblock(int sockfd) {
      return !(fcntl(sockfd, F_GETFL) & O_NONBLOCK);
   }

   struct temporary_noblock : h2_once {
      int sockfd;
      int flags;
      temporary_noblock(int sockfd_) : sockfd(sockfd_) {
         flags = fcntl(sockfd, F_GETFL);
         fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
      }
      ~temporary_noblock() { fcntl(sockfd, F_SETFL, flags); }
   };

   static const char* getsockname(int sockfd, char* s, struct sockaddr_in* a = nullptr) {
      struct sockaddr_in b;
      if (!a) a = &b;
      socklen_t l = sizeof(struct sockaddr_in);
      ::getsockname(sockfd, (struct sockaddr*)a, &l);
      return iport_tostring(a, s);
   }

   h2_dns* find(const char* hostname) {
      h2_list_for_each_entry(p, &dnses, h2_dns, y) if (streq("*", p->hostname) || streq(hostname, p->hostname)) return p;
      return nullptr;
   }

   static int getaddrinfo(const char* hostname, const char* servname, const struct addrinfo* hints, struct addrinfo** res) {
      h2_dns* dns = I().find(hostname);
      if (!dns) return -1;

      static struct addrinfo addrinfos[32];
      static struct sockaddr_in sockaddrs[32];
      memset(addrinfos, 0, sizeof(addrinfos));
      memset(sockaddrs, 0, sizeof(sockaddrs));

      struct addrinfo** pp = res;
      for (int i = 0; i < dns->count; ++i) {
         struct addrinfo* a = &addrinfos[i];
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(dns->array[i], b)) {
            a->ai_addr = (struct sockaddr*)b;
            a->ai_addrlen = sizeof(struct sockaddr_in);
         } else
            a->ai_canonname = dns->array[i];
         if (hints) {
            a->ai_family = hints->ai_family;
            a->ai_socktype = hints->ai_socktype;
            a->ai_protocol = hints->ai_protocol;
         } else {
            a->ai_family = AF_INET;
            a->ai_socktype = SOCK_STREAM;
            a->ai_protocol = IPPROTO_TCP;
         }
         a->ai_next = nullptr;
         *pp = a, pp = &a->ai_next;
      }
      *pp = nullptr;
      return 0;
   }

   static void freeaddrinfo(struct addrinfo* ai) {}

   static struct hostent* gethostbyname(char* name) {
      h2_dns* dns = I().find(name);
      if (!dns) return nullptr;

      static struct sockaddr_in sockaddrs[32];
      static char* h_aliases[32];
      static char* h_addr_list[32];
      static struct hostent h;

      h.h_name = name;
      h.h_addrtype = AF_INET;
      h.h_aliases = h_aliases;
      h.h_addr_list = h_addr_list;

      memset(sockaddrs, 0, sizeof(sockaddrs));
      memset(h_aliases, 0, sizeof(h_aliases));
      memset(h_addr_list, 0, sizeof(h_addr_list));

      for (int i = 0, a = 0, c = 0; i < dns->count; ++i) {
         struct sockaddr_in* b = &sockaddrs[i];
         if (inet_addr(dns->array[i], b))
            h_addr_list[a++] = (char*)&b->sin_addr;
         else
            h_aliases[c++] = dns->array[i];
      }
      return &h;
   }

   static void read_incoming(int sockfd, h2_list& list, bool sync, h2_packet*& packet) {
      bool block = isblock(sockfd);
      const char* local = getsockname(sockfd, (char*)alloca(64));
      do {
         h2_list_for_each_entry(p, &list, h2_packet, x) if (p->can_recv(local) && (!sync || p->data.length() == 0)) {
            p->x.out();
            packet = p;
            return;
         }

         if (block) h2_sleep(100);
      } while (block);
   }

   static int accept(int socket, struct sockaddr* address, socklen_t* address_len) {
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
      h2_packet* tcp = nullptr;
      read_incoming(socket, sock->incoming_tcps, true, tcp);
      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      iport_parse(tcp->from.c_str(), (struct sockaddr_in*)address);
      *address_len = sizeof(struct sockaddr_in);

      int fd = ::socket(AF_INET, SOCK_STREAM, 0);
      struct sockaddr_in a;
      const char* c = getsockname(socket, (char*)alloca(64), &a);
      ::bind(fd, (struct sockaddr*)&a, sizeof(a));
      sock->sockets.push_back({fd, c, tcp->from.c_str()});
      delete tcp;

      return fd;
   }

   static int connect(int socket, const struct sockaddr* address, socklen_t address_len) {
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
      sock->sockets.push_back({socket, getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)address, (char*)alloca(64))});
      h2_packet* tcp = nullptr;
      read_incoming(socket, sock->incoming_tcps, true, tcp);

      if (!tcp) {
         errno = EWOULDBLOCK;
         return -1;
      }
      delete tcp;
      return 0;
   }

   static ssize_t send(int socket, const void* buffer, size_t length, int flags) {
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
      if (sock) sock->put_outgoing_tcp(socket, (const char*)buffer, length);
      return length;
   }
   static ssize_t sendmsg(int socket, const struct msghdr* message, int flags) {
      return sendto(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, message->msg_namelen);
   }
   static ssize_t sendto(int socket, const void* buffer, size_t length, int flags, const struct sockaddr* dest_addr, socklen_t dest_len) {
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
      if (sock) sock->put_outgoing_udp(getsockname(socket, (char*)alloca(64)), iport_tostring((struct sockaddr_in*)dest_addr, (char*)alloca(64)), (const char*)buffer, length);
      return length;
   }
   static ssize_t recv(int socket, void* buffer, size_t length, int flags) {
      ssize_t ret = 0;
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);

      h2_packet* tcp = nullptr;
      read_incoming(socket, sock->incoming_tcps, false, tcp);

      if (tcp) {
         ret = tcp->data.copy((char*)buffer, tcp->data.length(), 0);
         delete tcp;
      }
      return ret;
   }
   static ssize_t recvfrom(int socket, void* buffer, size_t length, int flags, struct sockaddr* address, socklen_t* address_len) {
      ssize_t ret = 0;
      h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);

      h2_packet* udp = nullptr;
      read_incoming(socket, sock->incoming_udps, false, udp);

      if (udp) {
         ret = udp->data.copy((char*)buffer, udp->data.length(), 0);
         iport_parse(udp->from.c_str(), (struct sockaddr_in*)address);
         *address_len = sizeof(struct sockaddr_in);
         delete udp;
      }
      return ret;
   }
   static ssize_t recvmsg(int socket, struct msghdr* message, int flags) {
      return recvfrom(socket, message->msg_iov[0].iov_base, message->msg_iov[0].iov_len, 0, (struct sockaddr*)message->msg_name, &message->msg_namelen);
   }

   h2_stubs stubs;
   h2_network() {
      stubs.add((void*)::getaddrinfo, (void*)getaddrinfo, "", "", __FILE__, __LINE__);
      stubs.add((void*)::freeaddrinfo, (void*)freeaddrinfo, "", "", __FILE__, __LINE__);
      stubs.add((void*)::gethostbyname, (void*)gethostbyname, "", "", __FILE__, __LINE__);
   }
   ~h2_network() { stubs.clear(); }
};

h2_inline bool h2_packet::can_recv(const char* local_iport) { return h2_wildcard_match(to.c_str(), local_iport); }

h2_inline h2_sock::h2_sock() {
   stubs.add((void*)::sendto, (void*)h2_network::sendto, "", "", __FILE__, __LINE__);
   stubs.add((void*)::recvfrom, (void*)h2_network::recvfrom, "", "", __FILE__, __LINE__);
   stubs.add((void*)::sendmsg, (void*)h2_network::sendmsg, "", "", __FILE__, __LINE__);
   stubs.add((void*)::recvmsg, (void*)h2_network::recvmsg, "", "", __FILE__, __LINE__);
   stubs.add((void*)::send, (void*)h2_network::send, "", "", __FILE__, __LINE__);
   stubs.add((void*)::recv, (void*)h2_network::recv, "", "", __FILE__, __LINE__);
   stubs.add((void*)::accept, (void*)h2_network::accept, "", "", __FILE__, __LINE__);
   stubs.add((void*)::connect, (void*)h2_network::connect, "", "", __FILE__, __LINE__);
   strcpy(last_to, "0.0.0.0:0");
}

h2_inline h2_sock::~h2_sock() {
   x.out();
   y.out();
   stubs.clear();
}

h2_inline void h2_sock::put_outgoing_udp(const char* from, const char* to, const char* data, size_t size) {
   strcpy(last_to, to);
   outgoing_udps.push_back(&(new h2_packet(from, to, data, size))->x);
}

h2_inline void h2_sock::put_incoming_udp(const char* from, const char* to, const char* data, size_t size) {
   incoming_udps.push_back(&(new h2_packet(from ? from : last_to, to, data, size))->x);
}

h2_inline void h2_sock::put_outgoing_tcp(int fd, const char* data, size_t size) {
   char from[128], to[128];

   for (auto& t : sockets)
      if (t.fd == fd) {
         strcpy(from, t.from.c_str());
         strcpy(to, t.to.c_str());
         break;
      }

   strcpy(last_to, to);

   h2_packet* tcp = nullptr;
   h2_list_for_each_entry(p, &outgoing_tcps, h2_packet, x) h2_if_find_break(p->from == from && p->to == to, p, tcp);
   if (tcp)
      tcp->data.append(data, size);
   else
      outgoing_tcps.push_back(&(new h2_packet(from, to, data, size))->x);
}

h2_inline void h2_sock::put_incoming_tcp(const char* from, const char* to, const char* data, size_t size) {
   from = from ? from : last_to;
   h2_packet* tcp = nullptr;
   h2_list_for_each_entry(p, &incoming_tcps, h2_packet, x) h2_if_find_break(p->from == from && p->to == to, p, tcp);
   if (tcp)
      tcp->data.append(data, size);
   else
      incoming_tcps.push_back(&(new h2_packet(from, to, data, size))->x);
}

h2_inline void h2_dnses::add(h2_dns* dns) { s.push(&dns->x); }
h2_inline void h2_dnses::clear() {
   h2_list_for_each_entry(p, &s, h2_dns, x) {
      p->x.out();
      p->y.out();
      delete p;
   }
}

h2_inline void h2_network_exporter::setaddrinfo(int n, ...) {
   if (n == 0) return;

   const char* array[32];
   int count = 0;
   va_list a;
   va_start(a, n);
   for (int i = 0; i < n && i < 32; ++i)
      array[count++] = va_arg(a, const char*);
   va_end(a);

   const char* hostname = "*";
   for (int i = 0; i < count; ++i)
      if (!is_ipv4(array[i]))
         if (strlen(hostname) < 2 || strlen(array[i]) < strlen(hostname))
            hostname = array[i];

   h2_dns* dns = new h2_dns(hostname);

   for (int i = 0; i < count; ++i)
      if (!streq(hostname, array[i]))
         strcpy(dns->array[dns->count++], array[i]);

   h2_network::I().dnses.push(&dns->y);
   if (h2_task::I().current_case) h2_task::I().current_case->dnses.add(dns);
}

h2_inline h2_packet* h2_network_exporter::sock_start_and_fetch() {
   if (!h2_task::I().current_case) return nullptr;

   h2_sock* sock = h2_task::I().current_case->sock;
   if (!sock) {
      sock = h2_task::I().current_case->sock = new h2_sock();
      h2_network::I().socks.push(&sock->y);
   }

   h2_packet* ret = nullptr;
   ret = h2_list_pop_entry(&sock->outgoing_udps, h2_packet, x);
   if (!ret)
      ret = h2_list_pop_entry(&sock->outgoing_tcps, h2_packet, x);
   return ret;
}

h2_inline void h2_network_exporter::udp_inject_received(const unsigned char* packet, size_t size, const char* from, const char* to) {
   h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
   if (sock) sock->put_incoming_udp(from, to, (const char*)packet, size);
}

h2_inline void h2_network_exporter::tcp_inject_received(const unsigned char* packet, size_t size, const char* from, const char* to) {
   h2_sock* sock = h2_list_top_entry(&h2_network::I().socks, h2_sock, y);
   if (sock) sock->put_incoming_tcp(from, to, (const char*)packet, size);
}

struct getopt {
   int argc;
   const char* argv[100];
   int offset;
   getopt(int argc_, const char** argv_) : argc(argc_), offset(0) {
      for (int i = 0; i < argc; ++i)
         argv[i] = argv_[i];
   }
   const char* next() {
      for (; offset < argc; ++offset)
         if (argv[offset])
            return argv[offset++];
      return nullptr;
   }
   const char* extract() {
      const char **pp = nullptr, *p = nullptr;
      for (int i = offset; i < argc; ++i)
         if (argv[i]) {
            pp = argv + i;
            break;
         }

      if (pp && *pp[0] != '-') p = *pp, *pp = nullptr;
      return p;
   }
   const char* parseint(const char* p, int& value) const {
      for (value = 0; ::isdigit(*p); p++) value = value * 10 + (*p - '0');
      return p - 1;
   }

   void args(char* s) {
      for (int i = 0; i < argc; ++i)
         s += sprintf(s, " %s", argv[i]);
   }
};

h2_inline void h2_option::parse(int argc_, const char** argv_) {
   path = argv_[0];
   getopt get(argc_ - 1, argv_ + 1);
   get.args(args);

   for (const char* p; p = get.next();) {
      if (p[0] != '-') continue;
      for (const char* t; *p; p++) {
         switch (*p) {
         case 'v': verbose = 1; break;
         case 'l':
            listing = 'A';
            if ((t = get.extract())) listing = t[0];
            break;
         case 'b':
            breakable = 1;
            if (::isdigit(*(p + 1)))
               p = get.parseint(p + 1, breakable);
            else if ((t = get.extract()))
               breakable = atoi(t);
            break;
         case 'c': colorable = !colorable; break;
         case 'r':
            randomize = 'A';
            if ((t = get.extract())) randomize = t[0];
            break;
         case 'm': memory_check = !memory_check; break;
         case 'd': debug = "gdb new"; break;
         case 'D': debug = "gdb attach"; break;
         case 'j':
            sprintf(junit, "%s.xml", path);
            if ((t = get.extract())) strcpy(junit, t);
            break;
         case 'i':
            while ((t = get.extract())) include_patterns.push_back(t);
            break;
         case 'x':
            while ((t = get.extract())) exclude_patterns.push_back(t);
            break;
         case '-': break;
         case 'h':
         case '?':
            usage();
            exit(0);
         default:
            printf("unknown option: -%c, -h for help\n", *p);
            exit(0);
         }
      }
   }
}

h2_inline void h2_option::usage() {
   printf("Usage:\n"
          "-v                  Make the operation more talkative\n"
          "-l [sca]            List out suites and cases\n"
          "-b [n]              Breaking test once n (default is 1) failures occurred\n"
          "-c                  Output in black-white color mode\n"
          "-r [sca]            Run cases in random order\n"
          "-m                  Run cases without memory check\n"
          "-d/D                Debug mode, -D for gdb attach but sudo requires password\n"
          "-j [path]           Generate junit report, default is .xml\n"
          "-i {patterns}       Run cases which case name, suite name or file name matches\n"
          "-x {patterns}       Run cases which case name, suite name and file name not matches\n");
}

static inline bool match3(const std::vector<const char*>& patterns, const char* subject) {
   for (auto pattern : patterns)
      if (strcspn(pattern, "?*+^$\\.[]") < strlen(pattern)) {
         if (h2_regex_match(pattern, subject)) return true;
         if (strcspn(pattern, "+^$\\.[]") == strlen(pattern))
            if (h2_wildcard_match(pattern, subject)) return true;
      } else {
         if (strstr(subject, pattern)) return true;
      }
   return false;
}

h2_inline bool h2_option::filter(const char* suitename, const char* casename, const char* filename) const {
   if (include_patterns.size())
      if (!match3(include_patterns, suitename) && !match3(include_patterns, casename) && !match3(include_patterns, filename))
         return true;
   if (exclude_patterns.size())
      if (match3(exclude_patterns, suitename) || match3(exclude_patterns, casename) || match3(exclude_patterns, filename))
         return true;
   return false;
}

h2_inline const char* h2_option::style(const char* s) const {
   static char shift_buffer[32][128];
   static long shift_index = 0;
   if (!colorable) return "";
   return h2_style(s, shift_buffer[++shift_index % 32]);
}

struct h2_stdio {
   h2_singleton(h2_stdio);

   char buffer[1024 * 1024], *p;
   int offset, size;

   // write, pwrite, writev
   static ssize_t write(int fd, const void* buf, size_t count) {
      if (I().p && I().offset + count < I().size) {
         memcpy(I().p + I().offset, buf, count);
         I().offset += count;
         I().p[I().offset] = '\0';
      }
      return count;
   }

   static int vprintf(const char* format, va_list ap) {
      int ret = 0;
      if (I().p) {
         ret = vsnprintf(I().p + I().offset, I().size - I().offset, format, ap);
         I().offset += ret;
      }
      return ret;
   }

   static int printf(const char* format, ...) {
      va_list a;
      va_start(a, format);
      int ret = vprintf(format, a);
      va_end(a);
      return ret;
   }

   static int putchar(int c) {
      if (I().p && I().offset + 1 < I().size) {
         I().p[I().offset++] = c;
         I().p[I().offset] = '\0';
      }
      return c;
   }

   static int puts(const char* s) {
      int len = strlen(s);
      if (I().p && I().offset + len < I().size) {
         strcpy(I().p + I().offset, s);
         I().offset += len;
      }
      return 1;
   }

   static int fprintf(FILE* stream, const char* format, ...) {
      if (stream != stdout && stream != stderr) {
         va_list a, b;
         va_start(a, format);
         int len = vsnprintf(nullptr, 0, format, a);
         va_end(a);
         char* t = (char*)alloca(len + 1);
         va_start(b, format);
         len = vsnprintf(t, len + 1, format, b);
         va_end(b);
         return h2_libc::write(stream, t, len);
      }

      va_list a;
      va_start(a, format);
      int ret = vprintf(format, a);
      va_end(a);
      return ret;
   }

   static int vfprintf(FILE* stream, const char* format, va_list ap) {
      if (stream != stdout && stream != stderr) {
         va_list a, b;
         va_copy(a, ap);
         va_copy(b, ap);

         int len = vsnprintf(nullptr, 0, format, a);
         char* t = (char*)alloca(len + 1);
         len = vsnprintf(t, len + 1, format, b);
         return h2_libc::write(stream, t, len);
      }

      return vprintf(format, ap);
   }

   static int fputc(int c, FILE* stream) {
      if (stream != stdout && stream != stderr) {
         unsigned char t = c;
         return 1 == h2_libc::write(stream, &t, 1) ? c : EOF;
      }

      return putchar(c);
   }

   static int fputs(const char* s, FILE* stream) {
      if (stream != stdout && stream != stderr) {
         return h2_libc::write(stream, s, strlen(s));
      }

      return puts(s);
   }

   static size_t fwrite(const void* ptr, size_t size, size_t nitems, FILE* stream) {
      if (stream != stdout && stream != stderr) {
         return h2_libc::write(stream, ptr, size * nitems);
      }

      size_t len = size * nitems;
      if (I().p && I().offset + len < I().size) {
         memcpy(I().p + I().offset, ptr, len);
         I().offset += len;
         I().p[I().offset] = '\0';
      }
      return len;
   }

   static const void start_capture(char* buffer, int size) {
      h2_stub_g((void*)::printf, (void*)printf, "", "", "", 0);
      h2_stub_g((void*)::putchar, (void*)putchar, "", "", "", 0);
      h2_stub_g((void*)::puts, (void*)puts, "", "", "", 0);
      h2_stub_g((void*)::vprintf, (void*)vprintf, "", "", "", 0);

      h2_stub_g((void*)::fprintf, (void*)fprintf, "", "", "", 0);
      h2_stub_g((void*)::fputc, (void*)fputc, "", "", "", 0);
      h2_stub_g((void*)::putc, (void*)fputc, "", "", "", 0);
      h2_stub_g((void*)::fputs, (void*)fputs, "", "", "", 0);
      h2_stub_g((void*)::fwrite, (void*)fwrite, "", "", "", 0);

#if defined __APPLE__
      h2_stub_g((void*)::vfprintf, (void*)vfprintf, "", "", "", 0);
#endif

      if (buffer) {
         I().size = size;
         I().p = buffer;
      } else {
         I().size = sizeof(I().buffer);
         I().p = I().buffer;
      }

      I().offset = 0;
      I().p[0] = '\0';
   }

   static const char* stop_capture() {
      fflush(stdout);
      fflush(stderr);
      std::cout << std::flush;

      char* buffer = I().p;
      I().offset = 0;
      I().size = 0;
      I().p = nullptr;

      h2_stub_g((void*)::printf, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::putchar, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::puts, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::vprintf, (void*)0, "", "", "", 0);

      h2_stub_g((void*)::fprintf, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::fputc, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::putc, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::fputs, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::fwrite, (void*)0, "", "", "", 0);

#if defined __APPLE__
      h2_stub_g((void*)::vfprintf, (void*)0, "", "", "", 0);
#endif

      return buffer;
   }
};

h2_inline void h2_stdio_exporter::capture_cout(char* buffer, int size) {
   h2_stdio::I().start_capture(buffer, size);
}

h2_inline const char* h2_stdio_exporter::capture_cout() {
   return h2_stdio::I().stop_capture();
}

h2_inline bool h2_string::equals(h2_string __str, bool caseless) const {
   if (!caseless) return *this == __str;
   return tolower(c_str()) == tolower(__str);
}

h2_inline bool h2_string::contains(h2_string __substr, bool caseless) const {
   if (!caseless) return find(__substr) != h2_string::npos;
   return tolower(c_str()).find(tolower(__substr)) != h2_string::npos;
}

h2_inline bool h2_string::startswith(h2_string __prefix, bool caseless) const {
   if (!caseless) return find(__prefix) == 0;
   return tolower(c_str()).find(tolower(__prefix)) == 0;
}

h2_inline bool h2_string::endswith(h2_string __suffix, bool caseless) const {
   if (!caseless) return rfind(__suffix) == length() - __suffix.length();
   return tolower(c_str()).rfind(tolower(__suffix)) == length() - __suffix.length();
}

h2_inline bool h2_string::wildcard_match(h2_string __pattern, bool caseless) const {
   h2_wildcard_match(__pattern.c_str(), c_str());
   if (!caseless) return h2_wildcard_match(__pattern.c_str(), c_str());
   return h2_wildcard_match(tolower(__pattern).c_str(), tolower(c_str()).c_str());
}

h2_inline bool h2_string::regex_match(h2_string __pattern, bool caseless) const {
   return h2_regex_match(__pattern.c_str(), c_str(), caseless);
}

h2_inline h2_string& h2_string::tolower() {
   for (auto& c : *this) c = ::tolower(c);
   return *this;
}

h2_inline h2_string h2_string::acronym(int atmost) const {
   char t[256];
   strncpy(t, c_str(), atmost);
   strcpy(t + atmost, O.colorable ? "\033[90m...\033[0m" : "...");
   return h2_string(t);
}

h2_inline h2_string& h2_string::center(int width) {
   int left = (width - length()) / 2, right = width - left - length();
   insert(0, left, ' ');
   append(right, ' ');
   return *this;
}

h2_inline h2_string& h2_string::sprintf(const char* format, ...) {
   va_list a;
   va_start(a, format);
#if defined _WIN32
   int length = _vscprintf(format, a);
#else
   int length = vsnprintf(nullptr, 0, format, a);
#endif
   va_end(a);
   char* t = (char*)alloca(length + 1);
   va_start(a, format);
   vsprintf(t, format, a);
   va_end(a);
   append(t);
   return *this;
}
// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_thunk {
#if defined __x86_64__
   unsigned char saved_code[sizeof(void*) + 4];
#elif defined __i386__
   unsigned char saved_code[sizeof(void*) + 1];
#endif

   void* save(void* befp) {
      static uintptr_t pagesize = (uintptr_t)sysconf(_SC_PAGE_SIZE);
      uintptr_t start = reinterpret_cast<uintptr_t>(befp);
      uintptr_t pagestart = start & (~(pagesize - 1));
      int pagecount = ::ceil((start + sizeof(saved_code) - pagestart) / (double)pagesize);

      if (mprotect(reinterpret_cast<void*>(pagestart), pagecount * pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         ::printf("STUB: mprotect PROT_READ | PROT_WRITE | PROT_EXEC failed %s\n", strerror(errno));
         return nullptr;
      }

      memcpy(saved_code, befp, sizeof(saved_code));
      return befp;
   }

   void set(void* befp, void* tofp) {
      unsigned char* I = reinterpret_cast<unsigned char*>(befp);
      ptrdiff_t delta = (unsigned char*)tofp - (unsigned char*)befp;

#if defined(__i386__) || defined(__x86_64__)
      //x86 __asm("jmp $tofp") : 0xE9 {offset=tofp-befp-5}
      //x86 __asm("movl $tofp, %eax; jmpl %eax") : 0xB8 {tofp} 0xFF 0xE0
      //x86_64 __asm("movq $tofp, %rax; jmpq %rax") : 0x48 0xB8 {tofp} 0xFF 0xE0
      if (delta < INT_MIN || INT_MAX < delta) {
         unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
         memcpy(C + 2, &tofp, sizeof(void*));
         memcpy(I, C, sizeof(C));
      } else {
         *I++ = 0xE9;
         *(int32_t*)I = delta - 5;
      }
#endif
   }

   void* reset(void* befp) { return memcpy(befp, saved_code, sizeof(saved_code)); }
};

struct h2_native : h2_thunk {
   h2_list x;
   void* befp;
   int refcount;
   h2_native(void* befp_) : befp(befp_), refcount(0) { save(befp); }
   void restore() { reset(befp); }
};

struct h2_natives {
   h2_singleton(h2_natives);

   h2_list s;
   h2_native* get(void* befp) {
      h2_list_for_each_entry(p, &s, h2_native, x) if (p->befp == befp) return p;
      return nullptr;
   }
   void dec(void* befp) {
      h2_native* p = get(befp);
      if (!p) return;
      if (--p->refcount == 0) {
         p->x.out();
         h2_libc::free(p);
      }
   }
   void inc(void* befp) {
      h2_native* p = get(befp);
      if (!p) {
         p = new (h2_libc::malloc(sizeof(h2_native))) h2_native(befp);
         s.push(&p->x);
      }
      p->refcount++;
   }
};

h2_inline h2_stub::h2_stub(void* befp_, const char* file_, int line_) : file(file_), line(line_) {
   h2_natives::I().inc(befp_);
   befp = ((h2_thunk*)thunk)->save(befp_);
}
h2_inline h2_stub::~h2_stub() {
   h2_natives::I().dec(befp);
}

h2_inline void h2_stub::set(void* tofp_) { ((h2_thunk*)thunk)->set(befp, tofp = tofp_); }
h2_inline void h2_stub::reset() { befp && ((h2_thunk*)thunk)->reset(befp); }
h2_inline void h2_stub::restore() { h2_natives::I().get(befp)->restore(); }

h2_inline bool h2_stubs::add(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line) {
   h2_stub* stub = nullptr;
   h2_list_for_each_entry(p, &s, h2_stub, x) h2_if_find_break(p->befp == befp, p, stub);

   if (!tofp) { /* unstub */
      if (stub) {
         stub->reset();
         stub->x.out();
         delete stub;
      }
      return true;
   }

   if (!stub) {
      stub = new h2_stub(befp, file, line);
      s.push(&stub->x);
   }
   stub->set(tofp);
   return true;
}

h2_inline void h2_stubs::clear() {
   h2_list_for_each_entry(p, &s, h2_stub, x) {
      p->reset();
      p->x.out();
      delete p;
   }
}

h2_inline bool h2_mocks::add(h2_mock* mock) {
   h2_list_for_each_entry(p, &s, h2_mock, x) if (p == mock) return false;
   s.push(&mock->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear() {
   h2_fail* fail = nullptr;
   h2_list_for_each_entry(p, &s, h2_mock, x) {
      h2_fail::append_x(fail, p->times_check());
      p->reset();
      p->x.out();
   }
   return fail;
}

h2_inline h2_suite::h2_suite(const char* name_, void (*p)(h2_suite*, h2_case*), const char* file_, int line_)
  : name(name_), file(file_), line(line_), seq(0), status_stats{0}, jumpable(false), test_code_plus(p) {
   h2_directory::I().suites.push_back(this);
}

h2_inline void h2_suite::cleanup() {
   stubs.clear();
   mocks.clear();
}

h2_inline std::vector<h2_case*>& h2_suite::cases() {
   if (enumerate)
      test_code_plus(this, nullptr); /* enumerate case by static local h2_case variable inside of h2_suite_test_code_plus() */
   return case_list;
}

h2_inline void h2_suite::execute(h2_case* c) {
   c->prev_setup();
   test_code_plus(this, c); /* include setup(); c->post_setup() and c->prev_cleanup(); cleanup() */
   c->post_cleanup();
}

inline h2_task::h2_task() : state(0), status_stats{0}, current_suite(nullptr), current_case(nullptr) {
   
}

inline void h2_task::prepare() {
   state = 100;
   h2_heap::dosegv();
   if (O.listing) h2_directory::list_then_exit();

   logs.init();
   h2_directory::sort();

   h2_heap::stack::root();
   h2_heap::dohook();
   state = 199;
}

inline void h2_task::postpare() {
   state = 300;
   h2_heap::unhook();
   stubs.clear();
   if (status_stats[h2_case::FAILED] == 0) h2_directory::drop_last_order();
   state = 399;
}

inline void h2_task::execute() {
   state = 200;
   logs.on_task_start(h2_directory::count());
   for (auto& setup : global_setups) setup();
   for (auto& s : h2_directory::I().suites) {
      current_suite = s;
      logs.on_suite_start(s);
      for (auto& setup : global_suite_setups) setup();
      s->setup();
      for (auto& c : s->cases()) {
         if (0 < O.breakable && O.breakable <= status_stats[h2_case::FAILED]) break;
         current_case = c;
         logs.on_case_start(s, c);
         for (auto& setup : global_case_setups) setup();
         if (O.filter(s->name, c->name, c->file)) c->status = h2_case::FILTED;
         if (h2_case::INITED == c->status) s->execute(c);
         for (auto& teardown : global_case_teardowns) teardown();
         logs.on_case_endup(s, c);
         status_stats[c->status] += 1;
         s->status_stats[c->status] += 1;
      }
      s->cleanup();
      for (auto& teardown : global_suite_teardowns) teardown();
      logs.on_suite_endup(s);
   }
   for (auto& teardown : global_teardowns) teardown();
   logs.on_task_endup(status_stats);
   state = 299;
}
}  // namespace h2

#if defined _WIN32
#   define h2_weak_attribute __declspec(selectany)
#else
#   define h2_weak_attribute __attribute__((weak))
#endif

h2_weak_attribute int main(int argc, const char** argv) {
   h2_main(argc, argv);
   return 0;
}
#endif
