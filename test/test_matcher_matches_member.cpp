#include "../source/h2_unit.cpp"

class C7 {
 public:
   char a = 'a';
   int b = 42;
   const char* s;

   C7() : s("s") {}
   virtual ~C7() {}

 private:
   static const char* static_f0() { return "C7::static_f0"; }
   const char* normal_f0() { return "C7::normal_f0"; }
   const char* normal_s0() noexcept { return "C7::normal_s0"; }
   const char* normal_c0() const { return "C7::normal_c0"; }
   const char* normal_z0() const noexcept { return "C7::normal_z0"; }
   virtual const char* virtual_f0() { return "C7::virtual_f0"; }
   virtual const char* virtual_s0() noexcept { return "C7::virtual_s0"; }
   virtual const char* virtual_c0() const { return "C7::virtual_c0"; }
   virtual const char* virtual_z0() const noexcept { return "C7::virtual_z0"; }

   const char* normal_f1(int _1) { return "C7::normal_f1"; }
   virtual const char* virtual_f1(int _1) { return "C7::virtual_f1"; }

   const char* normal_f2(int _1, const char* _2) { return "C7::normal_f2"; }
   virtual const char* virtual_f2(int _1, const char* _2) { return "C7::virtual_f2"; }

   const char* normal_f3(int _1, const char* _2, double _3) { return "C7::normal_f3"; }
   virtual const char* virtual_f3(int _1, const char* _2, double _3) { return "C7::virtual_f3"; }
};

SUITE(matches member)
{
   Case(h2_member_matches)
   {
   }
}

SUITE(Member Data)
{
   C7 c7;
   Case(object)
   {
      OK(Member('a', &C7::a), c7);
      OK(Member(42, &C7::b), c7);
      OK(Member("s", &C7::s), c7);
   }

   Case(addr)
   {
      OK(Member('a', &C7::a), &c7);
      OK(Member(42, &C7::b), &c7);
      OK(Member("s", &C7::s), &c7);
   }

   Case(ref)
   {
      C7& b = c7;
      OK(Member('a', &C7::a), b);
      OK(Member(42, &C7::b), b);
      OK(Member("s", &C7::s), b);
   }

   Case(ptr)
   {
      C7* b = &c7;
      OK(Member('a', &C7::a), b);
      OK(Member(42, &C7::b), b);
      OK(Member("s", &C7::s), b);
   }

   Case(combine)
   {
      OK(Member('a', &C7::a) && Member(42, &C7::b) && Member("s", &C7::s), c7);
   }
}

SUITE(Member Function)
{
   C7 c7;

   Case(normal0)
   {
      OK(Member("C7::normal_f0", &C7::normal_f0), c7);
   }

   Case(normal0 noexcept)
   {
      OK(Member("C7::normal_s0", &C7::normal_s0), c7);
   }

   Case(normal0 const)
   {
      OK(Member("C7::normal_c0", &C7::normal_c0), c7);
   }

   Case(normal0 const noexcept)
   {
      OK(Member("C7::normal_z0", &C7::normal_z0), c7);
   }

   Case(virtual0)
   {
      OK(Member("C7::virtual_f0", &C7::virtual_f0), c7);
   }

   Case(virtual0 noexcept)
   {
      OK(Member("C7::virtual_s0", &C7::virtual_s0), c7);
   }

   Case(virtual0 const)
   {
      OK(Member("C7::virtual_c0", &C7::virtual_c0), c7);
   }

   Case(virtual0 const)
   {
      OK(Member("C7::virtual_z0", &C7::virtual_z0), c7);
   }

   Case(static0)
   {
      // OK(Member("C7::static_f0", &C7::static_f0), c7);
   }

   Case(normal1)
   {
      OK(Member("C7::normal_f1", &C7::normal_f1, 0), c7);
   }

   Case(virtual1)
   {
      OK(Member("C7::virtual_f1", &C7::virtual_f1, 0), c7);
   }

   Case(normal2)
   {
      OK(Member("C7::normal_f2", &C7::normal_f2, 0, "1"), c7);
   }

   Case(virtual2)
   {
      OK(Member("C7::virtual_f2", &C7::virtual_f2, 0, "1"), c7);
   }

   Case(normal3)
   {
      OK(Member("C7::normal_f3", &C7::normal_f3, 0, "1", 3.14), c7);
   }

   Case(virtual3)
   {
      OK(Member("C7::virtual_f3", &C7::virtual_f3, 0, "1", 3.14), c7);
   }

   Case(ptr)
   {
      C7* b = &c7;
      OK(Member("C7::normal_f0", &C7::normal_f0), b);
      OK(Member("C7::normal_f1", &C7::normal_f1, 0), b);
      OK(Member("C7::normal_f2", &C7::normal_f2, 0, "1"), b);
      OK(Member("C7::normal_f3", &C7::normal_f3, 0, "1", 3.14), b);
      OK(Member("C7::normal_f3", &C7::normal_f3, 0, "1", 3.14), b);
   }
}
