struct h2_exempt {
   h2_singleton(h2_exempt);
   void* fps[10000];
   int nfp = 0;
   static void setup();
   static void add_by_fp(void* fp);
   static void add_by_name(const char* fn);
};
