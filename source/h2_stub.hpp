
struct h2_stub : h2_nohook {
   h2_list x;
   unsigned char thunk[64];
   void *befp, *tofp;
   const char* file;
   int line;

   h2_stub(void* befp_, const char* file_ = nullptr, int line_ = 0);

   void replace(void* tofp_);
   void restore();

   struct temporary_restore : h2_once {
      h2_stub* thus;
      temporary_restore(h2_stub* stub) : thus(stub) { thus->restore(); }
      ~temporary_restore() { thus->replace(thus->tofp); }
   };
};

struct h2_stubs {
   h2_list stubs;
   bool add(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line);
   void clear();
};