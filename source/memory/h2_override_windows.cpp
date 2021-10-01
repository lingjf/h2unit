// https://github.com/microsoft/mimalloc
// https://github.com/gperftools/gperftools

struct h2_override_platform {
   h2_stubs stubs;

   // windows specific free_base, free_dbg called by CRT internal functions or operator delete
   // windows specific _msize, _expand
   // A MS CRT "internal" function, implemented using _calloc_impl
   // obscured: _aligned_offset_malloc, _aligned_realloc, _aligned_recalloc, _aligned_offset_realloc, _aligned_offset_recalloc, _malloca, _freea ,_recalloc

   static void _free_base(void* ptr) { h2_override::free(ptr); }
   static void* _expand(void* memblock, size_t size) { return NULL; }
   // When _DEBUG _CRTDBG_MAP_ALLOC (default undefined) is defined CRT maps all to _*_dbg, bug CRT Debug version enabled.
#ifndef NDEBUG
   static void _free_dbg(void* userData, int blockType) { h2_override::free(userData); }
   static void* _malloc_dbg(size_t size, int blockType, const char* filename, int linenumber) { return h2_override::malloc(size); }
   static void* _realloc_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return h2_override::realloc(userData, newSize); }
   static void* _calloc_dbg(size_t num, size_t size, int blockType, const char* filename, int linenumber) { return h2_override::calloc(num, size); }
   static size_t _msize_dbg(void* userData, int blockType) { return h2_override::size(userData); }
   static void* _expand_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return NULL; }
#endif
   static void* _aligned_malloc(size_t size, size_t alignment) { return h2_override::aligned_alloc(size, alignment); }
   static void _aligned_free(void* memblock) { h2_override::free(memblock); }

   static char* _strdup(char* s)
   {
      char* ret = (char*)h2_override::malloc(strlen(s) + 1);
      if (ret) strcpy(ret, s);
      return ret;
   }

   void set()
   {
      stubs.add((void*)::_free_base, (void*)_free_base, {__FILE__, __LINE__, "_free_base"});
      stubs.add((void*)::_msize, (void*)h2_override::size, {__FILE__, __LINE__, "_msize"});
      stubs.add((void*)::_expand, (void*)_expand, {__FILE__, __LINE__, "_expand"});
#ifndef NDEBUG
      stubs.add((void*)::_free_dbg, (void*)_free_dbg, {__FILE__, __LINE__, "_free_dbg"});
      // stubs.add((void*)::_malloc_dbg, (void*)_malloc_dbg, {__FILE__, __LINE__, "_malloc_dbg"});
      // stubs.add((void*)::_realloc_dbg, (void*)_realloc_dbg, {__FILE__, __LINE__, "_realloc_dbg"});
      // stubs.add((void*)::_calloc_dbg, (void*)_calloc_dbg, {__FILE__, __LINE__, "_calloc_dbg"});
      // stubs.add((void*)::_expand_dbg, (void*)_expand_dbg, {__FILE__, __LINE__, "_expand_dbg"});
#endif
      //// stubs.add((void*)::_calloc_crt, (void*)h2_override::calloc, {__FILE__, __LINE__, "_calloc_crt"});
      stubs.add((void*)::_aligned_malloc, (void*)_aligned_malloc, {__FILE__, __LINE__, "_aligned_malloc"});
      stubs.add((void*)::_aligned_free, (void*)_aligned_free, {__FILE__, __LINE__, "_aligned_free"});
      stubs.add((void*)::_strdup, (void*)_strdup, {__FILE__, __LINE__, "_strdup"});  // strdup call to _strdup
   }

   void reset() { stubs.clear(); }
};
