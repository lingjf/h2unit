#include "../source/h2_unit.cpp"

bool bool_true = true;
const bool const_bool_true = true;

bool bool_false = false;
const bool const_bool_false = false;

#define BOOL_TRUE_LIST 1, 2, -1, /*(void*)1,*/ \
                       true,                   \
                       bool_true,              \
                       const_bool_true

#define BOOL_FALSE_LIST 0, /*(void*)0, NULL, nullptr,*/ \
                        false,                          \
                        bool_false,                     \
                        const_bool_false
SUITE(Boolean)
{
   Case(OK)
   {
#define TheCheck(x) OK(x);
      ForForEach(TheCheck, BOOL_TRUE_LIST);
#undef TheCheck
#define TheCheck(x) OK(!x);
      ForForEach(TheCheck, BOOL_FALSE_LIST);
#undef TheCheck
   }

   Case(OK IsTrue)
   {
#define TheCheck(x) OK(IsTrue, x);
      ForForEach(TheCheck, BOOL_TRUE_LIST);
#undef TheCheck
   }

   Case(OK IsFalse)
   {
#define TheCheck(x) OK(IsFalse, x);
      ForForEach(TheCheck, BOOL_FALSE_LIST);
#undef TheCheck
   }

   Case(OK Not)
   {
#define TheCheck(x)    \
   OK(Not(IsTrue), x); \
   OK(IsTrue || IsFalse, x);
      ForForEach(TheCheck, BOOL_FALSE_LIST);
#undef TheCheck
   }

   Case(OK Not)
   {
#define TheCheck(x, y) OK(Not(x), y);
      ForFullmesh(TheCheck, (BOOL_FALSE_LIST), (BOOL_TRUE_LIST));
#undef TheCheck
   }
}

CASE(Boolean stringify)
{
#define TheCheck(x) h2::h2_stringify(x);
   ForForEach(TheCheck, BOOL_FALSE_LIST);
#undef TheCheck
}
