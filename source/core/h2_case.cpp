
h2_inline void h2_case::clear()
{
   h2_sock::clear();
   if (fails) delete fails;
   fails = nullptr;
   asserts = 0;
}

h2_inline void h2_case::prev_setup()
{
   status = passed;
   h2_memory::stack::push(file, line);
}

h2_inline void h2_case::post_cleanup()
{
   footprint = h2_memory::stack::footprint();
   dnses.clear();
   stubs.clear();
   do_fail(mocks.clear(true), true, O.verbose);
   do_fail(h2_memory::stack::pop(), true, O.verbose);
}

h2_inline void h2_case::do_fail(h2_fail* fail, bool defer, bool append)
{
   if (fail) {
      status = failed;
      if (fails && !append)
         delete fail;
      else
         h2_fail::append_subling(fails, fail);
      if (!defer) ::longjmp(ctx, 1);
   }
}
