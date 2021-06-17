
struct h2_stubs {
   h2_list stubs;
   bool add(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* file, int line);
   void clear(void* origin_fp);
   void clear();
};

struct h2_stub_temporary_restore : h2_once {
   unsigned char saved[32];
   void* origin_fp;
   h2_stub_temporary_restore(void* origin_fp);
   ~h2_stub_temporary_restore();
};
