#include "../source/h2_unit.cpp"
#include "test_types.hpp"

#include <vector>
#include <deque>
#include <array>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <tuple>
#include <valarray>

static int int_array[] = {1, 2, 3};
static const int const_int_array[] = {1, 2, 3};
static intptr_t intptr_t_array[] = {1, 2, 3};
static const intptr_t const_intptr_t_array[] = {1, 2, 3};
static std::vector<int> stdvector = {1, 2, 3};
static const std::vector<int> const_stdvector = {1, 2, 3};
static std::array<int, 3> stdarray = {1, 2, 3};
static const std::array<int, 3> const_stdarray = {1, 2, 3};
static std::deque<int> stddeque = {1, 2, 3};
static const std::deque<int> const_stddeque = {1, 2, 3};
static std::list<int> stdlist = {1, 2, 3};
static const std::list<int> const_stdlist = {1, 2, 3};
static std::forward_list<int> stdforward_list = {1, 2, 3};
static const std::forward_list<int> const_stdforward_list = {1, 2, 3};
static std::set<int> stdset = {1, 2, 3};
static const std::set<int> const_stdset = {1, 2, 3};
static std::multiset<int> stdmultiset = {1, 2, 3};
static const std::multiset<int> const_stdmultiset = {1, 2, 3};
static std::unordered_set<int> stdunordered_set = {1, 2, 3};
static const std::unordered_set<int> const_stdunordered_set = {1, 2, 3};
static std::unordered_multiset<int> stdunordered_multiset = {1, 2, 3};
static const std::unordered_multiset<int> const_stdunordered_multiset = {1, 2, 3};

static std::map<int, int> stdmap = {{1, 111}, {2, 222}, {3, 333}};
static std::multimap<int, int> stdmultimap = {{1, 111}, {2, 222}, {3, 333}};
static std::unordered_map<int, int> stdunordered_map = {{1, 111}, {2, 222}, {3, 333}};
static std::unordered_multimap<int, int> stdunordered_multimap = {{1, 111}, {2, 222}, {3, 333}};

#define LISTOF_FOR_LIST int_array,                                \
                        const_int_array,                          \
                        intptr_t_array, /*const_intptr_t_array,*/ \
                        stdvector,                                \
                        const_stdvector,                          \
                        stdarray,                                 \
                        const_stdarray,                           \
                        stddeque,                                 \
                        const_stddeque,                           \
                        stdlist,                                  \
                        const_stdlist,                            \
                        stdforward_list,                          \
                        const_stdforward_list

#define HAS_FOR_LIST stdvector,                     \
                     const_stdvector,               \
                     stdarray,                      \
                     const_stdarray,                \
                     stddeque,                      \
                     const_stddeque,                \
                     stdlist,                       \
                     const_stdlist,                 \
                     stdforward_list,               \
                     const_stdforward_list, stdset, \
                     const_stdset,                  \
                     stdmultiset,                   \
                     const_stdmultiset,             \
                     stdunordered_set,              \
                     const_stdunordered_set,        \
                     stdunordered_multiset,         \
                     const_stdunordered_multiset

#define HAS_FOR_MAP stdmap,           \
                    stdmultimap,      \
                    stdunordered_map, \
                    stdunordered_multimap

SUITE(cast containers)
{
   Case(OK ListOf)
   {
#define TheCheck(x) OK(ListOf(1, 2, 3), x);
      H2Foreach(TheCheck, (LISTOF_FOR_LIST));
#undef TheCheck
   }

   Case(OK Has for list)
   {
#define TheCheck(x) OK(Has(1), x);
      H2Foreach(TheCheck, (HAS_FOR_LIST));
#undef TheCheck
   }

   Case(OK Has for map)
   {
#define TheCheck(x) OK(Has(Pair(3, 333)), x);
      H2Foreach(TheCheck, (HAS_FOR_MAP));
#undef TheCheck
   }

   Case(OK AllOf)
   {
#define TheCheck(x) OK(AllOf(ListOf(1, 2, 3), _), x);
      H2Foreach(TheCheck, (LISTOF_FOR_LIST));
#undef TheCheck
#define TheCheck(x) OK(AllOf(Has(2), _), x);
      H2Foreach(TheCheck, (HAS_FOR_LIST));
#undef TheCheck
   }
}
