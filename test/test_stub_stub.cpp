#include "../source/h2_unit.cpp"

namespace {

int foobar(int, const int&)
{
   return 1;
}

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
   }

   Case(stub overwrite)
   {
      STUB(foobar, STUB_foobar);
      STUB(foobar, STUB2_foobar);

      OK(-2, foobar(0, 1));
   }

   Case(stub chained)
   {
      STUB(foobar, STUB_foobar);
      STUB(STUB_foobar, STUB2_foobar);

      OK(-2, foobar(0, 1));
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

SUITE(stub member function)
{
   Case(lambdas normal function)
   {
      STUB(int, foobar, (int a, const int& b)) { return a + b; };

      OK(222, foobar(111, 111));
   }

   Case(lambdas normal member function)
   {
      STUB(int, Shape, go, (int a, int b)) { return a + b; };

      Shape shape;
      OK(222, shape.go(111, 111));
   }

   Case(lambdas virtual member function)
   {
      STUB(int, Shape, work, (int a, int b)) { return a + b; };

      Shape shape;
      OK(222, shape.work(111, 111));
   }

   Case(lambdas static member function)
   {
      STUB(int, Shape::fly, (int a, int b)) { return a + b; };

      OK(222, Shape::fly(111, 111));
   }
}

template <typename T>
int template_foobar1(T a)
{
   return 1;
}
int STUB_template_foobar1(int a)
{
   return -1;
}

template <typename T1, typename T2>
int template_foobar2(T1 a, T2 b)
{
   return 2;
}
int STUB_template_foobar2(int a, int b)
{
   return -2;
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

SUITE(stub template)
{
   Case(function 1 typename)
   {
      int ret = template_foobar1<int>(0);
      OK(1, ret);
      STUB(template_foobar1<int>, STUB_template_foobar1);
      OK(-1, template_foobar1<int>(0));
   }

   Case(function 2 typename)
   {
      int ret = template_foobar2<int, int>(0, 0);
      OK(2, ret);
      STUB((template_foobar2<int, int>), STUB_template_foobar2);
      OK(-2, (template_foobar2<int, int>(0, 0)));
   }

   Case(member function 1 typename)
   {
      STUB(int, template_Foobar1<int>, foobar1<int>, (int a)) { return -1; };

      template_Foobar1<int> a1;
      OK(-1, a1.foobar1(0));
   }

   Case(static member function 1 typename)
   {
      STUB(int, template_Foobar1<int>::foobar2, (int a)) { return -1; };
      OK(-1, template_Foobar1<int>::foobar2(0));
   }

   Case(member function 2 typename)
   {
      STUB(int, (template_Foobar2<int, float>), (foobar1<int, float>), (int a, float b)) { return -1; };
      template_Foobar2<int, float> a1;
      OK(-1, (a1.foobar1<int, float>(0, 0)));
   }

   Case(static member function 2 typename)
   {
      STUB(int, (template_Foobar2<int, float>::foobar2<int, float>), (int a, float b)) { return -1; };
      OK(-1, (template_Foobar2<int, float>::foobar2<int, float>(0, 0)));
   }
}

}  // namespace

extern "C" {
int foobar_bystub(int a)
{
   return 0;
}
}

int STUB_foobar_bystub(int a)
{
   return -1;
}

SUITE(stub name)
{
   Case("foobar_bystub")
   {
      STUB("foobar_bystub", STUB_foobar_bystub);
      OK(-1, foobar_bystub(0));
      STUB(int, "foobar_bystub", (int)) { return -2; };
      OK(-2, foobar_bystub(0));
   }
}
