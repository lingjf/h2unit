
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
#   define __H2_LINE__ 0
#else
#   define __H2_LINE__ __LINE__
#endif

#define __H2MOCK2(OriginFunction, Signature) \
   h2::h2_mocker<__COUNTER__, __H2_LINE__, std::false_type, Signature>::I(h2::h2_fp(OriginFunction), #OriginFunction, __FILE__, __LINE__)

#define __H2MOCK3(Class, Method, Signature) \
   h2::h2_mocker<__COUNTER__, __H2_LINE__, H2PP_REMOVE_PARENTHESES_IF(Class), Signature>::I(h2::h2_mfp<H2PP_REMOVE_PARENTHESES_IF(Class), Signature>::A(&H2PP_REMOVE_PARENTHESES_IF(Class)::H2PP_REMOVE_PARENTHESES_IF(Method)), #Class "::" #Method, __FILE__, __LINE__)

#define H2MOCK(...) H2PP_VARIADIC_CALL(__H2MOCK, __VA_ARGS__)
