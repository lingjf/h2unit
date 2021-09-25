h2_inline h2_timer::h2_timer(int ms_, const char* file_, int line_) : file(file_), line(line_), cpu_ms(ms_)
{
   start = ::clock();
}

h2_inline h2_timer::~h2_timer()
{
   h2_assert_g();
   double delta = (::clock() - start) * 1000.0 / CLOCKS_PER_SEC;
   if (cpu_ms < delta) {
      h2_sentence st = "performance expect < ";
      st.printf("green", "%d", cpu_ms).printf("", " ms, but actually cost ").printf("red", "%d", (int)delta).printf("", " ms");
      h2_fail_g(h2_fail::new_normal(st, file, line));
   }
}
