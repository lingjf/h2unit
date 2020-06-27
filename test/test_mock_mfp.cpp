#include "../source/h2_unit.cpp"

namespace {

class Shape {
 public:
   int x, y;

   Shape() : x(0), y(0) {}

   static int fly(int x, int y) { return 0; }

   int go(int x, int y) { return 1; }

   int go(int xy) { return 2; }

   virtual int work(int x, int y) { return 3; }
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

class Six : public Rect {
 public:
   Six(int _1, int _2, int _3, int _4, int _5, int _6) : Rect(0, 0) {}
   virtual int work(int x, int y) { return 6; }
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
      typedef int (*Shape_Go1)(int, int);
      OK(1, ((Shape_Go1)p1)(1, 1));

      void* p2 = h2::h2_mfp<Shape, int(int)>::A(&Shape::go);
      OK(NotNull, p2);
      typedef int (*Shape_Go2)(int);
      OK(2, ((Shape_Go2)p2)(1));

      void* p3 = h2::h2_mfp<Rect, int(int, int)>::A(&Rect::go);
      OK(NotNull, p3);
      typedef int (*Rect_Go1)(int, int);
      OK(1, ((Rect_Go1)p3)(1, 1));

      void* p4 = h2::h2_mfp<Circle, int(int)>::A(&Circle::go);
      OK(NotNull, p4);
      typedef int (*Circle_Go2)(int);
      OK(2, ((Circle_Go2)p4)(1));
   }

   Case(virtual member function)
   {
      void* p1 = h2::h2_mfp<Shape, int(int, int)>::A(&Shape::work);
      OK(NotNull, p1);
      typedef int (*Shape_Work)(int, int);
      OK(3, ((Shape_Work)p1)(1, 1));

      void* p2 = h2::h2_mfp<Rect, int(int, int)>::A(&Rect::work);
      OK(NotNull, p2);
      typedef int (*Rect_Work)(int, int);
      OK(4, ((Rect_Work)p2)(1, 1));

      void* p3 = h2::h2_mfp<Circle, int(int, int)>::A(&Circle::work);
      OK(NotNull, p3);
      typedef int (*Circle_Work)(int, int);
      OK(5, ((Circle_Work)p3)(1, 1));

      void* p4 = h2::h2_mfp<Six, int(int, int)>::A(&Six::work);
      OK(NotNull, p4);
      typedef int (*Six_Work)(int, int);
      OK(6, ((Six_Work)p4)(1, 1));
   }

   Case(no default constructor)
   {
      MOCK(A_construct_class, func, int()).once() = []() {
         return 1;
      };
      A_construct_class a(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      OK(1, a.func());
   }

   Case(abstract class)
   {
      MOCK(A_abstract_class, f1, int()).once() = []() { return -1; };
      MOCK(A_abstract_class, f2, int()).times(0) = []() { return -2; };

      A_derived_class a;
      OK(-1, a.f1());
      OK(2, a.f2());
   }
}

}  // namespace
