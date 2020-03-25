
h2_inline h2_case::h2_case(const char* name_, int todo, const char* file_, int line_)
  : name(name_), file(file_), line(line_), status(todo ? TODOED : INITED), fails(nullptr), sock(nullptr) {}

h2_inline void h2_case::prev_setup() {
   status = PASSED;
   h2_heap::stack::push(file, line);
}

h2_inline void h2_case::post_cleanup() {
   if (sock) delete sock;
   dnses.clear();
   stubs.clear();
   h2_fail* fail = mocks.clear();
   h2_append_x_fail(fail, h2_heap::stack::pop());

   if (!fail) return;
   if (status != FAILED)
      h2_append_x_fail(fails, fail);
   else
      delete fail;
   status = FAILED;
}

h2_inline void h2_case::do_fail(h2_fail* fail) {
   status = FAILED;
   h2_append_x_fail(fails, fail);
   ::longjmp(jump, 1);
}
