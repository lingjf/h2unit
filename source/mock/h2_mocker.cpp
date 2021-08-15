
h2_inline h2_row h2_mocker_base::argument(int seq)
{
   h2_row row;
   row += gray("(");
   for (int i = 0; i < argument_types.size(); ++i) {
      if (i) row += gray(", ");
      if (seq == i)
         row += color(argument_types[i], "red,bold");
      else
         row.push_back(argument_types[i]);
   }
   row += gray(")");
   return row;
}

h2_inline h2_row h2_mocker_base::signature()
{
   return "MOCK" + gray("<") + delta(return_type, "cyan") + " " + delta(class_function, "green") + argument() + gray(">");
}

h2_inline void h2_mocker_base::mock()
{
   x.out();
   h2_mock_g(this);
   h2_stub_g(srcfp, dstfp, class_function, file, line);
}

h2_inline h2_fail* h2_mocker_base::times_check()
{
   h2_fail* fails = nullptr;
   for (size_t i = 0; i < checkin_array.size(); ++i) {
      h2_fail* fail = checkin_array[i].check(class_function, i, checkin_array.size(), nullptr, 0);
      if (fail) fail->seqno = i;
      h2_fail::append_subling(fails, fail);
      h2_assert_g();
   }
   if (!fails) return nullptr;
   h2_fail* fail = h2_fail::new_normal(signature(), file, line);
   h2_fail::append_child(fail, fails);
   return fail;
}
