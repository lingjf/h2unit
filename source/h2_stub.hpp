
struct h2_stubs {
   h2_list stubs;
   bool add(void* befp, void* tofp, const char* befn = "", const char* tofn = "", const char* file = nullptr, int line = 0);
   void clear();
};

struct h2_stub_temporary_restore : h2_once {
   void* befp;
   char current[64];
   h2_stub_temporary_restore(void* befp_);
   ~h2_stub_temporary_restore();
};
