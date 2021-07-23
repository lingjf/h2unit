
template <typename Signature = void>
struct h2_fp {
   template <typename T>
   static void* A(T fp) { return reinterpret_cast<void*>(fp); }
   static void* A(const char* fn) { return h2_load::get_by_fn(fn); }
};

template <typename ReturnType, typename... Args>
struct h2_fp<ReturnType(Args...)> {
   static void* A(ReturnType (*fp)(Args...)) { return (void*)fp; }
   static void* A(const char* fn) { return h2_load::get_by_fn(fn); }
};
