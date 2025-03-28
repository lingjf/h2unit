// https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html
// since libc-2.34 __malloc_hook was removed.
struct h2_override_platform {
   static void free_hook(void* __ptr, const void* caller) { h2_override::free(__ptr); }
   static void* malloc_hook(size_t __size, const void* caller) { return h2_override::malloc(__size); }
   static void* realloc_hook(void* __ptr, size_t __size, const void* caller) { return h2_override::realloc(__ptr, __size); }
   static void* memalign_hook(size_t __alignment, size_t __size, const void* caller) { return h2_override::aligned_alloc(__alignment, __size); }

   void (*saved__free_hook)(void*, const void*);
   void* (*saved__malloc_hook)(size_t, const void*);
   void* (*saved__realloc_hook)(void*, size_t, const void*);
   void* (*saved__memalign_hook)(size_t, size_t, const void*);

   h2_override_platform()
   {
      // saved__free_hook = __free_hook;
      // saved__malloc_hook = __malloc_hook;
      // saved__realloc_hook = __realloc_hook;
      // saved__memalign_hook = __memalign_hook;
   }

   void set()
   {
      // __free_hook = free_hook;
      // __malloc_hook = malloc_hook;
      // __realloc_hook = realloc_hook;
      // __memalign_hook = memalign_hook;
   }

   void reset()
   {
      // __free_hook = saved__free_hook;
      // __malloc_hook = saved__malloc_hook;
      // __realloc_hook = saved__realloc_hook;
      // __memalign_hook = saved__memalign_hook;
   }
};
