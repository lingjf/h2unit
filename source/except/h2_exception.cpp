struct h2_exception {
   h2_singleton(h2_exception);
   h2_list stubs;
   h2_backtrace last_bt;
   char last_type[1024];

#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
   static void RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ULONG_PTR* lpArguments)
   {
      I().last_bt = h2_backtrace::dump(1);
      if (O.exception_as_fail) h2_runner::failing(h2_fail::new_exception("was thrown", "", I().last_bt));
      h2::h2_stub_temporary_restore t((void*)::RaiseException);
      ::RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
   }
#else
   static void __cxa_throw(void* thrown_exception, std::type_info* tinfo, void (*dest)(void*))
   {  // https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
      I().last_bt = h2_backtrace::dump(1);
      h2_cxa::demangle(tinfo->name(), I().last_type);
      if (O.exception_as_fail) h2_runner::failing(h2_fail::new_exception("was thrown", I().last_type, I().last_bt));
      h2::h2_stub_temporary_restore t((void*)abi::__cxa_throw);
      abi::__cxa_throw(thrown_exception, tinfo, dest);
   }
#endif

   static void initialize()
   {
#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
      h2_stubs::add(I().stubs, (void*)::RaiseException, (void*)RaiseException, "RaiseException", H2_FILE);
#else
      h2_stubs::add(I().stubs, (void*)abi::__cxa_throw, (void*)__cxa_throw, "__cxa_throw", H2_FILE);
#endif
      if (!O.debugger_trap) h2_crash::install();
   }
};
