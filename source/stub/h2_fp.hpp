
static inline void* get_fp_by_fn(const char* fn)
{
   h2_symbol* res[16];
   int n = h2_nm::get_by_name(fn, res, 16);
   if (n != 1) {
      h2_color::prints("yellow", n ? "\nFind multiple %s :\n" : "\nDon't find %s\n", fn);
      for (int i = 0; i < n; ++i)
         h2_color::prints("yellow", "  %d. %s \n", i + 1, res[i]->name);
      return nullptr;
   }
   return h2_load::addr_to_ptr(res[0]->addr);
}

template <typename Signature = void>
struct h2_fp {
   template <typename T>
   static void* A(T p)
   {
      void* fp = (void*)p;
      if (std::is_convertible<T, h2::h2_string>::value)
         fp = get_fp_by_fn((const char*)p);
      return fp;
   }
};

template <typename ReturnType, typename... Args>
struct h2_fp<ReturnType(Args...)> {
   static void* A(ReturnType (*f)(Args...))
   {
      return (void*)f;
   }
   static void* A(const char* f)
   {
      return get_fp_by_fn(f);
   }
};