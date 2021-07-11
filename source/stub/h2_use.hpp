
#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB_, __VA_ARGS__)
#define __H2STUB_2(Src, Dst) h2::h2_stub_g(h2::h2_fp<>::A(Src), (void*)Dst, #Src, __FILE__, __LINE__)
#define __H2STUB_4(Function, ReturnType, Arguments, Dst) h2::h2_stub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(Dst), #Function, __FILE__, __LINE__)
#define __H2STUB_5(Class, Method, ReturnType, Arguments, Dst) h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(Dst), #Class "::" #Method, __FILE__, __LINE__)

#define H2UNSTUB(...) H2PP_VARIADIC_CALL(__H2UNSTUB_, __VA_ARGS__)
#define __H2UNSTUB_1(Src) h2::h2_unstub_g(h2::h2_fp<>::A(Src))
#define __H2UNSTUB_3(Function, ReturnType, Arguments) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUB_4(Class, Method, ReturnType, Arguments) h2::h2_unstub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))

#define STUB2 H2STUB2
#define H2STUB2(...) H2PP_VARIADIC_CALL(__H2STUB2_, __VA_ARGS__)
#define __H2STUB2_2(Src, Dst) h2::h2_stub_g(h2::h2_fp<>::A(Src), (void*)Dst, #Src, __FILE__, __LINE__)
#define __H2STUB2_3(Function, Signature, Dst) h2::h2_stub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(Dst), #Function, __FILE__, __LINE__)
#define __H2STUB2_4(Class, Method, Signature, Dst) h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(Dst), #Class "::" #Method, __FILE__, __LINE__)

#define UNSTUB2 H2UNSTUB2
#define H2UNSTUB2(...) H2PP_VARIADIC_CALL(__H2UNSTUB2_, __VA_ARGS__)
#define __H2UNSTUB2_1(Src) h2::h2_unstub_g(h2::h2_fp<>::A(Src))
#define __H2UNSTUB2_2(Function, Signature) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUB2_3(Class, Method, Signature) h2::h2_unstub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))

////////////////////////////////////////////////////////////////

#define STUBS(...) H2PP_VARIADIC_CALL(__H2STUBS_, __VA_ARGS__)
#define __H2STUBS_2(Src, Dst) h2::h2_stub_g(h2::h2_fp<>::A(Src), (void*)Dst, #Src, __FILE__, __LINE__)
#define __H2STUBS_3(Function, ReturnType, Arguments) ___H2STUBS_3(Function, ReturnType, Arguments, H2PP_UNIQUE(t_stub3))
#define __H2STUBS_4(Class, Method, ReturnType, Arguments) H2PP_IF_ELSE(H2PP_IS_EMPTY Arguments, __H2STUBS_4_0(Class, Method, ReturnType, Arguments, H2PP_UNIQUE(t_stub40)), __H2STUBS_4_1(Class, Method, ReturnType, Arguments, H2PP_UNIQUE(t_stub41)))

#define ___H2STUBS_3(Function, ReturnType, Arguments, Q)                                                                                                                     \
   struct {                                                                                                                                                                  \
      void operator=(ReturnType(*dstfp) Arguments)                                                                                                                           \
      {                                                                                                                                                                      \
         h2::h2_stub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(dstfp), #Function, __FILE__, __LINE__); \
      }                                                                                                                                                                      \
   } Q;                                                                                                                                                                      \
   Q = [] Arguments -> ReturnType /* captureless lambda implicit cast to function pointer */

#define __H2STUBS_4_0(Class, Method, ReturnType, Arguments, Q)                                                                                                                                                                   \
   struct {                                                                                                                                                                                                                      \
      void operator=(ReturnType (*dstfp)(H2PP_REMOVE_PARENTHESES_IF(Class) *))                                                                                                                                                   \
      {                                                                                                                                                                                                                          \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), ReturnType Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(dstfp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                          \
   } Q;                                                                                                                                                                                                                          \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> ReturnType

#define __H2STUBS_4_1(Class, Method, ReturnType, Arguments, Q)                                                                                                                                                                   \
   struct {                                                                                                                                                                                                                      \
      void operator=(ReturnType (*dstfp)(H2PP_REMOVE_PARENTHESES_IF(Class) *, H2PP_REMOVE_PARENTHESES(Arguments)))                                                                                                               \
      {                                                                                                                                                                                                                          \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), ReturnType Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(dstfp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                          \
   } Q;                                                                                                                                                                                                                          \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> ReturnType
