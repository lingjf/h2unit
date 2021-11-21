#include "../source/h2_unit.cpp"

bool bool_true = true;
const bool const_bool_true = true;
bool& bool_ref_true = bool_true;

bool bool_false = false;
const bool const_bool_false = false;
const bool& const_bool_ref_false = false;
bool&& bool_rref_false = false;

#define BOOL_TRUE_LIST 1, 2, -1, /*(void*)1,*/ \
                       true,                   \
                       bool_true,              \
                       const_bool_true,        \
                       bool_ref_true

#define BOOL_FALSE_LIST 0, /*(void*)0, NULL, nullptr,*/ \
                        false,                          \
                        bool_false,                     \
                        const_bool_false,               \
                        const_bool_ref_false,           \
                        bool_rref_false
SUITE(cast bool)
{
   Case(OK)
   {
#define TheCheck(x) OK(x);
      H2Foreach(TheCheck, (BOOL_TRUE_LIST));
#undef TheCheck
#define TheCheck(x) OK(!x);
      H2Foreach(TheCheck, (BOOL_FALSE_LIST));
#undef TheCheck
   }

   Case(OK Is True)
   {
#define TheCheck(x) OK(true, x);
      H2Foreach(TheCheck, (BOOL_TRUE_LIST));
#undef TheCheck
   }

   Case(OK Is False)
   {
#define TheCheck(x) OK(false, x);
      H2Foreach(TheCheck, (BOOL_FALSE_LIST));
#undef TheCheck
   }

   Case(OK Not True)
   {
#define TheCheck(x) OK(Not(true), x);
      H2Foreach(TheCheck, (BOOL_FALSE_LIST));
#undef TheCheck
   }

   Case(OK Not)
   {
#define TheCheck(x, y) OK(Not(x), y);
      H2Fullmesh(TheCheck, (BOOL_FALSE_LIST), (BOOL_TRUE_LIST));
#undef TheCheck
   }
}
