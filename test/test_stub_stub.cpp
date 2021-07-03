#include "../source/h2_unit.cpp"

namespace {

int foobar(int, const int&)
{
   return 1;
}

namespace ns111 {
   int foobar(int, const int&)
   {
      return 1;
   }
}  // namespace ns111

int STUB_foobar(int, const int&)
{
   return -1;
}
int STUB2_foobar(int, const int&)
{
   return -2;
}

SUITE(stub)
{
   Case(stub simple)
   {
      OK(1, foobar(0, 1));

      STUB(foobar, STUB_foobar);
      OK(-1, foobar(0, 1));

      UNSTUB(foobar);
      OK(1, foobar(0, 1));
   }

   Case(stub overwrite)
   {
      OK(1, foobar(0, 1));

      STUB(foobar, STUB_foobar);
      OK(-1, foobar(0, 1));
      STUB(foobar, STUB2_foobar);
      OK(-2, foobar(0, 1));

      UNSTUB(foobar);
      OK(1, foobar(0, 1));
   }

   Case(stub chained)
   {
      OK(1, foobar(0, 1));

      STUB(foobar, STUB_foobar);
      OK(-1, foobar(0, 1));

      STUB(STUB_foobar, STUB2_foobar);
      OK(-2, foobar(0, 1));

      UNSTUB(STUB_foobar);
      OK(-1, foobar(0, 1));

      UNSTUB(foobar);
      OK(1, foobar(0, 1));
   }

   Case(stub in namespace)
   {
      OK(1, ns111::foobar(0, 1));

      STUB(ns111::foobar, STUB_foobar);
      OK(-1, ns111::foobar(0, 1));

      UNSTUB(ns111::foobar);
      OK(1, ns111::foobar(0, 1));
   }
}

class Shape {
 private:
   int x, y;

 public:
   Shape() : x(0), y(0) {}

   int value;

   static int fly(int x, int y) { return 0; }

   int go(int x, int y)
   {
      this->x += x;
      this->y += y;
      return 0;
   }

   int go(int xy)
   {
      this->x += xy;
      this->y += xy;
      return 0;
   }

   virtual int work(int x, int y)
   {
      this->x *= x;
      this->y *= y;
      return 0;
   }
};

namespace ns112 {
   class Rect : public Shape {
    public:
      int go(int x, int y)
      {
         return 0;
      }
   };
}  // namespace ns112

int Shape_go_fake(Shape* shape, int x, int y)
{
   return x + y;
}
int Shape_work_fake(Shape* shape, int x, int y)
{
   return x + y;
}
int Shape_fly_fake(int x, int y)
{
   return x + y;
}

int Rect_go_fake(ns112::Rect* rect, int x, int y)
{
   return x + y;
}

SUITE(stub member function)
{
   Case(normal member function)
   {
      Shape shape;
      OK(0, shape.go(111, 111));

      STUB(Shape, go, int, (int a, int b), Shape_go_fake);
      OK(222, shape.go(111, 111));

      UNSTUB(Shape, go, int, (int a, int b));
      OK(0, shape.go(111, 111));
   }

   Case(virtual member function)
   {
      Shape shape;
      OK(0, shape.work(111, 111));

      STUB(Shape, work, int, (int a, int b), Shape_work_fake);
      OK(222, shape.work(111, 111));

      UNSTUB(Shape, work, int, (int a, int b));
      OK(0, shape.work(111, 111));
   }

   Case(static member function)
   {
      OK(0, Shape::fly(111, 111));

      STUB(Shape::fly, int, (int a, int b), Shape_fly_fake);
      OK(222, Shape::fly(111, 111));

      UNSTUB(Shape::fly, int, (int a, int b));
      OK(0, Shape::fly(111, 111));
   }

   Case(in namespace)
   {
      ns112::Rect rect;
      OK(0, rect.go(111, 111));

      STUB(ns112::Rect, go, int, (int a, int b), Rect_go_fake);
      OK(222, rect.go(111, 111));

      UNSTUB(ns112::Rect, go, int, (int a, int b));
      OK(0, rect.go(111, 111));
   }
}

template <typename T>
int foobar1(T a)
{
   return 1;
}
int STUB_foobar1(int a)
{
   return -1;
}

template <typename T1, typename T2>
int foobar2(T1 a, T2 b)
{
   return 2;
}
int STUB_foobar2(int a, float b)
{
   return -2;
}

template <typename T>
struct Foo1 {
   T m = 0;
   template <typename U>
   int bar1(U a)
   {
      return 1;
   }
   static int bar2(T a)
   {
      return 2;
   }
};

int STUB_Foo1bar1(void* foo1, int a)
{
   return -1;
}

int STUB_Foo1bar2(int a)
{
   return -2;
}

template <typename T1, typename T2>
struct Foo2 {
   T1 m1 = 0;
   T2 m2 = 0;

   template <typename U1, typename U2>
   int bar1(U1 a, U2 b)
   {
      return 1;
   }

   template <typename U1, typename U2>
   static int bar2(U1 a, U2 b)
   {
      return 2;
   }

   template <typename U1, typename U2>
   std::pair<U1, U2> bar3(U1 a, U2 b)
   {
      return std::make_pair(a, b);
   }
};

int STUB_Foo2bar1(void* foo2, int a, float b)
{
   return -1;
}

int STUB_Foo2bar2(int a, float b)
{
   return -2;
}

std::pair<int, float> STUB_Foo2bar3(void* foo2, int a, float b)
{
   return std::make_pair(-3, a + b);
}

SUITE(stub template)
{
   Case(function 1 typename)
   {
      OK(1, foobar1<int>(0));

      STUB(foobar1<int>, STUB_foobar1);
      OK(-1, foobar1<int>(0));

      UNSTUB(foobar1<int>);
      OK(1, foobar1<int>(0));
   }

   Case(function 2 typename)
   {
      OK(2, foobar2<int, float>(0, 0));

      STUB((foobar2<int, float>), STUB_foobar2);
      OK(-2, (foobar2<int, float>(0, 0)));

      UNSTUB((foobar2<int, float>));
      OK(2, (foobar2<int, float>(0, 0)));
   }

   Case(member function 1 typename)
   {
      Foo1<int> a1;
      OK(1, a1.bar1(0));

      STUB(Foo1<int>, bar1<int>, int, (int a), STUB_Foo1bar1);
      OK(-1, a1.bar1(0));

      UNSTUB(Foo1<int>, bar1<int>, int, (int a));
      OK(1, a1.bar1(0));
   }

   Case(static member function 1 typename)
   {
      STUB(Foo1<int>::bar2, int, (int a), STUB_Foo1bar2);
      OK(-2, Foo1<int>::bar2(0));

      UNSTUB(Foo1<int>::bar2, int, (int a));
      OK(2, Foo1<int>::bar2(0));
   }

   Case(member function 2 typename)
   {
      Foo2<int, float> a1;
      OK(1, (a1.bar1<int, float>(0, 0)));

      STUB((Foo2<int, float>), (bar1<int, float>), int, (int a, float b), STUB_Foo2bar1);
      OK(-1, (a1.bar1<int, float>(0, 0)));

      UNSTUB((Foo2<int, float>), (bar1<int, float>), int, (int a, float b));
      OK(1, (a1.bar1<int, float>(0, 0)));
   }

   Case(static member function 2 typename)
   {
      STUB((Foo2<int, float>::bar2<int, float>), int, (int a, float b), STUB_Foo2bar2);
      OK(-2, (Foo2<int, float>::bar2<int, float>(0, 0)));

      UNSTUB((Foo2<int, float>::bar2<int, float>), int, (int a, float b));
      OK(2, (Foo2<int, float>::bar2<int, float>(0, 0)));
   }

   Case(return template)
   {
      Foo2<int, float> a1;
      OK(Pair(0, 0), (a1.bar3<int, float>(0, 0)));

      STUB((Foo2<int, float>), (bar3<int, float>), (std::pair<int, float>), (int a, float b), STUB_Foo2bar3);
      OK(Pair(-3, 0), (a1.bar3<int, float>(0, 0)));

      UNSTUB((Foo2<int, float>), (bar3<int, float>), (std::pair<int, float>), (int a, float b));
      OK(Pair(0, 0), (a1.bar3<int, float>(0, 0)));
   }
}

SUITE(STUBS)
{
   Case(lambdas normal function)
   {
      STUBS(foobar, int, (int a, const int& b)) { return a + b; };
      OK(222, foobar(111, 111));

      UNSTUB(foobar, int, (int a, const int& b));
      OK(1, foobar(111, 111));
   }

   Case(lambdas normal member function)
   {
      Shape shape;

      STUBS(Shape, go, int, (int a, int b)) { return a + b; };
      OK(222, shape.go(111, 111));

      UNSTUB(Shape, go, int, (int a, int b));
      OK(0, shape.go(111, 111));
   }

   Case(lambdas virtual member function)
   {
      Shape shape;

      STUBS(Shape, work, int, (int a, int b)) { return a + b; };
      OK(222, shape.work(111, 111));

      UNSTUB(Shape, work, int, (int a, int b));
      OK(0, shape.work(111, 111));
   }

   Case(lambdas static member function)
   {
      STUBS(Shape::fly, int, (int a, int b)) { return a + b; };
      OK(222, Shape::fly(111, 111));

      UNSTUB(Shape::fly, int, (int a, int b));
      OK(0, Shape::fly(111, 111));
   }
}

}  // namespace

extern "C" {
int foobar1_bystub(int a)
{
   return 0;
}
}

int foobar2_bystub(int a)
{
   return 0;
}

namespace ns113 {
int foobar3_bystub(int a)
{
   return 0;
}
}  // namespace ns113

int STUB_foobar_bystub(int a)
{
   return -1;
}

SUITE(stub name)
{
   Case("foobar1_bystub")
   {
      OK(0, foobar1_bystub(0));

      STUB("foobar1_bystub", STUB_foobar_bystub);
      OK(-1, foobar1_bystub(0));

      UNSTUB("foobar1_bystub");
      OK(0, foobar1_bystub(0));
   }

   Case("foobar2_bystub")
   {
      OK(0, foobar2_bystub(0));

      STUB("foobar2_bystub", STUB_foobar_bystub);
      OK(-1, foobar2_bystub(0));

      UNSTUB("foobar2_bystub");
      OK(0, foobar2_bystub(0));
   }

   Case("foobar3_bystub")
   {
      OK(0, ns113::foobar3_bystub(0));

      STUB("ns113::foobar3_bystub", STUB_foobar_bystub);
      OK(-1, ns113::foobar3_bystub(0));

      UNSTUB("ns113::foobar3_bystub");
      OK(0, ns113::foobar3_bystub(0));
   }

#ifndef _WIN32
   Case("foobar2_bystub(int)")
   {
      OK(0, foobar2_bystub(0));

      STUB("foobar2_bystub(int)", STUB_foobar_bystub);
      OK(-1, foobar2_bystub(0));

      UNSTUB("foobar2_bystub(int)");
      OK(0, foobar2_bystub(0));
   }

   Case("ns113::foobar3_bystub(int)")
   {
      OK(0, ns113::foobar3_bystub(0));

      STUB("ns113::foobar3_bystub(int)", STUB_foobar_bystub);
      OK(-1, ns113::foobar3_bystub(0));

      UNSTUB("ns113::foobar3_bystub(int)");
      OK(0, ns113::foobar3_bystub(0));
   }
#endif
}
