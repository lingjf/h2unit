
h2_inline h2_case::h2_case(const char* name_, int todo, const char* file_, int line_)
  : name(name_), file(file_), line(line_), status(todo ? TODOED : INITED), sock(nullptr), fails(nullptr) {}

h2_inline void h2_case::prev_setup() {
   status = PASSED;
   h2_heap::stack::push(file, line);
}

h2_inline void h2_case::post_setup() {}

h2_inline void h2_case::prev_cleanup() {}

h2_inline void h2_case::post_cleanup() {
   undo_dns();
   undo_sock();
   undo_stub();
   h2_fail* fail = undo_mock();
   h2_append_x_fail(fail, h2_heap::stack::pop());

   if (fail) {
      if (status != FAILED) {
         h2_append_x_fail(fails, fail);
         status = FAILED;
      } else
         delete fail;
   }
}

h2_inline void h2_case::do_stub(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line) {
   h2_stub* stub = nullptr;
   h2_list_for_each_entry(p, &stubs, h2_stub, x) if (p->befp == befp) {
      stub = p;
      break;
   }

   if (!tofp) { /* unstub */
      if (stub) {
         stub->restore();
         stub->x.out();
         h2_libc::free(stub);
      }
      return;
   }

   if (!stub) {
      stub = new h2_stub(befp, file, line);
      stubs.push(&stub->x);
   }
   stub->replace(tofp);
}

h2_inline void h2_case::undo_stub() {
   h2_list_for_each_entry(p, &stubs, h2_stub, x) {
      p->restore();
      p->x.out();
      delete p;
   }
}

h2_inline bool h2_case::do_mock(h2_mock* mock) {
   h2_list_for_each_entry(p, &mocks, h2_mock, x) if (p == mock) return true;
   do_stub(mock->befp, mock->tofp, mock->befn, "", mock->file, mock->line);
   mocks.push(&mock->x);
   return true;
}

h2_inline h2_fail* h2_case::undo_mock() {
   h2_fail* fail = nullptr;
   h2_list_for_each_entry(p, &mocks, h2_mock, x) {
      h2_append_x_fail(fail, p->times_check());
      p->reset();
      p->x.out();
   }
   return fail;
}

h2_inline void h2_case::do_dns(h2_dns* dns) { dnss.push(&dns->x); }

h2_inline void h2_case::undo_dns() {
   h2_list_for_each_entry(p, &dnss, h2_dns, x) {
      p->x.out();
      p->y.out();
      delete p;
   }
}

h2_inline h2_sock* h2_case::do_sock(h2_sock* sock) {
   if (sock) this->sock = sock;
   return this->sock;
}

h2_inline void h2_case::undo_sock() {
   if (sock) delete sock;
   sock = nullptr;
}

h2_inline void h2_case::do_fail(h2_fail* fail) {
   status = FAILED;
   h2_append_x_fail(fails, fail);
   ::longjmp(jump, 1);
}
