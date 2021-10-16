struct h2_stats {
   int passed = 0, failed = 0, todo = 0, filtered = 0, ignored = 0;
   int asserts = 0;
   long long footprint = 0;
   void clear() { passed = 0, failed = 0, todo = 0, filtered = 0, ignored = 0, asserts = 0, footprint = 0; }
};

struct h2_suite {
   h2_fs fs;
   const char* name;
   h2_list x;
   int seq = 0;
   h2_stats stats;
   bool filtered = false;
   jmp_buf ctx;
   void (*test_code)(h2_suite*, h2_case*);
   h2_list cases;
   h2_stubs stubs;
   h2_mocks mocks;

   h2_suite(const char* name, void (*)(h2_suite*, h2_case*), const h2_fs& fs);
   void clear();

   void enumerate();
   void test(h2_case* c);

   void setup();
   void cleanup();

   struct registor {
      registor(h2_suite* s, h2_case* c);
   };

   struct cleaner : h2_once {
      h2_suite* thus;
      cleaner(h2_suite* s) : thus(s) {}
      ~cleaner();
   };
};
