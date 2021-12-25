static inline void drop_last_order() { ::remove(".last_order"); }

static inline void save_last_order(h2_list& suites)
{
   FILE* f = ::fopen(".last_order", "w");
   if (!f) return;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         ::fprintf(f, " file: %s\nsuite: %s\n case: %s\n test: %s\n\n", c->file, s->name, c->name, c->failed ? "failed" : "passed");
   ::fclose(f);
}

static inline void __find_case(h2_list& suites, char* filename, char* suitename, char* casename, bool failed)
{
   static int seq = 0;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      if (!strcmp(suitename, s->name))
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            if (!strcmp(casename, c->name) && !strcmp(filename, c->file)) {
               s->seq = c->seq = ++seq;
               if (failed) c->last_failed = true;
            }
}

static inline bool fgets(FILE* f, char* b)
{
   if (!::fgets(b, 512, f)) return false;
   b[strlen(b) - 1] = '\0'; /* remove \n in save_last_order */
   return true;
}

static inline int sequence_last_order(h2_list& suites)
{
   int count = 0;
   char filename[512], suitename[512], casename[512], test[512], br[512];
   FILE* f = ::fopen(".last_order", "r");
   if (!f) return 0;
   while (fgets(f, filename) && fgets(f, suitename) && fgets(f, casename) && fgets(f, test) && fgets(f, br)) {
      __find_case(suites, filename + 7, suitename + 7, casename + 7, !strcmp("failed", test + 7));
      count++;
   }
   ::fclose(f);
   return count;
}

h2_inline void h2_runner::enumerate()
{
   double dots = 0, dps = (h2_console::width() - 11) / (suites.count() + 0.000001);
   if (O.progressing) h2_console::prints("dark gray", "Collecting ");
   h2_list_for_each_entry (s, i, suites, h2_suite, x) {
      if (O.progressing)
         for (; dots <= i * dps; ++dots) h2_console::prints("dark gray", ".");

      for (int i = 0; global_suite_setups[i]; ++i) global_suite_setups[i]();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (int i = 0; global_suite_cleanups[i]; ++i) global_suite_cleanups[i]();
   }
   if (O.progressing) h2_console::prints("", "\33[2K\r");
}

h2_inline void h2_runner::filter()
{
   h2_list_for_each_entry (s, i, suites, h2_suite, x) {
      h2_filter_suite(s);
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         h2_filter_case(s, c);
   }
}

struct shuffle_comparison {
   static int seq(h2_list* a, h2_list* b)
   {
      return h2_list_entry(a, h2_test, x)->seq - h2_list_entry(b, h2_test, x)->seq;
   }
   static int name(h2_list* a, h2_list* b)
   {
      return strcasecmp(h2_list_entry(a, h2_test, x)->name, h2_list_entry(b, h2_test, x)->name);
   }
   static int file(h2_list* a, h2_list* b)
   {
      int t = strcasecmp(h2_list_entry(a, h2_test, x)->file, h2_list_entry(b, h2_test, x)->file);
      return t != 0 ? t : h2_list_entry(a, h2_test, x)->line - h2_list_entry(b, h2_test, x)->line;
   }
   static int cmp(h2_list* a, h2_list* b)
   {
      int reverse = O.shuffles & ShuffleReverse ? -1 : 1;
      if (O.shuffles & ShuffleName) return name(a, b) * reverse;
      if (O.shuffles & ShuffleFile) return file(a, b) * reverse;
      return seq(a, b) * reverse;  // Random or Code Declare
   }
};

h2_inline void h2_runner::shuffle()
{
   if ((lasts = sequence_last_order(suites))) {
      suites.sort(shuffle_comparison::seq);
      h2_list_for_each_entry (s, suites, h2_suite, x)
         s->cases.sort(shuffle_comparison::seq);
      return;  // run in last order if last failed
   }

   if (O.shuffles) {
      ::srand(::clock());
      if (O.shuffles & ShuffleRandom)
         h2_list_for_each_entry (s, suites, h2_suite, x)
            h2_list_for_each_entry (c, s->cases, h2_case, x)
               s->seq = c->seq = ::rand();

      suites.sort(shuffle_comparison::cmp);
      h2_list_for_each_entry (s, suites, h2_suite, x)
         s->cases.sort(shuffle_comparison::cmp);
   }
}

h2_inline int h2_runner::main(int argc, const char** argv)
{
#if defined _MSC_VER
   SetConsoleOutputCP(65001);  // set console code page to utf-8
   SymInitialize(GetCurrentProcess(), NULL, TRUE);
#endif
   h2_option::I().parse(argc, argv);
   h2_report::initialize();
   h2_memory::initialize();
   h2_exception::initialize();
   h2_stdio::initialize();
   h2_dns::initialize();

   stats.timecost = h2_now();
   for (int i = 0; global_setups[i]; ++i) global_setups[i]();
   enumerate();
   filter();

   h2_report::I().on_runner_start(this);
   for (rounds = 0; rounds < O.run_rounds; ++rounds) {
      shuffle();
      h2_list_for_each_entry (s, suites, h2_suite, x) {
         current_suite = s;
         h2_report::I().on_suite_start(s);
         for (int i = 0; global_suite_setups[i]; ++i) global_suite_setups[i]();
         s->setup();
         h2_list_for_each_entry (c, s->cases, h2_case, x) {
            if ((0 < O.break_after_fails && O.break_after_fails <= stats.failed) || (O.only_last_failed && !c->last_failed)) c->ignored = true;
            if (c->ignored) stats.ignored++, s->stats.ignored++;
            else if (c->filtered) stats.filtered++, s->stats.filtered++;
            else if (c->todo) stats.todo++, s->stats.todo++;

            current_case = c;
            h2_report::I().on_case_start(s, c);
            if (!O.lists && !c->todo && !c->filtered && !c->ignored) {
               for (int i = 0; global_case_setups[i]; ++i) global_case_setups[i]();
               s->test(c);
               for (int i = 0; global_case_cleanups[i]; ++i) global_case_cleanups[i]();
               c->failed ? (stats.failed++, s->stats.failed++) : (stats.passed++, s->stats.passed++);
            }
            h2_report::I().on_case_endup(s, c);
            c->clear();
         }
         s->cleanup();
         for (int i = 0; global_suite_cleanups[i]; ++i) global_suite_cleanups[i]();
         h2_report::I().on_suite_endup(s);
         s->clear();
      }
      if (stats.failed == 0) drop_last_order();
      else if (lasts == 0) save_last_order(suites);
   }
   stats.timecost = h2_now() - stats.timecost;
   h2_report::I().on_runner_endup(this);
   for (int i = 0; global_cleanups[i]; ++i) global_cleanups[i]();

   h2_stubs::clear(stubs);
   h2_mocks::clear(mocks, false);
   h2_memory::finalize();
   return O.quit_exit_code ? stats.failed : 0;
}

h2_inline void h2_runner::stub(void* srcfp, void* dstfp, const char* srcfn, const char* filine)
{
   if (!srcfp || !dstfp) return;
   if (h2_runner::I().current_case) h2_stubs::add(h2_runner::I().current_case->stubs, srcfp, dstfp, srcfn, filine);
   else if (h2_runner::I().current_suite) h2_stubs::add(h2_runner::I().current_suite->stubs, srcfp, dstfp, srcfn, filine);
   else h2_stubs::add(h2_runner::I().stubs, srcfp, dstfp, srcfn, filine);
}

h2_inline void h2_runner::unstub(void* srcfp)
{
   if (!srcfp) return;
   if (h2_runner::I().current_case) h2_stubs::clear(h2_runner::I().current_case->stubs, srcfp);
   else if (h2_runner::I().current_suite) h2_stubs::clear(h2_runner::I().current_suite->stubs, srcfp);
   else h2_stubs::clear(h2_runner::I().stubs, srcfp);
}

h2_inline void h2_runner::mock(void* mocker)
{
   if (h2_runner::I().current_case) h2_mocks::add(h2_runner::I().current_case->mocks, mocker);
   else if (h2_runner::I().current_suite) h2_mocks::add(h2_runner::I().current_suite->mocks, mocker);
   else h2_mocks::add(h2_runner::I().mocks, mocker);
}

h2_inline void h2_runner::failing(h2_fail* fail)
{
   if (!fail) return;
   if (O.debugger_trap && h2_attach_debugger()) h2_raise_trap();
   if (h2_runner::I().current_case) h2_runner::I().current_case->failing(fail, O.continue_assert, true);
}

h2_inline void h2_runner::asserts()
{
   if (h2_runner::I().current_case) h2_runner::I().current_case->stats.asserts += 1;
   if (h2_runner::I().current_suite) h2_runner::I().current_suite->stats.asserts += 1;
   h2_runner::I().stats.asserts += 1;
}
