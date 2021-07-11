#include "../source/h2_unit.cpp"
#include "test_types.hpp"
#include <time.h>

SUITE(mocks function)
{
   Case(once)
   {
      MOCKS(foobar2, int, (int, const char*), Once(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(twice)
   {
      MOCKS(foobar2, int, (int, const char*), Twice(Eq(1), _)) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "BC"));
   }

   Case(3 times)
   {
      MOCKS(foobar2, int, (int, const char*), Times(3).With(Ge(1), "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(any 0)
   {
      MOCKS(foobar2, int, (int, const char*), Any(1, "A")) { return 11; };
   }

   Case(any 1)
   {
      MOCKS(foobar2, int, (int, const char*), Any().With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(any 2)
   {
      MOCKS(foobar2, int, (int, const char*), Any().With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(atleast 2)
   {
      MOCKS(foobar2, int, (int, const char*), Atleast(2).With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(atleast 3)
   {
      MOCKS(foobar2, int, (int, const char*), Atleast(2).With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(atmost 1)
   {
      MOCKS(foobar2, int, (int, const char*), Atmost(2).With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(atmost 2)
   {
      MOCKS(foobar2, int, (int, const char*), Atmost(2).With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(between)
   {
      MOCKS(foobar2, int, (int, const char*), Between(2, 4).With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(void return )
   {
      MOCKS(foobar21, void, (int& a, char*), Once(1, (char*)"A")){};

      char t[32] = "A";
      int a1 = 1;
      foobar21(a1, t);
   }

   Case(Th0)
   {
      MOCKS(foobar21, void, (int& a, char*), Once().Th0(1)){};

      char t[32] = "";
      int a1 = 1;
      foobar21(a1, t);
   }

   Case(Th1)
   {
      MOCKS(foobar21, void, (int& a, char*), Once().Th1((char*)"A")){};

      char t[32] = "A";
      int a1 = 1;
      foobar21(a1, t);
   }

   Case(zero parameter)
   {
      MOCKS(foobar22, void, (), Once()){};
      foobar22();
   }

   Case(IsNull Matcher)
   {
      MOCKS(foobar2, int, (int, const char*), Once(1, IsNull)) { return 11; };
      OK(11, foobar2(1, NULL));
   }

   Case(Substr Matcher)
   {
      MOCKS(foobar2, int, (int, const char*), Once(1, Substr("BC"))) { return 11; };
      OK(11, foobar2(1, "ABCD"));
   }

   Case(Not Matcher)
   {
      MOCKS(foobar2, int, (int, const char*), Once(Not(2), _)) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(AllOf Matcher)
   {
      MOCKS(foobar2, int, (int, const char*), Once(AllOf(1, Ge(1)), _)) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(AnyOf Matcher)
   {
      MOCKS(foobar2, int, (int, const char*), Once(AnyOf(Le(1), Gt(2)), _)) { return 11; };
      OK(11, foobar2(1, "A"));
   }

   Case(NoneOf Matcher)
   {
      MOCKS(foobar2, int, (int, const char*), Once(NoneOf(1, Ge(1)), _)) { return 11; };
      OK(11, foobar2(0, "A"));
   }

   Case(arguments up to 15)
   {
      MOCKS(foobar16, int, (int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int), Once()) { return 11; };
      OK(11, foobar16(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
   }
}

SUITE(mocks member function)
{
   B_DerivedClass b;

   Case(static member function)
   {
      MOCKS(B_DerivedClass::static_f2, const char*, (int a, int b), Once(1, 2)) { return "mocked B.static_f2"; };
      OK("mocked B.static_f2", B_DerivedClass::static_f2(1, 2));
   }

   Case(normal member function)
   {
      MOCKS(B_DerivedClass, normal_f2, const char*, (int, int), Once(1, 2)) { return "mocked B.normal_f2"; };
      OK("mocked B.normal_f2", b.normal_f2(1, 2));
   }

#if !defined WIN32
   Case(virtual member function)
   {
      MOCKS(B_DerivedClass, virtual_f2, const char*, (int, int), Once(1, 2))
      {
         OK(This != nullptr);
         return "mocked B.virtual_f2";
      };
      OK("mocked B.virtual_f2", b.virtual_f2(1, 2));
   }

   Case(no default constructor)
   {
      MOCKS(D_NoConstructorClass, virtual_f3, const char*, (int, int, int), Once())
      {
         return "mocked D.virtual_f3";
      };
      D_NoConstructorClass d(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      OK("mocked D.virtual_f3", d.virtual_f3(1, 2, 3));
   }

   Case(abstract class)
   {
      MOCKS(A_AbstractClass, virtual_f1, const char*, (int), Once()) { return "mocked A.virtual_f1"; };

      OK("mocked A.virtual_f1", b.virtual_f1(1));
   }
#endif
}

SUITE(mocks template function)
{
   Case(function 1 typename)
   {
      OK(4, foobar4<int>(0));

      MOCKS(foobar4<int>, int, (int a), Once()) { return -4; };
      OK(-4, foobar4<int>(0));
   }

   Case(function 2 typename)
   {
      OK(5, foobar5<int, int>(1, 2));

      MOCKS((foobar5<int, int>), int, (int a, int b), Once()) { return -5; };
      OK(-5, foobar5<int, int>(1, 2));
   }
}

SUITE(mocks template member function)
{
   Case(member function 1 typename)
   {
      F_TemplateClass<int> f;
      OK("F.static_f1", f.static_f1(0));
      OK("F.normal_f1", f.normal_f1(0));
      OK("F.virtual_f1", f.virtual_f1(0));

      MOCKS(F_TemplateClass<int>::static_f1, const char*, (int a)) { return "MOCK"; };
      OK("MOCK", f.static_f1(0));
      MOCKS(F_TemplateClass<int>, normal_f1<int>, const char*, (int a)) { return "MOCK"; };
      OK("MOCK", f.normal_f1(0));
#ifndef _WIN32
      MOCKS(F_TemplateClass<int>, virtual_f1, const char*, (int a))
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

      MOCKS((G_TemplateClass<int, int>::static_f2<int, int>), const char*, (int a, int b)) { return "MOCK"; };
      OK("MOCK", (g.static_f2<int, int>(0, 0)));
      MOCKS((G_TemplateClass<int, int>), (normal_f2<int, int>), const char*, (int a, int b)) { return "MOCK"; };
      OK("MOCK", (g.normal_f2<int, int>(0, 0)));
#ifndef _WIN32
      MOCKS((G_TemplateClass<int, int>), (virtual_f2<int, int>), (std::pair<const char*, const char*>), (int a, int b))
      {
         return std::make_pair("MOCK", "MOCK");
      };
      OK(Pair("MOCK", "MOCK"), (g.virtual_f2<int, int>(0, 0)));
#endif
   }
}

SUITE(mocks omit checkin)
{
   Case(only With)
   {
      MOCKS(foobar2, int, (int, const char*), With(1, "A")) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(only Th0)
   {
      MOCKS(foobar2, int, (int, const char*), Th0(1)) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(only Return)
   {
      MOCKS(foobar2, int, (int, const char*), Return(22)) { return 11; };
      OK(22, foobar2(1, "A"));
      OK(22, foobar2(1, "A"));
   }

   Case(only None)
   {
      MOCKS(foobar2, int, (int, const char*)) { return 11; };
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }
}

SUITE(mocks by function name)
{
   Todo(time)  // nm undefined time()
   {
      MOCKS("time", time_t, (time_t*), Once()) { return 42; };
      OK(42, time(NULL));
   }

   Case(foobar)
   {
      MOCKS("foobar0", int, (), Once()) { return 1; };
      OK(1, foobar0());
   }
}
