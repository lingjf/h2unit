#include "../source/h2_unit.cpp"
#include "test_types.hpp"

static int foobar2_stub(int a, const char* b)
{
   return -22;
}

SUITE(stub function)
{
   Case(stub simple)
   {
      OK(2, foobar2(0, "1"));

      STUB(foobar2, foobar2_fake);
      OK(-2, foobar2(0, "1"));

      UNSTUB(foobar2);
      OK(2, foobar2(0, "1"));
   }

   Case(stub overwrite)
   {
      OK(2, foobar2(0, "1"));

      STUB(foobar2, foobar2_fake);
      OK(-2, foobar2(0, "1"));
      STUB(foobar2, foobar2_stub);
      OK(-22, foobar2(0, "1"));
   }

   Case(stub chained)
   {
      OK(2, foobar2(0, "1"));

      STUB(foobar2, foobar2_fake);
      OK(-2, foobar2(0, "1"));

      STUB(foobar2_fake, foobar2_stub);
      OK(-22, foobar2(0, "1"));
   }

   Case(stub in namespace)
   {
      OK(2, test_ns::foobar2(0, "1"));

      STUB(test_ns::foobar2, foobar2_fake);
      OK(-2, test_ns::foobar2(0, "1"));
   }
}

SUITE(stub member function)
{
   Case(static member function)
   {
      OK("A.static_f1", B_DerivedClass::static_f1(1));

      STUB(B_DerivedClass::static_f1, const char*, (int a), B_static_f1_fake);
      OK("-B.static_f1", B_DerivedClass::static_f1(1));
   }

   Case(myself normal member function)
   {
      B_DerivedClass b;
      OK("B.normal_f2", b.normal_f2(1, 2));

      STUB(B_DerivedClass, normal_f2, const char*, (int a, int b), B_normal_f2_fake);
      OK("-B.normal_f2", b.normal_f2(1, 2));
   }

   Case(abstract normal member function)
   {
      B_DerivedClass b;
      OK("A.normal_f1", b.normal_f1(1));

      STUB(A_AbstractClass, normal_f1, const char*, (int a), A_normal_f1_fake);
      OK("-A.normal_f1", b.normal_f1(1));
   }

   Case(derived normal member function)
   {
      B_DerivedClass b;
      OK("A.normal_f1", b.normal_f1(1));

      STUB(B_DerivedClass, normal_f1, const char*, (int a), B_normal_f1_fake);
      OK("-B.normal_f1", b.normal_f1(1));
   }

   Case(override normal member function)
   {
      C_OverrideClass c;
      OK("C.normal_f1", c.normal_f1(1));

      STUB(C_OverrideClass, normal_f1, const char*, (int a), C_normal_f1_fake);
      OK("-C.normal_f1", c.normal_f1(1));
   }

#if !defined WIN32
   Case(myself virtual member function)
   {
      B_DerivedClass b;
      OK("B.virtual_f2", b.virtual_f2(1, 2));

      STUB(B_DerivedClass, virtual_f2, const char*, (int a, int b), B_virtual_f2_fake);
      OK("-B.virtual_f2", b.virtual_f2(1, 2));
   }

   Case(abstract virtual member function)
   {
      B_DerivedClass b;
      OK("A.virtual_f1", b.virtual_f1(1));

      STUB(A_AbstractClass, virtual_f1, const char*, (int a), A_virtual_f1_fake);
      OK("-A.virtual_f1", b.virtual_f1(1));
   }

   Case(derived virtual member function)
   {
      B_DerivedClass b;
      OK("A.virtual_f1", b.virtual_f1(1));

      STUB(B_DerivedClass, virtual_f1, const char*, (int a), B_virtual_f1_fake);
      OK("-B.virtual_f1", b.virtual_f1(1));
   }

   Case(override virtual member function)
   {
      C_OverrideClass c;
      OK("C.virtual_f1", c.virtual_f1(1));

      STUB(C_OverrideClass, virtual_f1, const char*, (int a), C_virtual_f1_fake);
      OK("-C.virtual_f1", c.virtual_f1(1));
   }

   Case(in namespace)
   {
      test_ns::E_NamespaceClass e;
      OK("A.virtual_f1", e.virtual_f1(1));

      STUB(test_ns::E_NamespaceClass, virtual_f1, const char*, (int a), E_virtual_f1_fake);
      OK("-E.virtual_f1", e.virtual_f1(1));
   }
#endif
}

SUITE(stub template function)
{
   Case(function 1 typename)
   {
      OK(4, foobar4<int>(0));

      STUB(foobar4<int>, foobar4_fake);
      OK(-4, foobar4<int>(0));

      UNSTUB(foobar4<int>);
      OK(4, foobar4<int>(0));
   }

   Case(function 2 typename)
   {
      OK(5, foobar5<int, float>(0, 0));

      STUB((foobar5<int, float>), foobar5_fake);
      OK(-5, (foobar5<int, float>(0, 0)));
   }
}

SUITE(stub template class)
{
   Case(member function 1 typename)
   {
      F_TemplateClass<int> f;
      OK("F.static_f1", f.static_f1(0));
      OK("F.normal_f1", f.normal_f1(0));
      OK("F.virtual_f1", f.virtual_f1(0));

      STUB(F_TemplateClass<int>::static_f1, const char*, (int a), F_static_f1_fake);
      OK("-F.static_f1", f.static_f1(0));
      STUB(F_TemplateClass<int>, normal_f1<int>, const char*, (int a), F_normal_f1_fake);
      OK("-F.normal_f1", f.normal_f1(0));
#if !defined WIN32
      STUB(F_TemplateClass<int>, virtual_f1, const char*, (int a), F_virtual_f1_fake);
      OK("-F.virtual_f1", f.virtual_f1(0));
#endif
   }

   Case(member function 2 typename)
   {
      G_TemplateClass<int, int> g;
      OK("G.static_f2", (g.static_f2<int, int>(0, 0)));
      OK("G.normal_f2", (g.normal_f2<int, int>(0, 0)));
      OK(Pair("G", "virtual_f2"), (g.virtual_f2<int, int>(0, 0)));

      STUB((G_TemplateClass<int, int>::static_f2<int, int>), const char*, (int a, int b), G_static_f2_fake);
      OK("-G.static_f2", (g.static_f2<int, int>(0, 0)));
      STUB((G_TemplateClass<int, int>), (normal_f2<int, int>), const char*, (int a, int b), G_normal_f2_fake);
      OK("-G.normal_f2", (g.normal_f2<int, int>(0, 0)));
#if !defined WIN32
      STUB((G_TemplateClass<int, int>), (virtual_f2<int, int>), (std::pair<const char*, const char*>), (int a, int b), G_virtual_f2_fake);
      OK(Pair("-G", "virtual_f2"), (g.virtual_f2<int, int>(0, 0)));
#endif
      UNSTUB((G_TemplateClass<int, int>::static_f2<int, int>), const char*, (int a, int b));
      OK("G.static_f2", (g.static_f2<int, int>(0, 0)));
      UNSTUB((G_TemplateClass<int, int>), (normal_f2<int, int>), const char*, (int a, int b));
      OK("G.normal_f2", (g.normal_f2<int, int>(0, 0)));
#if !defined WIN32
      UNSTUB((G_TemplateClass<int, int>), (virtual_f2<int, int>), (std::pair<const char*, const char*>), (int a, int b));
      OK(Pair("G", "virtual_f2"), (g.virtual_f2<int, int>(0, 0)));
#endif
   }
}

SUITE(stub by function name)
{
   Case("foobar0")
   {
      OK(0, foobar0());

      STUB("foobar0", foobar0_fake);
      OK(-1, foobar0());

      UNSTUB("foobar0");
      OK(0, foobar0());
   }

   Case("test_ns::foobar1")
   {
      OK(1, test_ns::foobar1(0));

      STUB("test_ns::foobar1", foobar1_fake);
      OK(-1, test_ns::foobar1(0));
   }

#if !defined WIN32
   Case("foobar1(int)")
   {
      OK(1, foobar1(0));

      STUB("foobar1(int)", foobar1_fake);
      OK(-1, foobar1(0));
   }

   Case("test_ns::foobar1(float)")
   {
      OK(1, test_ns::foobar1(0));

      STUB("test_ns::foobar1(float)", foobar1_fake);
      OK(-1, test_ns::foobar1(0));
   }
#endif
}
