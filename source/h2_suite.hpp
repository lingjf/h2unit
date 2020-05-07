
struct h2_suite {
   const char* name;
   const char* file;
   int line;
   long long seq;
   h2_stubs stubs;
   h2_mocks mocks;
   int status_stats[8];
   jmp_buf jump;
   bool jumpable;
   void (*test_code_plus)(h2_suite*, h2_case*);
   std::vector<h2_case*> case_list;
   h2_once enumerate;

   h2_suite(const char* name_, void (*p)(h2_suite*, h2_case*), const char* file_, int line_);

   std::vector<h2_case*>& cases();
   void execute(h2_case* c);

   void setup() {}
   void cleanup();

   struct installer {
      installer(h2_suite* s, h2_case* c)
      {
         static long long seq = INT_MAX;
         s->case_list.push_back(c);
         s->seq = c->seq = ++seq;
      }
   };

   struct cleaner : h2_once {
      h2_suite* thus;
      cleaner(h2_suite* s) : thus(s) {}
      ~cleaner()
      {
         if (thus->jumpable) ::longjmp(thus->jump, 1);
      }
   };
};
