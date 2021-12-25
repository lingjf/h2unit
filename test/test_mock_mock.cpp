#include "../source/h2_unit.cpp"
#include "test_types.hpp"

SUITE(mock function)
{
   Case(once)
   {
      MOCK(foobar2, int(int, const char*)).Once(1, "A").Return(11);
      OK(11, foobar2(1, "A"));
   }

   Case(twice)
   {
      MOCK(foobar2, int(int, const char*)).Twice(Eq(1), _).Return(11);
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "BC"));
   }

   Case(3 times)
   {
      MOCK(foobar2, int(int, const char*)).Times(3).With(Ge(1), "A").Return(11);
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(any 0)
   {
      MOCK(foobar2, int(int, const char*)).Any(1, "A");
   }

   Case(any 1)
   {
      MOCK(foobar2, int(int, const char*)).Any().With(1, "A").Return(11);
      OK(11, foobar2(1, "A"));
   }

   Case(any 2)
   {
      MOCK(foobar2, int(int, const char*)).Any().With(1, "A").Return(11);
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(atleast 2)
   {
      MOCK(foobar2, int(int, const char*)).Atleast(2).With(1, "A").Return(11);
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(atleast 3)
   {
      MOCK(foobar2, int(int, const char*)).Atleast(2).With(1, "A").Return(11);
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(atmost 1)
   {
      MOCK(foobar2, int(int, const char*)).Atmost(2).With(1, "A").Return(11);
      OK(11, foobar2(1, "A"));
   }

   Case(atmost 2)
   {
      MOCK(foobar2, int(int, const char*)).Atmost(2).With(1, "A").Return(11);
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(between)
   {
      MOCK(foobar2, int(int, const char*)).Between(2, 4).With(1, "A").Return(11);
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(void return)
   {
      MOCK(foobar21, void(int& a, char*)).Once(1, (char*)"A");

      char t[32] = "A";
      int a1 = 1;
      foobar21(a1, t);
   }

   Case(Th0)
   {
      MOCK(foobar21, void(int& a, char*)).Once().Th0(1);

      char t[32] = "";
      int a1 = 1;
      foobar21(a1, t);
   }

   Case(Th1)
   {
      MOCK(foobar21, void(int& a, char*)).Once().Th1((char*)"A");

      char t[32] = "A";
      int a1 = 1;
      foobar21(a1, t);
   }

   Case(zero parameter)
   {
      MOCK(foobar22, void()).Once();
      foobar22();
   }

   Case(Is Null Matcher)
   {
      MOCK(foobar2, int(int, const char*)).Once(1, NULL).Return(11);
      OK(11, foobar2(1, NULL));
   }

   Case(Substr Matcher)
   {
      MOCK(foobar2, int(int, const char*)).Once(1, Substr("BC")).Return(11);
      OK(11, foobar2(1, "ABCD"));
   }

   Case(Not Matcher)
   {
      MOCK(foobar2, int(int, const char*)).Once(Not(2), _).Return(11);
      OK(11, foobar2(1, "A"));
   }

   Case(AllOf Matcher)
   {
      MOCK(foobar2, int(int, const char*)).Once(AllOf(1, Ge(1)), _).Return(11);
      OK(11, foobar2(1, "A"));
   }

   Case(AnyOf Matcher)
   {
      MOCK(foobar2, int(int, const char*)).Once(AnyOf(Le(1), Gt(2)), _).Return(11);
      OK(11, foobar2(1, "A"));
   }

   Case(NoneOf Matcher)
   {
      MOCK(foobar2, int(int, const char*)).Once(NoneOf(1, Ge(1)), _).Return(11);
      OK(11, foobar2(0, "A"));
   }

   Case(arguments up to 15)
   {
      MOCK(foobar16, int(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int)).Once().Return(11);
      OK(11, foobar16(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
   }
}

SUITE(multi line)
{
   Case(1 checkin)
   {
      MOCK(foobar2, int(int, const char*)).Once(1, "A").Return(11).Once().With(2, "B").Return(22).Twice(3, "C").Return(33);

      OK(11, foobar2(1, "A"));
      OK(22, foobar2(2, "B"));
      OK(33, foobar2(3, "C"));
      OK(33, foobar2(3, "C"));
   }

   Case(2 checkin)
   {
      MOCK(foobar2, int(int, const char*)).Once(1, "A").Return(11).Twice().With(2, "B").Return(22);

      OK(11, foobar2(1, "A"));
      OK(22, foobar2(2, "B"));
      OK(22, foobar2(2, "B"));
   }
}

SUITE(mock greed)
{
   Case(greed default true)
   {
      MOCK(foobar2, int(int, const char*))
        //   .greed(true) // default is true
        .Between(1, 3)
        .With(1, "A")
        .Return(11)
        .Once(1, _)
        .Return(22);

      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
      OK(22, foobar2(1, "A"));
   }

   Case(greed false)
   {
      MOCK(foobar2, int(int, const char*)).greed(false).Between(1, 3).With(1, "A").Return(11).Once(1, _).Return(22);

      OK(11, foobar2(1, "A"));
      OK(22, foobar2(1, "A"));
   }
}

SUITE(mock Return)
{
   Case(delegate to origin without Return)
   {
      MOCK(foobar2, int(int, const char*)).Once(1, "A");
      OK(2, foobar2(1, "A"));
   }
}

SUITE(mock member function)
{
   B_DerivedClass b;

   Case(static member function)
   {
      MOCK(B_DerivedClass::static_f2, const char*(int a, int b)).Once(1, 2).Return("+B.static_f2");
      OK("+B.static_f2", B_DerivedClass::static_f2(1, 2));
   }

   Case(normal member function)
   {
      MOCK(B_DerivedClass, normal_f2, const char*(int, int)).Once(1, 2).Return("+B.normal_f2");
      OK("+B.normal_f2", b.normal_f2(1, 2));
   }

   Case(const member function)
   {
      MOCK(B_DerivedClass, const_f, const char*(int)).Once(1, 2).Return("+B.const_f");
      OK("+B.const_f", b.const_f(1));
   }

   Case(noexcept member function)
   {
      MOCK(B_DerivedClass, noexcept_f, const char*(int)).Once(1, 2).Return("+B.noexcept_f");
      OK("+B.noexcept_f", b.noexcept_f(1));
   }

   Case(const noexcept member function)
   {
      MOCK(B_DerivedClass, const_noexcept_f, const char*(int)).Once(1, 2).Return("+B.const_noexcept_f");
      OK("+B.const_noexcept_f", b.const_noexcept_f(1));
   }

   Case(overload member function)
   {
      C_OverrideClass c;

      // MOCK(C_OverrideClass, overload_f, const char*()).Once().Return("+C.overload_f0");
      // OK("+C.overload_f0", c.overload_f());

      MOCK(C_OverrideClass, overload_f, const char*(int, int)).Once(1, 2).Return("+C.overload_f2");
      OK("+C.overload_f2", c.overload_f(1, 2));

      MOCK(C_OverrideClass, overload_f, const char*(int, int, int)).Once(1, 2, 3).Return("+C.overload_f3");
      OK("+C.overload_f3", c.overload_f(1, 2, 3));
   }

   Case(virtual member function)
   {
      MOCK(B_DerivedClass, virtual_f2, const char*(int, int)).Once(1, 2).Return("+B.virtual_f2");
      OK("+B.virtual_f2", b.virtual_f2(1, 2));
   }

#if !defined WIN32
   Case(no default constructor)
   {
      MOCK(D_NoConstructorClass, virtual_f3, const char*(int, int, int)).Once().Return("+D.virtual_f3");
      D_NoConstructorClass d(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      OK("+D.virtual_f3", d.virtual_f3(1, 2, 3));
   }

   Case(abstract class)
   {
      MOCK(A_AbstractClass, virtual_f1, const char*(int)).Once().Return("+A.virtual_f1");
      OK("+A.virtual_f1", b.virtual_f1(1));
   }
#endif

   Case(abstract virtual member function with object)
   {
      B_DerivedClass b;
      OK("A.virtual_f1(1)a", b.virtual_f1(1));
      A_AbstractClass* a = dynamic_cast<A_AbstractClass*>(&b);

      MOCK(a, A_AbstractClass, virtual_f1, const char*(int)).Once().Return("+A.virtual_f1");
      OK("+A.virtual_f1", b.virtual_f1(1));
   }

   Case(no default constructor with object)
   {
      D_NoConstructorClass d(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
      MOCK(d, D_NoConstructorClass, virtual_f3, const char*(int, int, int)).Once().Return("+D.virtual_f3");
      OK("+D.virtual_f3", d.virtual_f3(1, 2, 3));
   }
}

SUITE(mock template function)
{
   Case(function 1 typename)
   {
      OK(4, foobar4<int>(0));

      MOCK(foobar4<int>, int(int a)).Once().Return(-4);
      OK(-4, foobar4<int>(0));
   }

   Case(function 2 typename)
   {
      OK(5, (foobar5<int, int>(1, 2)));

      MOCK((foobar5<int, int>), int(int a, int b)).Once().Return(-5);
      OK(-5, (foobar5<int, int>(1, 2)));
   }
}

SUITE(mock template member function)
{
   Case(member function 1 typename)
   {
      F_TemplateClass<int> f;
      OK("F.static_f1(1)", f.static_f1(1));
      OK("F.normal_f1(1)f", f.normal_f1(1));
      OK("F.virtual_f1(1)f", f.virtual_f1(1));

      MOCK(F_TemplateClass<int>::static_f1, const char*(int a)).Return("+F.static_f1");
      OK("+F.static_f1", f.static_f1(0));
      MOCK(F_TemplateClass<int>, normal_f1<int>, const char*(int a)).Return("+F.normal_f1");
      OK("+F.normal_f1", f.normal_f1(0));
      MOCK(F_TemplateClass<int>, virtual_f1, const char*(int a)).Return("+F.virtual_f1");
      OK("+F.virtual_f1", f.virtual_f1(0));
   }

   Case(member function 2 typename)
   {
      G_TemplateClass<int, int> g;
      OK("G.static_f2(1,2)", (g.static_f2<int, int>(1, 2)));
      OK("G.normal_f2(1,2)g", (g.normal_f2<int, int>(1, 2)));
      OK(Pair("G.virtual_f2", 12.7), (g.virtual_f2<int, int>(1, 2)));

      MOCK((G_TemplateClass<int, int>::static_f2<int, int>), const char*(int a, int b)).Return("+G.static_f2");
      OK("+G.static_f2", (g.static_f2<int, int>(0, 0)));
      MOCK((G_TemplateClass<int, int>), (normal_f2<int, int>), const char*(int a, int b)).Return("+G.normal_f2");
      OK("+G.normal_f2", (g.normal_f2<int, int>(0, 0)));
#if !defined WIN32  // Suck when member return Object
      MOCK((G_TemplateClass<int, int>), (virtual_f2<int, int>), (std::pair<const char*, double>)(int a, int b)).Return(std::make_pair("+G.virtual_f2", 0.0));
      OK(Pair("+G.virtual_f2", 0.0), (g.virtual_f2<int, int>(0, 0)));
#endif
   }
}

SUITE(mock omit checkin)
{
   Case(only With)
   {
      MOCK(foobar2, int(int, const char*)).With(1, "A").Return(11);
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(only Th0)
   {
      MOCK(foobar2, int(int, const char*)).Th0(1).Return(11);
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }

   Case(only Return)
   {
      MOCK(foobar2, int(int, const char*)).Return(22);
      OK(22, foobar2(1, "A"));
      OK(22, foobar2(1, "A"));
   }

   Case(only None)
   {
      MOCK(foobar2, int(int, const char*)).Return(11);
      OK(11, foobar2(1, "A"));
      OK(11, foobar2(1, "A"));
   }
}

SUITE(mock by function name)
{
   Todo(time)  // nm undefined time()
   {
      MOCK("time", time_t(time_t*)).Once().Return(11);
      OK(42, time(NULL));
   }

   Todo(time)
   {
      MOCK(time, time_t(time_t*)).Once().Return((time_t)0);
   }

   Case(foobar)
   {
      MOCK("foobar0", int()).Once().Return(1);
      OK(1, foobar0());
   }
}
