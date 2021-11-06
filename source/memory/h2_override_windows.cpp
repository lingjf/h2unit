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
   // When _DEBUG _CRTDBG_MAP_ALLOC (default undefined) is defined CRT maps all to _*_dbg, bug CRT Debug version enabled.
#ifndef NDEBUG
   static void _free_dbg(void* userData, int blockType)
   {
      h2_override::free(userData);
   }
   static void* _malloc_dbg(size_t size, int blockType, const char* filename, int linenumber) { return h2_override::malloc(size); }
   static void* _realloc_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return h2_override::realloc(userData, newSize); }
   static void* _calloc_dbg(size_t num, size_t size, int blockType, const char* filename, int linenumber) { return h2_override::calloc(num, size); }
   static size_t _msize_dbg(void* userData, int blockType) { return h2_override::size(userData); }
   static void* _expand_dbg(void* userData, size_t newSize, int blockType, const char* filename, int linenumber) { return NULL; }
#endif
   static void* _aligned_malloc(size_t size, size_t alignment)
   {
      return h2_override::aligned_alloc(size, alignment);
   }
   static void _aligned_free(void* memblock) { h2_override::free(memblock); }

   void set()
   {
      h2_stubs::add(stubs, (void*)::_free_base, (void*)_free_base, "_free_base", H2_FILE);
      h2_stubs::add(stubs, (void*)::_msize, (void*)h2_override::size, "_msize", H2_FILE);
      h2_stubs::add(stubs, (void*)::_expand, (void*)_expand, "_expand", H2_FILE);
#ifndef NDEBUG
      h2_stubs::add(stubs, (void*)::_free_dbg, (void*)_free_dbg, "_free_dbg", H2_FILE);
      // h2_stubs::add(stubs,(void*)::_malloc_dbg, (void*)_malloc_dbg, "_malloc_dbg", H2_FILE);
      // h2_stubs::add(stubs,(void*)::_realloc_dbg, (void*)_realloc_dbg, "_realloc_dbg", H2_FILE);
      // h2_stubs::add(stubs,(void*)::_calloc_dbg, (void*)_calloc_dbg, "_calloc_dbg", H2_FILE);
      // h2_stubs::add(stubs,(void*)::_expand_dbg, (void*)_expand_dbg, "_expand_dbg", H2_FILE);
#endif
      //// h2_stubs::add(stubs,(void*)::_calloc_crt, (void*)h2_override::calloc, "_calloc_crt", H2_FILE);
      h2_stubs::add(stubs, (void*)::_aligned_malloc, (void*)_aligned_malloc, "_aligned_malloc", H2_FILE);
      h2_stubs::add(stubs, (void*)::_aligned_free, (void*)_aligned_free, "_aligned_free", H2_FILE);
      h2_stubs::add(stubs, (void*)::_strdup, (void*)h2_override_stdlib::strdup, "_strdup", H2_FILE);  // strdup call to _strdup
   }

   void reset() { h2_stubs::clear(stubs); }
};
