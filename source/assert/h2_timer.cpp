h2_inline h2_timer::h2_timer(int ms_, const h2_fs& fs_) : fs(fs_), cpu_ms(ms_)
{
   start = ::clock();
}

h2_inline h2_timer::~h2_timer()
{
   h2_assert_g();
   double delta = (::clock() - start) * 1000.0 / CLOCKS_PER_SEC;
   if (cpu_ms < delta) {
      h2_line line = "performance expect < ";
      line.printf("green", "%d", cpu_ms).printf("", " ms, but actually cost ").printf("red", "%d", (int)delta).printf("", " ms");
      h2_fail_g(h2_fail::new_normal(line, fs));
   }
}
