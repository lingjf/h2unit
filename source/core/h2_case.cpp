h2_inline void h2_case::clear()
{
   h2_sock::clear();
   h2_memory::hook();
   if (fails) delete fails, fails = nullptr;
   stats.clear();
}

h2_inline void h2_case::prev_setup()
{
   failed = warning = false;
   h2_memory::stack::push(filine);
   stats.timecost = h2_now();
}

h2_inline void h2_case::post_cleanup()
{
   stats.timecost = h2_now() - stats.timecost;
   stats.footprint = h2_memory::stack::footprint();
   h2_dnses::clear(dnses);
   h2_stubs::clear(stubs);
   failing(h2_mocks::clear(mocks, true), true, O.continue_assert);
   failing(h2_memory::stack::pop(), true, O.continue_assert);
}

h2_inline void h2_case::failing(h2_fail* fail, bool defer, bool append)
{
   if (fail) {
      if (fail->warning) warning = true;
      else failed = true;
      if (fails && !append) delete fail;
      else h2_fail::append_subling(fails, fail);
      if (!defer && !fail->warning) ::longjmp(fail_hole, 1);
   }
}
