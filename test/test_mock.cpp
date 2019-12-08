#include "../source/h2_unit.h"

int bar1(int a, const char* b)
{
   return 0;
}
void bar2(int a, char* b)
{
}
void bar3()
{
}

H2UNIT (h2_mock) {
   void setup() {}

   void teardown() {}
};

H2CASE(h2_mock, once)
{
   H2MOCK(bar1, int(int, const char*)).once(1, "A").returns(11);
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, twice)
{
   H2MOCK(bar1, int(int, const char*)).twice(Eq(1), _).returns(11);
   H2EQ(11, bar1(1, "A"));
   H2EQ(11, bar1(1, "BC"));
}

H2CASE(h2_mock, 3 times)
{
   H2MOCK(bar1, int(int, const char*)).times(3).with(Ge(1), "A").returns(11);
   H2EQ(11, bar1(1, "A"));
   H2EQ(11, bar1(1, "A"));
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, any 0)
{
   H2MOCK(bar1, int(int, const char*)).any(1, "A");
}

H2CASE(h2_mock, any 1)
{
   H2MOCK(bar1, int(int, const char*)).any().with(1, "A").returns(11);
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, any 2)
{
   H2MOCK(bar1, int(int, const char*)).any().with(1, "A").returns(11);
   H2EQ(11, bar1(1, "A"));
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, atleast 2)
{
   H2MOCK(bar1, int(int, const char*)).atleast(2).with(1, "A").returns(11);
   H2EQ(11, bar1(1, "A"));
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, atleast 3)
{
   H2MOCK(bar1, int(int, const char*)).atleast(2).with(1, "A").returns(11);
   H2EQ(11, bar1(1, "A"));
   H2EQ(11, bar1(1, "A"));
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, atmost 1)
{
   H2MOCK(bar1, int(int, const char*)).atmost(2).with(1, "A").returns(11);
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, atmost 2)
{
   H2MOCK(bar1, int(int, const char*)).atmost(2).with(1, "A").returns(11);
   H2EQ(11, bar1(1, "A"));
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, between)
{
   H2MOCK(bar1, int(int, const char*)).between(2, 4).with(1, "A").returns(11);
   H2EQ(11, bar1(1, "A"));
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, void return )
{
   char t[32] = "A";
   H2MOCK(bar2, void(int a, char*)).once(1, (char*)"A");
   bar2(1, t);
}

H2CASE(h2_mock, multi - line)
{
   H2MOCK(bar1, int(int, const char*)).once(1, "A").returns(11).once().with(2, "B").returns(22).twice(3, "C").returns(33);

   H2EQ(11, bar1(1, "A"));
   H2EQ(22, bar1(2, "B"));
   H2EQ(33, bar1(3, "C"));
   H2EQ(33, bar1(3, "C"));
}

H2CASE(h2_mock, th0)
{
   char t[32] = "";
   H2MOCK(bar2, void(int a, char*)).once().th0(1);
   bar2(1, t);
}

H2CASE(h2_mock, th1)
{
   char t[32] = "A";
   H2MOCK(bar2, void(int a, char*)).once().th1((char*)"A");
   bar2(1, t);
}

H2CASE(h2_mock, zero parameter)
{
   H2MOCK(bar3, void()).once();
   bar3();
}

H2CASE(h2_mock, Null Matcher)
{
   H2MOCK(bar1, int(int, const char*)).once(1, Null()).returns(11);
   H2EQ(11, bar1(1, NULL));
}

H2CASE(h2_mock, HasSubstr Matcher)
{
   H2MOCK(bar1, int(int, const char*)).once(1, HasSubstr("BC")).returns(11);
   H2EQ(11, bar1(1, "ABCD"));
}

H2CASE(h2_mock, Not Matcher)
{
   H2MOCK(bar1, int(int, const char*)).once(Not(2), _).returns(11);
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, AllOf Matcher)
{
   H2MOCK(bar1, int(int, const char*)).once(AllOf(1, Ge(1)), _).returns(11);
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, AnyOf Matcher)
{
   H2MOCK(bar1, int(int, const char*)).once(AnyOf(Le(1), Gt(2)), _).returns(11);
   H2EQ(11, bar1(1, "A"));
}

H2CASE(h2_mock, NoneOf Matcher)
{
   H2MOCK(bar1, int(int, const char*)).once(NoneOf(1, Ge(1)), _).returns(11);
   H2EQ(11, bar1(0, "A"));
}

// H2UNIT(h2_mock_with)
// {
//     void setup() {
//     }

//     void teardown() {
//     }
// };

// H2CASE(h2_mock_with, lambda_check argument)
// {
//     H2MOCK(bar1, int(int , const char *)).once().th0([](int a)->bool{
//         return a == 101;
//     });
//     bar1(101, "A");
// }

H2UNIT (h2_mock_does) {
   void setup() {}

   void teardown() {}
};

H2CASE(h2_mock_does, lambda_does)
{
   H2MOCK(bar1, int(int, const char*)).once() = [](int a, const char* b) -> int { return a + 1; };
   H2EQ(1, bar1(0, "A"));
}

H2CASE(h2_mock_does, lambda_does check)
{
   H2MOCK(bar1, int(int, const char*)).once() = [](int a, const char* b) -> int {
      H2EQ(0, a);
      H2EQ("A", b);
      return a + 1;
   };
   H2EQ(1, bar1(0, "A"));
}

H2CASE(h2_mock_does, lambda_does modify parameter)
{
   char t[128] = "hello";
   H2MOCK(bar2, void(int, char*)).once() = [](int a, char* b) {strcpy(b, "world"); return; };
   bar2(0, t);
   H2EQ("world", (const char*)t);
}

H2CASE(h2_mock_does, lambda_does void)
{
   H2MOCK(bar3, void()).once() = []() { return; };
   bar3();
}

H2UNIT (h2_mock_cplusplus) {
   void setup() {}

   void teardown() {}

   class Shape
   {
    private:
      int x, y;

    public:
      Shape(const char* _id)
        : x(0), y(0), id(_id)
      {
      }

      const char* id;

      static int fly(int x, int y)
      {
         return 0;
      }

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
};

H2CASE(h2_mock_cplusplus, static member function)
{
   Shape shape("111");

   H2MOCK(Shape::fly, int(int, int)).once(1, 2).returns(11);
   H2EQ(11, Shape::fly(1, 2));

   H2MOCK(Shape::fly, int(int, int)).once() = [](int, int) -> int {
      return 22;
   };
   H2EQ(22, Shape::fly(1, 2));

   static_assert(std::is_function<decltype(Shape::fly)>::value, "Must a member function ");
}

H2CASE(h2_mock_cplusplus, member function)
{
   Shape shape("222");

   H2MOCK(Shape, go, int(int, int)).once(1, 2).returns(11);
   H2MOCK(Shape, go, int(int)).once(1) = [](int) -> int {
      return 22;
   };
   H2EQ(11, shape.go(1, 2));
   H2EQ(22, shape.go(1));
}

H2CASE(h2_mock_cplusplus, virtual member function lambda)
{
   Shape shape("333");

   H2MOCK(Shape, work, int(int, int)).once(1, 2).returns(11);
   H2EQ(11, shape.work(1, 2));

   H2MOCK(Shape, work, int(int, int)).once(1, 2) = [](int, int) -> int {
      return 22;
   };
   H2EQ(22, shape.work(1, 2));
}


H2CASE(h2_mock_cplusplus, virtual member function lambda with this object pointer)
{
   Shape shape("333");

   H2MOCK(Shape, work, int(int, int)).once(1, 2) = [](Shape* that, int x, int y) -> int {
      H2EQ(that != nullptr);
      H2EQ("333", that->id);
      H2EQ(1, x);
      H2EQ(2, y);
      return 33;
   };
   H2EQ(33, shape.work(1, 2));
}
