#include "../source/h2_unit.cpp"

char char0 = 0;
const char const_char0 = 0;
unsigned char unsigned_char0 = 0;
const unsigned char const_unsigned_char0 = 0;
signed char signed_char0 = 0;
const signed char const_signed_char0 = 0;

short short0 = 0;
const short const_short0 = 0;
unsigned short unsigned_short0 = 0;
const unsigned short const_unsigned_short0 = 0;

int int0 = 0;
const int const_int0 = 0;
unsigned int unsigned_int0 = 0;
const unsigned int const_unsigned_int0 = 0;
long long0 = 0;
const long const_long0 = 0;
unsigned long unsigned_long0 = 0;
const unsigned long const_unsigned_long0 = 0;
unsigned unsigned0 = 0;
const unsigned const_unsigned0 = 0;

long long long_long0 = 0LL;
const long long const_long_long0 = 0LL;
unsigned long long unsigned_long_long0 = 0ULL;
const unsigned long long const_unsigned_long_long0 = 0ULL;

intptr_t intptr_t0 = 0;
const intptr_t const_intptr_t0 = 0;
uintptr_t uintptr_t0 = 0;
const uintptr_t const_uintptr_t0 = 0;

size_t size_t0 = 0;
const size_t const_size_t0 = 0;
ssize_t ssize_t0 = 0;
const ssize_t const_ssize_t0 = 0;

float float0 = 0;
const float const_float0 = 0;
double double0 = 0;
const double const_double0 = 0;
long double long_double0 = 0;
const long double const_long_double0 = 0;

enum {
   enum0 = 0
};

bool bool0 = false;
const bool const_bool0 = false;

char char1 = 1;
const char const_char1 = 1;
unsigned char unsigned_char1 = 1;
const unsigned char const_unsigned_char1 = 1;
signed char signed_char1 = 1;
const signed char const_signed_char1 = 1;

short short1 = 1;
const short const_short1 = 1;
unsigned short unsigned_short1 = 1;
const unsigned short const_unsigned_short1 = 1;

int int1 = 1;
const int const_int1 = 1;
unsigned int unsigned_int1 = 1;
const unsigned int const_unsigned_int1 = 1;
long long1 = 1;
const long const_long1 = 1;
unsigned long unsigned_long1 = 1;
const unsigned long const_unsigned_long1 = 1;
unsigned unsigned1 = 1;
const unsigned const_unsigned1 = 1;

long long long_long1 = 1LL;
const long long const_long_long1 = 1LL;
unsigned long long unsigned_long_long1 = 1ULL;
const unsigned long long const_unsigned_long_long1 = 1ULL;

intptr_t intptr_t1 = 1;
const intptr_t const_intptr_t1 = 1;
uintptr_t uintptr_t1 = 1;
const uintptr_t const_uintptr_t1 = 1;

size_t size_t1 = 1;
const size_t const_size_t1 = 1;
ssize_t ssize_t1 = 1;
const ssize_t const_ssize_t1 = 1;

float float1 = 1;
const float const_float1 = 1;
double double1 = 1;
const double const_double1 = 1;
long double long_double1 = 1;
const long double const_long_double1 = 1;

enum {
   enum1 = 1
};

bool bool1 = true;
const bool const_bool1 = true;

#define NUMBER0_LIST 0, 0L, 0LL,                \
                     char0,                     \
                     const_char0,               \
                     unsigned_char0,            \
                     const_unsigned_char0,      \
                     signed_char0,              \
                     const_signed_char0,        \
                     short0,                    \
                     const_short0,              \
                     unsigned_short0,           \
                     const_unsigned_short0,     \
                     int0,                      \
                     const_int0,                \
                     unsigned_int0,             \
                     const_unsigned_int0,       \
                     long0,                     \
                     const_long0,               \
                     unsigned_long0,            \
                     const_unsigned_long0,      \
                     unsigned0,                 \
                     const_unsigned0,           \
                     long_long0,                \
                     const_long_long0,          \
                     unsigned_long_long0,       \
                     const_unsigned_long_long0, \
                     intptr_t0,                 \
                     const_intptr_t0,           \
                     uintptr_t0,                \
                     const_uintptr_t0,          \
                     size_t0,                   \
                     const_size_t0,             \
                     ssize_t0,                  \
                     const_ssize_t0,            \
                     float0,                    \
                     const_float0,              \
                     double0,                   \
                     const_double0,             \
                     long_double0,              \
                     const_long_double0,        \
                     enum0,                     \
                     false,                     \
                     bool0,                     \
                     const_bool0

#define NUMBER1_LIST 1, 1L, 1LL,                \
                     char1,                     \
                     const_char1,               \
                     unsigned_char1,            \
                     const_unsigned_char1,      \
                     signed_char1,              \
                     const_signed_char1,        \
                     short1,                    \
                     const_short1,              \
                     unsigned_short1,           \
                     const_unsigned_short1,     \
                     int1,                      \
                     const_int1,                \
                     unsigned_int1,             \
                     const_unsigned_int1,       \
                     long1,                     \
                     const_long1,               \
                     unsigned_long1,            \
                     const_unsigned_long1,      \
                     unsigned1,                 \
                     const_unsigned1,           \
                     long_long1,                \
                     const_long_long1,          \
                     unsigned_long_long1,       \
                     const_unsigned_long_long1, \
                     intptr_t1,                 \
                     const_intptr_t1,           \
                     uintptr_t1,                \
                     const_uintptr_t1,          \
                     size_t1,                   \
                     const_size_t1,             \
                     ssize_t1,                  \
                     const_ssize_t1,            \
                     float1,                    \
                     const_float1,              \
                     double1,                   \
                     const_double1,             \
                     long_double1,              \
                     const_long_double1,        \
                     enum1,                     \
                     true,                      \
                     bool1,                     \
                     const_bool1

#define TheCheck(x) static void foo1##x(decltype(x) v){};
ForForEach(TheCheck, NUMBER0_LIST);
#undef TheCheck

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
#define TheCheck(x)                          \
   MOCK(foo1##x, void(decltype(x))).once(x); \
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