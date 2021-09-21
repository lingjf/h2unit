#include "../source/h2_unit.cpp"

SUITE(OK Primitive)
{
   Case(Any)
   {
      OK(_, 1);
      OK(Any, "A");
      OK((_), "A");
      OK((Any), "A");
   }

   Case(IsNull NotNull)
   {
      OK(IsNull, 0);
      OK((NotNull), 1);
   }

   Case(Je)
   {
      OK(~Je("{'name': /hello.*world/, 'age': 18}"), "{'Name': \"hello world\", 'age': 18}");
      OK(!Je("{'name': /hello.*world/, 'age': 18}"), "{'Name': \"hello world\", 'age': 18}");
   }
}

SUITE(JE Primitive)
{
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

SUITE(Ok Primitive)
{
   Case(bool)
   {
      Ok(true);
      Ok(1);
   }

   Case(compare integer)
   {
      Ok(1 == 1);
      Ok(1 != 2);
      Ok(1 < 2);
      Ok(1 <= 2);
      Ok(1 <= 1);
      Ok(2 > 1);
      Ok(2 >= 1);
      Ok(2 >= 2);
   }

   Case(compare double)
   {
      Ok(1.0 == 1);
      Ok(1.0 != 2);
      Ok(1.0 < 2);
      Ok(1.0 <= 2);
      Ok(1.0 <= 1);
      Ok(2.0 > 1);
      Ok(2.0 >= 1);
      Ok(2.0 >= 2);
   }

   Case(compare string)
   {
      Ok("abc" == "abc");
      Ok("abc" != "xyz");
   }
}
