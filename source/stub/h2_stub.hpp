// STUB(                              Src      , Dst )
// STUB(                    Function, Signature, Dst )
// STUB(         ClassType, Method  , Signature, Dst )
// STUB( Object, ClassType, Method  , Signature, Dst )
#define H2STUB(...) H2PP_VARIADIC_CALL(__H2STUB_, __VA_ARGS__)
#define __H2STUB_2(Src, Dst) h2::h2_stub_g(h2::h2_fp<>::A(Src), (void*)Dst, {__FILE__, __LINE__, #Src})
#define __H2STUB_3(Function, Signature, Dst) h2::h2_stub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(Dst), {__FILE__, __LINE__, #Function})
#define __H2STUB_4(ClassType, Method, Signature, Dst) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = Dst
#define __H2STUB_5(Object, ClassType, Method, Signature, Dst) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = Dst

#define H2UNSTUB(...) H2PP_VARIADIC_CALL(__H2UNSTUB_, __VA_ARGS__)
#define __H2UNSTUB_1(Src) h2::h2_unstub_g(h2::h2_fp<>::A(Src))
#define __H2UNSTUB_2(Function, Signature) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUB_3(ClassType, Method, Signature) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define __H2UNSTUB_4(Object, ClassType, Method, Signature) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(Signature)>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)))

// STUBS(                    Function, ReturnType, ArgumentTypes ) { }
// STUBS(         ClassType, Method  , ReturnType, ArgumentTypes ) { }
// STUBS( Object, ClassType, Method  , ReturnType, ArgumentTypes ) { }
#define H2UNSTUBS(...) H2PP_VARIADIC_CALL(__H2UNSTUBS_, __VA_ARGS__)
#define __H2UNSTUBS_1(Src) h2::h2_unstub_g(h2::h2_fp<>::A(Src))
#define __H2UNSTUBS_3(Function, ReturnType, ArgumentTypes) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUBS_4(ClassType, Method, ReturnType, ArgumentTypes) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define __H2UNSTUBS_5(Object, ClassType, Method, ReturnType, ArgumentTypes) h2::h2_unstub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)))

#define H2STUBS(...) H2PP_VARIADIC_CALL(__H2STUBS_, __VA_ARGS__)
#define __H2STUBS_3(Function, ReturnType, ArgumentTypes) ___H2STUBS_3(Function, ReturnType, ArgumentTypes, H2PP_UNIQUE(t_stub3))
#define __H2STUBS_4(ClassType, Method, ReturnType, ArgumentTypes) H2PP_CAT(__H2STUBS_4_, H2PP_IS_EMPTY ArgumentTypes)(ClassType, Method, ReturnType, ArgumentTypes)
#define __H2STUBS_4_0(ClassType, Method, ReturnType, ArgumentTypes) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This, H2PP_REMOVE_PARENTHESES(ArgumentTypes)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2STUBS_4_1(ClassType, Method, ReturnType, ArgumentTypes) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(&H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2STUBS_5(Object, ClassType, Method, ReturnType, ArgumentTypes) H2PP_CAT(__H2STUBS_5_, H2PP_IS_EMPTY ArgumentTypes)(Object, ClassType, Method, ReturnType, ArgumentTypes)
#define __H2STUBS_5_0(Object, ClassType, Method, ReturnType, ArgumentTypes) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This, H2PP_REMOVE_PARENTHESES(ArgumentTypes)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2STUBS_5_1(Object, ClassType, Method, ReturnType, ArgumentTypes) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ClassType), H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::B(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(ClassType)::H2PP_REMOVE_PARENTHESES_IF(Method)), {__FILE__, __LINE__, #ClassType "::" #Method}) = [](H2PP_REMOVE_PARENTHESES_IF(ClassType) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)

#define ___H2STUBS_3(Function, ReturnType, ArgumentTypes, Q)                                                                                                                       \
   struct {                                                                                                                                                                        \
      void operator=(ReturnType(*dstfp) ArgumentTypes)                                                                                                                             \
      {                                                                                                                                                                            \
         h2::h2_stub_g(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) ArgumentTypes>::A(H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(dstfp), {__FILE__, __LINE__, #Function}); \
      }                                                                                                                                                                            \
   } Q;                                                                                                                                                                            \
   Q = [] ArgumentTypes -> ReturnType /* captureless lambda implicit cast to function pointer */
