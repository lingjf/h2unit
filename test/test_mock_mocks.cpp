#include "../source/h2_unit.cpp"
#include "test_types.hpp"

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
      MOCKS(B_DerivedClass::static_f2, const char*, (int x, int y), Once(1, 2))
      {
         return sprintf(buffer, "*B.static_f2(%d,%d)", x, y), buffer;
      };
      OK("*B.static_f2(1,2)", B_DerivedClass::static_f2(1, 2));
   }

   Case(normal member function)
   {
      MOCKS(B_DerivedClass, normal_f2, const char*, (int x, int y), Once(1, 2))
      {
         return sprintf(buffer, "*B.normal_f2(%d,%d)%c", x, y, This->b), buffer;
      };
      OK("*B.normal_f2(1,2)b", b.normal_f2(1, 2));
   }

   Case(virtual member function)
   {
      MOCKS(B_DerivedClass, virtual_f2, const char*, (int x, int y), Once(1, 2))
      {
         OK(This != nullptr);
         return sprintf(buffer, "*B.virtual_f2(%d,%d)%c", x, y, This->b), buffer;
      };
      OK("*B.virtual_f2(1,2)b", b.virtual_f2(1, 2));
   }

#if !defined WIN32
   Case(no default constructor)
   {
      MOCKS(D_NoConstructorClass, virtual_f3, const char*, (int x, int y, int z), Once())
      {
         return sprintf(buffer, "*D.virtual_f3(%d,%d,%d)%c", x, y, z, This->d), buffer;
      };
      D_NoConstructorClass d(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      OK("*D.virtual_f3(1,2,3)d", d.virtual_f3(1, 2, 3));
   }

   Case(abstract class)
   {
      MOCKS(A_AbstractClass, virtual_f1, const char*, (int x), Once())
      {
         return sprintf(buffer, "*A.virtual_f1(%d)%c", x, This->a), buffer;
      };
      OK("*A.virtual_f1(1)a", b.virtual_f1(1));
   }
#endif

   Case(abstract virtual member function with object)
   {
      B_DerivedClass b;
      OK("A.virtual_f1(1)a", b.virtual_f1(1));
      A_AbstractClass* a = dynamic_cast<A_AbstractClass*>(&b);

      MOCKS(a, A_AbstractClass, virtual_f1, const char*, (int x), Once())
      {
         return sprintf(buffer, "*A.virtual_f1(%d)%c", x, This->a), buffer;
      };
      OK("*A.virtual_f1(1)a", b.virtual_f1(1));
   }

   Case(no default constructor with object)
   {
      D_NoConstructorClass d(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      MOCKS(d, D_NoConstructorClass, virtual_f3, const char*, (int x, int y, int z), Once())
      {
         return sprintf(buffer, "*D.virtual_f3(%d,%d,%d)%c", x, y, z, This->d), buffer;
      };
      OK("*D.virtual_f3(1,2,3)d", d.virtual_f3(1, 2, 3));
   }
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
      OK("F.static_f1(1)", f.static_f1(1));
      OK("F.normal_f1(1)f", f.normal_f1(1));
      OK("F.virtual_f1(1)f", f.virtual_f1(1));

      MOCKS(F_TemplateClass<int>::static_f1, const char*, (int x), Any())
      {
         return sprintf(buffer, "*F.static_f1(%d)", x), buffer;
      };
      OK("*F.static_f1(1)", f.static_f1(1));
      MOCKS(F_TemplateClass<int>, normal_f1<int>, const char*, (int x), Any())
      {
         return sprintf(buffer, "*F.normal_f1(%d)%c", x, This->f), buffer;
      };
      OK("*F.normal_f1(1)f", f.normal_f1(1));
      MOCKS(F_TemplateClass<int>, virtual_f1, const char*, (int x), Any())
      {
         return sprintf(buffer, "*F.virtual_f1(%d)%c", x, This->f), buffer;
      };
      OK("*F.virtual_f1(1)f", f.virtual_f1(1));
   }

   Case(member function 2 typename)
   {
      G_TemplateClass<int, int> g;
      OK("G.static_f2(1,2)", (g.static_f2<int, int>(1, 2)));
      OK("G.normal_f2(1,2)g", (g.normal_f2<int, int>(1, 2)));
      OK(Pair("G.virtual_f2", 12.7), (g.virtual_f2<int, int>(1, 2)));

      MOCKS((G_TemplateClass<int, int>::static_f2<int, int>), const char*, (int x, int y), Any())
      {
         return sprintf(buffer, "*G.static_f2(%d,%d)", x, y), buffer;
      };
      OK("*G.static_f2(1,2)", (g.static_f2<int, int>(1, 2)));
      MOCKS((G_TemplateClass<int, int>), (normal_f2<int, int>), const char*, (int x, int y), Any())
      {
         return sprintf(buffer, "*G.normal_f2(%d,%d)%c", x, y, This->g), buffer;
      };
      OK("*G.normal_f2(1,2)g", (g.normal_f2<int, int>(1, 2)));
#if !defined WIN32 // Suck when member return Object
      MOCKS((G_TemplateClass<int, int>), (virtual_f2<int, int>), (std::pair<const char*, double>), (int x, int y), Any())
      {
         return std::make_pair("*G.virtual_f2", x * 10 + y + This->G / 10.0);
      };
      OK(Pair("*G.virtual_f2", 12.7), (g.virtual_f2<int, int>(1, 2)));
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
      MOCKS(foobar2, int, (int, const char*), Any()) { return 11; };
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
