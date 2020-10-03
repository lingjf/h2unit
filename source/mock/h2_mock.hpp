
/* clang-format off */
#define __H2ARGV0() {}
#define __H2ARGV1(_1) {#_1}
#define __H2ARGV2(_1, _2) {#_1, #_2}
#define __H2ARGV3(_1, _2, _3) {#_1, #_2, #_3}
#define __H2ARGV4(_1, _2, _3, _4) {#_1, #_2, #_3, #_4}
#define __H2ARGV5(_1, _2, _3, _4, _5) {#_1, #_2, #_3, #_4, #_5}
#define __H2ARGV6(_1, _2, _3, _4, _5, _6) {#_1, #_2, #_3, #_4, #_5, #_6}
#define __H2ARGV7(_1, _2, _3, _4, _5, _6, _7) {#_1, #_2, #_3, #_4, #_5, #_6, #_7}
#define __H2ARGV8(_1, _2, _3, _4, _5, _6, _7, _8) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8}
#define __H2ARGV9(_1, _2, _3, _4, _5, _6, _7, _8, _9) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9}
#define __H2ARGV10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10}
#define __H2ARGV11(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10, #_11}
#define __H2ARGV12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10, #_11, #_12}
#define __H2ARGV13(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10, #_11, #_12, #_13}
#define __H2ARGV14(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10, #_11, #_12, #_13, #_14}
#define __H2ARGV15(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) {#_1, #_2, #_3, #_4, #_5, #_6, #_7, #_8, #_9, #_10, #_11, #_12, #_13, #_14, #_15}

#define __H2ARGV(...) H2PP_CAT(__H2ARGV, H2PP_NARG(__VA_ARGS__))(__VA_ARGS__)  // = H2PP_VARIADIC_CALL to avoid recursion
#define H2ARGV(...) __H2ARGV(H2PP_REMOVE_PARENTHESES(__VA_ARGS__))


#define __H2MOCK_1_3(Function, ReturnType, Args) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), "", __FILE__, __LINE__) = [] Args -> H2PP_RPS(ReturnType)

#define __H2MOCK_1_4(Function, ReturnType, Args, Inspect1) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1, __FILE__, __LINE__).Inspect1 = [] Args -> H2PP_RPS(ReturnType)

#define __H2MOCK_1_5(Function, ReturnType, Args, Inspect1, Inspect2) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2, __FILE__, __LINE__).Inspect1.Inspect2 = [] Args -> H2PP_RPS(ReturnType)

#define __H2MOCK_1_6(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3 = [] Args -> H2PP_RPS(ReturnType)

#define __H2MOCK_1_7(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4 = [] Args -> H2PP_RPS(ReturnType)

#define __H2MOCK_1_8(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5 = [] Args -> H2PP_RPS(ReturnType)

#define __H2MOCK_1_9(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6 = [] Args -> H2PP_RPS(ReturnType)

#define __H2MOCK_1_10(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7 = [] Args -> H2PP_RPS(ReturnType)

#define __H2MOCK_1_11(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " Inspect8, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8 = [] Args -> H2PP_RPS(ReturnType)

#define __H2MOCK_1_12(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " Inspect8 " " Inspect9, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9 = [] Args -> H2PP_RPS(ReturnType)

#define __H2MOCK_0_4_1(Class, Method, ReturnType, Args) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), "", __FILE__, __LINE__) = [](H2PP_RPS(Class) * This) -> H2PP_RPS(ReturnType)
#define __H2MOCK_0_4_0(Class, Method, ReturnType, Args) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), "", __FILE__, __LINE__) = [](H2PP_RPS(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_RPS(ReturnType)

#define __H2MOCK_0_5_1(Class, Method, ReturnType, Args, Inspect1) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1, __FILE__, __LINE__).Inspect1 = [](H2PP_RPS(Class) * This) -> H2PP_RPS(ReturnType)
#define __H2MOCK_0_5_0(Class, Method, ReturnType, Args, Inspect1) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1, __FILE__, __LINE__).Inspect1 = [](H2PP_RPS(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_RPS(ReturnType)

#define __H2MOCK_0_6_1(Class, Method, ReturnType, Args, Inspect1, Inspect2) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2, __FILE__, __LINE__).Inspect1.Inspect2 = [](H2PP_RPS(Class) * This) -> H2PP_RPS(ReturnType)
#define __H2MOCK_0_6_0(Class, Method, ReturnType, Args, Inspect1, Inspect2) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2, __FILE__, __LINE__).Inspect1.Inspect2 = [](H2PP_RPS(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_RPS(ReturnType)

#define __H2MOCK_0_7_1(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3 = [](H2PP_RPS(Class) * This) -> H2PP_RPS(ReturnType)
#define __H2MOCK_0_7_0(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3 = [](H2PP_RPS(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_RPS(ReturnType)

#define __H2MOCK_0_8_1(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4 = [](H2PP_RPS(Class) * This) -> H2PP_RPS(ReturnType)
#define __H2MOCK_0_8_0(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4 = [](H2PP_RPS(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_RPS(ReturnType)

#define __H2MOCK_0_9_1(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5 = [](H2PP_RPS(Class) * This) -> H2PP_RPS(ReturnType)
#define __H2MOCK_0_9_0(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5 = [](H2PP_RPS(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_RPS(ReturnType)

#define __H2MOCK_0_10_1(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6 = [](H2PP_RPS(Class) * This) -> H2PP_RPS(ReturnType)
#define __H2MOCK_0_10_0(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6 = [](H2PP_RPS(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_RPS(ReturnType)

#define __H2MOCK_0_11_1(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6 " " Inspect7, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7 = [](H2PP_RPS(Class) * This) -> H2PP_RPS(ReturnType)
#define __H2MOCK_0_11_0(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6 " " Inspect7, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7 = [](H2PP_RPS(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_RPS(ReturnType)

#define __H2MOCK_0_12_1(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6 " " Inspect7 " " Inspect8, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8 = [](H2PP_RPS(Class) * This) -> H2PP_RPS(ReturnType)
#define __H2MOCK_0_12_0(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6 " " Inspect7 " " Inspect8, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8 = [](H2PP_RPS(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_RPS(ReturnType)

#define __H2MOCK_0_13_1(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6 " " Inspect7 " " Inspect8 " " Inspect9, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9 = [](H2PP_RPS(Class) * This) -> H2PP_RPS(ReturnType)
#define __H2MOCK_0_13_0(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6 " " Inspect7 " " Inspect8 " " Inspect9, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9 = [](H2PP_RPS(Class) * This, H2PP_REMOVE_PARENTHESES(Args)) -> H2PP_RPS(ReturnType)

#define __H2MOCK_0_4(Class, Method, ReturnType, Args) H2PP_CAT(__H2MOCK_0_4_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args)
#define __H2MOCK_0_5(Class, Method, ReturnType, Args, Inspect1) H2PP_CAT(__H2MOCK_0_5_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args, Inspect1)
#define __H2MOCK_0_6(Class, Method, ReturnType, Args, Inspect1, Inspect2) H2PP_CAT(__H2MOCK_0_6_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args, Inspect1, Inspect2)
#define __H2MOCK_0_7(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3) H2PP_CAT(__H2MOCK_0_7_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3)
#define __H2MOCK_0_8(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4) H2PP_CAT(__H2MOCK_0_8_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4)
#define __H2MOCK_0_9(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) H2PP_CAT(__H2MOCK_0_9_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5)
#define __H2MOCK_0_10(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) H2PP_CAT(__H2MOCK_0_10_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6)
#define __H2MOCK_0_11(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) H2PP_CAT(__H2MOCK_0_11_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7)
#define __H2MOCK_0_12(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) H2PP_CAT(__H2MOCK_0_12_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8)
#define __H2MOCK_0_13(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) H2PP_CAT(__H2MOCK_0_13_, H2PP_IS_EMPTY Args)(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9)

#define __H2MOCK_0(...) H2PP_VARIADIC_CALL(__H2MOCK_0_, __VA_ARGS__) // normal function
#define __H2MOCK_1(...) H2PP_VARIADIC_CALL(__H2MOCK_1_, __VA_ARGS__) // class member method

// normal function 3rd is (...) i.e. arguments, 4th is not (...) or not exist
#define H2MOCK(...) H2PP_CAT(__H2MOCK_, H2PP_AND(H2PP_IBP(H2PP_TH2(__VA_ARGS__)), H2PP_NOT(H2PP_IBP(H2PP_TH3(__VA_ARGS__))))) (__VA_ARGS__)

#define H2UNMOCK H2UNSTUB

////////////////////////////////////////////////////////////////

#define __H3MOCK_1_3(Function, ReturnType, Args) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), "", __FILE__, __LINE__)

#define __H3MOCK_1_4(Function, ReturnType, Args, Inspect1) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1, __FILE__, __LINE__).Inspect1

#define __H3MOCK_1_5(Function, ReturnType, Args, Inspect1, Inspect2) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2, __FILE__, __LINE__).Inspect1.Inspect2

#define __H3MOCK_1_6(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3

#define __H3MOCK_1_7(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4

#define __H3MOCK_1_8(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5

#define __H3MOCK_1_9(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6

#define __H3MOCK_1_10(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7

#define __H3MOCK_1_11(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " Inspect8, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8

#define __H3MOCK_1_12(Function, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) \
   h2::h2_mocker<__COUNTER__, std::false_type, H2PP_RPS(ReturnType) Args>::I(h2::h2_fp(Function), #ReturnType, #Function, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " #Inspect5 " " #Inspect6 " " #Inspect7 " " Inspect8 " " Inspect9, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9

#define __H3MOCK_0_4(Class, Method, ReturnType, Args) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), "", __FILE__, __LINE__)

#define __H3MOCK_0_5(Class, Method, ReturnType, Args, Inspect1) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1, __FILE__, __LINE__).Inspect1

#define __H3MOCK_0_6(Class, Method, ReturnType, Args, Inspect1, Inspect2) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2, __FILE__, __LINE__).Inspect1.Inspect2

#define __H3MOCK_0_7(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3

#define __H3MOCK_0_8(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4

#define __H3MOCK_0_9(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5

#define __H3MOCK_0_10(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6

#define __H3MOCK_0_11(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6 " " Inspect7, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7

#define __H3MOCK_0_12(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6 " " Inspect7 " " Inspect8, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8

#define __H3MOCK_0_13(Class, Method, ReturnType, Args, Inspect1, Inspect2, Inspect3, Inspect4, Inspect5, Inspect6, Inspect7, Inspect8, Inspect9) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::I(h2::h2_mfp<H2PP_RPS(Class), H2PP_RPS(ReturnType) Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #ReturnType, #Class "::" #Method, H2ARGV(Args), #Inspect1 " " #Inspect2 " " #Inspect3 " " #Inspect4 " " Inspect5 " " Inspect6 " " Inspect7 " " Inspect8 " " Inspect9, __FILE__, __LINE__).Inspect1.Inspect2.Inspect3.Inspect4.Inspect5.Inspect6.Inspect7.Inspect8.Inspect9

#define __H3MOCK_0(...) H2PP_VARIADIC_CALL(__H3MOCK_0_, __VA_ARGS__)
#define __H3MOCK_1(...) H2PP_VARIADIC_CALL(__H3MOCK_1_, __VA_ARGS__)

#define MOCKS(...) H2PP_CAT(__H3MOCK_, H2PP_AND(H2PP_IBP(H2PP_TH2(__VA_ARGS__)), H2PP_NOT(H2PP_IBP(H2PP_TH3(__VA_ARGS__))))) (__VA_ARGS__)
