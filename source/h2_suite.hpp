
struct h2_suite {
   h2_list registered, sorted;
   const char* name;
   const char* file;
   int line;
   int seq;
   h2_stubs stubs;
   h2_mocks mocks;
   int status_stats[8];
   jmp_buf jump;
   bool jumpable;
   void (*test_code)(h2_suite*, h2_case*);
   h2_list registered_cases, sorted_cases;

   h2_suite(const char* name_, void (*test_code_)(h2_suite*, h2_case*), const char* file_, int line_);

   void enumerate();
   void execute(h2_case* c);

   void setup() {}
   void cleanup();

   struct installer {
      installer(h2_suite* s, h2_case* c);
   };

   struct cleaner : h2_once {
      h2_suite* thus;
      cleaner(h2_suite* s);
      ~cleaner();
   };
};
