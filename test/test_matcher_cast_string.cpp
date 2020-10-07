#include "../source/h2_unit.cpp"

const char* const const_char_const_p1 = "h2unit";
const char* const_char_p1 = "h2unit";
char* const char_const_p1 = "h2unit";
char* char_p1 = "h2unit";
char* &char_ref_p1 = char_p1;
char char_array1[1024] = "h2unit";
const char const_char_array1[1024] = "h2unit";
const h2::h2_string const_h2string1 = "h2unit";
h2::h2_string h2string1 = "h2unit";
h2::h2_string& h2stringref1 = h2string1;
const std::string const_stdstring1 = "h2unit";
std::string stdstring1 = "h2unit";
std::string& stdstringref1 = stdstring1;

const char* const const_char_const_p2 = "{}";
const char* const_char_p2 = "{}";
char* const char_const_p2 = "{}";
char* char_p2 = "{}";
char* &char_ref_p2 = char_p2;
char char_array2[1024] = "{}";
const char const_char_array2[1024] = "{}";
const h2::h2_string const_h2string2 = "{}";
h2::h2_string h2string2 = "{}";
h2::h2_string& h2stringref2 = h2string2;
const std::string const_stdstring2 = "{}";
std::string stdstring2 = "{}";
std::string& stdstringref2 = stdstring2;

#define STRING_LIST const_char_const_p1, \
                    const_char_p1,       \
                    char_const_p1,       \
                    char_p1,             \
                    char_ref_p1,         \
                    char_array1,         \
                    const_char_array1,   \
                    const_h2string1,     \
                    h2string1,           \
                    const_stdstring1,    \
                    stdstring1,          \
                    stdstringref1

#define STRING1_LIST "h2unit",            \
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

#define STRING2_LIST "{}",                \
                     const_char_const_p2, \
                     const_char_p2,       \
                     char_const_p2,       \
                     char_p2,             \
                     char_ref_p2,         \
                     char_array2,         \
                     const_char_array2,   \
                     const_h2string2,     \
                     h2string2,           \
                     h2stringref2,        \
                     const_stdstring2,    \
                     stdstring2,          \
                     stdstringref2

#define TheCheck(x) static void foobar##x(decltype(x) v){};
H2Foreach(TheCheck, STRING_LIST);
#undef TheCheck

SUITE(String)
{
   Case(OK)
   {
#define TheCheck(x, y)   \
   OK(x, y);             \
   OK(Eq(x), y);         \
   OK(CaseLess(x), y);   \
   OK(~Eq(x), y);        \
   OK(Substr(x), y);     \
   OK(StartsWith(x), y); \
   OK(EndsWith(x), y);   \
   OK(We(x), y);         \
   OK(Re(x), y);
      H2Fullmesh(TheCheck, STRING1_LIST);
#undef TheCheck
   }

   Case(OK Je)
   {
#define TheCheck(x, y) OK(Je(x), y);
      H2Fullmesh(TheCheck, STRING2_LIST);
#undef TheCheck
   }

   Case(OK AllOf, AnyOf, NoneOf)
   {
#define TheCheck(x, y) \
   OK(AllOf(_, x), y); \
   OK(AnyOf(x), y);    \
   OK(!!NoneOf(Nq(x)), y);
      H2Fullmesh(TheCheck, STRING1_LIST);
#undef TheCheck
   }

   Case(OK Nq)
   {
#define TheCheck(x, y) \
   OK(Nq(x), y);       \
   OK(Not(x), y);
      H2Fullmesh(TheCheck, (STRING1_LIST), (STRING2_LIST));
#undef TheCheck
   }

   Case(MOCK)
   {
#define TheCheck(x, y) MOCK(foobar##x, void, (decltype(x)), Any(y)){};

#define C_STR_LIST const_char_const_p1, \
                   const_char_p1,       \
                   char_const_p1,       \
                   char_p1,             \
                   char_ref_p1,         \
                   char_array1,         \
                   const_char_array1

      H2Fullmesh(TheCheck, C_STR_LIST);
#undef TheCheck

#define TheCheck(x, y) MOCK(foobar##x, void, (decltype(x)), Any(y)){};

#define STR_LIST const_stdstring1, \
                 stdstring1, stdstringref1

      H2Fullmesh(TheCheck, STR_LIST);
#undef TheCheck
   }
}

CASE(String stringify)
{
#define TheCheck(x) OK("h2unit", h2::h2_stringify(x));
   H2Foreach(TheCheck, STRING1_LIST);
#undef TheCheck
}
