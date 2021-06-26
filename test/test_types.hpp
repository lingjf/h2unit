
#define NUMBER0_DECL_LIST \
   static char char_0 = 0;                      \
   static signed char signed_char_0 = 0;        \
   static short int short_int_0 = 0;            \
   static int int_0 = 0;                        \
   static long int long_int_0 = 0;              \
   static long long int long_long_int_0 = 0LL;  \
   static float float_0 = 0;                    \
   static double double_0 = 0;                  \
   static long double long_double_0 = 0;        \
   static bool bool_0 = false;                  \
   static int& int_ref_0 = int_0;               \
   static const int& const_int_ref_0 = 0;       \
   static const char const_char_0 = 0;          \
   static const int const_int_0 = 0;            \
   static const float const_float_0 = 0;        \
   static const bool const_bool_0 = false;      \
   enum { enum_0 = 0 };

#define NUMBER1_DECL_LIST \
   static char char_1 = 1;                      \
   static signed char signed_char_1 = 1;        \
   static short int short_int_1 = 1;            \
   static int int_1 = 1;                        \
   static long int long_int_1 = 1;              \
   static long long int long_long_int_1 = 1LL;  \
   static float float_1 = 1;                    \
   static double double_1 = 1;                  \
   static long double long_double_1 = 1;        \
   static bool bool_1 = true;                   \
   static int& int_ref_1 = int_1;               \
   static const int& const_int_ref_1 = 1;       \
   static const char const_char_1 = 1;          \
   static const int const_int_1 = 1;            \
   static const float const_float_1 = 1;        \
   static const bool const_bool_1 = true;       \
   enum { enum_1 = 1 };

#define NUMBER0_VAR_LIST   \
   0, 0L, 0LL,             \
   char_0,                 \
   signed_char_0,          \
   short_int_0,            \
   int_0,                  \
   long_int_0,             \
   long_long_int_0,        \
   float_0,                \
   double_0,               \
   long_double_0,          \
   bool_0,                 \
   int_ref_0,              \
   const_int_ref_0,        \
   const_char_0,           \
   const_int_0,            \
   const_float_0,          \
   const_bool_0,           \
   enum_0

#define NUMBER1_VAR_LIST   \
   1, 1L, 1LL,             \
   char_1,                 \
   signed_char_1,          \
   short_int_1,            \
   int_1,                  \
   long_int_1,             \
   long_long_int_1,        \
   float_1,                \
   double_1,               \
   long_double_1,          \
   bool_1,                 \
   int_ref_1,              \
   const_int_ref_1,        \
   const_char_1,           \
   const_int_1,            \
   const_float_1,          \
   const_bool_1,           \
   enum_1

#define STRING_VAR_LIST   \
   "h2unit",              \
     const_char_const_p1, \
     const_char_p1,       \
     char_const_p1,       \
     char_p1,             \
     char_ref_p1,         \
     char_array1,         \
     const_char_array1,   \
     const_h2string1,     \
     h2string1,           \
     h2stringref1,        \
     const_stdstring1,    \
     stdstring1,          \
     stdstringref1

#define STRING_DECL_LIST                             \
   const char* const const_char_const_p1 = "h2unit"; \
   const char* const_char_p1 = "h2unit";             \
   char* const char_const_p1 = "h2unit";             \
   char* char_p1 = "h2unit";                         \
   char*& char_ref_p1 = char_p1;                     \
   char char_array1[1024] = "h2unit";                \
   const char const_char_array1[1024] = "h2unit";    \
   const h2::h2_string const_h2string1 = "h2unit";   \
   h2::h2_string h2string1 = "h2unit";               \
   h2::h2_string& h2stringref1 = h2string1;          \
   const std::string const_stdstring1 = "h2unit";    \
   std::string stdstring1 = "h2unit";                \
   std::string& stdstringref1 = stdstring1;

struct test_ptr
{
   int a;
   float b;
};

#define PTR_FILL_DECL_LIST \
   static void* void_ptr = (void*)"0123456789";                    \
   static const void* const_void_ptr = (const void*)"0123456789";  \
   static char* char_ptr = (char*)"0123456789";                    \
   static const char* const_char_ptr = (const char*)"0123456789";  \
   static int* int_ptr = (int*)"0123456789";                       \
   static const int* const_int_ptr = (const int*)"0123456789";     \
   static test_ptr* test_ptr_ptr = (test_ptr*)"0123456789";        \
   static const test_ptr* const_test_ptr_ptr = (const test_ptr*)"0123456789";

#define PTR_NULL_DECL_LIST \
   static void* void_nullptr = nullptr;             \
   static const void* const_void_nullptr = nullptr; \
   static char* char_nullptr = nullptr;             \
   static const char* const_char_nullptr = nullptr; \
   static int* int_nullptr = nullptr;               \
   static const int* const_int_nullptr = nullptr;   \
   static test_ptr* test_ptr_nullptr = nullptr;     \
   static const test_ptr* const_test_ptr_nullptr = nullptr;

#define PTR_LIST  \
   void_nullptr,           \
   const_void_nullptr,     \
   char_nullptr,           \
   const_char_nullptr,     \
   int_nullptr,            \
   const_int_nullptr,      \
   test_ptr_nullptr,       \
   const_test_ptr_nullptr

#define PTR_FILL_VALUE_LIST   \
   "0123456789",              \
   void_ptr,                  \
   const_void_ptr,            \
   char_ptr,                  \
   const_char_ptr,            \
   int_ptr,                   \
   const_int_ptr,             \
   test_ptr_ptr,              \
   const_test_ptr_ptr

#define PTR_NULL_VALUE_LIST \
   nullptr,                 \
   void_nullptr,            \
   const_void_nullptr,      \
   char_nullptr,            \
   const_char_nullptr,      \
   int_nullptr,             \
   const_int_nullptr,       \
   test_ptr_nullptr,        \
   const_test_ptr_nullptr,  \
   NULL, 0