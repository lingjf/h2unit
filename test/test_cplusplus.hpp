// https://en.cppreference.com/w/cpp/container

// Sequence containers
#include <vector>
#include <deque>
#include <array>
#include <list>
#include <forward_list>

// Associative containers
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

// Container adaptors
#include <queue>  // std::priority_queue, std:queue
#include <stack>  // std::stack

// Other
#include <valarray>
#include <tuple>

// https://en.cppreference.com/w/cpp/language/types
#define I_integer I_short, I_int, I_long, I_long_long

#define I_short I_signed_short I_unsigned_short
#define I_int I_signed_int I_unsigned_int
#define I_long I_signed_long I_unsigned_long
#define I_long_long I_signed_long_long I_unsigned_long_long

#define I_signed_short short,        \
                       short int,    \
                       signed short, \
                       signed short int

#define I_unsigned_short unsigned short, \
                         unsigned short int

#define I_signed_int int,    \
                     signed, \
                     signed int

#define I_unsigned_int unsigned, \
                       unsigned int

#define I_signed_long long,        \
                      long int,    \
                      signed long, \
                      signed long int

#define I_unsigned_long unsigned long, \
                        unsigned long int

#define I_signed_long_long long long,        \
                           long long int,    \
                           signed long long, \
                           signed long long int

#define I_unsigned_long_long unsigned long long, \
                             unsigned long long int

#define I_float_point float, double, long double
