namespace {
template <int Counter, typename ClassType, typename Signature>
struct h2_stuber;
template <int Counter, typename ClassType, typename ReturnType, typename... ArgumentTypes>
struct h2_stuber<Counter, ClassType, ReturnType(ArgumentTypes...)> {
   h2_singleton(h2_stuber);
   void* srcfp;
   const char* srcfn;
   const char* filine;

   ReturnType (*dstfp)(ClassType*, ArgumentTypes...);
   struct member_function_stub {  // wrap for calling conversions
      ReturnType fx(ArgumentTypes... arguments) { return I().dstfp((ClassType*)this, std::forward<ArgumentTypes>(arguments)...); }
   };

   static h2_stuber& I(void* srcfp, const char* srcfn, const char* filine)
   {
      I().srcfp = srcfp;
      I().srcfn = srcfn;
      I().filine = filine;
      return I();
   }

   void operator=(ReturnType (*dstfp_)(ClassType*, ArgumentTypes...))
   {
#if defined _WIN32 && (defined __i386__ || defined _M_IX86)  // https://docs.microsoft.com/en-us/cpp/cpp/calling-conventions
      dstfp = dstfp_;
      h2_runner::stub(srcfp, h2_fp<member_function_stub, ReturnType(ArgumentTypes...)>::A(&member_function_stub::fx), srcfn, filine);
#else
      h2_runner::stub(srcfp, (void*)dstfp_, srcfn, filine);
#endif
   }
};
}
