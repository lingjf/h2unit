#include "../source/h2_unit.cpp"

void* void_ptr = (void*)"0123456789";
const void* const_void_ptr = (const void*)"0123456789";
char* char_ptr = (char*)"0123456789";
const char* const_char_ptr = (const char*)"0123456789";
int* int_ptr = (int*)"0123456789";
const int* const_int_ptr = (const int*)"0123456789";
std::string* stdstring_ptr = (std::string*)"0123456789";
const std::string* const_stdstring_ptr = (const std::string*)"0123456789";

void* void_nullptr = nullptr;
const void* const_void_nullptr = nullptr;
char* char_nullptr = nullptr;
const char* const_char_nullptr = nullptr;
int* int_nullptr = nullptr;
const int* const_int_nullptr = nullptr;
std::string* stdstring_nullptr = nullptr;
const std::string* const_stdstring_nullptr = nullptr;

#define PTR_LIST void_nullptr,       \
                 const_void_nullptr, \
                 char_nullptr,       \
                 const_char_nullptr, \
                 int_nullptr,        \
                 const_int_nullptr,  \
                 stdstring_nullptr,  \
                 const_stdstring_nullptr

#define FILLPTR_LIST "0123456789",   \
                     void_ptr,       \
                     const_void_ptr, \
                     char_ptr,       \
                     const_char_ptr, \
                     int_ptr,        \
                     const_int_ptr,  \
                     stdstring_ptr,  \
                     const_stdstring_ptr

#define NULLPTR_LIST nullptr,                 \
                     void_nullptr,            \
                     const_void_nullptr,      \
                     char_nullptr,            \
                     const_char_nullptr,      \
                     int_nullptr,             \
                     const_int_nullptr,       \
                     stdstring_nullptr,       \
                     const_stdstring_nullptr, \
                     NULL, 0

#define TheCheck(x) static void foobar##x(decltype(x) v){};
ForForEach(TheCheck, PTR_LIST);
#undef TheCheck

SUITE(Pointer)
{
   Case(OK IsNull)
   {
#define TheCheck(x) OK(IsNull, x);
      ForForEach(TheCheck, NULLPTR_LIST);
#undef TheCheck
   }

   Case(OK NotNull)
   {
#define TheCheck(x) OK(NotNull, x);
      ForForEach(TheCheck, FILLPTR_LIST);
#undef TheCheck
   }

   Case(OK Me)
   {
#define TheCheck(x, y) OK(Me(x, 10), y);
      ForFullmesh(TheCheck, FILLPTR_LIST);
#undef TheCheck
   }

   Case(OK AllOf, AnyOf, NoneOf)
   {
#define TheCheck(x, y)      \
   OK(AllOf(_, IsNull), y); \
   OK(AnyOf(_, IsNull), y); \
   OK(!!NoneOf(NotNull), y);
      ForFullmesh(TheCheck, NULLPTR_LIST);
#undef TheCheck
   }

   Case(OK Pointee)
   {
      char char_value = 'C';
      char* char_ptr = &char_value;
      const char* const_char_ptr = &char_value;
      int int_value = 1;
      int* int_ptr = &int_value;
      const int* const_int_ptr = &int_value;
      std::string stdstring_value = "S";
      std::string* stdstring_ptr = &stdstring_value;
      const std::string* const_stdstring_ptr = &stdstring_value;

      OK(Pointee(1), int_ptr);
      OK(Pointee(1), const_int_ptr);
      OK(Pointee('C'), char_ptr);
      OK(Pointee('C'), const_char_ptr);
      OK(Pointee("S"), stdstring_ptr);
      OK(Pointee("S"), const_stdstring_ptr);
   }

   Case(MOCK)
   {
#define TheCheck(x, y)                                      \
   MOCK(foobar##x, void, (decltype(x)), Once(IsNull)){};    \
   foobar##x((decltype(x))y);
      ForFullmesh(TheCheck, (PTR_LIST), (NULLPTR_LIST));
#undef TheCheck
   }
}

CASE(Ptr stringify)
{
#define TheCheck(x) h2::h2_stringify(x);
   ForForEach(TheCheck, FILLPTR_LIST);
#undef TheCheck
}
