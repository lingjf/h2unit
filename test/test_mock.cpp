
#if defined(__GNUC__) && __GNUC__ >= 5

int
bar1(int a, const char* b)
{
  return 0;
}
void
bar2(int a, char* b)
{}
void
bar3()
{}

SUITE(mock c - function)
{
  Case(once)
  {
    MOCK(bar1, int(int, const char*)).once(1, "A").returns(11);
    OK(11, bar1(1, "A"));
  }

  Case(twice)
  {
    MOCK(bar1, int(int, const char*)).twice(Eq(1), _).returns(11);
    OK(11, bar1(1, "A"));
    OK(11, bar1(1, "BC"));
  }

  Case(3 times)
  {
    MOCK(bar1, int(int, const char*)).times(3).with(Ge(1), "A").returns(11);
    OK(11, bar1(1, "A"));
    OK(11, bar1(1, "A"));
    OK(11, bar1(1, "A"));
  }

  Case(any 0) { MOCK(bar1, int(int, const char*)).any(1, "A"); }

  Case(any 1)
  {
    MOCK(bar1, int(int, const char*)).any().with(1, "A").returns(11);
    OK(11, bar1(1, "A"));
  }

  Case(any 2)
  {
    MOCK(bar1, int(int, const char*)).any().with(1, "A").returns(11);
    OK(11, bar1(1, "A"));
    OK(11, bar1(1, "A"));
  }

  Case(atleast 2)
  {
    MOCK(bar1, int(int, const char*)).atleast(2).with(1, "A").returns(11);
    OK(11, bar1(1, "A"));
    OK(11, bar1(1, "A"));
  }

  Case(atleast 3)
  {
    MOCK(bar1, int(int, const char*)).atleast(2).with(1, "A").returns(11);
    OK(11, bar1(1, "A"));
    OK(11, bar1(1, "A"));
    OK(11, bar1(1, "A"));
  }

  Case(atmost 1)
  {
    MOCK(bar1, int(int, const char*)).atmost(2).with(1, "A").returns(11);
    OK(11, bar1(1, "A"));
  }

  Case(atmost 2)
  {
    MOCK(bar1, int(int, const char*)).atmost(2).with(1, "A").returns(11);
    OK(11, bar1(1, "A"));
    OK(11, bar1(1, "A"));
  }

  Case(between)
  {
    MOCK(bar1, int(int, const char*)).between(2, 4).with(1, "A").returns(11);
    OK(11, bar1(1, "A"));
    OK(11, bar1(1, "A"));
  }

  Case(void return )
  {
    char t[32] = "A";
    MOCK(bar2, void(int a, char*)).once(1, (char*)"A");
    bar2(1, t);
  }

  Case(multi - line)
  {
    MOCK(bar1, int(int, const char*))
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
    char t[32] = "";
    MOCK(bar2, void(int a, char*)).once().th1(1);
    bar2(1, t);
  }

  Case(th1)
  {
    char t[32] = "A";
    MOCK(bar2, void(int a, char*)).once().th2((char*)"A");
    bar2(1, t);
  }

  Case(zero parameter)
  {
    MOCK(bar3, void()).once();
    bar3();
  }

  Case(IsNull Matcher)
  {
    MOCK(bar1, int(int, const char*)).once(1, IsNull).returns(11);
    OK(11, bar1(1, NULL));
  }

  Case(Contains Matcher)
  {
    MOCK(bar1, int(int, const char*)).once(1, Contains("BC")).returns(11);
    OK(11, bar1(1, "ABCD"));
  }

  Case(Not Matcher)
  {
    MOCK(bar1, int(int, const char*)).once(Not(2), _).returns(11);
    OK(11, bar1(1, "A"));
  }

  Case(AllOf Matcher)
  {
    MOCK(bar1, int(int, const char*)).once(AllOf(1, Ge(1)), _).returns(11);
    OK(11, bar1(1, "A"));
  }

  Case(AnyOf Matcher)
  {
    MOCK(bar1, int(int, const char*)).once(AnyOf(Le(1), Gt(2)), _).returns(11);
    OK(11, bar1(1, "A"));
  }

  Case(NoneOf Matcher)
  {
    MOCK(bar1, int(int, const char*)).once(NoneOf(1, Ge(1)), _).returns(11);
    OK(11, bar1(0, "A"));
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
    MOCK(bar1, int(int, const char*)).once() = [](int a, const char* b) -> int {
      return a + 1;
    };
    OK(1, bar1(0, "A"));
  }

  Case(lambda_does check)
  {
    MOCK(bar1, int(int, const char*)).once() = [](int a, const char* b) -> int {
      OK(0, a);
      OK("A", b);
      return a + 1;
    };
    OK(1, bar1(0, "A"));
  }

  Case(lambda_does modify parameter)
  {
    char t[128] = "hello";
    MOCK(bar2, void(int, char*)).once() = [](int a, char* b) {
      strcpy(b, "world");
      return;
    };
    bar2(0, t);
    OK("world", (const char*)t);
  }

  Case(lambda_does void)
  {
    MOCK(bar3, void()).once() = []() { return; };
    bar3();
  }
}

SUITE(mock C++ Class member)
{
  class Shape
  {
  private:
    int x, y;

  public:
    Shape(const char* _id)
      : x(0)
      , y(0)
      , id(_id)
    {}

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

    MOCK(Shape::fly, int(int, int)).once(1, 2).returns(11);
    OK(11, Shape::fly(1, 2));

    MOCK(Shape::fly, int(int, int)).once() = [](int, int) -> int { return 22; };
    OK(22, Shape::fly(1, 2));

    static_assert(std::is_function<decltype(Shape::fly)>::value,
                  "Must a member function ");
  }

  Case(member function)
  {
    Shape shape("222");

    MOCK(Shape, go, int(int, int)).once(1, 2).returns(11);
    MOCK(Shape, go, int(int)).once(1) = [](int) -> int { return 22; };
    OK(11, shape.go(1, 2));
    OK(22, shape.go(1));
  }

  Case(virtual member function lambda)
  {
    Shape shape("333");

    MOCK(Shape, work, int(int, int)).once(1, 2).returns(11);
    OK(11, shape.work(1, 2));

    MOCK(Shape, work, int(int, int)).once(1, 2) = [](int, int) -> int {
      return 22;
    };
    OK(22, shape.work(1, 2));
  }

  Case(virtual member function lambda with this object pointer)
  {
    Shape shape("333");

    MOCK(Shape, work, int(int, int)).once(1, 2) =
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

SUITE(Mock in shared_code)
{
  MOCK(bar1, int(int, const char*)).once(1, "A").returns(11);

  Case(a) { OK(11, bar1(1, "A")); }

  Case(b) { OK(11, bar1(1, "A")); }
}

#endif
