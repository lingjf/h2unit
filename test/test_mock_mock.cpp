#include "../source/h2_unit.cpp"

extern "C" {
int foobar1_bymock(int a)
{
   return 0;
}
}
int foobar2_bymock(int a)
{
   return 0;
}

namespace {

int bar1(int a, const char* b)
{
   return 100;
}
void bar2(int& a, char* b)
{
}
void bar3()
{
}

int bar16(int _0, int _1, int _2, int _3, int _4, int _5, int _6, int _7, int _8, int _9, int _10, int _11, int _12, int _13, int _14, int _15)
{
}

class Shape {
 private:
   int x, y;

 public:
   Shape(const char* _id)
     : x(0), y(0), id(_id)
   {
   }

   const char* id;

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

   int go()
   {
      this->x = 0;
      this->y = 0;
      return 0;
   }

   virtual int work(int x, int y)
   {
      this->x *= x;
      this->y *= y;
      return 0;
   }
   virtual int work(int xy)
   {
      this->x *= xy;
      this->y *= xy;
      return 0;
   }

 private:
   int move(int x, int y)
   {
      this->x += x;
      this->y += y;
      return 0;
   }
};

SUITE(mock c - function)
{
   Case(once)
   {
      MOCK(bar1, int, (int, const char*), Once(1, "A")) { return 11; };
      OK(11, bar1(1, "A"));
   }

   Case(twice)
   {
      MOCK(bar1, int, (int, const char*), Twice(Eq(1), _)) { return 11; };
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "BC"));
   }

   Case(3 times)
   {
      MOCK(bar1, int, (int, const char*), Times(3).With(Ge(1), "A")) { return 11; };
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(any 0)
   {
      MOCK(bar1, int, (int, const char*), Any(1, "A")){};
   }

   Case(any 1)
   {
      MOCK(bar1, int, (int, const char*), Any().With(1, "A")) { return 11; };
      OK(11, bar1(1, "A"));
   }

   Case(any 2)
   {
      MOCK(bar1, int, (int, const char*), Any().With(1, "A")) { return 11; };
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(atleast 2)
   {
      MOCK(bar1, int, (int, const char*), Atleast(2).With(1, "A")) { return 11; };
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(atleast 3)
   {
      MOCK(bar1, int, (int, const char*), Atleast(2).With(1, "A")) { return 11; };
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(atmost 1)
   {
      MOCK(bar1, int, (int, const char*), Atmost(2).With(1, "A")) { return 11; };
      OK(11, bar1(1, "A"));
   }

   Case(atmost 2)
   {
      MOCK(bar1, int, (int, const char*), Atmost(2).With(1, "A")) { return 11; };
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(between)
   {
      MOCK(bar1, int, (int, const char*), Between(2, 4).With(1, "A")) { return 11; };
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(void return )
   {
      MOCK(bar2, void, (int& a, char*), Once(1, (char*)"A")){};

      char t[32] = "A";
      int a1 = 1;
      bar2(a1, t);
   }

   Case(Th0)
   {
      MOCK(bar2, void, (int& a, char*), Once().Th0(1)){};

      char t[32] = "";
      int a1 = 1;
      bar2(a1, t);
   }

   Case(Th1)
   {
      MOCK(bar2, void, (int& a, char*), Once().Th1((char*)"A")){};

      char t[32] = "A";
      int a1 = 1;
      bar2(a1, t);
   }

   Case(zero parameter)
   {
      MOCK(bar3, void, (), Once()){};
      bar3();
   }

   Case(IsNull Matcher)
   {
      MOCK(bar1, int, (int, const char*), Once(1, IsNull)) { return 11; };
      OK(11, bar1(1, NULL));
   }

   Case(Substr Matcher)
   {
      MOCK(bar1, int, (int, const char*), Once(1, Substr("BC"))) { return 11; };
      OK(11, bar1(1, "ABCD"));
   }

   Case(Not Matcher)
   {
      MOCK(bar1, int, (int, const char*), Once(Not(2), _)) { return 11; };
      OK(11, bar1(1, "A"));
   }

   Case(AllOf Matcher)
   {
      MOCK(bar1, int, (int, const char*), Once(AllOf(1, Ge(1)), _)) { return 11; };
      OK(11, bar1(1, "A"));
   }

   Case(AnyOf Matcher)
   {
      MOCK(bar1, int, (int, const char*), Once(AnyOf(Le(1), Gt(2)), _)) { return 11; };
      OK(11, bar1(1, "A"));
   }

   Case(NoneOf Matcher)
   {
      MOCK(bar1, int, (int, const char*), Once(NoneOf(1, Ge(1)), _)) { return 11; };
      OK(11, bar1(0, "A"));
   }

   Case(arguments up to 15)
   {
      MOCK(bar16, int, (int, int, int, int, int, int, int, int, int, int, int, int, int, int, int), Once()) { return 11; };
      OK(11, bar16(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
   }
}

SUITE(multi line)
{
   Case(1 checkin)
   {
      MOCK(bar1, int, (int, const char*),
           Once(1, "A")
             .Return(11)
             .Once()
             .With(2, "B")
             .Return(22)
             .Twice(3, "C")
             .Return(33)){};

      OK(11, bar1(1, "A"));
      OK(22, bar1(2, "B"));
      OK(33, bar1(3, "C"));
      OK(33, bar1(3, "C"));
   }

   Case(2 checkin)
   {
      MOCK(bar1, int, (int, const char*),
           Once(1, "A").Return(11),
           Twice().With(2, "B").Return(22)){};

      OK(11, bar1(1, "A"));
      OK(22, bar1(2, "B"));
      OK(22, bar1(2, "B"));
   }
}

SUITE(mock greed)
{
   Case(greed default true)
   {
      MOCK(bar1, int, (int, const char*),
           //   .greed(true) // default is true
           Between(1, 3)
             .With(1, "A")
             .Return(11)
             .Once(1, _)
             .Return(22)){};

      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
      OK(22, bar1(1, "A"));
   }

   Case(greed false)
   {
      MOCK(bar1, int, (int, const char*),
           greed(false)
             .Between(1, 3)
             .With(1, "A")
             .Return(11)
             .Once(1, _)
             .Return(22)){};

      OK(11, bar1(1, "A"));
      OK(22, bar1(1, "A"));
   }
}

SUITE(mock Return)
{
   Case(delegate to origin)
   {
      MOCK(bar1, int, (int, const char*), Once(1, "A").Return()){};
      OK(100, bar1(1, "A"));
   }

   Case(return uninitialized value)
   {
      MOCK(bar1, int, (int, const char*), Once(1, "A")){};
      OK(_, bar1(1, "A"));
   }
}

SUITE(mock C++ Class member)
{
   Case(static member function)
   {
      Shape shape("111");

      MOCK(Shape::fly, int, (int x, int y), Once(1, 2)) { return x + y; };
      OK(3, Shape::fly(1, 2));
   }

   Case(member function)
   {
      Shape shape("222");

      MOCK(Shape, go, int, (int, int), Once(1, 2)) { return 11; };
      MOCK(Shape, go, int, (int), Once(1)) { return 22; };
      MOCK(Shape, go, int, (), Once()) { return 33; };
      OK(11, shape.go(1, 2));
      OK(22, shape.go(1));
      OK(33, shape.go());
   }

   Case(virtual member function)
   {
      Shape shape("333");

      MOCK(Shape, work, int, (int, int), Once(1, 2)) { return 11; };
      OK(11, shape.work(1, 2));
      MOCK(Shape, work, int, (int x, int y), Once(1, 2))
      {
         OK(This != nullptr);
         OK("333", This->id);
         OK(1, x);
         OK(2, y);
         return 22;
      };
      OK(22, shape.work(1, 2));
   }

   Case(private member function)
   {
      Shape shape("444");

      MOCK(Shape, move, int, (int, int), Once(1, 2)) { return 11; };
      OK(11, shape.move(1, 2));
   }
}

template <typename T>
int foobar1(T a)
{
   return 1;
}

template <typename T1, typename T2>
int foobar2(T1 a, T2 b)
{
   return 2;
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
};

template <typename T1, typename T2>
struct Foo3 {
   template <typename U1, typename U2>
   std::pair<U1, U2> bar(U1 a, U2 b)
   {
      return std::make_pair(a, b);
   }
};

SUITE(Mock template)
{
   Case(function 1 typename)
   {
      int ret = foobar1<int>(0);
      OK(1, ret);

      MOCK(foobar1<int>, int, (int a), Once())
      {
         return -1;
      };
      OK(-1, foobar1<int>(0));
   }

   Case(function 2 typename)
   {
      int ret = foobar2<int, int>(0, 0);
      OK(2, ret);
      MOCK((foobar2<int, int>), int, (int, int), Once())
      {
         return -2;
      };
      OK(-2, (foobar2<int, int>(0, 0)));
   }

   Case(member function 1 typename)
   {
      MOCK(Foo1<int>, bar1<int>, int, (int), Once())
      {
         return -1;
      };
      Foo1<int> a1;
      OK(-1, a1.bar1(0));
   }

   Case(static member function 1 typename)
   {
      MOCK(Foo1<int>::bar2, int, (int), Once())
      {
         return -1;
      };
      OK(-1, Foo1<int>::bar2(0));
   }

   Case(member function 2 typename)
   {
      MOCK((Foo2<int, float>), (bar1<int, float>), int, (int, float), Once())
      {
         return -1;
      };
      Foo2<int, float> a1;
      OK(-1, (a1.bar1<int, float>(0, 0)));
   }

   Case(static member function 2 typename)
   {
      MOCK((Foo2<int, float>::bar2<int, float>), int, (int, float), Once())
      {
         return -1;
      };
      OK(-1, (Foo2<int, float>::bar2<int, float>(0, 0)));
   }

   Case(return template)
   {
      MOCK((Foo3<int, float>), (bar<int, float>), (std::pair<int, float>), (int, float), Once())
      {
         return std::make_pair(1, 1);
      };
      Foo3<int, float> a1;
      OK(Pair(1, 1), (a1.bar<int, float>(0, 0)));
   }
}

SUITE(omit Checkin)
{
   Case(only With)
   {
      MOCK(bar1, int, (int, const char*), With(1, "A")) { return 11; };
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(only Th0)
   {
      MOCK(bar1, int, (int, const char*), Th0(1)) { return 11; };
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(only Return)
   {
      MOCK(bar1, int, (int, const char*), Return(22)) { return 11; };
      OK(22, bar1(1, "A"));
      OK(22, bar1(1, "A"));
   }

   Case(only None)
   {
      MOCK(bar1, int, (int, const char*)) { return 11; };
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }
}

SUITE(mock name)
{
   Case("foobar1_bymock")
   {
      MOCK("foobar1_bymock", int, (int), Once(0)) { return -1; };
      OK(-1, foobar1_bymock(0));
   }
   Case("foobar2_bymock")
   {
      MOCK("foobar2_bymock", int, (int), Once(0)) { return -1; };
      OK(-1, foobar2_bymock(0));
   }
}

CASE(MOCKS)
{
   MOCKS(bar1, int, (int, const char*), Return(11));
   OK(11, bar1(1, "A"));
}

SUITE(UNMOCK)
{
   Case(normal function)
   {
      MOCK(foobar1_bymock, int, (int), Once(0)) { return -1; };
      OK(-1, foobar1_bymock(0));
      UNMOCK(foobar1_bymock);
      OK(0, foobar1_bymock(0));
   }

   Case(template member function)
   {
      Foo1<int> a1;
      MOCK(Foo1<int>, bar1<int>, int, (int), Once())
      {
         return -1;
      };
      OK(-1, a1.bar1(0));

      UNMOCK(Foo1<int>, bar1<int>, int, (int));
      OK(1, a1.bar1(0));
   }

   Case("foobar1_bymock")
   {
      MOCK("foobar1_bymock", int, (int), Once(0)) { return -1; };
      OK(-1, foobar1_bymock(0));
      UNMOCK("foobar1_bymock");
      OK(0, foobar1_bymock(0));
   }
}
}  // namespace
