
h2_inline h2_suite::h2_suite(const char* name_, void (*test_code_)(h2_suite*, h2_case*), const char* file_, int lino_) : name(name_), file(file_), lino(lino_), test_code(test_code_)
{
   memset(ctx, 0, sizeof(jmp_buf));
   h2_task::I().suites.push_back(x);
}

h2_inline void h2_suite::clear()
{
   stats.clear();
}

h2_inline void h2_suite::setup()
{
   h2_memory::stack::push(file, lino);
}

h2_inline void h2_suite::cleanup()
{
   stubs.clear();
   mocks.clear(false);
   stats.footprint = h2_memory::stack::footprint();
   h2_memory::stack::pop();
}

h2_inline void h2_suite::enumerate()
{
   test_code(this, nullptr); /* enumerate case by static local h2_case variable inside of h2_suite_test_CmLn() */
}

h2_inline void h2_suite::execute(h2_case* c)
{
   h2_string ex;
   c->prev_setup();
   try {
      test_code(this, c); /* include Setup(); c->post_setup() and c->prev_cleanup(); Cleanup() */
   } catch (std::exception& e) {
      ex = e.what();
   } catch (std::string& m) {
      ex = m.c_str();
   } catch (const char* m) {
      ex = m;
   } catch (...) {
      ex = "Unknown exception";
   }
   c->post_cleanup(ex);
}

h2_inline h2_suite::registor::registor(h2_suite* s, h2_case* c)
{
   static int seq = INT_MAX / 4;
   s->cases.push_back(c->x);
   s->seq = c->seq = ++seq;
}

h2_inline h2_suite::cleaner::~cleaner()
{
   static const jmp_buf zero = {0};
   if (memcmp((const void*)thus->ctx, (const void*)zero, sizeof(jmp_buf)))
      ::longjmp(thus->ctx, 1);
}
