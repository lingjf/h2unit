h2_inline void h2_case::clear()
{
   h2_sock::clear();
   h2_memory::hook();
   if (fails) delete fails;
   fails = nullptr;
   stats.clear();
}

h2_inline void h2_case::prev_setup()
{
   failed = false;
   h2_memory::stack::push(file);
}

h2_inline void h2_case::post_cleanup()
{
   stats.footprint = h2_memory::stack::footprint();
   dnses.clear();
   h2_stubs::clear(stubs);
   failing(h2_mocks::clear(mocks, true), true, O.contiguous);
   failing(h2_memory::stack::pop(), true, O.contiguous);
}

h2_inline void h2_case::failing(h2_fail* fail, bool defer, bool append)
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
