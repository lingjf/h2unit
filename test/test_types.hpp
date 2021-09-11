
extern char buffer[1024];

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

struct A_PlainStruct {
   int a;
   double b;
   char c[100];
};

struct B_ClassStruct {
   int a;
   int f(int b) { return a + b; }
};

class A_AbstractClass {
 public:
   char a;

   A_AbstractClass() : a('a') {}

 private:
   static const char* static_f1(int x) { return sprintf(buffer, "A.static_f1(%d)", x), buffer; }
   const char* normal_f1(int x) { return sprintf(buffer, "A.normal_f1(%d)%c", x, a), buffer; }
   virtual const char* virtual_f1(int x) { return sprintf(buffer, "A.virtual_f1(%d)%c", x, a), buffer; }
   virtual const char* pure_f0() = 0;
};

class B_DerivedClass : public A_AbstractClass {
 public:
   char b;
   char* s;

   B_DerivedClass() : b('b') { s = (char*)malloc(11); }
   ~B_DerivedClass() { free(s); }

 private:
   static const char* static_f2(int x, int y) { return sprintf(buffer, "B.static_f2(%d,%d)", x, y), buffer; }
   const char* normal_f2(int x, int y) { return sprintf(buffer, "B.normal_f2(%d,%d)%c", x, y, b), buffer; }
   virtual const char* virtual_f2(int x, int y) { return sprintf(buffer, "B.virtual_f2(%d,%d)%c", x, y, b), buffer; }
   virtual const char* pure_f0() { return sprintf(buffer, "B.pure_f0()%c", b), buffer; }
};

class C_OverrideClass : public A_AbstractClass {
 public:
   char c;
   std::string s;

   C_OverrideClass() : c('c'), s(10000, 's') {}

 private:
   static const char* static_f1(int x) { return sprintf(buffer, "C.static_f1(%d)", x), buffer; }
   static const char* static_f2(int x, int y) { return sprintf(buffer, "C.static_f2(%d,%d)", x, y), buffer; }

   const char* normal_f1(int x) { return sprintf(buffer, "C.normal_f1(%d)%c", x, c), buffer; }
   const char* normal_f2(int x, int y) { return sprintf(buffer, "C.normal_f2(%d,%d)%c", x, y, c), buffer; }

   virtual const char* virtual_f1(int x) { return sprintf(buffer, "C.virtual_f1(%d)%c", x, c), buffer; }
   virtual const char* virtual_f2(int x, int y) { return sprintf(buffer, "C.virtual_f2(%d,%d)%c", x, y, c), buffer; }

   virtual const char* pure_f0() { return sprintf(buffer, "C.pure_f0()%c", c), buffer; }
};

class D_NoConstructorClass : public B_DerivedClass, public C_OverrideClass {
 public:
   char d;
   D_NoConstructorClass() = delete;
   D_NoConstructorClass(int, int, int, int, int, int, int, int, int, int, int) : d('d') {}

 private:
   static const char* static_f1(int x) { return sprintf(buffer, "D.static_f1(%d)", x), buffer; }
   static const char* static_f2(int x, int y) { return sprintf(buffer, "D.static_f2(%d,%d)", x, y), buffer; }

   static const char* static_f3(int x, int y, int z) { return sprintf(buffer, "D.static_f3(%d,%d,%d)", x, y, z), buffer; }

   const char* normal_f1(int x) { return sprintf(buffer, "D.normal_f1(%d)%c", x, d), buffer; }
   const char* normal_f2(int x, int y) { return sprintf(buffer, "D.normal_f2(%d,%d)%c", x, y, d), buffer; }
   const char* normal_f3(int x, int y, int z) { return sprintf(buffer, "D.normal_f3(%d,%d,%d)%c", x, y, z, d), buffer; }

   virtual const char* virtual_f1(int x) { return sprintf(buffer, "D.virtual_f1(%d)%c", x, d), buffer; }
   virtual const char* virtual_f2(int x, int y) { return sprintf(buffer, "D.virtual_f2(%d,%d)%c", x, y, d), buffer; }
   virtual const char* virtual_f3(int x, int y, int z) { return sprintf(buffer, "D.virtual_f3(%d,%d,%d)%c", x, y, z, d), buffer; }
};

namespace test_ns {
class E_NamespaceClass : public A_AbstractClass {
 public:
   char e;
   E_NamespaceClass() : e('e') {}

 private:
   virtual const char* pure_f0() { return sprintf(buffer, "E.pure_f0()%c", e), buffer; }
};
}  // namespace test_ns

static const char* A_normal_f1_fake(A_AbstractClass* This, int x) { return sprintf(buffer, "-A.normal_f1(%d)%c", x, This->a), buffer; }
static const char* A_virtual_f1_fake(A_AbstractClass* This, int x) { return sprintf(buffer, "-A.virtual_f1(%d)%c", x, This->a), buffer; }
static const char* B_static_f1_fake(int x) { return sprintf(buffer, "-B.static_f1(%d)", x), buffer; }
static const char* B_normal_f1_fake(B_DerivedClass* This, int x) { return sprintf(buffer, "-B.normal_f1(%d)%c", x, This->b), buffer; }
static const char* B_normal_f2_fake(B_DerivedClass* This, int x, int y) { return sprintf(buffer, "-B.normal_f2(%d,%d)%c", x, y, This->b), buffer; }
static const char* B_virtual_f1_fake(B_DerivedClass* This, int x) { return sprintf(buffer, "-B.virtual_f1(%d)%c", x, This->b), buffer; }
static const char* B_virtual_f2_fake(B_DerivedClass* This, int x, int y) { return sprintf(buffer, "-B.virtual_f2(%d,%d)%c", x, y, This->b), buffer; }
static const char* C_normal_f1_fake(C_OverrideClass* This, int x) { return sprintf(buffer, "-C.normal_f1(%d)%c", x, This->c), buffer; }
static const char* C_virtual_f1_fake(C_OverrideClass* This, int x) { return sprintf(buffer, "-C.virtual_f1(%d)%c", x, This->c), buffer; }
static const char* D_virtual_f3_fake(D_NoConstructorClass* This, int x, int y, int z) { return sprintf(buffer, "-D.virtual_f3(%d,%d,%d)%c", x, y, z, This->d), buffer; }
static const char* E_virtual_f1_fake(test_ns::E_NamespaceClass* This, int x) { return sprintf(buffer, "-E.virtual_f1(%d)%c", x, This->e), buffer; }

template <typename T>
struct F_TemplateClass {
   char f = 'f';
   static const char* static_f1(T x) { return sprintf(buffer, "F.static_f1(%d)", x), buffer; }
   template <typename U>
   const char* normal_f1(U x) { return sprintf(buffer, "F.normal_f1(%d)%c", x, f), buffer; }
   virtual const char* virtual_f1(T x) { return sprintf(buffer, "F.virtual_f1(%d)%c", x, f), buffer; }
};

template <typename T1, typename T2>
struct G_TemplateClass {
   char g = 'g';
   T1 G = 7;
   template <typename U1, typename U2>
   static const char* static_f2(U1 x, U2 y) { return sprintf(buffer, "G.static_f2(%d,%d)", x, y), buffer; }
   template <typename U1, typename U2>
   const char* normal_f2(U1 x, U2 y) { return sprintf(buffer, "G.normal_f2(%d,%d)%c", x, y, g), buffer; }
   template <typename U1, typename U2>
   std::pair<const char*, double> virtual_f2(U1 x, U2 y) { return std::make_pair("G.virtual_f2", x * 10 + y + G / 10.0); }
};

static const char* F_static_f1_fake(int x) { return sprintf(buffer, "-F.static_f1(%d)", x), buffer; }
static const char* F_normal_f1_fake(F_TemplateClass<int>* This, int x) { return sprintf(buffer, "-F.normal_f1(%d)%c", x, This->f), buffer; }
static const char* F_virtual_f1_fake(F_TemplateClass<int>* This, int x) { return sprintf(buffer, "-F.virtual_f1(%d)%c", x, This->f), buffer; }
static const char* G_static_f2_fake(int x, int y) { return sprintf(buffer, "-G.static_f2(%d,%d)", x, y), buffer; }
static const char* G_normal_f2_fake(G_TemplateClass<int, int>* This, int x, int y) { return sprintf(buffer, "-G.normal_f2(%d,%d)%c", x, y, This->g), buffer; }
static std::pair<const char*, double> G_virtual_f2_fake(G_TemplateClass<int, int>* This, int x, int y) { return std::make_pair("-G.virtual_f2", x * 10 + y + This->G / 10.0); }

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

int my_printf(const char* fmt, ...);
int my_fprintf(FILE* stream, const char* fmt, ...);
int my_sprintf(char* t, const char* fmt, ...);
int my_snprintf(char* t, int n, const char* fmt, ...);
void* my_pthread(void* arg);

enum A_Enum { America = 1,
              Bazil = 2,
              China = 3 };

typedef int (*A_FunctionPointer)(int a, int b);

#if defined _MSC_VER
#   define CloseSocket closesocket
#else
#   define CloseSocket close
#endif

#if defined _MSC_VER
#   define _long_long "__int64"
#   define _enum "enum "
#   define _struct "struct "
#   define _class "class "
#else
#   define _long_long "long long"
#   define _enum
#   define _struct
#   define _class
#endif

#if defined _MSC_VER && (defined __x86_64__ || defined _M_X64)
#   define _pointer " * __ptr64"
#   define _fptr "__cdecl*"
#else
#   define _pointer "*"
#   define _fptr "*"
#endif
