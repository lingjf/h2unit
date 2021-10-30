struct h2_stubs {
   static bool add(h2_list& stubs, void* srcfp, void* dstfp, const char* srcfn, const char* file);
   static void clear(h2_list& stubs, void* srcfp);
   static void clear(h2_list& stubs);
};
