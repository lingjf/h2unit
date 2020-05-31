
h2_inline h2_suite::h2_suite(const char* name_, void (*test_code_)(h2_suite*, h2_case*), const char* file_, int line_)
  : name(name_), file(file_), line(line_), seq(0), status_stats{0}, jumpable(false), test_code(test_code_)
{
   h2_directory::I().registered_suites.push_back(&registered);
}

h2_inline void h2_suite::cleanup()
{
   stubs.clear();
   mocks.clear();
}

h2_inline void h2_suite::enumerate()
{
   /* enumerate case by static local h2_case variable inside of h2_suite_test_code() */
   test_code(this, nullptr);
}

h2_inline void h2_suite::execute(h2_case* c)
{
   c->prev_setup();
   test_code(this, c); /* include setup(); c->post_setup() and c->prev_cleanup(); cleanup() */
   c->post_cleanup();
}

h2_inline h2_suite::installer::installer(h2_suite* s, h2_case* c)
{
   static int seq = INT_MAX / 2;
   s->registered_cases.push_back(&c->registered);
   s->seq = c->seq = ++seq;
}

h2_inline h2_suite::cleaner::cleaner(h2_suite* s) : thus(s) {}
h2_inline h2_suite::cleaner::~cleaner()
{
   if (thus->jumpable) ::longjmp(thus->jump, 1);
}