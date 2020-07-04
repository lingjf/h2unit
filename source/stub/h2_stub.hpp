
struct h2_stubs {
   h2_list stubs;
   bool add(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* file, int line);
   void clear();
};

struct h2_stub_temporary_restore : h2_once {
   unsigned char saved[32];
   void* origin_fp;
   h2_stub_temporary_restore(void* origin_fp);
   ~h2_stub_temporary_restore();
};

#define __H2STUB2(OriginFunction, SubstituteFunction)                                                           \
   do {                                                                                                         \
      h2::h2_stub_g(h2::h2_fp(OriginFunction), (void*)SubstituteFunction, #OriginFunction, __FILE__, __LINE__); \
   } while (0)

#define ____H2STUB3(OriginFunction, Return, Args, Q)                                                            \
   struct {                                                                                                     \
      void operator=(Return(*substitute_fp) Args)                                                               \
      {                                                                                                         \
         h2::h2_stub_g(h2::h2_fp(OriginFunction), (void*)(substitute_fp), #OriginFunction, __FILE__, __LINE__); \
      }                                                                                                         \
   } Q;                                                                                                         \
   Q = [] Args -> Return /* captureless lambda implicit cast to function pointer */

#define __H2STUB3(OriginFunction, Return, Args) ____H2STUB3(OriginFunction, Return, Args, H2Q(t_stub3))

#define __H2STUB40(Class, Method, Return, Args, Q)                                                                                                                                                                              \
   struct {                                                                                                                                                                                                                     \
      void operator=(Return (*substitute_fp)(H2PP_REMOVE_PARENTHESES_IF(Class) * that))                                                                                                                                         \
      {                                                                                                                                                                                                                         \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), Return Args>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(substitute_fp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                         \
   } Q;                                                                                                                                                                                                                         \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * that) -> Return

#define __H2STUB41(Class, Method, Return, Args, Q)                                                                                                                                                                              \
   struct {                                                                                                                                                                                                                     \
      void operator=(Return (*substitute_fp)(H2PP_REMOVE_PARENTHESES_IF(Class) * that, H2PP_REMOVE_PARENTHESES(Args)))                                                                                                          \
      {                                                                                                                                                                                                                         \
         h2::h2_stub_g(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), Return Args>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), (void*)(substitute_fp), #Class "::" #Method, __FILE__, __LINE__); \
      }                                                                                                                                                                                                                         \
   } Q;                                                                                                                                                                                                                         \
   Q = [](H2PP_REMOVE_PARENTHESES_IF(Class) * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB4(Class, Method, Return, Args) \
   H2PP_IF_ELSE(H2PP_IS_EMPTY Args, __H2STUB40(Class, Method, Return, Args, H2Q(t_stub40)), __H2STUB41(Class, Method, Return, Args, H2Q(t_stub41)))

#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__)
