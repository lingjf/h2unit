
struct h2_suite {
   typedef void (*P)(h2_suite*, h2_case*);

   const char* name;
   P p;
   std::function<void()> setup, teardown;
   int status[8];
   const char* file;
   int line;

   int enumerates;
   std::vector<h2_case*> case_list;

   h2_suite(const char* name_, P p_, const char* file_, const int line_, int enumerates_)
     : name(name_), p(p_), setup(), teardown(), status{0}, file(file_), line(line_), enumerates(enumerates_) {
      suites().push_back(this);
   }

   std::vector<h2_case*>& cases() {
      if (!enumerates++) p(this, nullptr); /* enumerate cases */
      return case_list;
   }

   static void execute(h2_suite*, h2_case* _case) {
      if (_case) _case->execute();
   }

   /* clang-format off */
   static std::vector<h2_suite*>& suites() { static std::vector<h2_suite*> __; return __; }
   /* clang-format off */
};

static inline void h2_suite_case_g(h2_suite* suite, void* case_) { suite->case_list.push_back((h2_case*)case_); }

static inline void h2_suite_setup_g(h2_suite* suite) {
   if (suite && suite->setup)
      suite->setup(), suite->setup = nullptr;
}

static inline void h2_suite_teardown_g(h2_suite* suite) {
   if (suite && suite->teardown)
      suite->teardown(), suite->teardown = nullptr;
}
