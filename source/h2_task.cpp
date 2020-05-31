
inline h2_task::h2_task() : state(0), round(0), status_stats{0}, current_suite(nullptr), current_case(nullptr) {}

inline int h2_task::execute()
{
   state = 10;
   reports.initialize();
   h2_heap::initialize();
   h2_heap::dohook();
   h2_stdio::initialize();
   h2_dns::initialize();
   h2_patch::initialize();

   state = 20;
   for (auto& setup : global_setups) setup();
   h2_list_for_each_entry (s, &h2_directory::I().registered_suites, h2_suite, registered) {
      for (auto& setup : global_suite_setups) setup();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (auto& teardown : global_suite_teardowns) teardown();
   }
   int cases = h2_directory::sort();

   reports.on_task_start(cases);
   for (round = 0; round < O.rounds && !status_stats[h2::h2_case::FAILED]; ++round) {
      h2_list_for_each_entry (s, &h2_directory::I().sorted_suites, h2_suite, sorted) {
         current_suite = s;
         reports.on_suite_start(s);
         for (auto& setup : global_suite_setups) setup();
         s->setup();
         h2_list_for_each_entry (c, &s->sorted_cases, h2_case, sorted) {
            if (0 < O.breakable && O.breakable <= status_stats[h2_case::FAILED]) break;
            current_case = c;
            if (O.filter(s->name, c->name, c->file)) c->status = h2_case::FILTED;
            reports.on_case_start(s, c);
            if (h2_case::INITED == c->status && !O.listing) {
               for (auto& setup : global_case_setups) setup();
               s->execute(c);
               for (auto& teardown : global_case_teardowns) teardown();
            }
            reports.on_case_endup(s, c);
            status_stats[c->status] += 1;
            s->status_stats[c->status] += 1;
         }
         s->cleanup();
         for (auto& teardown : global_suite_teardowns) teardown();
         reports.on_suite_endup(s);
      }
   }
   reports.on_task_endup(status_stats, round);
   for (auto& teardown : global_teardowns) teardown();

   state = 30;
   h2_heap::unhook();
   stubs.clear();
   if (status_stats[h2_case::FAILED] == 0) h2_directory::drop_last_order();

   state = 40;
   return status_stats[h2::h2_case::FAILED];
}
