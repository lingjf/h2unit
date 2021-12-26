// MOCK(                Function, ReturnType(Args...) )
// MOCK(         Class, Method  , ReturnType(Args...) )
// MOCK( Object, Class, Method  , ReturnType(Args...) )
#define H2MOCK(...) H2PP_VCALL(H2MOCK_, __VA_ARGS__)
#define H2MOCK_2(Function, Signature) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(Function), #Function, H2_FILINE)
#define H2MOCK_3(Class, Method, Signature) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE)
#define H2MOCK_4(Object, Class, Method, Signature) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(Signature)>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE)

// MOCKS(                Function, ReturnType, (Args...), Inspection ) { }
// MOCKS(         Class, Method  , ReturnType, (Args...), Inspection ) { }
// MOCKS( Object, Class, Method  , ReturnType, (Args...), Inspection ) { }
#define H2MOCKS(...) H2PP_VCALL(H2MOCKS_, __VA_ARGS__)
#define H2MOCKS_4(Function, ReturnType, Args, Inspection) h2::h2_mocker<__COUNTER__, std::false_type, H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(Function), #Function, H2_FILINE).Inspection = [] Args -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define H2MOCKS_5(Class, Method, ReturnType, Args, Inspection) H2PP_CAT(H2MOCKS_5_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args, Inspection)
#define H2MOCKS_5_0(Class, Method, ReturnType, Args, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define H2MOCKS_5_1(Class, Method, ReturnType, Args, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define H2MOCKS_6(Object, Class, Method, ReturnType, Args, Inspection) H2PP_CAT(H2MOCKS_6_, H2PP_IS_EMPTY Args)(Object, Class, Method, ReturnType, Args, Inspection)
#define H2MOCKS_6_0(Object, Class, Method, ReturnType, Args, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
#define H2MOCKS_6_1(Object, Class, Method, ReturnType, Args, Inspection) h2::h2_mocker<__COUNTER__, H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::I(h2::h2_fp<H2PP_REMOVE_PARENTHESES_IF(Class), H2PP_REMOVE_PARENTHESES_IF(ReturnType) Args>::get(h2::h2_pointer_if(Object), &H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, H2_FILINE).Inspection = [](H2PP_REMOVE_PARENTHESES_IF(Class) * This) -> H2PP_REMOVE_PARENTHESES_IF(ReturnType)
