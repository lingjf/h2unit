
static inline void save_last_order(h2_list& suites)
{
   h2_with f(fopen(".last_order", "w"));
   if (!f.f) return;
   h2_list_for_each_entry (s, suites, h2_suite, x)
      h2_list_for_each_entry (c, s->cases, h2_case, x)
         fprintf(f.f, "%s\n%s\n", s->name, c->name);
}

static inline void __mark_seq(h2_list& suites, char* suitename, char* casename)
{
   static int seq = INT_MIN / 4;

   h2_list_for_each_entry (s, suites, h2_suite, x)
      if (!strcmp(suitename, s->name))
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            if (!strcmp(casename, c->name))
               s->seq = c->seq = ++seq;
}

static inline int mark_last_order(h2_list& suites)
{
   int count = 0;
   char suitename[512], casename[512];
   h2_with f(fopen(".last_order", "r"));
   if (!f.f) return 0;
   while (1 == fscanf(f.f, "%[^\n]\n", suitename) && 1 == fscanf(f.f, "%[^\n]\n", casename)) {
      __mark_seq(suites, suitename, casename);
      count++;
   }
   return count;
}

static inline void drop_last_order()
{
   ::remove(".last_order");
}

h2_inline void h2_task::sort()
{
   int last = mark_last_order(suites);
   srand(h2_now());
   if (O.shuffle && last == 0)
      h2_list_for_each_entry (s, suites, h2_suite, x)
         h2_list_for_each_entry (c, s->cases, h2_case, x)
            s->seq = c->seq = rand();

   suites.sort([](h2_list* a, h2_list* b) {
      return h2_list_entry(a, h2_suite, x)->seq - h2_list_entry(b, h2_suite, x)->seq;
   });
   h2_list_for_each_entry (s, suites, h2_suite, x) {
      s->cases.sort([](h2_list* a, h2_list* b) {
         return h2_list_entry(a, h2_case, x)->seq - h2_list_entry(b, h2_case, x)->seq;
      });
   }

   if (O.shuffle && last == 0)
      save_last_order(suites);
}

h2_inline void h2_task::enumerate()
{
   h2_list_for_each_entry (s, suites, h2_suite, x) {
      for (auto& setup : global_suite_setups) setup();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (auto& teardown : global_suite_teardowns) teardown();
   }
}

h2_inline int h2_task::execute()
{
   h2_report::initialize();
   h2_memory::initialize();
   h2_patch::initialize();
   h2_stdio::initialize();
   h2_dns::initialize();

   for (auto& setup : global_setups) setup();
   enumerate();
   h2_report::I().on_task_start(this);
   for (rounds = 0; rounds < O.rounds && !stats[h2::h2_case::failed]; ++rounds) {
      sort();
      h2_list_for_each_entry (s, suites, h2_suite, x) {
         current_suite = s;
         h2_report::I().on_suite_start(s);
         for (auto& setup : global_suite_setups) setup();
         s->setup();
         h2_list_for_each_entry (c, s->cases, h2_case, x) {
            if (0 < O.breakable && O.breakable <= stats[h2_case::failed]) break;
            current_case = c;
            if (O.filter(s->name, c->name, c->file)) c->status = h2_case::filtered;
            h2_report::I().on_case_start(s, c);
            if (h2_case::initial == c->status && !O.listing) {
               for (auto& setup : global_case_setups) setup();
               s->execute(c);
               for (auto& teardown : global_case_teardowns) teardown();
            }
            h2_report::I().on_case_endup(s, c);
            stats[c->status] += 1;
            s->stats[c->status] += 1;
            c->clear();
         }
         s->cleanup();
         for (auto& teardown : global_suite_teardowns) teardown();
         h2_report::I().on_suite_endup(s);
         s->clear();
      }
      if (stats[h2_case::failed] == 0) drop_last_order();
   }
   h2_report::I().on_task_endup(this);
   for (auto& teardown : global_teardowns) teardown();

   stubs.clear();
   mocks.clear(false);
   h2_memory::finalize();

   return stats[h2::h2_case::failed];
}
