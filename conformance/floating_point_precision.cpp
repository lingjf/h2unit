#include "__common.hpp"

#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>

template <typename T>
std::string f(T a)
{
   std::ostringstream oss;
   oss << std::fixed << std::setprecision(std::numeric_limits<T>::digits10 + 1)
       << a;
   return oss.str();
}

#define F2(value)              \
   {H2PP_STR(value),           \
             f((float)value),  \
             f((double)value), \
             f((long double)value)},

// a##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s##t##u

#define F3(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) \
   F2(a##.)                                                               \
   F2(a##.##b)                                                            \
   F2(a##.##b##c)                                                         \
   F2(a##.##b##c##d)                                                      \
   F2(a##.##b##c##d##e)                                                   \
   F2(a##.##b##c##d##e##f)                                                \
   F2(a##.##b##c##d##e##f##g)                                             \
   F2(a##.##b##c##d##e##f##g##h)                                          \
   F2(a##.##b##c##d##e##f##g##h##i)                                       \
   F2(a##.##b##c##d##e##f##g##h##i##j)                                    \
   F2(a##.##b##c##d##e##f##g##h##i##j##k)                                 \
   F2(a##.##b##c##d##e##f##g##h##i##j##k##l)                              \
   F2(a##.##b##c##d##e##f##g##h##i##j##k##l##m)                           \
   F2(a##.##b##c##d##e##f##g##h##i##j##k##l##m##n)                        \
   F2(a##.##b##c##d##e##f##g##h##i##j##k##l##m##n##o)                     \
   F2(a##.##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p)                  \
   F2(a##.##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q)               \
   F2(a##.##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r)            \
   F2(a##.##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s)         \
   F2(a##.##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s##t)      \
   F2(a##.##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s##t##u)

#define F4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u) \
   F2(a##.##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##.##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##.##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##.##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##.##f##g##h##i##j##k##l##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##.##g##h##i##j##k##l##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##.##h##i##j##k##l##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##.##i##j##k##l##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##.##j##k##l##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##.##k##l##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##k##.##l##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##k##l##.##m##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##k##l##m##.##n##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##k##l##m##n##.##o##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##k##l##m##n##o##.##p##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##.##q##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##.##r##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##.##s##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s##.##t##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s##t##.##u)   \
   F2(a##b##c##d##e##f##g##h##i##j##k##l##m##n##o##p##q##r##s##t##u##.)

std::string
highlight(std::string literals, std::string str)
{
   int len1 = literals.length();
   int len2 = str.length();

   int i = 0;
   for (; i < len2 && i < len1; ++i) {
      if (literals[i] != str[i]) break;
   }

   std::string ret;
   ret.append("\033[32m");
   ret.append(str.c_str(), i);
   ret.append("\033[0m");
   ret.append(str.c_str() + i);

   return ret;
}

int main(int argc, char** argv)
{
   printf("%s %s\n\n", cc_version(), std_version());

   //  3.14159265358979323846
   //  2.71828182845904523536

   std::cout << "\033[90mliterals                  float        double                long double  \033[0m" << std::endl;

   struct A1 {
      const char* a;
      std::string float_s;
      std::string double_s;
      std::string long_double_s;
   };

   std::vector<A1> as = {
     F3(3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3, 2, 3, 8, 4, 6)
       F3(2, 7, 1, 8, 2, 8, 1, 8, 2, 8, 4, 5, 9, 0, 4, 5, 2, 3, 5, 3, 6)};

   for (size_t i = 0; i < as.size(); i++) {
      std::cout << std::setw(22) << std::left << as[i].a << "    ";
      std::cout << highlight(as[i].a, as[i].float_s) << "    ";
      std::cout << highlight(as[i].a, as[i].double_s) << "    ";
      std::cout << highlight(as[i].a, as[i].long_double_s) << "    " << std::endl;
   }

   // std::vector<A1> bs = {
   //   F4(3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3, 2, 3, 8, 4, 6)
   //     F4(2, 7, 1, 8, 2, 8, 1, 8, 2, 8, 4, 5, 9, 0, 4, 5, 2, 3, 5, 3, 6)};

   // for (size_t i = 0; i < bs.size(); i++) {
   //    std::cout << std::setw(22) << std::left << bs[i].a << "    ";
   //    std::cout << highlight(bs[i].a, bs[i].float_s) << "    ";
   //    std::cout << highlight(bs[i].a, bs[i].double_s) << "    ";
   //    std::cout << highlight(bs[i].a, bs[i].long_double_s) << "    " << std::endl;
   // }

   std::cout << std::endl;
   return 0;
}
