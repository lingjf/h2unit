
h2_inline bool h2_mocks::add(void* mock)
{
   h2_mocker_base* mocker = (h2_mocker_base*)mock;
   h2_list_for_each_entry (p, mocks, h2_mocker_base, x)
      if (p == mocker) return false;
   mocks.push(mocker->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear(bool check)
{
   h2_fail* fails = nullptr;
   h2_list_for_each_entry (p, mocks, h2_mocker_base, x) {
      if (check) h2_fail::append_subling(fails, p->check());
      p->reset();
      p->x.out();
   }
   return fails;
}
