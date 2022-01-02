h2_inline h2_suite::h2_suite(const char* filine, const char* file, int line, const char* describe, void (*test_fp_)(h2_suite*, h2_case*)) : h2_test(filine, file, line, describe), test_fp(test_fp_)
{
   memset(cleanup_hole, 0, sizeof(jmp_buf));
   h2_runner::I().suites.push_back(x);
}

h2_inline void h2_suite::clear()
{
   stats.clear();
}

h2_inline void h2_suite::setup()
{
   h2_memory::stack::push(filine);
   stats.timecost = h2_now();
}

h2_inline void h2_suite::cleanup()
{
   stats.timecost = h2_now() - stats.timecost;
   h2_stubs::clear(stubs);
   h2_mocks::clear(mocks, false);
   stats.footprint = h2_memory::stack::footprint();
   h2_memory::stack::pop();
}

h2_inline void h2_suite::enumerate()
{
   test_fp(this, nullptr); /* enumerate case by static local h2_case variable inside of h2_suite_test_CmLn() */
}

h2_inline void h2_suite::test(h2_case* c)
{
   bool uncaught = false;
   h2_exception::I().last_bt.clear();
   c->prev_setup();
   try {
      test_fp(this, c); /* include Setup(); c->post_setup() and c->prev_cleanup(); Cleanup() */
   } catch (...) {
      uncaught = true;
   }
   if (uncaught) c->failing(h2_fail::new_exception("was thrown but uncaught", h2_exception::I().last_type, h2_exception::I().last_bt, O.as_waring_uncaught), true, O.continue_assert);
   c->post_cleanup();
}

h2_inline h2_suite::registor::registor(h2_suite* s, h2_case* c)
{
   static int seq = 0x0fffffff;
   s->cases.push_back(c->x);
   s->seq = c->seq = ++seq;
}

h2_inline h2_suite::cleaner::~cleaner()
{
   static const unsigned char zero[sizeof(jmp_buf)] = {0};
   if (memcmp((const void*)thus->cleanup_hole, (const void*)zero, sizeof(jmp_buf)))
      ::longjmp(thus->cleanup_hole, 1);
}
