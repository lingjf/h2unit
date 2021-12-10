struct h2_exempt {
   h2_singleton(h2_exempt);
   void* fps[4096]{nullptr};
   static void setup();
   static void add_by_fp(void* fp);
   static void add_by_name(const char* fn);
};
