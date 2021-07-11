
extern "C" {
int foobar0();
}
static int foobar0_fake()
{
   return -1;
}

int foobar1(int a);

static int foobar1_fake(int a)
{
   return -1;
}

int foobar2(int a, const char* b);

static int foobar2_fake(int a, const char* b)
{
   return -2;
}
static void foobar21(int& a, char* b)
{
}
static void foobar22()
{
}

int foobar3(int a);
const char* foobar3(const char* a);

template <typename T1>
int foobar4(T1 a)
{
   return 4;
}
static int foobar4_fake(int a)
{
   return -4;
}

template <typename T1, typename T2>
int foobar5(T1 a, T2 b)
{
   return 5;
}
static int foobar5_fake(int a, float b)
{
   return -5;
}

int foobar6(char* a, ...);

static int foobar6_fake(char* a, ...)
{
   return -6;
}

static int foobar16(int _0, int _1, int _2, int _3, int _4, int _5, int _6, int _7, int _8, int _9, int _10, int _11, int _12, int _13, int _14, int _15)
{
   return 0;
}

namespace test_ns {
int foobar1(float a);

int foobar2(int a, const char* b);

}  // namespace test_ns

class A_AbstractClass {
 public:
   int x, y;

   A_AbstractClass() : x(0), y(0) {}

 private:
   static const char* static_f1(int a) { return "A.static_f1"; }
   const char* normal_f1(int a) { return "A.normal_f1"; }
   virtual const char* virtual_f1(int a) { return "A.virtual_f1"; }
   virtual const char* pure_f0() = 0;
};

class B_DerivedClass : public A_AbstractClass {
 public:
   B_DerivedClass() {}

 private:
   static const char* static_f2(int a, int b) { return "B.static_f2"; }

   const char* normal_f2(int a, int b) { return "B.normal_f2"; }

   virtual const char* virtual_f2(int a, int b) { return "B.virtual_f2"; }

   virtual const char* pure_f0() { return "B.pure_f0"; }
};

class C_OverrideClass : public A_AbstractClass {
 public:
   C_OverrideClass() {}

 private:
   static const char* static_f1(int a) { return "C.static_f1"; }
   static const char* static_f2(int a, int b) { return "C.static_f2"; }

   const char* normal_f1(int a) { return "C.normal_f1"; }
   const char* normal_f2(int a, int b) { return "C.normal_f2"; }

   virtual const char* virtual_f1(int a) { return "C.virtual_f1"; }
   virtual const char* virtual_f2(int a, int b) { return "C.virtual_f2"; }

   virtual const char* pure_f0() { return "C.pure_f0"; }
};

class D_NoConstructorClass : public B_DerivedClass, public C_OverrideClass {
 public:
   D_NoConstructorClass() = delete;
   D_NoConstructorClass(int, int, int, int, int, int, int, int, int, int, int) {}

 private:
   static const char* static_f1(int a) { return "D.static_f1"; }
   static const char* static_f2(int a, int b) { return "D.static_f2"; }
   static const char* static_f3(int a, int b, int c) { return "D.static_f3"; }

   const char* normal_f1(int a) { return "D.normal_f1"; }
   const char* normal_f2(int a, int b) { return "D.normal_f2"; }
   const char* normal_f3(int a, int b, int c) { return "D.normal_f3"; }

   virtual const char* virtual_f1(int a) { return "D.virtual_f1"; }
   virtual const char* virtual_f2(int a, int b) { return "D.virtual_f2"; }
   virtual const char* virtual_f3(int a, int b, int c) { return "D.virtual_f3"; }
};

namespace test_ns {
class E_NamespaceClass : public A_AbstractClass {
 public:
   E_NamespaceClass() {}

 private:
   virtual const char* pure_f0() { return "E.pure_f0"; }
};
}  // namespace test_ns

static const char* A_normal_f1_fake(A_AbstractClass* This, int a) { return "-A.normal_f1"; }
static const char* A_virtual_f1_fake(A_AbstractClass* This, int a) { return "-A.virtual_f1"; }
static const char* B_static_f1_fake(int a) { return "-B.static_f1"; }
static const char* B_normal_f1_fake(B_DerivedClass* This, int a) { return "-B.normal_f1"; }
static const char* B_normal_f2_fake(B_DerivedClass* This, int a, int b) { return "-B.normal_f2"; }
static const char* B_virtual_f1_fake(B_DerivedClass* This, int a) { return "-B.virtual_f1"; }
static const char* B_virtual_f2_fake(B_DerivedClass* This, int a, int b) { return "-B.virtual_f2"; }
static const char* C_normal_f1_fake(C_OverrideClass* This, int a) { return "-C.normal_f1"; }
static const char* C_virtual_f1_fake(C_OverrideClass* This, int a) { return "-C.virtual_f1"; }
static const char* E_virtual_f1_fake(test_ns::E_NamespaceClass* This, int a) { return "-E.virtual_f1"; }

template <typename T>
struct F_TemplateClass {
   T m = 0;
   static const char* static_f1(T a)
   {
      return "F.static_f1";
   }
   template <typename U>
   const char* normal_f1(U a)
   {
      return "F.normal_f1";
   }
   virtual const char* virtual_f1(T a)
   {
      return "F.virtual_f1";
   }
};

template <typename T1, typename T2>
struct G_TemplateClass {
   T1 m1 = 0;
   T2 m2 = 0;

   template <typename U1, typename U2>
   static const char* static_f2(U1 a, U2 b)
   {
      return "G.static_f2";
   }

   template <typename U1, typename U2>
   const char* normal_f2(U1 a, U2 b)
   {
      return "G.normal_f2";
   }

   template <typename U1, typename U2>
   std::pair<const char*, const char*> virtual_f2(U1 a, U2 b)
   {
      return std::make_pair("G", "virtual_f2");
   }
};

static const char* F_static_f1_fake(int a) { return "-F.static_f1"; }
static const char* F_normal_f1_fake(void* This, int a) { return "-F.normal_f1"; }
static const char* F_virtual_f1_fake(void* This, int a) { return "-F.virtual_f1"; }

static const char* G_static_f2_fake(int a, int b) { return "-G.static_f2"; }
static const char* G_normal_f2_fake(void* This, int a, int b) { return "-G.normal_f2"; }
static std::pair<const char*, const char*> G_virtual_f2_fake(void* This, int a, int b) { return std::make_pair("-G", "virtual_f2"); }

#define NUMBER0_DECL_LIST                      \
   static char char_0 = 0;                     \
   static signed char signed_char_0 = 0;       \
   static short int short_int_0 = 0;           \
   static int int_0 = 0;                       \
   static long int long_int_0 = 0;             \
   static long long int long_long_int_0 = 0LL; \
   static float float_0 = 0;                   \
   static double double_0 = 0;                 \
   static long double long_double_0 = 0;       \
   static bool bool_0 = false;                 \
   static int& int_ref_0 = int_0;              \
   static const int& const_int_ref_0 = 0;      \
   static const char const_char_0 = 0;         \
   static const int const_int_0 = 0;           \
   static const float const_float_0 = 0;       \
   static const bool const_bool_0 = false;     \
   enum { enum_0 = 0 };

#define NUMBER1_DECL_LIST                      \
   static char char_1 = 1;                     \
   static signed char signed_char_1 = 1;       \
   static short int short_int_1 = 1;           \
   static int int_1 = 1;                       \
   static long int long_int_1 = 1;             \
   static long long int long_long_int_1 = 1LL; \
   static float float_1 = 1;                   \
   static double double_1 = 1;                 \
   static long double long_double_1 = 1;       \
   static bool bool_1 = true;                  \
   static int& int_ref_1 = int_1;              \
   static const int& const_int_ref_1 = 1;      \
   static const char const_char_1 = 1;         \
   static const int const_int_1 = 1;           \
   static const float const_float_1 = 1;       \
   static const bool const_bool_1 = true;      \
   enum { enum_1 = 1 };

#define NUMBER0_VAR_LIST \
   0, 0L, 0LL,           \
     char_0,             \
     signed_char_0,      \
     short_int_0,        \
     int_0,              \
     long_int_0,         \
     long_long_int_0,    \
     float_0,            \
     double_0,           \
     long_double_0,      \
     bool_0,             \
     int_ref_0,          \
     const_int_ref_0,    \
     const_char_0,       \
     const_int_0,        \
     const_float_0,      \
     const_bool_0,       \
     enum_0

#define NUMBER1_VAR_LIST \
   1, 1L, 1LL,           \
     char_1,             \
     signed_char_1,      \
     short_int_1,        \
     int_1,              \
     long_int_1,         \
     long_long_int_1,    \
     float_1,            \
     double_1,           \
     long_double_1,      \
     bool_1,             \
     int_ref_1,          \
     const_int_ref_1,    \
     const_char_1,       \
     const_int_1,        \
     const_float_1,      \
     const_bool_1,       \
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

struct test_ptr {
   int a;
   float b;
};

#define PTR_FILL_DECL_LIST                                              \
   static void* void_ptr = (void*)"h2unit0123456789";                   \
   static const void* const_void_ptr = (const void*)"h2unit0123456789"; \
   static char* char_ptr = (char*)"h2unit0123456789";                   \
   static const char* const_char_ptr = (const char*)"h2unit0123456789"; \
   static int* int_ptr = (int*)"h2unit0123456789";                      \
   static const int* const_int_ptr = (const int*)"h2unit0123456789";    \
   static test_ptr* test_ptr_ptr = (test_ptr*)"h2unit0123456789";       \
   static const test_ptr* const_test_ptr_ptr = (const test_ptr*)"h2unit0123456789";

#define PTR_NULL_DECL_LIST                          \
   static void* void_nullptr = nullptr;             \
   static const void* const_void_nullptr = nullptr; \
   static char* char_nullptr = nullptr;             \
   static const char* const_char_nullptr = nullptr; \
   static int* int_nullptr = nullptr;               \
   static const int* const_int_nullptr = nullptr;   \
   static test_ptr* test_ptr_nullptr = nullptr;     \
   static const test_ptr* const_test_ptr_nullptr = nullptr;

#define PTR_LIST         \
   void_nullptr,         \
     const_void_nullptr, \
     char_nullptr,       \
     const_char_nullptr, \
     int_nullptr,        \
     const_int_nullptr,  \
     test_ptr_nullptr,   \
     const_test_ptr_nullptr

#define PTR_FILL_VALUE_LIST \
   "h2unit0123456789",      \
     void_ptr,              \
     const_void_ptr,        \
     char_ptr,              \
     const_char_ptr,        \
     int_ptr,               \
     const_int_ptr,         \
     test_ptr_ptr,          \
     const_test_ptr_ptr

#define PTR_NULL_VALUE_LIST  \
   nullptr,                  \
     void_nullptr,           \
     const_void_nullptr,     \
     char_nullptr,           \
     const_char_nullptr,     \
     int_nullptr,            \
     const_int_nullptr,      \
     test_ptr_nullptr,       \
     const_test_ptr_nullptr, \
     NULL, 0
