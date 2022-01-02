#include "../source/h2_unit.cpp"

SUITE(OK Primitive [api])
{
   Case(unary)
   {
      OK(1);
      OK(!0);
      OK(true);
      OK((std::is_same<long, long int>::value));
   }

   Case(compare integer)
   {
      OK(1 == 1);
      OK(1 != 2);
      OK(1 < 2);
      OK(1 <= 2);
      OK(1 <= 1);
      OK(2 > 1);
      OK(2 >= 1);
      OK(2 >= 2);
   }

   Case(compare double)
   {
      OK(1.0 == 1);
      OK(1.0 != 2);
      OK(1.0 < 2);
      OK(1.0 <= 2);
      OK(1.0 <= 1);
      OK(2.0 > 1);
      OK(2.0 >= 1);
      OK(2.0 >= 2);
   }

   Case(compare string)
   {
      OK("abc" == "abc");
      OK("abc" != "xyz");

      const char* a1 = "def";
      OK("abc" != a1);
   }

   Case(compare pointer)
   {
      const char* a1 = "def";
      OK(NULL != a1);
      OK(a1 != NULL);

      const char* a2 = nullptr;
      OK(NULL == a2);
      OK(a2 == NULL);

      int* b1 = (int*)12345678;
      OK(NULL != b1);
      OK(b1 != NULL);

      int* b2 = nullptr;
      OK(NULL == b2);
      OK(b2 == NULL);

      void* c1 = (void*)12345678;
      OK(NULL != c1);
      OK(c1 != NULL);

      void* c2 = nullptr;
      OK(NULL == c2);
      OK(c2 == NULL);
   }

   Case(dual)
   {
      OK(1, 1);
      OK(true, (std::is_same<long, long int>::value));
   }

   Case(Any)
   {
      OK(_, 1);
      OK(Any(), "A");
      OK((_), "A");
      OK((Any()), "A");
   }

   Case(Je)
   {
      OK(~Je("{'name': /hello.*world/, 'age': 18}"), "{'Name': \"hello world\", 'age': 18}");
      OK(!Je("{'name': /hello.*world/, 'age': 18}"), "{'Name': \"hello world\", 'age': 18}");
   }
}

SUITE(JE Primitive)
{
   Case(normal)
   {
      JE("{'name': \"hello world\"}", "{'name': \"hello world\", 'age': [18, 20]}");
   }

   Case(selector)
   {
      JE("hello world", "{'name': \"hello world\", 'age': [18, 20]}", ".name");
      JE("18", "{'name': \"hello world\", 'age': [18, 20]}", ".age[0]");
      JE("20", "{'name': \"hello world\", 'age': [18, 20]}", ".age[-1]");

      OK(Je("hello world", ".name"), "{'name': \"hello world\", 'age': [18, 20]}");
      OK(Je("18", ".age[0]"), "{'name': \"hello world\", 'age': [18, 20]}");
   }

   Case(selector empty)
   {
      OK(!Je("hello world", ".say"), "{'name': \"hello world\", 'age': [18, 20]}");
   }
}

SUITE(KO Primitive [api])
{
   Case(unary)
   {
      KO(!1);
      KO(0);
      KO(false);
      KO((std::is_same<long long, long int>::value));
   }

   Case(compare integer)
   {
      KO(1 != 1);
      KO(1 == 2);
      KO(1 > 2);
      KO(1 >= 2);
   }

   Case(compare string)
   {
      KO("abc" != "abc");
      KO("abc" == "xyz");
   }

   Case(compare pointer)
   {
      const char* a1 = "def";
      KO(NULL == a1);
      KO(a1 == NULL);

      const char* a2 = nullptr;
      KO(NULL != a2);
      KO(a2 != NULL);
   }

   Case(dual)
   {
      KO(1, 2);
      KO(false, (std::is_same<long, long int>::value));
   }

   Case(Je)
   {
      KO(Je("{'name': /hello.*world/, 'age': 18}"), "{'Name': \"hello world\", 'age': 18}");
   }
}

SUITE(Warning Primitive [api])
{
   Case(warning OK)
   {
      Warning OK(1);
      Warning OK("abc" != "xyz");
   }

   Case(warning JE)
   {
      Warning JE("[1]", "[1]");
   }
}
