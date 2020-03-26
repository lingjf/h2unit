
#define H2Q(_Prefix) H2PP_CAT5(_Prefix, _L, __LINE__, C, __COUNTER__)

template <typename U, typename = void>
struct h2_decay_impl {
   typedef U type;
};
template <>
struct h2_decay_impl<char*> {
   typedef const char* type;
};
template <typename U>
struct h2_decay_impl<U, typename std::enable_if<std::is_enum<U>::value>::type> {
   typedef int type;
};

template <typename T>
struct h2_decay {
   using type = typename h2_decay_impl<typename std::decay<T>::type>::type;
};

struct h2_once {
   int bcc;
   h2_once() : bcc(0) {}
   operator bool() { return 0 == bcc++; }
};

struct h2_with {
   FILE* f;
   int (*c)(FILE*);
   h2_with(FILE* file, int (*close)(FILE*) = ::fclose) : f(file), c(close) {}
   ~h2_with() { (f && c) && c(f); }
};

/* clang-format off */

#define h2_singleton(_Class) static _Class& I() { static _Class i; return i; }
#define h2_if_return(x, y) do { auto t__ = x; if (t__) return y(t__); } while (0)
#define h2_if_find_break(_Cond, p, found) if (_Cond) { found = p; break; }
#define h2_out_delete(p) do { p->x.out(); delete p; } while (0)
