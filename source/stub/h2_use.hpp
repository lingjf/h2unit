
#define __H2STUB2(Src, Dst) h2::h2_stub_g(h2::h2_fp(Src), (void*)Dst, #Src, __FILE__, __LINE__)
#define __H2STUB4(Function, ReturnType, Arguments, Dst) h2::h2_stub_g(h2::h2_fp((H2PP_REMOVE_PARENTHESES_IF(ReturnType)(*) Arguments)H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(Dst), #Function, __FILE__, __LINE__)
#define __H2STUB5(Class, Method, ReturnType, Arguments, Dst) h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(Dst), #Class "::" #Method, __FILE__, __LINE__)
#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__)

#define __H2UNSTUB1(Src) h2::h2_unstub_g(h2::h2_fp(Src))
#define __H2UNSTUB3(Function, ReturnType, Arguments) h2::h2_unstub_g(h2::h2_fp((H2PP_REMOVE_PARENTHESES_IF(ReturnType)(*) Arguments)H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUB4(Class, Method, ReturnType, Arguments) h2::h2_unstub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define H2UNSTUB(...) H2PP_VARIADIC_CALL(__H2UNSTUB, __VA_ARGS__)

////////////////////////////////////////////////////////////////

#define __H3STUB2(Src, Dst) \
   h2::h2_stub_g(h2::h2_fp(Src), (void*)Dst, #Src, __FILE__, __LINE__)

#define ____H3STUB3(Function, ReturnType, Arguments, Q)                                     \
   struct {                                                                                 \
      void operator=(ReturnType(*dstfp) Arguments)                                          \
      {                                                                                     \
         h2::h2_stub_g(h2::h2_fp(Function), (void*)(dstfp), #Function, __FILE__, __LINE__); \
      }                                                                                     \
   } Q;                                                                                     \
   Q = [] Arguments -> ReturnType /* captureless lambda implicit cast to function pointer */

#define __H3STUB3(Function, ReturnType, Arguments) ____H3STUB3(Function, ReturnType, Arguments, H2PP_UNIQUE(t_stub3))

#define __H3STUB40(Class, Method, ReturnType, Arguments, Q)                                                                                                                                                                      \
   struct {                                                                                                                                                                                                                      \
      void operator=(ReturnType (*dstfp)(H2PP_REMOVE_PARENTHESES_IF(Class) *))                                                                                                                                                   \
      {                                                                                                                                                                                                                          \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), ReturnType Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(dstfp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                          \
   } Q;                                                                                                                                                                                                                          \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> ReturnType

#define __H3STUB41(Class, Method, ReturnType, Arguments, Q)                                                                                                                                                                      \
   struct {                                                                                                                                                                                                                      \
      void operator=(ReturnType (*dstfp)(H2PP_REMOVE_PARENTHESES_IF(Class) *, H2PP_REMOVE_PARENTHESES(Arguments)))                                                                                                               \
      {                                                                                                                                                                                                                          \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), ReturnType Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(dstfp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                          \
   } Q;                                                                                                                                                                                                                          \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> ReturnType

#define __H3STUB4(Class, Method, ReturnType, Arguments) \
   H2PP_IF_ELSE(H2PP_IS_EMPTY Arguments, __H3STUB40(Class, Method, ReturnType, Arguments, H2PP_UNIQUE(t_stub40)), __H3STUB41(Class, Method, ReturnType, Arguments, H2PP_UNIQUE(t_stub41)))

#define Stub(...) H2PP_VARIADIC_CALL(__H3STUB, __VA_ARGS__)
