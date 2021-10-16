static inline void drop_last_order() { ::remove(".last_order"); }

static inline void save_last_order(h2_list& suites)
{
   FILE* f = ::fopen(".last_order", "w");
   if (!f) return;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         ::fprintf(f, "%s\n%s\n%d\n", ss(s->name), c->name, (int)c->failed);
   ::fclose(f);
}

static inline void __mark(h2_list& suites, char* suitename, char* casename, bool failed)
{
   static int seq = INT_MIN / 4;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      if (!strcmp(suitename, ss(s->name)))
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            if (!strcmp(casename, c->name))
               s->seq = c->seq = ++seq, c->last_failed = failed;
}

static inline int mark_last_order(h2_list& suites)
{
   int count = 0;
   char suitename[1024], casename[1024], failed[32];
   FILE* f = ::fopen(".last_order", "r");
   if (!f) return 0;
   while (::fgets(suitename, sizeof(suitename), f) && ::fgets(casename, sizeof(casename), f) && ::fgets(failed, sizeof(failed), f)) {
      suitename[strlen(suitename) - 1] = '\0'; /* remove \n in save_last_order */
      casename[strlen(casename) - 1] = '\0';
      failed[strlen(failed) - 1] = '\0';
      __mark(suites, suitename, casename, !!atoi(failed));
      count++;
   }
   ::fclose(f);
   return count;
}

struct h2_compare_wrapper {
   static int suite_cmp(h2_list* a, h2_list* b)
   {
      return h2_list_entry(a, h2_suite, x)->seq - h2_list_entry(b, h2_suite, x)->seq;
   }
   static int case_cmp(h2_list* a, h2_list* b)
   {
      return h2_list_entry(a, h2_case, x)->seq - h2_list_entry(b, h2_case, x)->seq;
   }
};

h2_inline void h2_runner::shuffle()
{
   last = mark_last_order(suites);
   ::srand(::clock());
   if (O.shuffle_cases && last == 0)
      h2_list_for_each_entry (s, suites, h2_suite, x)
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            s->seq = c->seq = ::rand();

   suites.sort(h2_compare_wrapper::suite_cmp);
   h2_list_for_each_entry (s, suites, h2_suite, x)
      s->cases.sort(h2_compare_wrapper::case_cmp);
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
   int cases = 0, i = 0;
   if (O.progressing) ::printf("enumerating...");
   h2_list_for_each_entry (s, suites, h2_suite, x) {
      for (auto& setup : global_suite_setups) setup();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (auto& cleanup : global_suite_cleanups) cleanup();
      int unfiltered = 0;
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         if (!(c->filtered = O.filter(ss(s->name), c->name, c->fs.file, c->fs.line)))
            unfiltered++;
      if (unfiltered == 0) s->filtered = O.filter(ss(s->name), "", s->fs.file, s->fs.line);
      cases += s->cases.count();
      if (O.progressing && 10 * i + i * i < cases && i < (int)h2_shell::I().cww - 20) i += ::printf(".");
   }
   if (O.progressing) ::printf("\33[2K\r");
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
         current_suite = s;
         h2_report::I().on_suite_start(s);
         for (auto& setup : global_suite_setups) setup();
         s->setup();
         h2_list_for_each_entry (c, s->cases, h2_case, x) {
            if ((0 < O.break_after_fails && O.break_after_fails <= stats.failed) || (O.last_failed && !c->last_failed)) c->ignored = true;
            if (c->ignored)
               stats.ignored++, s->stats.ignored++;
            else if (c->filtered)
               stats.filtered++, s->stats.filtered++;
            else if (c->todo)
               stats.todo++, s->stats.todo++;

            current_case = c;
            h2_report::I().on_case_start(s, c);
            if (!O.list_cases && !c->todo && !c->filtered && !c->ignored) {
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

   stubs.clear();
   mocks.clear(false);
   h2_memory::finalize();
   return O.verbose >= 6 ? stats.failed : 0;
}
