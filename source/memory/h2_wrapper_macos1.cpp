
struct h2_wrapper_specific {
   static size_t mz_size(malloc_zone_t* zone, const void* ptr) { return h2_override::size(ptr); }
   static void* mz_malloc(malloc_zone_t* zone, size_t size) { return h2_override::malloc(size); }
   static void* mz_calloc(malloc_zone_t* zone, size_t num_items, size_t size) { return h2_override::calloc(num_items, size); }
   static void* mz_valloc(malloc_zone_t* zone, size_t size) { return h2_override::malloc(size); }
   static void mz_free(malloc_zone_t* zone, void* ptr) { h2_override::free(ptr); }
   static void mz_free_definite_size(malloc_zone_t* zone, void* ptr, size_t size) { h2_override::free(ptr); }
   static void* mz_realloc(malloc_zone_t* zone, void* ptr, size_t size) { return h2_override::realloc(ptr, size); }
   static void* mz_memalign(malloc_zone_t* zone, size_t align, size_t size) { return h2_override::aligned_alloc(align, size); }
   static void mz_destroy(malloc_zone_t* zone) {}

   size_t (*saved__size)(malloc_zone_t* zone, const void* ptr);
   void* (*saved__malloc)(malloc_zone_t* zone, size_t size);
   void* (*saved__calloc)(malloc_zone_t* zone, size_t num_items, size_t size);
   void* (*saved__valloc)(malloc_zone_t* zone, size_t size);
   void (*saved__free)(malloc_zone_t* zone, void* ptr);
   void* (*saved__realloc)(malloc_zone_t* zone, void* ptr, size_t size);
   void (*saved__destroy)(malloc_zone_t* zone);
   void* (*saved__memalign)(malloc_zone_t* zone, size_t alignment, size_t size);
   void (*saved__free_definite_size)(malloc_zone_t* zone, void* ptr, size_t size);

   malloc_zone_t* zone;

   h2_wrapper_specific()
   {
      zone = malloc_default_zone();
      saved__size = zone->size;
      saved__malloc = zone->malloc;
      saved__calloc = zone->calloc;
      saved__valloc = zone->valloc;
      saved__free = zone->free;
      saved__realloc = zone->realloc;
#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
      saved__memalign = zone->memalign;
      saved__free_definite_size = zone->free_definite_size;
#endif
   }

   void overrides()
   {
      zone->size = mz_size;
      zone->malloc = mz_malloc;
      zone->calloc = mz_calloc;
      zone->valloc = mz_valloc;
      zone->free = mz_free;
      zone->realloc = mz_realloc;
#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
      zone->memalign = mz_memalign;
      zone->free_definite_size = mz_free_definite_size;
#endif
   }

   void restores()
   {
      zone->size = saved__size;
      zone->malloc = saved__malloc;
      zone->calloc = saved__calloc;
      zone->valloc = saved__valloc;
      zone->free = saved__free;
      zone->realloc = saved__realloc;
#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
      zone->memalign = saved__memalign;
      zone->free_definite_size = saved__free_definite_size;
#endif
   }
};
