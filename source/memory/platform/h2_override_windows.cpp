// https://github.com/microsoft/mimalloc
// https://github.com/gperftools/gperftools

struct h2_override_platform {
   h2_list stubs;

   // windows specific free_base, free_dbg called by CRT internal functions or operator delete
   // windows specific _msize, _expand
   // A MS CRT "internal" function, implemented using _calloc_impl
   // obscured: _aligned_offset_malloc, _aligned_realloc, _aligned_recalloc, _aligned_offset_realloc, _aligned_offset_recalloc, _malloca, _freea ,_recalloc

   static void _free_base(void* ptr) { h2_override::free(ptr); }
   static void* _expand(void* memblock, size_t size) { return NULL; }
   static void* _aligned_malloc(size_t size, size_t alignment) { return h2_override::aligned_alloc(size, alignment); }
   static void _aligned_free(void* memblock) { h2_override::free(memblock); }
   // When _DEBUG _CRTDBG_MAP_ALLOC (default undefined) is defined CRT maps all to _*_dbg, bug CRT Debug version enabled.
   void set()
   {
      h2_stubs::add(stubs, (void*)::_free_base, (void*)_free_base, "_free_base", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_msize, (void*)h2_override::size, "_msize", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_expand, (void*)_expand, "_expand", H2_FILINE);
      //// h2_stubs::add(stubs,(void*)::_calloc_crt, (void*)h2_override::calloc, "_calloc_crt", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_aligned_malloc, (void*)_aligned_malloc, "_aligned_malloc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_aligned_free, (void*)_aligned_free, "_aligned_free", H2_FILINE);
      h2_stubs::add(stubs, (void*)::_strdup, (void*)h2_override_stdlib::strdup, "_strdup", H2_FILINE);  // strdup call to _strdup
#ifndef NDEBUG
      h2_stubs::add(stubs, (void*)::_free_dbg, (void*)free_dbg, "_free_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_malloc_dbg, (void*)malloc_dbg, "_malloc_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_realloc_dbg, (void*)realloc_dbg, "_realloc_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_calloc_dbg, (void*)calloc_dbg, "_calloc_dbg", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::_expand_dbg, (void*)expand_dbg, "_expand_dbg", H2_FILINE);
#endif
   }
   static void free_dbg(void* userData, int blockType) { h2_override::free(userData); }
   static void* malloc_dbg(size_t size, int blockType, const char* filename, int linenumber) { return h2_override::malloc(size); }
   static void* realloc_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return h2_override::realloc(userData, newSize); }
   static void* calloc_dbg(size_t num, size_t size, int blockType, const char* filename, int linenumber) { return h2_override::calloc(num, size); }
   static size_t msize_dbg(void* userData, int blockType) { return h2_override::size(userData); }
   static void* expand_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return NULL; }

   void reset() { h2_stubs::clear(stubs); }
};
