
struct h2_stubs {
   h2_list stubs;
   bool add(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* file, int lino);
   void clear();
};

struct h2_stub_temporary_restore : h2_once {
   unsigned char saved[32];
   void* origin_fp;
   h2_stub_temporary_restore(void* origin_fp);
   ~h2_stub_temporary_restore();
};

#define __H2STUB2(Origin, Substitute)                                                   \
   do {                                                                                 \
      h2::h2_stub_g(h2::h2_fp(Origin), (void*)Substitute, #Origin, __FILE__, __LINE__); \
   } while (0)

#define __H2STUB4(Function, ReturnType, Args, Substitute)                                                                             \
   do {                                                                                                                               \
      h2::h2_stub_g(h2::h2_fp((H2PP_RPS(ReturnType)(*) Args)H2PP_RPS(Function)), (void*)(Substitute), #Function, __FILE__, __LINE__); \
   } while (0)

#define __H2STUB5(Class, Method, ReturnType, Args, Substitute)                                                                                                                    \
   do {                                                                                                                                                                           \
      h2::h2_stub_g(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), (void*)(Substitute), #Class "::" #Method, __FILE__, __LINE__); \
   } while (0)

#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__)

////////////////////////////////////////////////////////////////

#define __H3STUB2(Origin, Substitute)                                                   \
   do {                                                                                 \
      h2::h2_stub_g(h2::h2_fp(Origin), (void*)Substitute, #Origin, __FILE__, __LINE__); \
   } while (0)

#define ____H3STUB3(Function, ReturnType, Args, Q)                                                  \
   struct {                                                                                         \
      void operator=(ReturnType(*substitute_fp) Args)                                               \
      {                                                                                             \
         h2::h2_stub_g(h2::h2_fp(Function), (void*)(substitute_fp), #Function, __FILE__, __LINE__); \
      }                                                                                             \
   } Q;                                                                                             \
   Q = [] Args -> ReturnType /* captureless lambda implicit cast to function pointer */

#define __H3STUB3(Function, ReturnType, Args) ____H3STUB3(Function, ReturnType, Args, H2Q(t_stub3))

#define __H3STUB40(Class, Method, ReturnType, Args, Q)                                                                                                                        \
   struct {                                                                                                                                                                   \
      void operator=(ReturnType (*substitute_fp)(H2PP_RPS(Class) *))                                                                                                          \
      {                                                                                                                                                                       \
         h2::h2_stub_g(h2::h2_mfp<H2PP_RPS(Class), ReturnType Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), (void*)(substitute_fp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                       \
   } Q;                                                                                                                                                                       \
   Q = [](H2PP_RPS(Class) * This) -> ReturnType

#define __H3STUB41(Class, Method, ReturnType, Args, Q)                                                                                                                        \
   struct {                                                                                                                                                                   \
      void operator=(ReturnType (*substitute_fp)(H2PP_RPS(Class) *, H2PP_REMOVE_PARENTHESES(Args)))                                                                           \
      {                                                                                                                                                                       \
         h2::h2_stub_g(h2::h2_mfp<H2PP_RPS(Class), ReturnType Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), (void*)(substitute_fp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                       \
   } Q;                                                                                                                                                                       \
   Q = [](H2PP_RPS(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> ReturnType

#define __H3STUB4(Class, Method, ReturnType, Args) \
   H2PP_IF_ELSE(H2PP_IS_EMPTY Args, __H3STUB40(Class, Method, ReturnType, Args, H2Q(t_stub40)), __H3STUB41(Class, Method, ReturnType, Args, H2Q(t_stub41)))

#define STUBS(...) H2PP_VARIADIC_CALL(__H3STUB, __VA_ARGS__)
