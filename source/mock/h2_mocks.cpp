
h2_inline bool h2_mocks::add(void* mock)
{
   h2_mockee* mockee = (h2_mockee*)mock;
   h2_list_for_each_entry (p, mocks, h2_mockee, x)
      if (p == mockee) return false;
   mocks.push(mockee->x);
   return true;
}

h2_inline h2_fail* h2_mocks::clear(bool check)
{
   h2_fail* fails = nullptr;
   h2_list_for_each_entry (p, mocks, h2_mockee, x) {
      if (check) h2_fail::append_subling(fails, p->times_check());
      p->reset();
      p->x.out();
   }
   return fails;
}
