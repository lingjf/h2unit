struct h2_suite : h2_test {
   void (*test_code)(h2_suite*, h2_case*);
   jmp_buf cleanup_hole;
   h2_list cases;

   h2_suite(const char* filine, const char* file, int line, const char* describe, void (*)(h2_suite*, h2_case*));
   void clear();
   bool absent() const { return !describe; }  // nullptr describe means no SUITE wrapper (CASE/TODO ...)

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
