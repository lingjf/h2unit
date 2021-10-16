struct h2_stubs {
   h2_list stubs;
   bool add(void* srcfp, void* dstfp, const h2_fs& fs);
   void clear(void* srcfp);
   void clear();
};
