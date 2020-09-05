#include "../source/h2_unit.cpp"

SUITE(json select empty)
{
   Case(empty)
   {
      h2::h2_json_select select1("");
      OK(0, select1.values.size());
      h2::h2_json_select select2(" ");
      OK(0, select2.values.size());
   }

   Case(.empty)
   {
      h2::h2_json_select select(".");
      OK(0, select.values.size());
   }

   Case(..empty)
   {
      h2::h2_json_select select("..");
      OK(0, select.values.size());
   }

   Case([] empty)
   {
      h2::h2_json_select select1("[]");
      OK(0, select1.values.size());
      h2::h2_json_select select2("[ ]");
      OK(0, select2.values.size());
   }

   Case([][] empty)
   {
      h2::h2_json_select select("[][]");
      OK(0, select.values.size());
   }

   Case(.[] empty)
   {
      h2::h2_json_select select(".[]");
      OK(0, select.values.size());
   }
}

SUITE(json select)
{
   Case(.hello)
   {
      h2::h2_json_select select1(".hello");
      OK(1, select1.values.size());
      OK(0, select1.values[0].index);
      OK("hello", select1.values[0].key);

      h2::h2_json_select select2(". hello ");
      OK(1, select2.values.size());
      OK(0, select2.values[0].index);
      OK("hello", select2.values[0].key);
   }

   Case(.hello.world)
   {
      h2::h2_json_select select(".hello.world");
      OK(2, select.values.size());

      OK(0, select.values[0].index);
      OK("hello", select.values[0].key);
      OK(0, select.values[1].index);
      OK("world", select.values[1].key);
   }

   Case([hello])
   {
      h2::h2_json_select select1("[hello]");
      OK(1, select1.values.size());
      OK(0, select1.values[0].index);
      OK("hello", select1.values[0].key);

      h2::h2_json_select select2("[ hello ]");
      OK(1, select2.values.size());
      OK(0, select2.values[0].index);
      OK("hello", select2.values[0].key);
   }

   Case(["hello"])
   {
      h2::h2_json_select select("[\"hello\"]");
      OK(1, select.values.size());

      OK(0, select.values[0].index);
      OK("hello", select.values[0].key);
   }

   Case(['hello'])
   {
      h2::h2_json_select select("['hello']");
      OK(1, select.values.size());

      OK(0, select.values[0].index);
      OK("hello", select.values[0].key);
   }

   Case([hello][world])
   {
      h2::h2_json_select select("[hello][world]");
      OK(2, select.values.size());

      OK(0, select.values[0].index);
      OK("hello", select.values[0].key);
      OK(0, select.values[1].index);
      OK("world", select.values[1].key);
   }

   Case([1])
   {
      h2::h2_json_select select1("[1]");
      OK(1, select1.values.size());
      OK(1, select1.values[0].index);
      OK("", select1.values[0].key);

      h2::h2_json_select select2("[ -1 ]");
      OK(1, select2.values.size());
      OK(-1, select2.values[0].index);
      OK("", select2.values[0].key);
   }

   Case([1][-2])
   {
      h2::h2_json_select select1("[1][-2]");
      OK(2, select1.values.size());
      OK(1, select1.values[0].index);
      OK("", select1.values[0].key);
      OK(-2, select1.values[1].index);
      OK("", select1.values[1].key);
   }
}
