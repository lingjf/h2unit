
struct h2_stubs {
   h2_list stubs;
   bool add(void* befp, void* tofp, const char* befn = "", const char* tofn = "", const char* file = nullptr, int line = 0);
   void clear();
};

struct h2_stub_temporary_restore : h2_once {
   void* befp;
   char current[64];
   h2_stub_temporary_restore(void* befp_);
   ~h2_stub_temporary_restore();
};

#define __H2STUB3(BeFunc, ToFunc, _1)                                                    \
   do {                                                                                  \
      h2::h2_stub_g((void*)BeFunc, (void*)ToFunc, #BeFunc, #ToFunc, __FILE__, __LINE__); \
   } while (0)

#define __H2STUB4(Return, BeFunc, Args, Qt)                                         \
   struct {                                                                         \
      void operator=(Return(*toF) Args) {                                           \
         Return(*beF) Args = BeFunc;                                                \
         h2::h2_stub_g((void*)beF, (void*)(toF), #BeFunc, "~", __FILE__, __LINE__); \
      }                                                                             \
   } Qt;                                                                            \
   Qt = [] Args -> Return

#define __H2STUB50(Return, Class, Method, Args, Qt)                                                                                                                                                 \
   struct {                                                                                                                                                                                         \
      void operator=(Return (*toF)(Class * that)) {                                                                                                                                                 \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, "STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                                                                             \
   } Qt;                                                                                                                                                                                            \
   Qt = [](Class * that) -> Return

#define __H2STUB51(Return, Class, Method, Args, Qt)                                                                                                                                                 \
   struct {                                                                                                                                                                                         \
      void operator=(Return (*toF)(Class * that, H2PP_REMOVE_PARENTHESES(Args))) {                                                                                                                  \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, "STUB", #Return, #Class, #Method, #Args, __FILE__, __LINE__), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                                                                             \
   } Qt;                                                                                                                                                                                            \
   Qt = [](Class * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB5(Return, Class, Method, Args, Qt) \
   H2PP_IF_ELSE(H2PP_IS_EMPTY Args, __H2STUB50(Return, Class, Method, Args, Qt), __H2STUB51(Return, Class, Method, Args, Qt))

#define __H2STUB60(Return, Class, Method, Args, Instance, Qt)                                                                                    \
   struct {                                                                                                                                      \
      void operator=(Return (*toF)(Class * that)) {                                                                                              \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                          \
   } Qt;                                                                                                                                         \
   Qt = [](Class * that) -> Return

#define __H2STUB61(Return, Class, Method, Args, Instance, Qt)                                                                                    \
   struct {                                                                                                                                      \
      void operator=(Return (*toF)(Class * that, H2PP_REMOVE_PARENTHESES(Args))) {                                                               \
         h2::h2_stub_g(h2::h2_mfp<Class, Return Args>::A(&Class::Method, Instance), (void*)(toF), #Class "::" #Method, "~", __FILE__, __LINE__); \
      }                                                                                                                                          \
   } Qt;                                                                                                                                         \
   Qt = [](Class * that, H2PP_REMOVE_PARENTHESES(Args)) -> Return

#define __H2STUB6(Return, Class, Method, Args, Instance, Qt) \
   H2PP_IF_ELSE(H2PP_IS_EMPTY Args, __H2STUB60(Return, Class, Method, Args, Instance, Qt), __H2STUB61(Return, Class, Method, Args, Instance, Qt))

#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB, __VA_ARGS__, H2Q(t_stub))
