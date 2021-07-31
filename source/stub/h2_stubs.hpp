
struct h2_stubs {
   h2_list stubs;
   bool add(void* srcfp, void* dstfp, const char* srcfn, const char* file, int line);
   void clear(void* srcfp);
   void clear();
};
