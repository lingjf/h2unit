#include "../source/h2_unit.cpp"
#include "test_types.hpp"
#include <time.h>

SUITE(mock function)
{
   Case(once)
   {
      MOCK(foobar2, int, (int, const char*), Once(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(twice)
   {
      MOCK(foobar2, int, (int, const char*), Twice(Eq(1), _)) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "BC"));
   }

   Case(3 times)
   {
      MOCK(foobar2, int, (int, const char*), Times(3).With(Ge(1), "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(any 0)
   {
      MOCK(foobar2, int, (int, const char*), Any(1, "A"))
      {
#ifdef _WIN32
         return 11;
#endif
      };
   }

   Case(any 1)
   {
      MOCK(foobar2, int, (int, const char*), Any().With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(any 2)
   {
      MOCK(foobar2, int, (int, const char*), Any().With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(atleast 2)
   {
      MOCK(foobar2, int, (int, const char*), Atleast(2).With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(atleast 3)
   {
      MOCK(foobar2, int, (int, const char*), Atleast(2).With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(atmost 1)
   {
      MOCK(foobar2, int, (int, const char*), Atmost(2).With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(atmost 2)
   {
      MOCK(foobar2, int, (int, const char*), Atmost(2).With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(between)
   {
      MOCK(foobar2, int, (int, const char*), Between(2, 4).With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(void return )
   {
      MOCK(foobar21, void, (int& a, char*), Once(1, (char*)"A")){};

      char t[32] = "A";
      int a1 = 1;
      foobar21(a1, t);
   }

   Case(Th0)
   {
      MOCK(foobar21, void, (int& a, char*), Once().Th0(1)){};

      char t[32] = "";
      int a1 = 1;
      foobar21(a1, t);
   }

   Case(Th1)
   {
      MOCK(foobar21, void, (int& a, char*), Once().Th1((char*)"A")){};

      char t[32] = "A";
      int a1 = 1;
      foobar21(a1, t);
   }

   Case(zero parameter)
   {
      MOCK(foobar22, void, (), Once()){};
      foobar22();
   }

   Case(IsNull Matcher)
   {
      MOCK(foobar2, int, (int, const char*), Once(1, IsNull)) { return 11; };
      OK(11, foobar2(1, NULL));
   }

   Case(Substr Matcher)
   {
      MOCK(foobar2, int, (int, const char*), Once(1, Substr("BC"))) { return 11; };
      OK(11, foobar2(1, "ABCD"));
   }

   Case(Not Matcher)
   {
      MOCK(foobar2, int, (int, const char*), Once(Not(2), _)) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(AllOf Matcher)
   {
      MOCK(foobar2, int, (int, const char*), Once(AllOf(1, Ge(1)), _)) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(AnyOf Matcher)
   {
      MOCK(foobar2, int, (int, const char*), Once(AnyOf(Le(1), Gt(2)), _)) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(NoneOf Matcher)
   {
      MOCK(foobar2, int, (int, const char*), Once(NoneOf(1, Ge(1)), _)) { return 11; };
      OK(11, foobar2(0, "A"));
   }

   Case(arguments up to 15)
   {
      MOCK(foobar16, int, (int, int, int, int, int, int, int, int, int, int, int, int, int, int, int), Once()) { return 11; };
      OK(11, foobar16(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
   }
}

SUITE(multi line)
{
   Case(1 checkin)
   {
      MOCK(foobar2, int, (int, const char*),
           Once(1, "A")
             .Return(11)
             .Once()
             .With(2, "B")
             .Return(22)
             .Twice(3, "C")
             .Return(33))
      {
#ifdef _WIN32
         return 0; /*make msvc happy*/
#endif
      };

      OK(11, foobar2(1, "A"));
      OK(22, foobar2(2, "B"));
      OK(33, foobar2(3, "C"));
      OK(33, foobar2(3, "C"));
   }

   Case(2 checkin)
   {
      MOCK(foobar2, int, (int, const char*),
           Once(1, "A").Return(11),
           Twice().With(2, "B").Return(22))
      {
#ifdef _WIN32
         return 0; /*make msvc happy*/
#endif
      };

      OK(11, foobar2(1, "A"));
      OK(22, foobar2(2, "B"));
      OK(22, foobar2(2, "B"));
   }
}

SUITE(mock greed)
{
   Case(greed default true)
   {
      MOCK(foobar2, int, (int, const char*),
           //   .greed(true) // default is true
           Between(1, 3)
             .With(1, "A")
             .Return(11)
             .Once(1, _)
             .Return(22))
      {
#ifdef _WIN32
         return 0; /*make msvc happy*/
#endif
      };

      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
      OK(22, foobar2(1, "A"));
   }

   Case(greed false)
   {
      MOCK(foobar2, int, (int, const char*),
           greed(false)
             .Between(1, 3)
             .With(1, "A")
             .Return(11)
             .Once(1, _)
             .Return(22))
      {
#ifdef _WIN32
         return 0; /*make msvc happy*/
#endif
      };

      OK(11, foobar2(1, "A"));
      OK(22, foobar2(1, "A"));
   }
}

SUITE(mock Return)
{
   Case(delegate to origin)
   {
      MOCK(foobar2, int, (int, const char*), Once(1, "A").Return())
      {
#ifdef _WIN32
         return 0; /*make msvc happy*/
#endif
      };
      OK(2, foobar2(1, "A"));
   }

   Case(return uninitialized value)
   {
      MOCK(foobar2, int, (int, const char*), Once(1, "A"))
      {
#ifdef _WIN32
         return 0; /*make msvc happy*/
#endif
      };
      OK(_, foobar2(1, "A"));
   }
}

SUITE(mock member function)
{
   B_DerivedClass b;

   Case(static member function)
   {
      MOCK(B_DerivedClass::static_f2, const char*, (int a, int b), Once(1, 2)) { return "mocked B.static_f2"; };
      OK("mocked B.static_f2", B_DerivedClass::static_f2(1, 2));
   }

   Case(normal member function)
   {
      MOCK(B_DerivedClass, normal_f2, const char*, (int, int), Once(1, 2)) { return "mocked B.normal_f2"; };
      OK("mocked B.normal_f2", b.normal_f2(1, 2));
   }

#ifndef _WIN32
   Case(virtual member function)
   {
      MOCK(B_DerivedClass, virtual_f2, const char*, (int, int), Once(1, 2))
      {
         OK(This != nullptr);
         return "mocked B.virtual_f2";
      };
      OK("mocked B.virtual_f2", b.virtual_f2(1, 2));
   }

   Case(no default constructor)
   {
      MOCK(D_NoConstructorClass, virtual_f3, const char*, (int, int, int), Once())
      {
         return "mocked D.virtual_f3";
      };
      D_NoConstructorClass d(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      OK("mocked D.virtual_f3", d.virtual_f3(1, 2, 3));
   }

   Case(abstract class)
   {
      MOCK(A_AbstractClass, virtual_f1, const char*, (int), Once()) { return "mocked A.virtual_f1"; };

      OK("mocked A.virtual_f1", b.virtual_f1(1));
   }
#endif
}

SUITE(mock template function)
{
   Case(function 1 typename)
   {
      OK(4, foobar4<int>(0));

      MOCK(foobar4<int>, int, (int a), Once()) { return -4; };
      OK(-4, foobar4<int>(0));
   }

   Case(function 2 typename)
   {
      OK(5, foobar5<int, int>(1, 2));

      MOCK((foobar5<int, int>), int, (int a, int b), Once()) { return -5; };
      OK(-5, foobar5<int, int>(1, 2));
   }
}

SUITE(mock template member function)
{
   Case(member function 1 typename)
   {
      F_TemplateClass<int> f;
      OK("F.static_f1", f.static_f1(0));
      OK("F.normal_f1", f.normal_f1(0));
      OK("F.virtual_f1", f.virtual_f1(0));

      MOCK(F_TemplateClass<int>::static_f1, const char*, (int a)) { return "MOCK"; };
      OK("MOCK", f.static_f1(0));
      MOCK(F_TemplateClass<int>, normal_f1<int>, const char*, (int a)) { return "MOCK"; };
      OK("MOCK", f.normal_f1(0));
#ifndef _WIN32
      MOCK(F_TemplateClass<int>, virtual_f1, const char*, (int a))
      {
         return "MOCK";
      };
      OK("MOCK", f.virtual_f1(0));
#endif
   }

   Case(member function 2 typename)
   {
      G_TemplateClass<int, int> g;
      OK("G.static_f2", (g.static_f2<int, int>(0, 0)));
      OK("G.normal_f2", (g.normal_f2<int, int>(0, 0)));
      OK(Pair("G", "virtual_f2"), (g.virtual_f2<int, int>(0, 0)));

      MOCK((G_TemplateClass<int, int>::static_f2<int, int>), const char*, (int a, int b)) { return "MOCK"; };
      OK("MOCK", (g.static_f2<int, int>(0, 0)));
      MOCK((G_TemplateClass<int, int>), (normal_f2<int, int>), const char*, (int a, int b)) { return "MOCK"; };
      OK("MOCK", (g.normal_f2<int, int>(0, 0)));
#ifndef _WIN32
      MOCK((G_TemplateClass<int, int>), (virtual_f2<int, int>), (std::pair<const char*, const char*>), (int a, int b))
      {
         return std::make_pair("MOCK", "MOCK");
      };
      OK(Pair("MOCK", "MOCK"), (g.virtual_f2<int, int>(0, 0)));
#endif
   }
}

SUITE(mock omit checkin)
{
   Case(only With)
   {
      MOCK(foobar2, int, (int, const char*), With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(only Th0)
   {
      MOCK(foobar2, int, (int, const char*), Th0(1)) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(only Return)
   {
      MOCK(foobar2, int, (int, const char*), Return(22)) { return 11; };
      OK(22, foobar2(1, "A"));
      OK(22, foobar2(1, "A"));
   }

   Case(only None)
   {
      MOCK(foobar2, int, (int, const char*)) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }
}

SUITE(mock by function name)
{
   // Case(time)
   // {
   //    MOCK("time", time_t, (time_t*), Once()) { return 42; };
   //    OK(42, time(NULL));
   // }

   Case(foobar)
   {
      MOCK("foobar0", int, (), Once()) { return 1; };
      OK(1, foobar0());
   }
}

CASE(MOCKS)
{
   MOCKS(foobar2, int, (int, int), Return(11));
   OK(11, foobar2(0, 0));
}
