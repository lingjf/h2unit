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

static int c0 = 0;
static int c1 = 0;
static int c2 = 0;
static int a1 = 0;
static int a2 = 0;

static void Rv_reset()
{
   c0 = 0;
   c1 = 0;
   c2 = 0;
   a1 = 0;
   a2 = 0;
}

static int Rv_times()
{
   return c0 + c1 + c2 + a1 + a2;
}

#define RvPrint 0

struct Rv {
   int v = 0;

   Rv()
   {
      c0++;
#if RvPrint
      printf("Rv()\n");
#endif
   }

   Rv(const Rv& other)
   {
      c1++;
#if RvPrint
      printf("Rv(const Rv&)\n");
#endif
   }

   Rv(Rv&& other)
   {
      c2++;
#if RvPrint
      printf("Rv(const Rv&&)\n");
#endif
   }

   Rv& operator=(const Rv& other)
   {
      a1++;
#if RvPrint
      printf("Rv=const Rv&\n");
#endif
      return *this;
   }

   Rv& operator=(Rv&& other)
   {
      a2++;
#if RvPrint
      printf("Rv=const Rv&&\n");
#endif
      return *this;
   }
};

struct Rv_vector : std::vector<Rv> {
   Rv_vector(std::initializer_list<Rv> il) : std::vector<Rv>(il) {}

   Rv_vector()
   {
      c0++;
#if RvPrint
      printf("Rv_vector()\n");
#endif
   }

   Rv_vector(const Rv_vector& other)
   {
      c1++;
      for (auto& t : other) push_back(t);
#if RvPrint
      printf("Rv_vector(const Rv_vector&)\n");
#endif
   }

   Rv_vector(Rv_vector&& other)
   {
      c2++;
      for (auto& t : other) push_back(t);
#if RvPrint
      printf("Rv_vector(const Rv_vector&&)\n");
#endif
   }

   Rv_vector& operator=(const Rv_vector& other)
   {
      a1++;
      clear();
      for (auto& t : other) push_back(t);
#if RvPrint
      printf("Rv_vector=const Rv_vector&\n");
#endif
      return *this;
   }

   Rv_vector& operator=(Rv_vector&& other)
   {
      a2++;
      clear();
      for (auto& t : other) push_back(t);
#if RvPrint
      printf("Rv_vector=const Rv_vector&&\n");
#endif
      return *this;
   }
};

H2MATCHER(VisZero)
{
   return a.v == 0;
}

template <typename T>
static int f2(const T& a)
{
   return 1;
}

template <typename T>
static int f1(const T& a)
{
   return f2(a);
}

SUITE(non_copy)
{
   Case(C++)
   {
      Rv rv;
      Rv_reset();

      f1(rv);

      OK(0, Rv_times());
   }

   Case(OK)
   {
      Rv rv;
      Rv_reset();

      OK(VisZero(), rv);
      OK(0, Rv_times());
   }

   Case(vector)
   {
      std::vector<Rv> a(3);
      Rv_reset();

      OK(ListOf(VisZero(), VisZero(), VisZero()), a);
      OK(0, Rv_times());
   }

   Case(user vector)
   {
      Rv_vector a = {Rv(), Rv(), Rv()};
      Rv_reset();

      OK(ListOf(VisZero(), VisZero(), VisZero()), a);
      OK(0, Rv_times());
   }

   Case(cast)
   {
      Rv rv;
      Rv_reset();

      const Rv& r2 = rv;
      OK(VisZero(), r2);
      auto r3 = static_cast<Rv>(r2);
      OK(VisZero(), r3);
      OK(1, Rv_times());
   }
}
