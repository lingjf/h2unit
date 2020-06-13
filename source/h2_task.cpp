
inline int h2_task::execute()
{
   h2_report::initialize();
   h2_heap::initialize();
   h2_heap::dohook();
   h2_stdio::initialize();
   h2_dns::initialize();
   h2_patch::initialize();

   for (auto& setup : global_setups) setup();
   h2_list_for_each_entry (s, h2_directory::I().suites, h2_suite, x) {
      for (auto& setup : global_suite_setups) setup();
      s->setup();
      s->enumerate();
      s->cleanup();
      for (auto& teardown : global_suite_teardowns) teardown();
      suites += 1;
      cases += s->cases.count();
   }
   h2_report::I().on_task_start(this);
   for (rounds = 0; rounds < O.rounds && !stats[h2::h2_case::failed]; ++rounds) {
      h2_directory::sort();
      h2_list_for_each_entry (s, h2_directory::I().suites, h2_suite, x) {
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
         }
         s->cleanup();
         for (auto& teardown : global_suite_teardowns) teardown();
         h2_report::I().on_suite_endup(s);
      }
   }
   h2_report::I().on_task_endup(this);
   for (auto& teardown : global_teardowns) teardown();

   h2_heap::unhook();
   stubs.clear();
   if (stats[h2_case::failed] == 0) h2_directory::drop_last_order();

   return stats[h2::h2_case::failed];
}
