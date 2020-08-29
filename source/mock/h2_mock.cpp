
h2_inline h2_line h2_mock::argvs(int seq)
{
   h2_line line;
   for (int i = 0; i < argv.size(); ++i) {
      line.concat_back({"\033{+dark gray}", comma_if(i), "\033{-dark gray}"});
      if (seq == i) line.push_back("\033{red,bold}");
      line.push_back(argv[i]);
      if (seq == i) line.push_back("\033{reset}");
   }
   return line;
}

h2_inline h2_line h2_mock::signature()
{
   h2_line line = {"MOCK( ", "\033{cyan}", ret, " ", "\033{+green}", func, "\033{-green}", "\033{+dark gray}", "(", "\033{-dark gray}"};
   line.concat_back(argvs());
   line.concat_back({"\033{+dark gray}", ")", "\033{-dark gray}", "\033{reset}", " ) fails"});
   return line;
}

h2_inline void h2_mock::mock()
{
   x.out();
   h2_mock_g(this);
   h2_stub_g(origin_fp, substitute_fp, func, file, line);
}

h2_inline h2_fail* h2_mock::times_check()
{
   h2_fail* fails = nullptr;
   for (size_t i = 0; i < checkin_array.size(); ++i) {
      h2_fail* fail = checkin_array[i].check(func, nullptr, 0);
      if (fail) fail->seqno = i;
      h2_fail::append_subling(fails, fail);
      h2_check_g();
   }
   if (!fails) return nullptr;
   h2_fail* fail = h2_fail::new_normal(signature(), file, line);
   h2_fail::append_child(fail, fails);
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
