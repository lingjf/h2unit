h2_inline h2_line h2_mocker_base::argument(int seq, const char* def) const
{
   h2_line t;
   for (int i = 0; i < (int)argument_types.size(); ++i)
      t += (i ? gray(", ") : "") + color(argument_types[i], seq == i ? "red,bold" : def);
   return gray("(") + t + gray(")");
}

h2_inline h2_line h2_mocker_base::signature() const
{
   return "MOCK" + gray("<") + delta(return_type, "cyan") + " " + delta(fs.func, "green") + argument(-1, "cyan") + gray(">");
}

h2_inline void h2_mocker_base::mock()
{
   x.out();
   h2_mock_g(this);
   h2_stub_g(srcfp, dstfp, fs);
}

h2_inline h2_fail* h2_mocker_base::check() const
{
   h2_fail* fails = nullptr;
   for (size_t i = 0; i < checkin_array.size(); ++i) {
      h2_fail* fail = checkin_array[i].check(i, checkin_array.size(), fs);
      if (fail) fail->seqno = (int)i;
      h2_fail::append_subling(fails, fail);
      h2_assert_g();
   }
   if (!fails) return nullptr;
   h2_fail* fail = h2_fail::new_normal(signature(), fs);
   h2_fail::append_child(fail, fails);
   return fail;
}
