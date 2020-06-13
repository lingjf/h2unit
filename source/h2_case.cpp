
h2_inline void h2_case::prev_setup()
{
   status = passed;
   h2_heap::stack::push(file, line);
}

h2_inline void h2_case::post_cleanup()
{
   h2_stdio::capture_cancel();
   if (sock) delete sock;
   dnses.clear();
   stubs.clear();
   h2_fail* fail = mocks.clear();
   // should memory check stats into check_count ?
   h2_fail::append_subling(fail, h2_heap::stack::pop());

   if (!fail) return;
   if (status != failed)
      h2_fail::append_subling(fails, fail);
   else
      delete fail;
   status = failed;
}

h2_inline void h2_case::do_fail(h2_fail* fail, bool defer)
{
   status = failed;
   h2_fail::append_subling(fails, fail);
   if (!defer) ::longjmp(jump, 1);
}
