
struct h2_exception {
   h2_singleton(h2_exception);
   h2_stubs stubs;
   h2_backtrace last_bt;
   char last_type[1024];

#if !defined _WIN32
   static void __cxa_throw(void* thrown_exception, std::type_info* ti, void (*dest)(void*))
   {  // https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
      I().last_bt = h2_backtrace::dump(1);
      strcpy(I().last_type, demangle(ti->name()));
      if (O.exception_fails) h2_fail_g(h2_fail::new_exception("was thrown", I().last_type, I().last_bt));
      h2::h2_stub_temporary_restore t((void*)abi::__cxa_throw);
      abi::__cxa_throw(thrown_exception, ti, dest);
   }
#endif

   static void initialize()
   {
#if !defined _WIN32
      I().stubs.add((void*)abi::__cxa_throw, (void*)__cxa_throw, "__cxa_throw", __FILE__, __LINE__);
#endif
   }
};
