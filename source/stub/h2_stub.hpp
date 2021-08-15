
#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB_, __VA_ARGS__)
#define __H2STUB_2(Src, Dst) h2::h2_stub_g(h2::h2_fp<>::A(Src), (void*)Dst, #Src, __FILE__, __LINE__)
#define __H2STUB_3(Function, Signature, Dst) h2::h2_stub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(Dst), #Function, __FILE__, __LINE__)
#define __H2STUB_4(Class, Method, Signature, Dst) h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(Dst), #Class "::" #Method, __FILE__, __LINE__)
#define __H2STUB_5(Object, Class, Method, Signature, Dst) h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(Dst), #Class "::" #Method, __FILE__, __LINE__)

#define H2UNSTUB(...) H2PP_VARIADIC_CALL(__H2UNSTUB_, __VA_ARGS__)
#define __H2UNSTUB_1(Src) h2::h2_unstub_g(h2::h2_fp<>::A(Src))
#define __H2UNSTUB_2(Function, Signature) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUB_3(Class, Method, Signature) h2::h2_unstub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define __H2UNSTUB_4(Object, Class, Method, Signature) h2::h2_unstub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))

////////////////////////////////////////////////////////////////

#define H2UNSTUBS(...) H2PP_VARIADIC_CALL(__H2UNSTUBS_, __VA_ARGS__)
#define __H2UNSTUBS_1(Src) h2::h2_unstub_g(h2::h2_fp<>::A(Src))
#define __H2UNSTUBS_3(Function, ReturnType, Arguments) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUBS_4(Class, Method, ReturnType, Arguments) h2::h2_unstub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define __H2UNSTUBS_5(Object, Class, Method, ReturnType, Arguments) h2::h2_unstub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Arguments>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))

#define H2STUBS(...) H2PP_VARIADIC_CALL(__H2STUBS_, __VA_ARGS__)
#define __H2STUBS_2(Src, Dst) h2::h2_stub_g(h2::h2_fp<>::A(Src), (void*)Dst, #Src, __FILE__, __LINE__)
#define __H2STUBS_3(Function, ReturnType, Arguments) ___H2STUBS_3(Function, ReturnType, Arguments, H2PP_UNIQUE(t_stub3))
#define __H2STUBS_4(Class, Method, ReturnType, Arguments) H2PP_IF_ELSE(H2PP_IS_EMPTY Arguments, __H2STUBS_4_0(Class, Method, ReturnType, Arguments, H2PP_UNIQUE(t_stub40)), __H2STUBS_4_1(Class, Method, ReturnType, Arguments, H2PP_UNIQUE(t_stub41)))
#define __H2STUBS_5(Object, Class, Method, ReturnType, Arguments) H2PP_IF_ELSE(H2PP_IS_EMPTY Arguments, __H2STUBS_5_0(Object, Class, Method, ReturnType, Arguments, H2PP_UNIQUE(t_stub50)), __H2STUBS_5_1(Object, Class, Method, ReturnType, Arguments, H2PP_UNIQUE(t_stub51)))

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

#define __H2STUBS_5_0(Object, Class, Method, ReturnType, Arguments, Q)                                                                                                                                                                   \
   struct {                                                                                                                                                                                                                              \
      Class* object;                                                                                                                                                                                                                     \
      void operator=(ReturnType (*dstfp)(H2PP_REMOVE_PARENTHESES_IF(Class) *))                                                                                                                                                           \
      {                                                                                                                                                                                                                                  \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), ReturnType Arguments>::B(object, &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(dstfp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                                  \
   } Q;                                                                                                                                                                                                                                  \
   Q.object = h2::h2_pointer_if(Object);                                                                                                                                                                                                 \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> ReturnType

#define __H2STUBS_5_1(Object, Class, Method, ReturnType, Arguments, Q)                                                                                                                                                                   \
   struct {                                                                                                                                                                                                                              \
      Class* object;                                                                                                                                                                                                                     \
      void operator=(ReturnType (*dstfp)(H2PP_REMOVE_PARENTHESES_IF(Class) *, H2PP_REMOVE_PARENTHESES(Arguments)))                                                                                                                       \
      {                                                                                                                                                                                                                                  \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), ReturnType Arguments>::B(object, &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(dstfp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                                  \
   } Q;                                                                                                                                                                                                                                  \
   Q.object = h2::h2_pointer_if(Object);                                                                                                                                                                                                 \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Arguments)) -> ReturnType
