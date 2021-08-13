
template <typename R = void*, typename T>
static R h2_un(T fp)
{
   union h2_un {
      T fp;
      R p;
   } u;
   u.fp = fp;
   return u.p;
}

template <typename Signature = void>
struct h2_fp {
   template <typename T>
   static void* A(T fp) { return h2_un(fp); }
   static void* A(const char* fn) { return h2_load::get_by_fn(fn); }
};

template <typename ReturnType, typename... Args>
struct h2_fp<ReturnType(Args...)> {
   static void* A(ReturnType (*fp)(Args...)) { return h2_un(fp); }
   static void* A(const char* fn) { return h2_load::get_by_fn(fn); }
};
