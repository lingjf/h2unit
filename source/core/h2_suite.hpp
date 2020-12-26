
struct h2_jump {
   static constexpr int st_init = 0, st_does = 1, st_done = 2;
   jmp_buf ctx;
   bool has = false;
   int state = st_init;
};

struct h2_suite {
   const char* name;
   const char* file;
   int lino;
   h2_list x;
   int seq = 0;
   int stats[h2_case::n_st]{0};
   int asserts = 0;
   long long footprint = 0;
   h2_jump jump_setup, jump_cleanup;
   void (*test_code)(h2_suite*, h2_case*);
   h2_list cases;
   h2_stubs stubs;
   h2_mocks mocks;

   h2_suite(const char* name, void (*)(h2_suite*, h2_case*), const char* file, int lino);
   void clear();

   void enumerate();
   void execute(h2_case* c);

   void setup();
   void cleanup();

   struct registor {
      registor(h2_suite* s, h2_case* c);
   };

   struct cleaner : h2_once {
      h2_suite* thus;
      cleaner(h2_suite* s);
      ~cleaner();
   };
};
