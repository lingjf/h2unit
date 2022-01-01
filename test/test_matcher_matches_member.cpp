#include "../source/h2_unit.cpp"

/*
   In C++ terminology,
      overriding (relating to virtual methods in a class hierarchy);
      overloading (related to a function having the same name but taking different parameters);
      overwrite have hiding of names (via explicit declaration of the same name in a nested declarative region or scope);
*/

static char c7gs[256];

class A5 {
 public:
   char s[256]{'\0'};
   A5()
   {
      strcat(s, "a");
   }
   A5(const A5& a5)
   {
      strcpy(s, a5.s);
      strcat(s, "b");
   }
   A5(A5&& a5)
   {
      strcpy(s, a5.s);
      strcat(s, "c");
   }
};

class A7 {
 public:
   char a = 'a';
   static const char* s() { return "A7::s0"; }
   const char* f() { return "A::f"; }
   const char* f(int _1) { return "A7::f1"; }
   virtual const char* v() { return "A7::v0"; }
};

class B7 : public A7 {
 public:
   int b = 42;

 private:
   static const char* s() { return "B7::s0"; }
   const char* f() { return "B::f"; }
   const char* f(int _1, const char* _2) { return "B7::f2"; }
   virtual const char* v() { return "B7::v0"; }
};

class C7 : public B7 {
 public:
   const char* c;

   C7() : c("c") {}
   virtual ~C7() {}

 private:
   static const char* s() { return "C7::s0"; }
   const char* f() { return "C7::f0"; }
   const char* fx() noexcept { return "C7::fx0"; }
   const char* fc() const { return "C7::fc0"; }
   const char* fcx() const noexcept { return "C7::fcx0"; }
   virtual const char* v() { return "C7::v0"; }
   virtual const char* vx() noexcept { return "C7::vx0"; }
   virtual const char* vc() const { return "C7::vc0"; }
   virtual const char* vcx() const noexcept { return "C7::vcx0"; }

   virtual const char* v(int _1) { return "C7::v1"; }

   virtual const char* v(int _1, const char* _2) { return "C7::v2"; }

   const char* f(int _1, const char* _2, double _3) { return "C7::f3"; }
   virtual const char* v(int _1, const char* _2, double _3) { return "C7::v3"; }

   const char* ga(A5 a5) { return strcpy(c7gs, a5.s), c7gs; }
   const char* gc(const A5 a5) { return strcpy(c7gs, a5.s), c7gs; }
   const char* gr(A5& a5) { return strcpy(c7gs, a5.s), c7gs; }
};

SUITE(matches member)
{
   Case(h2_member_data_matches)
   {
   }

   Case(h2_member_method_matches)
   {
   }
}

CASE(mfp_decay)
{
   int a1 = 1;
   OK((std::is_same<int, typename h2::mfp_decay<decltype(a1)>::type>::value));
   int* a2 = &a1;
   OK((std::is_same<int*, typename h2::mfp_decay<decltype(a2)>::type>::value));
   int& a3 = a1;
   OK((std::is_same<int&, typename h2::mfp_decay<decltype(a3)>::type>::value));
   int&& a4 = 2;
   OK((std::is_same<int&&, typename h2::mfp_decay<decltype(a4)>::type>::value));

   int const a5 = 1;
   OK((std::is_same<const int, typename h2::mfp_decay<decltype(a5)>::type>::value));
   int const* a6 = &a5;
   OK((std::is_same<const int*, typename h2::mfp_decay<decltype(a6)>::type>::value));
   int const& a7 = a5;
   OK((std::is_same<const int&, typename h2::mfp_decay<decltype(a7)>::type>::value));
   int const&& a8 = 2;
   OK((std::is_same<const int&&, typename h2::mfp_decay<decltype(a8)>::type>::value));

   int* const a9 = nullptr;
   OK((std::is_same<int* const, typename h2::mfp_decay<decltype(a9)>::type>::value));
   int const* const a10 = nullptr;
   OK((std::is_same<const int* const, typename h2::mfp_decay<decltype(a10)>::type>::value));

   C7 c7;
   OK((std::is_same<C7, typename h2::mfp_decay<decltype(c7)>::type>::value));
   C7* c8;
   OK((std::is_same<C7*, typename h2::mfp_decay<decltype(c8)>::type>::value));
   C7& c9 = c7;
   OK((std::is_same<C7&, typename h2::mfp_decay<decltype(c9)>::type>::value));
   const C7 c10;
   OK((std::is_same<const C7, typename h2::mfp_decay<decltype(c10)>::type>::value));
   const C7* c11;
   OK((std::is_same<const C7*, typename h2::mfp_decay<decltype(c11)>::type>::value));
   const C7& c12 = c10;
   OK((std::is_same<const C7&, typename h2::mfp_decay<decltype(c12)>::type>::value));
   const C7* const c13 = nullptr;
   OK((std::is_same<const C7* const, typename h2::mfp_decay<decltype(c13)>::type>::value));

   OK((std::is_same<int&, typename h2::mfp_decay<decltype(std::ref(a1))>::type>::value));
   OK((std::is_same<C7&, typename h2::mfp_decay<decltype(std::ref(c7))>::type>::value));

   // std::cout << h2::h2_cxa::type_name<typename h2::mfp_decay<decltype(c13)>::type>() << std::endl;
}

SUITE(Member Data)
{
   C7 c7;
   Case(object)
   {
      OK(Member('a', &C7::a), c7);
      OK(Member(42, &C7::b), c7);
      OK(Member("c", &C7::c), c7);
   }

   Case(addr)
   {
      OK(Member('a', &C7::a), &c7);
      OK(Member(42, &C7::b), &c7);
      OK(Member("c", &C7::c), &c7);
   }

   Case(ref)
   {
      C7& b = c7;
      OK(Member('a', &C7::a), b);
      OK(Member(42, &C7::b), b);
      OK(Member("c", &C7::c), b);
   }

   Case(ptr)
   {
      C7* b = &c7;
      OK(Member('a', &C7::a), b);
      OK(Member(42, &C7::b), b);
      OK(Member("c", &C7::c), b);
   }

   Case(combine)
   {
      OK(Member('a', &C7::a) && Member(42, &C7::b) && Member("c", &C7::c), c7);
   }
}

#if __cplusplus >= 201402L || (defined _MSVC_LANG && _MSVC_LANG >= 201402L)

CASE(mfp)
{
   OK(!(std::is_same<decltype(std::mem_fn(&C7::ga)), decltype(h2::mfp<A5>::get(&C7::ga))>::value));
   OK(!(std::is_same<decltype(std::mem_fn(&C7::gr)), decltype(h2::mfp<A5&>::get(&C7::gr))>::value));
}

SUITE(Member Method)
{
   A5 a5;
   C7 c7;

   Case(normal0)
   {
      OK(Member("C7::f0", &C7::f), c7);
      OK(Member("C7::f0", &C7::f, ()), c7);
   }

   Case(normal0 const)
   {
      OK(Member("C7::fc0", &C7::fc), c7);
   }

   Case(normal0 noexcept)
   {
      OK(Member("C7::fx0", &C7::fx), c7);
   }

   Case(normal0 const noexcept)
   {
      OK(Member("C7::fcx0", &C7::fcx), c7);
   }

   Case(virtual0)
   {
      OK(Member("C7::v0", &A7::v), c7);
      OK(Member("C7::v0", &B7::v), c7);
      OK(Member("C7::v0", &C7::v), c7);
   }

   Case(virtual0 noexcept)
   {
      OK(Member("C7::vx0", &C7::vx), c7);
   }

   Case(virtual0 const)
   {
      OK(Member("C7::vc0", &C7::vc), c7);
   }

   Case(virtual0 const)
   {
      OK(Member("C7::vcx0", &C7::vcx), c7);
   }

   Case(static0)
   {
      // OK(Member("C7::s0", &C7::s), c7);
   }

   Case(normal1)
   {
      OK(Member("A7::f1", &A7::f, 0), c7);
      OK(Member("A7::f1", &A7::f, (0)), c7);
      OK(Member("A7::f1", &A7::f, (int)0), c7);
   }

   Case(virtual1)
   {
      OK(Member("C7::v1", &C7::v, 0), c7);
      OK(Member("C7::v1", &C7::v, (0)), c7);
   }

   Case(normal2)
   {
      OK(Member("B7::f2", &B7::f, 0, "1"), c7);
      OK(Member("B7::f2", &B7::f, (0, "1")), c7);
   }

   Case(virtual2)
   {
      OK(Member("C7::v2", &C7::v, 0, "1"), c7);
      OK(Member("C7::v2", &C7::v, (0, "1")), c7);
   }

   Case(normal3)
   {
      OK(Member("C7::f3", &C7::f, 0, "1", 3.14), c7);
      OK(Member("C7::f3", &C7::f, (0, "1", 3.14)), c7);
   }

   Case(virtual3)
   {
      OK(Member("C7::v3", &C7::v, 0, "1", 3.14), c7);
      OK(Member("C7::v3", &C7::v, (0, "1", 3.14)), c7);
   }

   Case(reference)
   {
      OK(Member("a", &C7::gr, std::ref(a5)), c7);

      A5& r5 = a5;
      OK(Member("a", &C7::gr, r5), c7);
   }

   Case(ptr)
   {
      C7* b = &c7;
      OK(Member("C7::f0", &C7::f), b);
      OK(Member("A7::f1", &A7::f, 0), b);
      OK(Member("B7::f2", &B7::f, 0, "1"), b);
      OK(Member("C7::f3", &C7::f, 0, "1", 3.14), b);
      OK(Member("a", &C7::gr, std::ref(a5)), b);
   }
}

SUITE(Member copy arguments)
{
   C7 c7;
   A5 a5;

   Case(invoke)
   {
      OK("ab", c7.ga(a5));
      OK("ab", c7.gc(a5));
      OK("a", c7.gr(a5));
   }

   Case(decltype)
   {
      decltype(std::make_tuple(1, a5)) a;
      OK("a", std::get<1>(a).s);
   }

   Case(variable)
   {
      OK(Member("abbbbbbbbb", &C7::ga, a5), c7);
      OK(Member("abbbbbbbbb", &C7::gc, a5), c7);
      OK(Member("a", &C7::gr, std::ref(a5)), c7);
   }

   Case(temporary)
   {
      OK(Member("acbbbbbbbb", &C7::ga, A5()), c7);
      OK(Member("acbbbbbbbb", &C7::gc, A5()), c7);
   }
}

#endif
