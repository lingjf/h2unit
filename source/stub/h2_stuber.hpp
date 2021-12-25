namespace {
template <int Counter, typename Class, typename Signature> struct h2_stuber;
template <int Counter, typename Class, typename ReturnType, typename... Args>
struct h2_stuber<Counter, Class, ReturnType(Args...)> {
   h2_singleton(h2_stuber);
   void* srcfp;
   const char* srcfn;
   const char* filine;

   static h2_stuber& I(void* srcfp, const char* srcfn, const char* filine)
   {
      I().srcfp = srcfp;
      I().srcfn = srcfn;
      I().filine = filine;
      return I();
   }

#if defined _WIN32 && (defined __i386__ || defined _M_IX86)
   ReturnType (*dstfp_)(Class*, Args...);
   struct member_calling_conversions_wrapper {
      ReturnType fx(Args... args) { return I().dstfp_((Class*)this, std::forward<Args>(args)...); }
   };
   void operator=(ReturnType (*dstfp)(Class*, Args...))
   {
      dstfp_ = dstfp;
      h2_runner::stub(srcfp, h2_fp<member_calling_conversions_wrapper, ReturnType(Args...)>::get(&member_calling_conversions_wrapper::fx), srcfn, filine);
   }
#else
   void operator=(ReturnType (*dstfp)(Class*, Args...))  // captureless lambda implicit cast to function pointer
   {
      h2_runner::stub(srcfp, (void*)dstfp, srcfn, filine);
   }
#endif
   void operator=(ReturnType (*dstfp)(Args...))  // stub normal function
   {
      h2_runner::stub(srcfp, (void*)dstfp, srcfn, filine);
   }
};
}
