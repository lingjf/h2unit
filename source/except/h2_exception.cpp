struct h2_exception_handler {
#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
   static void RaiseException(DWORD dwExceptionCode, DWORD dwExceptionFlags, DWORD nNumberOfArguments, const ULONG_PTR* lpArguments)
   {
      h2_exception::I().last_bt = h2_backtrace::dump(1);
      if (O.exception_as_fail) h2_runner::failing(h2_fail::new_exception("was thrown", "", h2_exception::I().last_bt));
      h2::h2_stub_temporary_restore t((void*)::RaiseException);
      ::RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
   }
#else
   static void __cxa_throw(void* thrown_exception, std::type_info* type_info, void (*dest)(void*))
   {  // https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
      h2_exception::I().thrown_exception = thrown_exception;
      h2_exception::I().type_info = type_info;
      h2_exception::I().last_bt = h2_backtrace::dump(1);
      h2_cxa::demangle(type_info->name(), h2_exception::I().last_type);
      if (h2_exception::I().catching) ::longjmp(h2_exception::I().catch_hole, 1);
      if (O.exception_as_fail) h2_runner::failing(h2_fail::new_exception("was thrown", h2_exception::I().last_type, h2_exception::I().last_bt));
      h2::h2_stub_temporary_restore t((void*)abi::__cxa_throw);
      abi::__cxa_throw(thrown_exception, type_info, dest);
   }
#endif
};

h2_inline void h2_exception::initialize()
{
   static h2_list stubs;
#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
   h2_stubs::add(stubs, (void*)::RaiseException, (void*)h2_exception_handler::RaiseException, "RaiseException", H2_FILINE);
#else
   h2_stubs::add(stubs, (void*)abi::__cxa_throw, (void*)h2_exception_handler::__cxa_throw, "__cxa_throw", H2_FILINE);
#endif
   if (!O.debugger_trap) h2_crash::install();
}

h2_inline h2_once h2_exception::try_catch()
{
   h2_exception::I().catching = true;
   h2_exception::I().thrown_exception = nullptr;
   h2_exception::I().type_info = nullptr;

   return h2_once();
}

h2_inline void h2_exception::try_catch_finally(h2_fail* fail, const char* what)
{
   // abi::__cxa_free_exception(h2_exception::I().thrown_exception);
   if (what) h2_memory::try_free((void*)what);
   h2_exception::I().thrown_exception = nullptr;
   h2_exception::I().type_info = nullptr;
   h2_exception::I().catching = false;

   h2_runner::failing(fail);
}
