namespace {
template <int Counter, typename ClassType, typename Signature> struct h2_stuber;
template <int Counter, typename ClassType, typename ReturnType, typename... ArgumentTypes>
struct h2_stuber<Counter, ClassType, ReturnType(ArgumentTypes...)> {
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
   ReturnType (*dstfp_)(ClassType*, ArgumentTypes...);
   struct member_calling_conversions_wrapper {
      ReturnType fx(ArgumentTypes... arguments) { return I().dstfp_((ClassType*)this, std::forward<ArgumentTypes>(arguments)...); }
   };
   void operator=(ReturnType (*dstfp)(ClassType*, ArgumentTypes...))
   {
      dstfp_ = dstfp;
      h2_runner::stub(srcfp, h2_fp<member_calling_conversions_wrapper, ReturnType(ArgumentTypes...)>::A(&member_calling_conversions_wrapper::fx), srcfn, filine);
   }
#else
   void operator=(ReturnType (*dstfp)(ClassType*, ArgumentTypes...))  // captureless lambda implicit cast to function pointer
   {
      h2_runner::stub(srcfp, (void*)dstfp, srcfn, filine);
   }
#endif
   void operator=(ReturnType (*dstfp)(ArgumentTypes...))  // stub normal function
   {
      h2_runner::stub(srcfp, (void*)dstfp, srcfn, filine);
   }
};
}
