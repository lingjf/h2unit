
struct h2_suite {
   const char* name;
   const char* file;
   int line;
   h2_list x;
   int seq = 0;
   int stats[h2_case::statuss]{0};
   int checks = 0;
   jmp_buf jump;
   bool jumpable = false;
   void (*test_code)(h2_suite*, h2_case*);
   h2_list cases;
   h2_stubs stubs;
   h2_mocks mocks;

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
