
h2_inline h2_row h2_mocker_base::argument(int seq)
{
   h2_row row;
   for (int i = 0; i < argument_types.size(); ++i)
      row += (i ? gray(", ") : "") + color(argument_types[i], seq == i ? "red,bold" : "cyan");
   return gray("(") + row + gray(")");
}

h2_inline h2_row h2_mocker_base::signature()
{
   return "MOCK" + gray("<") + delta(return_type, "cyan") + " " + delta(srcfn, "green") + argument() + gray(">");
}

h2_inline void h2_mocker_base::mock()
{
   x.out();
   h2_mock_g(this);
   h2_stub_g(srcfp, dstfp, srcfn, file, line);
}

h2_inline h2_fail* h2_mocker_base::check()
{
   h2_fail* fails = nullptr;
   for (size_t i = 0; i < checkin_array.size(); ++i) {
      h2_fail* fail = checkin_array[i].check(srcfn, i, checkin_array.size(), nullptr, 0);
      if (fail) fail->seqno = i;
      h2_fail::append_subling(fails, fail);
      h2_assert_g();
   }
   if (!fails) return nullptr;
   h2_fail* fail = h2_fail::new_normal(signature(), file, line);
   h2_fail::append_child(fail, fails);
   return fail;
}
