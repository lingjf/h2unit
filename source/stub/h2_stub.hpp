
struct h2_stubs {
   h2_list stubs;
   bool add(void* srcfp, void* dstfp, const char* srcfn, const char* file, int line);
   void clear(void* srcfp);
   void clear();
};

struct h2_stub_temporary_restore : h2_once {
   unsigned char current_opcode[32];
   void* srcfp;
   h2_stub_temporary_restore(void* srcfp);
   ~h2_stub_temporary_restore();
};
