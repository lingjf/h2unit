
h2_inline h2_suite::h2_suite(const char* name_, void (*p)(h2_suite*, h2_case*), const char* file_, int line_)
  : name(name_), file(file_), line(line_), seq(0), status_stats{0}, jumpable(false), test_code_plus(p) {
   h2_directory::I().suites.push_back(this);
}

h2_inline void h2_suite::cleanup() {
   stubs.clear();
   mocks.clear();
}

h2_inline std::vector<h2_case*>& h2_suite::cases() {
   if (enumerate) /* enumerate case by static local h2_case variable inside of h2_suite_test_code_plus() */
      test_code_plus(this, nullptr);
   return case_list;
}

h2_inline void h2_suite::execute(h2_case* c) {
   c->prev_setup();
   test_code_plus(this, c); /* include setup(); c->post_setup() and c->prev_cleanup(); cleanup() */
   c->post_cleanup();
}
