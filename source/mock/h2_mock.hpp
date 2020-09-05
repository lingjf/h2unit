
struct h2_mock : h2_libc {
   h2_list x;
   void *origin_fp, *substitute_fp;
   const char* ret;
   const char* func;
   h2_vector<const char*> argv;
   const char* file;
   int lino;

   h2_line argvs(int seq = -1);
   h2_line signature();

   h2_vector<h2_checkin> checkin_array;
   int checkin_index = 0;

   virtual void reset() = 0;
   void mock();
   h2_fail* times_check();
};

struct h2_mocks {
   h2_list mocks;
   bool add(h2_mock* mock);
   h2_fail* clear(bool check);
};

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

#define __H2ARGV(...) H2PP_CAT(__H2ARGV, H2PP_NARG(__VA_ARGS__))(__VA_ARGS__)  // Duplicated H2PP_VARIADIC_CALL avoid recursion
#define H2ARGV(...) __H2ARGV(H2PP_REMOVE_PARENTHESES(__VA_ARGS__))

#define __H2MOCK4(OriginFunction, Return, Args, Expression) \
   h2::h2_mocker<__COUNTER__, std::false_type, Return Args>::I(h2::h2_fp(OriginFunction), #Return, #OriginFunction, H2ARGV(Args), __FILE__, __LINE__)

#define __H2MOCK5(Class, Method, Return, Args, Expression) \
   h2::h2_mocker<__COUNTER__, H2PP_RPS(Class), Return Args>::I(h2::h2_mfp<H2PP_RPS(Class), Return Args>::A(&H2PP_RPS(Class)::H2PP_RPS(Method)), #Return, #Class "::" #Method, H2ARGV(Args), __FILE__, __LINE__)

#define __H2MOCK(Expression, ...) H2PP_VARIADIC_CALL(__H2MOCK, __VA_ARGS__, (Expression))
#define H2MOCK(...) __H2MOCK(#__VA_ARGS__, __VA_ARGS__)
