#include "../source/h2_unit.cpp"

SUITE(failure utils)
{
   h2::h2_string a_NULL = "NULL";
   h2::h2_string a_nullptr = "nullptr";
   h2::h2_string a_void = "((void *)0)";
   h2::h2_string a___null = "__null";
   h2::h2_string a_nil = "(nil)";
   h2::h2_string a_0 = "0";
   h2::h2_string a_0x0 = "0x0";

   h2::h2_string true_IsTrue = "IsTrue";
   h2::h2_string true_true = "true";
   h2::h2_string true_1 = "1";

   h2::h2_string false_IsFalse = "IsFalse";
   h2::h2_string false_false = "false";
   h2::h2_string false_0 = "0";

   Case(is_synonym null)
   {
#define NULL_SET \
   a_NULL,       \
     a_nullptr,  \
     a_void,     \
     a___null,   \
     a_0,        \
     a_0x0

#define TheCheck(x, y) OK(h2::is_synonym(x, y));
      H2Fullmesh(TheCheck, (NULL_SET));
#undef TheCheck
   }

   Case(is_synonym true)
   {
#define TRUE_SET true_IsTrue, true_true, true_1
#define TheCheck(x, y) OK(h2::is_synonym(x, y));
      H2Fullmesh(TheCheck, (TRUE_SET));
#undef TheCheck
   }

   Case(is_synonym false)
   {
#define FALSE_SET false_IsFalse, false_false, false_0
#define TheCheck(x, y) OK(h2::is_synonym(x, y));
      H2Fullmesh(TheCheck, (FALSE_SET));
#undef TheCheck
   }
}

#define H2_H2_FOREACH_FAIL(f, First)                                        \
   for (h2::h2_fail* x_fail = First; x_fail; x_fail = x_fail->subling_next) \
      for (h2::h2_fail* f = x_fail; f; f = f->child_next)

SUITE(failure)
{
   auto fa = new h2::h2_fail({}, "a");
   auto fb = new h2::h2_fail({}, "b");
   auto fc = new h2::h2_fail({}, "c");

   auto f1 = new h2::h2_fail({}, "1");
   auto f2 = new h2::h2_fail({}, "2");
   auto f3 = new h2::h2_fail({}, "3");
   auto f4 = new h2::h2_fail({}, "4");
   auto f5 = new h2::h2_fail({}, "5");
   auto f6 = new h2::h2_fail({}, "6");
   auto f7 = new h2::h2_fail({}, "7");
   auto f8 = new h2::h2_fail({}, "8");
   auto f9 = new h2::h2_fail({}, "9");

   int i = 0;

   Case(append x)
   {
      h2::h2_fail* fails = nullptr;

      h2::h2_fail::append_subling(fails, fa);
      h2::h2_fail::append_subling(fails, fb);
      h2::h2_fail::append_subling(fails, fc);

      // Subling ->  fa -> fb -> fc
      const char* e[] = {"a", "b", "c"};
      H2_H2_FOREACH_FAIL(fail, fails) { OK(e[i++], fail->filine); }
      fails->foreach([&](h2::h2_fail* fail, size_t, size_t) {
      });
   }

   Case(append y)
   {
      h2::h2_fail* fails = nullptr;

      h2::h2_fail::append_child(fails, fa);
      h2::h2_fail::append_child(fails, fb);
      h2::h2_fail::append_child(fails, fc);

      //  Y
      //  |
      //  v
      //  fa
      //  |
      //  v
      //  fb
      //  |
      //  v
      //  fc
      const char* e[] = {"a", "b", "c"};
      H2_H2_FOREACH_FAIL(fail, fails) { OK(e[i++], fail->filine); }
   }

   Case(append x y)
   {
      h2::h2_fail* fails = nullptr;

      h2::h2_fail::append_subling(fails, fa);
      h2::h2_fail::append_subling(fails, fb);
      h2::h2_fail::append_subling(fails, fc);

      h2::h2_fail::append_child(fa, f1);
      h2::h2_fail::append_child(fa, f2);
      h2::h2_fail::append_child(fa, f3);

      h2::h2_fail::append_child(fb, f4);
      h2::h2_fail::append_child(fb, f5);
      h2::h2_fail::append_child(fb, f6);

      h2::h2_fail::append_child(fc, f7);
      h2::h2_fail::append_child(fc, f8);
      h2::h2_fail::append_child(fc, f9);

      // X->
      // Y   fa -> fb -> fc
      // |   |     |     |
      // v   v     v     v
      //     f1    f4    f7
      //     |     |     |
      //     v     v     v
      //     f2    f5    f8
      //     |     |     |
      //     v     v     v
      //     f3    f6    f9

      const char* e[] = {"a", "1", "2", "3", "b", "4",
                         "5", "6", "c", "7", "8", "9"};
      H2_H2_FOREACH_FAIL(fail, fails) { OK(e[i++], fail->filine); }
   }
}
