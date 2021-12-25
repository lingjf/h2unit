// STUB(                          Source             , Destination )
// STUB(                Function, ReturnType(Args...), Destination )
// STUB(         Class, Method  , ReturnType(Args...), Destination )
// STUB( Object, Class, Method  , ReturnType(Args...), Destination )
#define H2STUB(...) H2PP_VCALL(__H2STUB_, __VA_ARGS__)
#define __H2STUB_2(Source, Destination) h2::h2_runner::stub(h2::h2_fp<>::get(Source), (void*)Destination, #Source, H2_FILINE)
#define __H2STUB_3(Function, Signature, Destination) h2::h2_runner::stub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(H2PP_REMOVE_PARENTHESES_IF(Function)), (void*)(Destination), #Function, H2_FILINE)
#define __H2STUB_4(Class, Method, Signature, Destination) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = Destination
#define __H2STUB_5(Object, Class, Method, Signature, Destination) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = Destination

#define H2UNSTUB(...) H2PP_VCALL(__H2UNSTUB_, __VA_ARGS__)
#define __H2UNSTUB_1(Source) h2::h2_runner::unstub(h2::h2_fp<>::get(Source))
#define __H2UNSTUB_2(Function, Signature) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUB_3(Class, Method, Signature) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define __H2UNSTUB_4(Object, Class, Method, Signature) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))

// STUBS(                Function, ReturnType, Args ) { }
// STUBS(         Class, Method  , ReturnType, Args ) { }
// STUBS( Object, Class, Method  , ReturnType, Args ) { }
#define H2UNSTUBS(...) H2PP_VCALL(__H2UNSTUBS_, __VA_ARGS__)
#define __H2UNSTUBS_1(Source) h2::h2_runner::unstub(h2::h2_fp<>::get(Source))
#define __H2UNSTUBS_3(Function, ReturnType, Args) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(H2PP_REMOVE_PARENTHESES_IF(Function)))
#define __H2UNSTUBS_4(Class, Method, ReturnType, Args) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))
#define __H2UNSTUBS_5(Object, Class, Method, ReturnType, Args) h2::h2_runner::unstub(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)))

#define H2STUBS(...) H2PP_VCALL(__H2STUBS_, __VA_ARGS__)
#define __H2STUBS_3(Function, ReturnType, Args) h2::h2_stuber<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(H2PP_REMOVE_PARENTHESES_IF(Function)), #Function, H2_FILINE) = [] Args -> ReturnType
#define __H2STUBS_4(Class, Method, ReturnType, Args) H2PP_CAT(__H2STUBS_4_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args)
#define __H2STUBS_4_0(Class, Method, ReturnType, Args) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2STUBS_4_1(Class, Method, ReturnType, Args) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2STUBS_5(Object, Class, Method, ReturnType, Args) H2PP_CAT(__H2STUBS_5_, H2PP_IS_EMPTY Args)(Object, Class, Method, ReturnType, Args)
#define __H2STUBS_5_0(Object, Class, Method, ReturnType, Args) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define __H2STUBS_5_1(Object, Class, Method, ReturnType, Args) h2::h2_stuber<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE) = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
