
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

h2_inline void h2_case::post_cleanup(const h2_string& ex)
{
   h2_stdio::capture_cancel();
   if (sock) delete sock;
   dnses.clear();
   stubs.clear();
   h2_fail* fail = mocks.clear(true);
   footprint = h2_memory::stack::footprint();
   h2_fail::append_subling(fail, h2_memory::stack::pop());
   // should memory assert stats into assert count ?

   if (status == failed) {
      if (fail) delete fail;
      return;
   }
   if (!ex.empty()) {
      h2_fail::append_subling(fails, h2_fail::new_normal({"Uncaught Exception : ", ex}));
      if (fail) delete fail;
      status = failed;
      return;
   }
   if (fail) {
      h2_fail::append_subling(fails, fail);
      status = failed;
      return;
   }
}

h2_inline void h2_case::do_fail(h2_fail* fail, bool defer)
{
   status = failed;
   h2_fail::append_subling(fails, fail);
   if (!defer) ::longjmp(ctx, 1);
}
