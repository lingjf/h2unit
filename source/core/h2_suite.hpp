struct h2_suite {
   h2_list x;
   const char* file;
   h2_describe describe;
   void (*test_code)(h2_suite*, h2_case*);
   bool filtered = false;
   int seq = 0;
   jmp_buf ctx;
   h2_list cases;
   h2_list stubs;
   h2_list mocks;
   h2_stats stats;

   h2_suite(const char* file, const char* describe, void (*)(h2_suite*, h2_case*));
   void clear();
   bool absent() const { return !describe.desc; }  // nullptr describe means no SUITE wrapper (CASE/TODO ...)

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
