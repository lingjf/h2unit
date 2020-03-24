
inline void h2_task::prepare() {
   h2_heap::dosegv();
   if (O.listing) h2_directory::list_then_exit();

   logs.init();
   h2_directory::sort();

   h2_heap::stack::root();
   h2_heap::dohook();
}

inline void h2_task::postpare() {
   h2_heap::unhook();
   if (status_stats[h2_case::FAILED] == 0) h2_directory::drop_last_order();
}

inline void h2_task::execute() {
   logs.on_task_start(h2_directory::count());
   for (auto& setup : global_setups) setup();
   for (auto& s : h2_directory::I().suites) {
      logs.on_suite_start(s);
      for (auto& setup : global_suite_setups) setup();
      for (auto& c : s->cases()) {
         if (0 < O.breakable && O.breakable <= status_stats[h2_case::FAILED]) break;
         current_case = c;
         logs.on_case_start(s, c);
         for (auto& setup : global_case_setups) setup();
         if (O.filter(s->name, c->name, c->file)) c->status = h2_case::FILTED;
         if (h2_case::INITED == c->status) s->execute(c);
         for (auto& teardown : global_case_teardowns) teardown();
         logs.on_case_endup(s, c);
         status_stats[c->status] += 1;
         s->status_stats[c->status] += 1;
      }
      for (auto& teardown : global_suite_teardowns) teardown();
      logs.on_suite_endup(s);
   }
   for (auto& teardown : global_teardowns) teardown();
   logs.on_task_endup(status_stats);
}
