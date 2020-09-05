#include "../source/h2_unit.cpp"

static char char_0 = 0;

static signed char signed_char_0 = 0;
static short int short_int_0 = 0;
static int int_0 = 0;
static long int long_int_0 = 0;
static long long int long_long_int_0 = 0LL;

static float float_0 = 0;
static double double_0 = 0;
static long double long_double_0 = 0;

static bool bool_0 = false;

static int& int_ref_0 = int_0;
static const int& const_int_ref_0 = 0;

static const char const_char_0 = 0;
static const int const_int_0 = 0;
static const float const_float_0 = 0;
static const bool const_bool_0 = false;

enum {
   enum_0 = 0
};

#define NUMBER0_LIST 0, 0L, 0LL,      \
                     char_0,          \
                     signed_char_0,   \
                     short_int_0,     \
                     int_0,           \
                     long_int_0,      \
                     long_long_int_0, \
                     float_0,         \
                     double_0,        \
                     long_double_0,   \
                     bool_0,          \
                     int_ref_0,       \
                     const_int_ref_0, \
                     const_char_0,    \
                     const_int_0,     \
                     const_float_0,   \
                     const_bool_0,    \
                     enum_0

static char char_1 = 1;

static signed char signed_char_1 = 1;
static short int short_int_1 = 1;
static int int_1 = 1;
static long int long_int_1 = 1;
static long long int long_long_int_1 = 1LL;

static float float_1 = 1;
static double double_1 = 1;
static long double long_double_1 = 1;

static bool bool_1 = true;

static int& int_ref_1 = int_1;
static const int& const_int_ref_1 = 1;

static const char const_char_1 = 1;
static const int const_int_1 = 1;
static const float const_float_1 = 1;
static const bool const_bool_1 = true;

enum {
   enum_1 = 1
};

#define NUMBER1_LIST 1, 1L, 1LL,      \
                     char_1,          \
                     signed_char_1,   \
                     short_int_1,     \
                     int_1,           \
                     long_int_1,      \
                     long_long_int_1, \
                     float_1,         \
                     double_1,        \
                     long_double_1,   \
                     bool_1,          \
                     int_ref_1,       \
                     const_int_ref_1, \
                     const_char_1,    \
                     const_int_1,     \
                     const_float_1,   \
                     const_bool_1,    \
                     enum_1

#define TheCheck(x) static void foo1##x(decltype(x) v){};
ForForEach(TheCheck, NUMBER0_LIST);
#undef TheCheck

SUITE(Same Number Type)
{
   Case(signed)
   {
      OK((std::is_same<short, short int>::value));
      OK((std::is_same<long, long int>::value));
      OK((std::is_same<long long, long long int>::value));
      OK((std::is_same<signed, int>::value) || (std::is_same<signed, long int>::value) || (std::is_same<signed, long long int>::value));
      OK((std::is_same<intptr_t, int>::value) || (std::is_same<intptr_t, long int>::value) || (std::is_same<intptr_t, long long int>::value));
      OK((std::is_same<ssize_t, int>::value) || (std::is_same<ssize_t, long int>::value) || (std::is_same<ssize_t, long long int>::value));
   }

   Case(unsigned)
   {
      OK((std::is_same<unsigned short, unsigned short int>::value));
      OK((std::is_same<unsigned long, unsigned long int>::value));
      OK((std::is_same<unsigned long long, unsigned long long int>::value));
      OK((std::is_same<unsigned, unsigned int>::value) || (std::is_same<unsigned, unsigned long int>::value) || (std::is_same<unsigned, unsigned long long int>::value));
      OK((std::is_same<uintptr_t, unsigned int>::value) || (std::is_same<uintptr_t, unsigned long int>::value) || (std::is_same<uintptr_t, unsigned long long int>::value));
      OK((std::is_same<size_t, unsigned int>::value) || (std::is_same<size_t, unsigned long int>::value) || (std::is_same<size_t, unsigned long long int>::value));
   }

   Case(const)
   {
      // OK((std::is_same<int, const int>::value));
      // OK((std::is_same<int, int&>::value));
   }
}

SUITE(number[integer, float, enum, bool])
{
   Case(OK boolean)
   {
#define TheCheck(x) OK(x);
      ForForEach(TheCheck, NUMBER1_LIST);
#undef TheCheck
   }

   Case(OK)
   {
#define TheCheck(x, y) OK(x, y);
      ForFullmesh(TheCheck, NUMBER1_LIST);
#undef TheCheck
   }

   Case(OK Eq, Ge, Gt, Le, Lt, Nq, !, &&, ||)
   {
#define TheCheck(x, y) OK(Ge(x) && !Lt(x) || Nq(x) && !Eq(x), y);
      ForFullmesh(TheCheck, (NUMBER0_LIST), (NUMBER1_LIST));
#undef TheCheck
   }

   Case(OK IsTrue)
   {
#define TheCheck(x) OK(IsTrue, x);
      ForForEach(TheCheck, NUMBER1_LIST);
#undef TheCheck
   }

   Case(OK IsFalse)
   {
#define TheCheck(x) OK(IsFalse, x);
      ForForEach(TheCheck, NUMBER0_LIST);
#undef TheCheck
   }

   Case(MOCK)
   {
#define TheCheck(x)                                 \
   MOCK(foo1##x, void, (decltype(x)), Once(x)){};   \
   foo1##x(x);
      ForForEach(TheCheck, NUMBER0_LIST);
#undef TheCheck
   }
}

CASE(Integer stringify)
{
#define TheCheck(x) h2::h2_stringify(x);
   ForForEach(TheCheck, NUMBER0_LIST);
#undef TheCheck
}