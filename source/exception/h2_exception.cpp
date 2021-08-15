
struct h2_exception {
   h2_singleton(h2_exception);
   h2_stubs stubs;
   h2_backtrace last_bt;
   char last_type[1024];

#if defined _WIN32
   static void RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ULONG_PTR* lpArguments)
   {
      I().last_bt = h2_backtrace::dump(1);
      if (O.exception_fails) h2_fail_g(h2_fail::new_exception("was thrown", "", I().last_bt));
      h2::h2_stub_temporary_restore t((void*)::RaiseException);
      ::RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
   }
#else
   static void __cxa_throw(void* thrown_exception, std::type_info* ti, void (*dest)(void*))
   {  // https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
      I().last_bt = h2_backtrace::dump(1);
      strcpy(I().last_type, h2_nm::demangle(ti->name()));
      if (O.exception_fails) h2_fail_g(h2_fail::new_exception("was thrown", I().last_type, I().last_bt));
      h2::h2_stub_temporary_restore t((void*)abi::__cxa_throw);
      abi::__cxa_throw(thrown_exception, ti, dest);
   }
#endif

   static void initialize()
   {
#if defined _WIN32
      I().stubs.add((void*)::RaiseException, (void*)RaiseException, "RaiseException", __FILE__, __LINE__);
#else
      I().stubs.add((void*)abi::__cxa_throw, (void*)__cxa_throw, "__cxa_throw", __FILE__, __LINE__);
#endif
      if (!O.debug) h2_crash::install();
   }
};
