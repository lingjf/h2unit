
inline h2_task::h2_task() : state(0), status_stats{0}, current_suite(nullptr), current_case(nullptr) {}

inline void h2_task::prepare()
{
   state = 10;
   if (O.listing) h2_directory::list_then_exit();
   reports.initialize();
   h2_directory::sort();
   h2_heap::initialize();
   h2_heap::dohook();
   h2_stdio::initialize();
   h2_dns::initialize();
   h2_patch::initialize();
   state = 19;
}

inline int h2_task::finalize()
{
   state = 30;
   h2_heap::unhook();
   stubs.clear();
   if (status_stats[h2_case::FAILED] == 0) h2_directory::drop_last_order();
   state = 39;
   return status_stats[h2::h2_case::FAILED];
}

inline void h2_task::execute()
{
   state = 20;
   reports.on_task_start(h2_directory::count());
   for (auto& setup : global_setups) setup();
   for (auto& s : h2_directory::I().suites) {
      current_suite = s;
      reports.on_suite_start(s);
      for (auto& setup : global_suite_setups) setup();
      s->setup();
      for (auto& c : s->cases()) {
         if (0 < O.breakable && O.breakable <= status_stats[h2_case::FAILED]) break;
         current_case = c;
         reports.on_case_start(s, c);
         for (auto& setup : global_case_setups) setup();
         if (O.filter(s->name, c->name, c->file)) c->status = h2_case::FILTED;
         if (h2_case::INITED == c->status) s->execute(c);
         for (auto& teardown : global_case_teardowns) teardown();
         reports.on_case_endup(s, c);
         status_stats[c->status] += 1;
         s->status_stats[c->status] += 1;
      }
      s->cleanup();
      for (auto& teardown : global_suite_teardowns) teardown();
      reports.on_suite_endup(s);
   }
   for (auto& teardown : global_teardowns) teardown();
   reports.on_task_endup(status_stats);
   state = 29;
}
