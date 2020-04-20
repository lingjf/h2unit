
h2_inline bool h2_mocks::add(h2_mock* mock) {
   h2_list_for_each_entry(p, &mocks, h2_mock, x) if (p == mock) return false;
   mocks.push(&mock->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear() {
   h2_fail* fail = nullptr;
   h2_list_for_each_entry(p, &mocks, h2_mock, x) {
      h2_fail::append_x(fail, p->times_check());
      p->reset();
      p->x.out();
   }
   return fail;
}
