#include "../source/h2_unit.cpp"

namespace {

class Shape {
 public:
   int x, y;

   Shape() : x(0), y(0) {}

 private:
   static int fly(int x, int y) { return 0; }
   static int fly(int xy) { return 1; }

   int go(int x, int y) { return 1; }
   int go(int xy) { return 2; }

   virtual int work(int x, int y) { return 3; }
   virtual int work(int xy) { return 4; }
};

class Rect : public Shape {
 public:
   Rect(int _x, double _y) {}
   virtual int work(int x, int y) { return 4; }
};

class Circle : public Shape {
 public:
   Circle(char* _x) {}
   virtual int work(int x, int y) { return 5; }
};

class Color {
 public:
   int r, g, b;
   virtual ~Color() {}
   virtual int print(int x, int y) { return 10; }
};

class Six : public Rect, public Color {
 public:
   Six(int _1, int _2, int _3, int _4, int _5, int _6) : Rect(0, 0) {}
   virtual int work(int x, int y) { return 6; }
   virtual int print(int x, int y) { return 20; }
};

class A_construct_class {
 public:
   A_construct_class() = delete;
   A_construct_class(int, int, int, int, int, int, int, int, int, int, int) {}
   int func() { return 0; }
};

class A_abstract_class {
 public:
   A_abstract_class() {}
   virtual ~A_abstract_class() {}
   int f1() { return 1; };
   virtual int f2() = 0;
};

class A_derived_class : public A_abstract_class {
 public:
   int f2() override { return 2; }
};

H2MATCHER(IsOdd, ("is not Odd"))
{
   return a % 2 == 1;
}

template <typename Class, typename Signature>
struct __h2_mfp;

template <typename Class, typename Return, typename... Args>
struct __h2_mfp<Class, Return(Args...)> {
   static long long B(Return (Class::*f)(Args...))
   {
      union {
         Return (Class::*f)(Args...);
         void* p;
         long long v;
      } u{f};

      return u.v;
   }
};

SUITE(mfp)
{
   Shape shape;
   Rect rect(0, 0);
   Circle circle("");
   Six six(1, 2, 3, 4, 5, 6);
   A_construct_class a_construct_class(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
   A_derived_class a_derived_class;

   Case(normal member function)
   {
      void* p1 = h2::h2_mfp<Shape, int(int, int)>::A(&Shape::go);
      OK(NotNull, p1);
      typedef int (*Shape_Go1)(Shape*, int, int);
      OK(1, ((Shape_Go1)p1)(nullptr, 1, 1));

      void* p2 = h2::h2_mfp<Shape, int(int)>::A(&Shape::go);
      OK(NotNull, p2);
      typedef int (*Shape_Go2)(Shape*, int);
      OK(2, ((Shape_Go2)p2)(nullptr, 1));

      void* p3 = h2::h2_mfp<Rect, int(int, int)>::A(&Rect::go);
      OK(NotNull, p3);
      typedef int (*Rect_Go1)(Rect*, int, int);
      OK(1, ((Rect_Go1)p3)(nullptr, 1, 1));

      void* p4 = h2::h2_mfp<Circle, int(int)>::A(&Circle::go);
      OK(NotNull, p4);
      typedef int (*Circle_Go2)(Circle*, int);
      OK(2, ((Circle_Go2)p4)(nullptr, 1));
   }

   Case(virtual member function)
   {
      void* p1 = h2::h2_mfp<Shape, int(int, int)>::A(&Shape::work);
      OK(NotNull, p1);
      typedef int (*Shape_Work)(Shape*, int, int);
      OK(3, ((Shape_Work)p1)(&shape, 1, 1));

      void* p2 = h2::h2_mfp<Rect, int(int, int)>::A(&Rect::work);
      OK(NotNull, p2);
      typedef int (*Rect_Work)(Rect*, int, int);
      OK(4, ((Rect_Work)p2)(&rect, 1, 1));

      void* p3 = h2::h2_mfp<Circle, int(int, int)>::A(&Circle::work);
      OK(NotNull, p3);
      typedef int (*Circle_Work)(Circle*, int, int);
      OK(5, ((Circle_Work)p3)(&circle, 1, 1));

      void* p4 = h2::h2_mfp<Six, int(int, int)>::A(&Six::work);
      OK(NotNull, p4);
      typedef int (*Six_Work)(Six*, int, int);
      OK(6, ((Six_Work)p4)(&six, 1, 1));

      void* p5 = h2::h2_mfp<Six, int(int, int)>::A(&Six::print);
      OK(NotNull, p5);
      typedef int (*Six_Print)(Six*, int, int);
      OK(20, ((Six_Print)p5)(&six, 1, 1));
   }

#if !defined _WIN32
   Case(B)
   {
      // OK(Ge(0x10000), __h2_mfp<Shape, int(int, int)>::B(&Shape::go));
      // OK(IsOdd && Lt(1000 * 8), __h2_mfp<Shape, int(int, int)>::B(&Shape::work));
      // OK(IsOdd && Lt(1000 * 8), __h2_mfp<Color, int(int, int)>::B(&Color::print));
      // OK(IsOdd && Lt(1000 * 8), __h2_mfp<Six, int(int, int)>::B(&Six::work));
      // OK(IsOdd && Lt(1000 * 8), __h2_mfp<Six, int(int, int)>::B(&Six::print));
   }
#endif

   Case(no default constructor)
   {
      MOCK(A_construct_class, func, int, (), Once())
      {
         return 1;
      };
      A_construct_class a(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      OK(1, a.func());
   }

   Case(abstract class)
   {
      MOCK(A_abstract_class, f1, int, (), Once()) { return -1; };
      MOCK(A_abstract_class, f2, int, (), Times(0)) { return -2; };

      A_derived_class a;
      OK(-1, a.f1());
      OK(2, a.f2());
   }
}

CASE(is static member function)
{
   OK(h2::h2_mfp<Shape, int(int, int)>::is_static_member_function(&Shape::fly));
   OK(h2::h2_mfp<Shape, int(int)>::is_static_member_function(&Shape::fly));

   OK(!h2::h2_mfp<Shape, int(int, int)>::is_static_member_function(&Shape::go));
   OK(!h2::h2_mfp<Shape, int(int)>::is_static_member_function(&Shape::go));

   OK(!h2::h2_mfp<Shape, int(int, int)>::is_static_member_function(&Shape::work));
   OK(!h2::h2_mfp<Shape, int(int)>::is_static_member_function(&Shape::work));
}

}  // namespace

#if 0

#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>

class A
{
public:
  long long a;
  A() {}
  const char* f0() { return "fa0"; }
  virtual const char* f1() { return "fa1"; }
  virtual const char* f2() { return "fa2"; }
};

class B: public A
{
public:
  long long b;
  B() {}
  const char* f0() { return "fb0"; }
  virtual const char* f1() { return "fb1"; }
  virtual const char* f2() { return "fb2"; }
};

class C: public A
{
public:
  long long c;
  C() {}
  const char* f0() { return "fc0"; }
  virtual const char* f1() { return "fc1"; }
  virtual const char* f2() { return "fc2"; }
};

class D: public B, public C
{
public:
  long long d;
  D() {}
  const char* f0() { return "fd0"; }
  virtual const char* f1() { return "fd1"; }
  virtual const char* f2() { return "fd2"; }
};

int
main(int argc, char* argv[])
{
  A a;
  B b;
  C c;
  D d;

  printf("&A=%p, &A.a=%p", &a, &a.a);
  printf(", &A.f0=%p", &A::f0);
  printf(", &A.f1=%p", &A::f1);
  printf(", &A.f2=%p\n", &A::f2);

  printf("&B=%p, &B.b=%p", &b, &b.b);
  printf(", &B.f0=%p", &B::f0);
  printf(", &B.f1=%p", &B::f1);
  printf(", &B.f2=%p\n", &B::f2);

  printf("&C=%p, &C.c=%p", &c, &c.c);
  printf(", &C.f0=%p", &C::f0);
  printf(", &C.f1=%p", &C::f1);
  printf(", &C.f2=%p\n", &C::f2);

  printf("&D=%p, &D.d=%p", &d, &d.d);
  printf(", &D.f0=%p", &D::f0);
  printf(", &D.f1=%p", &D::f1);
  printf(", &D.f2=%p\n", &D::f2);

  return 0;
}

// Linux & macOS
// &A=0x7ffeefae54b0, &A.a=0x7ffeefae54b8, &A.f0=0x10011abe0, &A.f1=0x1, &A.f2=0x9
// &B=0x7ffeefae5498, &B.b=0x7ffeefae54a8, &B.f0=0x10011ac00, &B.f1=0x1, &B.f2=0x9
// &C=0x7ffeefae5480, &C.c=0x7ffeefae5490, &C.f0=0x10011ac20, &C.f1=0x1, &C.f2=0x9
// &D=0x7ffeefae5448, &D.d=0x7ffeefae5478, &D.f0=0x10011ac40, &D.f1=0x1, &D.f2=0x9

// Windows
// &B=000000016B2FF828, &B.b=000000016B2FF838, &B.f0=00007FF7F3661145, &B.f1=00007FF7F36611EA, &B.f2=00007FF7F366123F
// &C=000000016B2FF858, &C.c=000000016B2FF868, &C.f0=00007FF7F3661217, &C.f1=00007FF7F366111D, &C.f2=00007FF7F366126C
// &D=000000016B2FF888, &D.d=000000016B2FF8B8, &D.f0=000000016B2FF920, &D.f1=000000016B2FF930, &D.f2=000000016B2FF940

#endif
