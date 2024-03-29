h2_inline h2_line h2_mocker_base::argument(int seq, const char* def) const
{
   h2_line t;
   for (int i = 0; i < (int)argument_types.size(); ++i)
      t += gray(comma_if(i)) + color(argument_types[i], seq == i ? "red,bold" : def);
   return gray("(") + t + gray(")");
}

h2_inline h2_line h2_mocker_base::signature() const
{
   return "MOCK" + gray("<") + delta(return_type, "cyan") + " " + delta(srcfn, "green") + argument(-1, "cyan") + gray(">");
}

h2_inline void h2_mocker_base::mock()
{
   x.out();
   h2_runner::mock(this);
   h2_runner::stub(srcfp, dstfp, srcfn, filine);
}

h2_inline h2_fail* h2_mocker_base::check() const
{
   h2_fail* fails = nullptr;
   for (size_t i = 0; i < checkin_array.size(); ++i) {
      h2_fail* fail = checkin_array[i].check(i, checkin_array.size(), srcfn);
      if (fail) fail->seqno = (int)i;
      h2_fail::append_subling(fails, fail);
      h2_runner::asserts();
   }
   if (!fails) return nullptr;
   h2_fail* fail = h2_fail::new_normal(signature(), filine);
   h2_fail::append_child(fail, fails);
   return fail;
}

h2_inline void h2_mocker_base::failing(h2_fail* fail, int checkin_offset) const
{
   fail->foreach([this, checkin_offset](h2_fail* f, size_t, size_t) {
      f->explain += gray("on ") + (srcfn + argument(f->seqno));
      if (1 < checkin_array.size()) f->explain += gray(" when ") + index_th((size_t)checkin_offset) + " " + color(checkin_array[checkin_offset].expr, "cyan");
   });
   h2_fail* fails = h2_fail::new_normal(signature(), filine);
   h2_fail::append_child(fails, fail);
   h2_runner::failing(fails);
}
