
h2_inline h2_perf::h2_perf(long long ms_, const char* file_, int line_) : file(file_), line(line_), ms(ms_)
{
   start = ::clock();
}

h2_inline h2_perf::~h2_perf()
{
   h2_assert_g();
   long long delta = (::clock() - start) * 1000 / CLOCKS_PER_SEC;
   if (ms < delta) {
      h2_row row = "performance expect < ";
      row.printf("green", "%lld", ms).printf("", " ms, but actually cost ").printf("red", "%lld", delta).printf("", " ms");
      h2_fail_g(h2_fail::new_normal(row, file, line));
   }
}
