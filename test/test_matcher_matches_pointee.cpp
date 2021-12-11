#include "../source/h2_unit.cpp"

SUITE(matches pointee)
{
   Case(h2_pointee_matches)
   {
      int int65 = 65, int66 = 66;
      h2::h2_pointee_matches<h2::h2_polymorphic_matcher<h2::h2_equation<int>>> a1(h2::h2_polymorphic_matcher<h2::h2_equation<int>>{h2::h2_equation<int>(65)});

      OK(nullptr == a1.matches(&int65, {}));
      OK(nullptr != a1.matches(&int66, {}));
      OK("65", a1.expection({}));
   }
}

SUITE(Pointee)
{
   Case(int*)
   {
      int a = 42;

      int* a1 = &a;
      int*& a2 = a1;
      const int* a3 = &a;
      const int* const a4 = &a;
      const int*& a5 = a3;

      OK(Pointee(42), a1);
      OK(Pointee(42), a2);
      OK(Pointee(42), a3);
      OK(Pointee(42), a4);
      OK(Pointee(42), a5);
   }

   Case(char*)
   {
      char a = 'C';

      char* a1 = &a;
      const char* a2 = &a;

      OK(Pointee('C'), a1);
      OK(Pointee('C'), a2);
   }

   Case(string*)
   {
      std::string a = "String";

      std::string* a1 = &a;
      const std::string* a2 = &a;
      const std::string* const a3 = &a;

      OK(Pointee("String"), a1);
      OK(Pointee("String"), a2);
      OK(Pointee("String"), a3);
   }

   Case(shared_ptr)
   {
      auto a1 = std::make_shared<int>(42);
      OK(Pointee(42), a1);
   }

   Case(unique_ptr)
   {
      std::unique_ptr<int> a2(new int(42));
      OK(Pointee(42), a2);
   }

   Case(weak_ptr)
   {
      // std::weak_ptr<int> a3(std::make_shared<int>(42));
      // OK(Pointee(42), a3);
   }
}

CASE(Nest Pointee)
{
   int a = 42;

   int* a1 = &a;
   int** a2 = &a1;
   int*** a3 = &a2;
   int**** a4 = &a3;
   int***** a5 = &a4;
   int****** a6 = &a5;
   int******* a7 = &a6;
   int******** a8 = &a7;
   int********* a9 = &a8;

   OK(Pointee(42), a1);
   OK(Pointee(Pointee(42)), a2);
   OK(Pointee(Pointee(Pointee(42))), a3);
   OK(Pointee(Pointee(Pointee(Pointee(42)))), a4);
   OK(Pointee(Pointee(Pointee(Pointee(Pointee(42))))), a5);
   OK(Pointee(Pointee(Pointee(Pointee(Pointee(Pointee(42)))))), a6);
   OK(Pointee(Pointee(Pointee(Pointee(Pointee(Pointee(Pointee(42))))))), a7);
   OK(Pointee(Pointee(Pointee(Pointee(Pointee(Pointee(Pointee(Pointee(42)))))))), a8);
   OK(Pointee(Pointee(Pointee(Pointee(Pointee(Pointee(Pointee(Pointee(Pointee(42))))))))), a9);
}
