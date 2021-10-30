h2_inline bool h2_mocks::add(h2_list& mocks, void* mocker)
{
   h2_mocker_base* m = (h2_mocker_base*)mocker;
   h2_list_for_each_entry (p, mocks, h2_mocker_base, x)
      if (p == m) return false;
   mocks.push(m->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear(h2_list& mocks, bool check)
{
   h2_fail* fails = nullptr;
   h2_list_for_each_entry (p, mocks, h2_mocker_base, x) {
      if (check) h2_fail::append_subling(fails, p->check());
      p->reset();
      p->x.out();
   }
   return fails;
}
