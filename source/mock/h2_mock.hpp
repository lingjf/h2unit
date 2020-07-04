
struct h2_mock : h2_libc {
   h2_list x;
   void *origin_fp, *substitute_fp;
   const char* origin_fn;
   const char* file;
   int line;

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

#ifdef _WIN32
#   define __H2_LINENO__ 0
#else
#   define __H2_LINENO__ __LINE__
#endif

#define __H2MOCK3(OriginFunction, Return, Args) \
   h2::h2_mocker<__COUNTER__, __H2_LINENO__, std::false_type, Return Args>::I(h2::h2_fp(OriginFunction), #OriginFunction, __FILE__, __LINE__)

#define __H2MOCK4(Class, Method, Return, Args) \
   h2::h2_mocker<__COUNTER__, __H2_LINENO__, H2PP_REMOVE_PARENTHESES_IF(Class), Return Args>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), Return Args>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, __FILE__, __LINE__)

#define H2MOCK(...) H2PP_VARIADIC_CALL(__H2MOCK, __VA_ARGS__)
