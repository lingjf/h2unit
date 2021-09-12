h2_inline void h2_case::clear()
{
   h2_sock::clear();
   h2_memory::hook();
   if (fails) delete fails;
   fails = nullptr;
   asserts = 0;
}

h2_inline void h2_case::prev_setup()
{
   failed = false;
   h2_memory::stack::push(file, line);
}

h2_inline void h2_case::post_cleanup()
{
   footprint = h2_memory::stack::footprint();
   dnses.clear();
   stubs.clear();
   do_fail(mocks.clear(true), true, O.verbose >= 5);
   do_fail(h2_memory::stack::pop(), true, O.verbose >= 5);
}

h2_inline void h2_case::do_fail(h2_fail* fail, bool defer, bool append)
{
   if (fail) {
      failed = true;
      if (fails && !append)
         delete fail;
      else
         h2_fail::append_subling(fails, fail);
      if (!defer) ::longjmp(ctx, 1);
   }
}
