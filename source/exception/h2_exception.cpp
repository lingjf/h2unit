
struct h2_exception_stub {
   static void __cxa_throw(void* thrown_exception, std::type_info* tinfo, void (*dest)(void*))
   {  // https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
      h2_fail_g(h2_fail::new_normal({"A exception was thrown : ", tinfo->name()}), false);
   }
};

h2_inline void h2_exception::initialize()
{
   static h2_stubs stubs;
   if (O.exception_fails) stubs.add((void*)abi::__cxa_throw, (void*)h2_exception_stub::__cxa_throw, "__cxa_throw", __FILE__, __LINE__);
}
