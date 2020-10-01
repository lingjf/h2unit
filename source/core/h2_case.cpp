
h2_inline h2_case::h2_case(const char* name_, int status_, const char* file_, int lino_)
  : name(name_), file(file_), lino(lino_), status(status_) {}

h2_inline void h2_case::clear()
{
   if (fails) delete fails;
   fails = nullptr;
   sock = nullptr;
   asserts = 0;
}

h2_inline void h2_case::prev_setup()
{
   status = passed;
   h2_memory::stack::push(file, lino);
}

h2_inline void h2_case::post_cleanup()
{
   h2_stdio::capture_cancel();
   if (sock) delete sock;
   dnses.clear();
   stubs.clear();
   h2_fail* fail = mocks.clear(true);
   footprint = h2_memory::stack::footprint();
   // should memory assert stats into assert count ?
   h2_fail::append_subling(fail, h2_memory::stack::pop());

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

h2_inline h2_case::cleaner::cleaner(h2_case* c) : thus(c) { thus->post_setup(); }
h2_inline h2_case::cleaner::~cleaner() { thus->prev_cleanup(); }
