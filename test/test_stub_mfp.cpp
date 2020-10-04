#include "../source/h2_unit.cpp"

namespace test_stub_mfp {

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
      OK(3, ((Shape_Work)p1)(nullptr, 1, 1));

      void* p2 = h2::h2_mfp<Rect, int(int, int)>::A(&Rect::work);
      OK(NotNull, p2);
      typedef int (*Rect_Work)(Rect*, int, int);
      OK(4, ((Rect_Work)p2)(nullptr, 1, 1));

      void* p3 = h2::h2_mfp<Circle, int(int, int)>::A(&Circle::work);
      OK(NotNull, p3);
      typedef int (*Circle_Work)(Circle*, int, int);
      OK(5, ((Circle_Work)p3)(nullptr, 1, 1));

      void* p4 = h2::h2_mfp<Six, int(int, int)>::A(&Six::work);
      OK(NotNull, p4);
      typedef int (*Six_Work)(Six*, int, int);
      OK(6, ((Six_Work)p4)(nullptr, 1, 1));

      void* p5 = h2::h2_mfp<Six, int(int, int)>::A(&Six::print);
      OK(NotNull, p5);
      typedef int (*Six_Print)(Six*, int, int);
      OK(20, ((Six_Print)p5)(nullptr, 1, 1));
   }

   Case(B)
   {
      OK(Ge(0x10000), __h2_mfp<Shape, int(int, int)>::B(&Shape::go));
      OK(IsOdd && Lt(1000 * 8), __h2_mfp<Shape, int(int, int)>::B(&Shape::work));
      OK(IsOdd && Lt(1000 * 8), __h2_mfp<Color, int(int, int)>::B(&Color::print));
      OK(IsOdd && Lt(1000 * 8), __h2_mfp<Six, int(int, int)>::B(&Six::work));
      OK(IsOdd && Lt(1000 * 8), __h2_mfp<Six, int(int, int)>::B(&Six::print));
   }

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

}  // namespace test_stub_mfp
