#include "../source/h2_unit.cpp"

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

SUITE(mock c - function)
{
   Case(once)
   {
      MOCK(bar1, int, (int, const char*)).once(1, "A").returns(11);
      OK(11, bar1(1, "A"));
   }

   Case(twice)
   {
      MOCK(bar1, int, (int, const char*)).twice(Eq(1), _).returns(11);
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "BC"));
   }

   Case(3 times)
   {
      MOCK(bar1, int, (int, const char*)).times(3).with(Ge(1), "A").returns(11);
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(any 0)
   {
      MOCK(bar1, int, (int, const char*)).any(1, "A");
   }

   Case(any 1)
   {
      MOCK(bar1, int, (int, const char*)).any().with(1, "A").returns(11);
      OK(11, bar1(1, "A"));
   }

   Case(any 2)
   {
      MOCK(bar1, int, (int, const char*)).any().with(1, "A").returns(11);
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(atleast 2)
   {
      MOCK(bar1, int, (int, const char*)).atleast(2).with(1, "A").returns(11);
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(atleast 3)
   {
      MOCK(bar1, int, (int, const char*)).atleast(2).with(1, "A").returns(11);
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(atmost 1)
   {
      MOCK(bar1, int, (int, const char*)).atmost(2).with(1, "A").returns(11);
      OK(11, bar1(1, "A"));
   }

   Case(atmost 2)
   {
      MOCK(bar1, int, (int, const char*)).atmost(2).with(1, "A").returns(11);
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(between)
   {
      MOCK(bar1, int, (int, const char*)).between(2, 4).with(1, "A").returns(11);
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
   }

   Case(void return )
   {
      MOCK(bar2, void, (int& a, char*)).once(1, (char*)"A");

      char t[32] = "A";
      int a1 = 1;
      bar2(a1, t);
   }

   Case(multi - line)
   {
      MOCK(bar1, int, (int, const char*))
        .once(1, "A")
        .returns(11)
        .once()
        .with(2, "B")
        .returns(22)
        .twice(3, "C")
        .returns(33);

      OK(11, bar1(1, "A"));
      OK(22, bar1(2, "B"));
      OK(33, bar1(3, "C"));
      OK(33, bar1(3, "C"));
   }

   Case(th0)
   {
      MOCK(bar2, void, (int& a, char*)).once().th0(1);

      char t[32] = "";
      int a1 = 1;
      bar2(a1, t);
   }

   Case(th1)
   {
      MOCK(bar2, void, (int& a, char*)).once().th1((char*)"A");

      char t[32] = "A";
      int a1 = 1;
      bar2(a1, t);
   }

   Case(zero parameter)
   {
      MOCK(bar3, void, ()).once();
      bar3();
   }

   Case(IsNull Matcher)
   {
      MOCK(bar1, int, (int, const char*)).once(1, IsNull).returns(11);
      OK(11, bar1(1, NULL));
   }

   Case(Substr Matcher)
   {
      MOCK(bar1, int, (int, const char*)).once(1, Substr("BC")).returns(11);
      OK(11, bar1(1, "ABCD"));
   }

   Case(Not Matcher)
   {
      MOCK(bar1, int, (int, const char*)).once(Not(2), _).returns(11);
      OK(11, bar1(1, "A"));
   }

   Case(AllOf Matcher)
   {
      MOCK(bar1, int, (int, const char*)).once(AllOf(1, Ge(1)), _).returns(11);
      OK(11, bar1(1, "A"));
   }

   Case(AnyOf Matcher)
   {
      MOCK(bar1, int, (int, const char*)).once(AnyOf(Le(1), Gt(2)), _).returns(11);
      OK(11, bar1(1, "A"));
   }

   Case(NoneOf Matcher)
   {
      MOCK(bar1, int, (int, const char*)).once(NoneOf(1, Ge(1)), _).returns(11);
      OK(11, bar1(0, "A"));
   }

   Case(arguments up to 15)
   {
      MOCK(bar16, int, (int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)).once().returns(11);
      OK(11, bar16(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
   }
}

SUITE(mock greed)
{
   Case(greed default true)
   {
      MOCK(bar1, int, (int, const char*))
        //   .greed(true) // default is true
        .between(1, 3)
        .with(1, "A")
        .returns(11)
        .once(1, _)
        .returns(22);

      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
      OK(11, bar1(1, "A"));
      OK(22, bar1(1, "A"));
   }

   Case(greed false)
   {
      MOCK(bar1, int, (int, const char*))
        .greed(false)
        .between(1, 3)
        .with(1, "A")
        .returns(11)
        .once(1, _)
        .returns(22);

      OK(11, bar1(1, "A"));
      OK(22, bar1(1, "A"));
   }
}

SUITE(mock returns)
{
   Case(delegate to origin when no returns / does /= =)
   {
      MOCK(bar1, int, (int, const char*)).once(1, "A");
      OK(100, bar1(1, "A"));
   }

   Case(return uninitialized value)
   {
      MOCK(bar1, int, (int, const char*)).once(1, "A").returns();
      OK(_, bar1(1, "A"));
   }
}

// TEST(lambda_check argument)
// {
//     MOCK(bar1, int(int , const char *)).once().th0([](int a)->bool{
//         return a == 101;
//     });
//     bar1(101, "A");
// }

SUITE(mock does)
{
   Case(lambda_does)
   {
      MOCK(bar1, int, (int, const char*)).once() = [](int a, const char* b) -> int {
         return a + 1;
      };
      OK(1, bar1(0, "A"));
   }

   Case(lambda_does check)
   {
      MOCK(bar1, int, (int, const char*)).once() = [](int a, const char* b) -> int {
         OK(0, a);
         OK("A", b);
         return a + 1;
      };
      OK(1, bar1(0, "A"));
   }

   Case(lambda_does modify parameter)
   {
      MOCK(bar2, void, (int&, char*)).once() = [](int& a, char* b) {
         strcpy(b, "world");
         return;
      };
      char t[128] = "hello";
      int a1 = 0;
      bar2(a1, t);
      OK("world", (const char*)t);
   }

   Case(lambda_does void)
   {
      MOCK(bar3, void, ()).once() = []() { return; };
      bar3();
   }
}

SUITE(mock C++ Class member)
{
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

      virtual int work(int x, int y)
      {
         this->x *= x;
         this->y *= y;
         return 0;
      }
   };

   Case(static member function)
   {
      Shape shape("111");

      MOCK(Shape::fly, int, (int, int)).once(1, 2).returns(11);
      OK(11, Shape::fly(1, 2));

      MOCK(Shape::fly, int, (int, int)).once() = [](int, int) -> int { return 22; };
      OK(22, Shape::fly(1, 2));

      static_assert(std::is_function<decltype(Shape::fly)>::value, "Must a member function ");
   }

   Case(member function)
   {
      Shape shape("222");

      MOCK(Shape, go, int, (int, int)).once(1, 2).returns(11);
      MOCK(Shape, go, int, (int)).once(1) = [](int) -> int { return 22; };
      OK(11, shape.go(1, 2));
      OK(22, shape.go(1));
   }

   Case(virtual member function lambda)
   {
      Shape shape("333");

      MOCK(Shape, work, int, (int, int)).once(1, 2).returns(11);
      OK(11, shape.work(1, 2));

      MOCK(Shape, work, int, (int, int)).once(1, 2) = [](int, int) -> int {
         return 22;
      };
      OK(22, shape.work(1, 2));
   }

   Case(virtual member function lambda with this object pointer)
   {
      Shape shape("333");

      MOCK(Shape, work, int, (int, int)).once(1, 2) =
        [](Shape* that, int x, int y) -> int {
         OK(that != nullptr);
         OK("333", that->id);
         OK(1, x);
         OK(2, y);
         return 33;
      };
      OK(33, shape.work(1, 2));
   }
}

template <typename T>
int template_foobar1(T a)
{
   return 1;
}

template <typename T1, typename T2>
int template_foobar2(T1 a, T2 b)
{
   return 2;
}

template <typename T>
struct template_Foobar1 {
   T m = 0;
   template <typename U>
   int foobar1(U a)
   {
      return 1;
   }
   static int foobar2(T a)
   {
      return 2;
   }
};

template <typename T1, typename T2>
struct template_Foobar2 {
   T1 m1 = 0;
   T2 m2 = 0;
   template <typename U1, typename U2>
   int foobar1(U1 a, U2 b)
   {
      return 1;
   }
   template <typename U1, typename U2>
   static int foobar2(U1 a, U2 b)
   {
      return 2;
   }
};

SUITE(Mock template)
{
   Case(function 1 typename)
   {
      int ret = template_foobar1<int>(0);
      OK(1, ret);

      MOCK(template_foobar1<int>, int, (int)).once() = [](int a) -> int {
         return -1;
      };
      OK(-1, template_foobar1<int>(0));
   }

   Case(function 2 typename)
   {
      int ret = template_foobar2<int, int>(0, 0);
      OK(2, ret);
      MOCK((template_foobar2<int, int>), int, (int, int)).once() = [](int a, int b) -> int {
         return -2;
      };
      OK(-2, (template_foobar2<int, int>(0, 0)));
   }

   Case(member function 1 typename)
   {
      MOCK(template_Foobar1<int>, foobar1<int>, int, (int)).once() = [](int) -> int {
         return -1;
      };
      template_Foobar1<int> a1;
      OK(-1, a1.foobar1(0));
   }

   Case(static member function 1 typename)
   {
      MOCK(template_Foobar1<int>::foobar2, int, (int)).once() = [](int) -> int {
         return -1;
      };
      OK(-1, template_Foobar1<int>::foobar2(0));
   }

   Case(member function 2 typename)
   {
      MOCK((template_Foobar2<int, float>), (foobar1<int, float>), int, (int, float)).once() = [](int, float) -> int {
         return -1;
      };
      template_Foobar2<int, float> a1;
      OK(-1, (a1.foobar1<int, float>(0, 0)));
   }

   Case(static member function 2 typename)
   {
      MOCK((template_Foobar2<int, float>::foobar2<int, float>), int, (int, float)).once() = [](int, float) -> int {
         return -1;
      };
      OK(-1, (template_Foobar2<int, float>::foobar2<int, float>(0, 0)));
   }
}

}  // namespace

extern "C" {
int foobar_bymock(int a)
{
   return 0;
}
}

SUITE(mock name)
{
   Case("foobar_bymock")
   {
      MOCK("foobar_bymock", int, (int)).once(0).returns(-1);
      OK(-1, foobar_bymock(0));
   }
}
