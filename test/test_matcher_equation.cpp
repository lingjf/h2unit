#include "../source/h2_unit.cpp"

CASE(void* type)
{
   OK(!(std::is_arithmetic<void*>::value));
}

SUITE(null matches)
{
   Case(Null)
   {
      int int65 = 65;

      h2::h2_matches_null ee1(false);
      OK(nullptr == ee1.matches(NULL, {}));
      OK(nullptr == ee1.matches(nullptr, {}));
      OK(nullptr != ee1.matches(&int65, {}));
      OK("NULL", ee1.expection({}));
      OK("!NULL", ee1.expection({0, true, false, false, false}));

      h2::h2_matches_null ee2(true);
      OK(nullptr == ee2.matches(&int65, {}));
      OK(nullptr != ee2.matches(nullptr, {}));
      OK("!NULL", ee2.expection({}));
      OK("NULL", ee2.expection({0, true, false, false, false}));
   }
}

SUITE(bool matches)
{
   Case(true)
   {
      h2::h2_matches_bool istrue(true);
      OK(nullptr == istrue.matches(true, {}));
      OK(nullptr != istrue.matches(false, {}));
      OK("true", istrue.expection({}));
      OK("false", istrue.expection({0, true, false, false, false}));
   }

   Case(false)
   {
      h2::h2_matches_bool isfalse(false);
      OK(nullptr == isfalse.matches(false, {}));
      OK(nullptr != isfalse.matches(true, {}));
      OK("false", isfalse.expection({}));
      OK("true", isfalse.expection({0, true, false, false, false}));
   }
}

SUITE(equation matches)
{
   Case(integer)
   {
      h2::h2_equation<int> a1(123);
      OK(nullptr == a1.matches(123, {}));
      h2::h2_equation<const int> a2(123);
      OK(nullptr == a2.matches(123, {}));
      h2::h2_equation<unsigned long long> a3(123);
      OK(nullptr == a3.matches(123, {}));
   }

   Case(string)
   {
      // cannot convert argument 1 from 'const char [4]' to 'const char *'
      // <ConformanceMode>false</ConformanceMode>; /permissive- ==> /permissive
      h2::h2_equation<char*> a1((char*)"abc");
      OK(nullptr == a1.matches((char*)"abc", {}));
      h2::h2_equation<h2::h2_string> a2("abc");
      OK(nullptr == a2.matches("abc", {}));
      h2::h2_equation<std::string> a3("abc");
      OK(nullptr == a3.matches("abc", {}));

      h2::h2_equation<char*> a4((char*)"*bc");
      OK(nullptr == a4.matches((char*)"abc", {}));
   }

   Case(float)
   {
      h2::h2_equation<float> a1(65.0);
      OK(nullptr == a1.matches(65, {}));
      h2::h2_equation<double> a2(65.0);
      OK(nullptr == a2.matches(65, {}));
   }

   Case(float default approximate)
   {
      h2::h2_equation<float> a1(65.000000001);
      OK(nullptr == a1.matches(65, {}));
      h2::h2_equation<double> a2(65.000000001);
      OK(nullptr == a2.matches(65, {}));
   }

   Case(float absolute approximate)
   {
      h2::h2_equation<double> a1(65.000000001, 0.001);
      OK(nullptr == a1.matches(65, {}));
      h2::h2_equation<double> a2(65.001, 0.0001);
      OK(nullptr != a2.matches(65, {}));
   }

   Case(float percentage approximate)
   {
      h2::h2_equation<float> a1(65.0, 0.01_p);  // 1%
      OK(nullptr == a1.matches(65.1, {}));
      h2::h2_equation<double> a2(65.0, 0.1_p);  // 10%
      OK(nullptr == a2.matches(66, {}));
   }
}
