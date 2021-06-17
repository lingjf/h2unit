
static inline void save_last_order(h2_list& suites)
{
   FILE* f = ::fopen(".last_order", "w");
   if (!f) return;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         ::fprintf(f, "%s\n%s\n%d\n", ss(s->name), c->name, c->status);
   ::fclose(f);
}

static inline void __mark(h2_list& suites, char* suitename, char* casename, int status)
{
   static int seq = INT_MIN / 4;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      if (!strcmp(suitename, ss(s->name)))
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            if (!strcmp(casename, c->name))
               s->seq = c->seq = ++seq, c->last_status = status;
}

static inline int mark_last_order(h2_list& suites)
{
   int count = 0;
   char suitename[1024], casename[1024], status[32];
   FILE* f = ::fopen(".last_order", "r");
   if (!f) return 0;
   while (::fgets(suitename, sizeof(suitename), f) && ::fgets(casename, sizeof(casename), f) && ::fgets(status, sizeof(status), f)) {
      suitename[strlen(suitename) - 1] = '\0';  /* remove \n in save_last_order */
      casename[strlen(casename) - 1] = '\0';
      status[strlen(status) - 1] = '\0';
      __mark(suites, suitename, casename, atoi(status));
      count++;
   }
   ::fclose(f);
   return count;
}

static inline void drop_last_order()
{
   ::remove(".last_order");
}

static int __suite_cmp(h2_list* a, h2_list* b)
{
   return h2_list_entry(a, h2_suite, x)->seq - h2_list_entry(b, h2_suite, x)->seq;
}

static int __case_cmp(h2_list* a, h2_list* b)
{
   return h2_list_entry(a, h2_case, x)->seq - h2_list_entry(b, h2_case, x)->seq;
}

h2_inline void h2_task::shuffle()
{
   last = mark_last_order(suites);
   srand(h2_now());
   if (O.shuffle_order && last == 0)
      h2_list_for_each_entry (s, suites, h2_suite, x)
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            s->seq = c->seq = rand();

   suites.sort(__suite_cmp);
   h2_list_for_each_entry (s, suites, h2_suite, x)
      s->cases.sort(__case_cmp);
}

h2_inline void h2_task::shadow()
{
   if (stats.failed == 0)
      drop_last_order();
   else if (last == 0)
      save_last_order(suites);
}

h2_inline void h2_task::enumerate()
{
   h2_list_for_each_entry (s, suites, h2_suite, x) {
      for (auto& setup : global_suite_setups) setup();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (auto& cleanup : global_suite_cleanups) cleanup();
      int unfiltered = 0;
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         if (!(c->filtered = O.filter(ss(s->name), c->name, c->file, c->line)))
            unfiltered++;
      if (unfiltered == 0) s->filtered = O.filter(ss(s->name), "", s->file, s->line);
   }
}

h2_inline void h2_task::execute()
{
   h2_report::initialize();
   h2_memory::initialize();
   h2_exception::initialize();
   h2_stdio::initialize();
   h2_dns::initialize();

   for (auto& setup : global_setups) setup();
   enumerate();

   h2_report::I().on_task_start(this);
   for (rounds = 0; rounds < O.rounds && !stats.failed; ++rounds) {
      shuffle();
      h2_list_for_each_entry (s, suites, h2_suite, x) {
         current_suite = s;
         h2_report::I().on_suite_start(s);
         for (auto& setup : global_suite_setups) setup();
         s->setup();
         h2_list_for_each_entry (c, s->cases, h2_case, x) {
            if (0 < O.break_after_fails && O.break_after_fails <= stats.failed) break;
            current_case = c;
            h2_report::I().on_case_start(s, c);
            if (!O.list_cases) {
               if (c->todo) {
                  stats.todo++, s->stats.todo++;
               } else if (c->filtered) {
                  stats.filtered++, s->stats.filtered++;
               } else {
                  if (O.only_execute_fails && c->last_status != h2_case::failed)
                     c->status = h2_case::ignored;
                  if (c->status != h2_case::ignored) {
                     for (auto& setup : global_case_setups) setup();
                     s->execute(c);
                     for (auto& cleanup : global_case_cleanups) cleanup();
                  }
                  if (c->status == h2_case::passed) stats.passed++, s->stats.passed++;
                  if (c->status == h2_case::failed) stats.failed++, s->stats.failed++;
                  if (c->status == h2_case::ignored) stats.ignored++, s->stats.ignored++;
               }
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
   h2_report::I().on_task_endup(this);
   for (auto& cleanup : global_cleanups) cleanup();

   stubs.clear();
   mocks.clear(false);
   h2_memory::finalize();
}
