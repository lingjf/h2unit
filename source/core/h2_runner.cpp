static inline void drop_last_order() { ::remove(".last_order"); }

static inline void save_last_order(h2_list& suites)
{
   FILE* f = ::fopen(".last_order", "w");
   if (!f) return;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         ::fprintf(f, "  file: %s\n suite: %s\n  case: %s\nstatus: %s\n\n", c->file, ss(s->name), c->name, c->failed ? "failed" : "passed");
   ::fclose(f);
}

static inline void __find_mark(h2_list& suites, char* fileline, char* suitename, char* casename, bool failed)
{
   static int seq = INT_MIN / 4;
   int founds = 0;
   h2_list_for_each_entry (s, suites, h2_suite, x)  // full match 3
      if (!strcmp(suitename, ss(s->name)))
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            if (!strcmp(casename, c->name) && !strcmp(fileline, c->file)) {
               s->seq = c->seq = ++seq;
               if (failed) c->last_failed = true;
               ++founds;
            }
   if (founds) return;
   h2_list_for_each_entry (s, suites, h2_suite, x)  // line position change, match2
      if (!strcmp(suitename, ss(s->name)))
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            if (!strcmp(casename, c->name)) {
               s->seq = c->seq = ++seq;
               if (failed) c->last_failed = true;
            }
}

static inline bool __read_line(FILE* f, char* b, size_t n)
{
   if (!::fgets(b, n, f)) return false;
   b[strlen(b) - 1] = '\0'; /* remove \n in save_last_order */
   return true;
}

static inline int mark_last_order(h2_list& suites)
{
   int count = 0;
   char fileline[1024], suitename[1024], casename[1024], status[32], br[32];
   FILE* f = ::fopen(".last_order", "r");
   if (!f) return 0;
   while (__read_line(f, fileline, sizeof(fileline)) && __read_line(f, suitename, sizeof(suitename)) && __read_line(f, casename, sizeof(casename)) && __read_line(f, status, sizeof(status)) && __read_line(f, br, sizeof(br))) {
      __find_mark(suites, fileline + 8, suitename + 8, casename + 8, !strcmp("failed", status + 8));
      count++;
   }
   ::fclose(f);
   return count;
}

h2_inline void h2_runner::shadow()
{
   if (stats.failed == 0)
      drop_last_order();
   else if (last == 0)
      save_last_order(suites);
}

h2_inline void h2_runner::enumerate()
{
   double dots = 0, dps = (h2_console::width() - 11) / (suites.count() + 0.000001);
   if (O.progressing) h2_console::prints("dark gray", "Collecting ");
   h2_list_for_each_entry (s, i, suites, h2_suite, x) {
      if (O.progressing)
         for (; dots <= i * dps; dots++)
            h2_console::prints("dark gray", ".");
      for (auto& setup : global_suite_setups) setup();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (auto& cleanup : global_suite_cleanups) cleanup();
      int unfiltered = 0;
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         if (!(c->filtered = O.filter(ss(s->name), c->name, c->file)))
            unfiltered++;
      if (unfiltered == 0) s->filtered = O.filter(ss(s->name), "", s->file);
   }
   if (O.progressing) h2_console::prints("", "\33[2K\r");
}

template <typename T, int R>
struct shuffle_comparison {
   static int seq(h2_list* a, h2_list* b)
   {
      return (h2_list_entry(a, T, x)->seq - h2_list_entry(b, T, x)->seq) * R;
   }
   static int name(h2_list* a, h2_list* b)
   {
      return strcasecmp(ss(h2_list_entry(a, T, x)->name), ss(h2_list_entry(b, T, x)->name)) * R;
   }
   static int file(h2_list* a, h2_list* b)
   {
      return strcasecmp(h2_list_entry(a, T, x)->file, h2_list_entry(b, T, x)->file) * R;
   }
};

h2_inline void h2_runner::shuffle()
{
   ::srand(::clock());
   last = mark_last_order(suites);

   int (*suite_cmp)(h2_list*, h2_list*) = shuffle_comparison<h2_suite, 1>::seq;
   int (*case_cmp)(h2_list*, h2_list*) = shuffle_comparison<h2_case, 1>::seq;

   if (last == 0) {
      if (O.shuffle_cases & ShuffleRandom)
         h2_list_for_each_entry (s, suites, h2_suite, x)
            h2_list_for_each_entry (c, s->cases, h2_case, x)
               s->seq = c->seq = ::rand();

      if (O.shuffle_cases & ShuffleReverse) {
         suite_cmp = shuffle_comparison<h2_suite, -1>::seq;
         case_cmp = shuffle_comparison<h2_case, -1>::seq;
         if (O.shuffle_cases & ShuffleName) {
            suite_cmp = shuffle_comparison<h2_suite, -1>::name;
            case_cmp = shuffle_comparison<h2_case, -1>::name;
         } else if (O.shuffle_cases & ShuffleFile) {
            suite_cmp = shuffle_comparison<h2_suite, -1>::file;
            case_cmp = shuffle_comparison<h2_case, -1>::file;
         }
      } else {
         if (O.shuffle_cases & ShuffleName) {
            suite_cmp = shuffle_comparison<h2_suite, 1>::name;
            case_cmp = shuffle_comparison<h2_case, 1>::name;
         } else if (O.shuffle_cases & ShuffleFile) {
            suite_cmp = shuffle_comparison<h2_suite, 1>::file;
            case_cmp = shuffle_comparison<h2_case, 1>::file;
         }
      }
   }

   if (last || O.shuffle_cases) {
      suites.sort(suite_cmp);
      h2_list_for_each_entry (s, suites, h2_suite, x)
         s->cases.sort(case_cmp);
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

   for (auto& setup : global_setups) setup();
   enumerate();

   h2_report::I().on_runner_start(this);
   for (rounds = 0; rounds < O.run_rounds; ++rounds) {
      shuffle();
      h2_list_for_each_entry (s, suites, h2_suite, x) {
         current_suite = (void*)s;
         h2_report::I().on_suite_start(s);
         for (auto& setup : global_suite_setups) setup();
         s->setup();
         h2_list_for_each_entry (c, s->cases, h2_case, x) {
            if ((0 < O.break_after_fails && O.break_after_fails <= stats.failed) || (O.only_last_failed && !c->last_failed)) c->ignored = true;
            if (c->ignored)
               stats.ignored++, s->stats.ignored++;
            else if (c->filtered)
               stats.filtered++, s->stats.filtered++;
            else if (c->todo)
               stats.todo++, s->stats.todo++;

            current_case = (void*)c;
            h2_report::I().on_case_start(s, c);
            if (!O.list_cases.size() && !c->todo && !c->filtered && !c->ignored) {
               for (auto& setup : global_case_setups) setup();
               s->test(c);
               for (auto& cleanup : global_case_cleanups) cleanup();
               c->failed ? (stats.failed++, s->stats.failed++) : (stats.passed++, s->stats.passed++);
            }
            h2_report::I().on_case_endup(s, c);
            c->clear();
         }
         s->cleanup();
         for (auto& cleanup : global_suite_cleanups) cleanup();
         h2_report::I().on_suite_endup(s);
         s->clear();
      }
      shadow();
   }
   h2_report::I().on_runner_endup(this);
   for (auto& cleanup : global_cleanups) cleanup();

   h2_stubs::clear(stubs);
   h2_mocks::clear(mocks, false);
   h2_memory::finalize();
   return O.quit_exit_code ? stats.failed : 0;
}

h2_inline void h2_runner::stub(void* srcfp, void* dstfp, const char* srcfn, const char* file)
{
   if (!srcfp || !dstfp) return;
   if (h2_runner::I().current_case)
      h2_stubs::add(((h2_case*)h2_runner::I().current_case)->stubs, srcfp, dstfp, srcfn, file);
   else if (h2_runner::I().current_suite)
      h2_stubs::add(((h2_suite*)h2_runner::I().current_suite)->stubs, srcfp, dstfp, srcfn, file);
   else
      h2_stubs::add(h2_runner::I().stubs, srcfp, dstfp, srcfn, file);
}

h2_inline void h2_runner::unstub(void* srcfp)
{
   if (!srcfp) return;
   if (h2_runner::I().current_case)
      h2_stubs::clear(((h2_case*)h2_runner::I().current_case)->stubs, srcfp);
   else if (h2_runner::I().current_suite)
      h2_stubs::clear(((h2_suite*)h2_runner::I().current_suite)->stubs, srcfp);
   else
      h2_stubs::clear(h2_runner::I().stubs, srcfp);
}

h2_inline void h2_runner::mock(void* mocker)
{
   if (h2_runner::I().current_case)
      h2_mocks::add(((h2_case*)h2_runner::I().current_case)->mocks, mocker);
   else if (h2_runner::I().current_suite)
      h2_mocks::add(((h2_suite*)h2_runner::I().current_suite)->mocks, mocker);
   else
      h2_mocks::add(h2_runner::I().mocks, mocker);
}

h2_inline void h2_runner::failing(h2_fail* fail)
{
   if (!fail) return;
   if (O.debugger_trap) h2_debugger::trap();
   if (h2_runner::I().current_case) ((h2_case*)h2_runner::I().current_case)->failing(fail, O.continue_assert, true);
}

h2_inline void h2_runner::asserts()
{
   if (h2_runner::I().current_case) ((h2_case*)h2_runner::I().current_case)->stats.asserts += 1;
   if (h2_runner::I().current_suite) ((h2_suite*)h2_runner::I().current_suite)->stats.asserts += 1;
   h2_runner::I().stats.asserts += 1;
}
