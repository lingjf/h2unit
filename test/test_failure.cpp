#include "../source/h2_unit.cpp"

#define H2_H2_FOREACH_FAIL(f, First)                                        \
   for (h2::h2_fail* x_fail = First; x_fail; x_fail = x_fail->subling_next) \
      for (h2::h2_fail* f = x_fail; f; f = f->child_next)

SUITE(failure)
{
   auto fa = new h2::h2_fail("a", 11, "ff", 2);
   auto fb = new h2::h2_fail("b", 12);
   auto fc = new h2::h2_fail("c", 13, "fx");

   auto f1 = new h2::h2_fail("1", 21);
   auto f2 = new h2::h2_fail("2", 22);
   auto f3 = new h2::h2_fail("3", 23);
   auto f4 = new h2::h2_fail("4", 24);
   auto f5 = new h2::h2_fail("5", 25);
   auto f6 = new h2::h2_fail("6", 26);
   auto f7 = new h2::h2_fail("7", 27);
   auto f8 = new h2::h2_fail("8", 28);
   auto f9 = new h2::h2_fail("9", 29);

   int i = 0;

   Case(append x)
   {
      h2::h2_fail* fails = nullptr;

      h2::h2_fail::append_subling(fails, fa);
      h2::h2_fail::append_subling(fails, fb);
      h2::h2_fail::append_subling(fails, fc);

      // Subling ->  fa -> fb -> fc
      const char* e[] = {"a", "b", "c"};
      H2_H2_FOREACH_FAIL(fail, fails) { OK(e[i++], fail->file); }
      fails->foreach ([&](h2::h2_fail* fail, int subling_index, int child_index) {
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
      H2_H2_FOREACH_FAIL(fail, fails) { OK(e[i++], fail->file); }
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
      H2_H2_FOREACH_FAIL(fail, fails) { OK(e[i++], fail->file); }
   }
}
