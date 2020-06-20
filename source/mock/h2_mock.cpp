
h2_inline void h2_mock::mock()
{
   x.out();
   h2_mock_g(this);
   h2_stub_g(origin_fp, substitute_fp, origin_fn, file, line);
}

h2_inline h2_fail* h2_mock::times_check()
{
   h2_fail* fail = nullptr;
   for (auto& c : checkin_array) {
      h2_fail::append_subling(fail, c.check(origin_fn, file, line));
      h2_check_g();
   }
   return fail;
}

h2_inline bool h2_mocks::add(h2_mock* mock)
{
   h2_list_for_each_entry (p, mocks, h2_mock, x)
      if (p == mock) return false;
   mocks.push(mock->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear(bool check)
{
   h2_fail* fail = nullptr;
   h2_list_for_each_entry (p, mocks, h2_mock, x) {
      if (check) h2_fail::append_subling(fail, p->times_check());
      p->reset();
      p->x.out();
   }
   return fail;
}
