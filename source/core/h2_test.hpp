struct h2_stats {
   int passed = 0, failed = 0, warning = 0, todo = 0, filtered = 0, ignored = 0;
   int asserts = 0;
   long long footprint = 0;
   long long timecost = 0;
   void clear() { passed = failed = warning = todo = filtered = ignored = 0, asserts = 0, footprint = 0, timecost = 0; }
};

struct h2_test {
   h2_list x;

   const char* filine;
   const char* file;  // base file name
   int line;

   const char* describe;
   const char* name = "";
   const char* tags[64]{nullptr};
   char name_buf[512], tags_buf[512];

   int seq = 0;
   bool filtered = false;
   h2_stats stats;
   h2_list stubs;
   h2_list mocks;
   h2_list dnses;

   h2_test(const char* filine, const char* file, int line, const char* describe);
   bool tagged(const char* tag_pattern) const;
};
