
#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
extern "C" malloc_zone_t* malloc_default_purgeable_zone(void) WEAK_IMPORT_ATTRIBUTE;
#endif
extern "C" void _malloc_fork_prepare();
extern "C" void _malloc_fork_parent();
extern "C" void _malloc_fork_child();
extern "C" void vfree(void*);
extern "C" int malloc_jumpstart(int);

static malloc_zone_t theDefaultZone;

struct h2_wrapper_specific {
   static size_t replace_malloc_usable_size(void* ptr) { return h2_override::size(ptr); }
   static size_t replace_malloc_good_size(size_t sz) { return sz; }

   static malloc_zone_t* replace_malloc_create_zone(vm_size_t, unsigned) { return nullptr; }
   static malloc_zone_t* replace_malloc_default_zone() { return &theDefaultZone; }
   static malloc_zone_t* replace_malloc_default_purgeable_zone() { return &theDefaultZone; }
   static void replace_malloc_destroy_zone(malloc_zone_t*) {}

   static kern_return_t replace_malloc_get_all_zones(task_t, memory_reader_t, vm_address_t** addresses, unsigned* count)
   {
      *addresses = 0;
      count = 0;
      return KERN_SUCCESS;
   }

   static const char* replace_malloc_get_zone_name(malloc_zone_t* z) { return ""; }
   static void replace_malloc_printf(const char*, ...) {}
   static size_t replace_internal_malloc_zone_size(malloc_zone_t*, const void* ptr) { return replace_malloc_usable_size((void*)ptr); }
   static int replace_malloc_jumpstart(int) { return 1; }
   static void replace_malloc_set_zone_name(malloc_zone_t*, const char*) {}
   static unsigned replace_malloc_zone_batch_malloc(malloc_zone_t*, size_t sz, void** results, unsigned num_requested)
   {
      for (unsigned i = 0; i < num_requested; i++)
         if ((results[i] = h2_override::malloc(sz)) == nullptr)
            return i;
      return num_requested;
   }

   static void replace_malloc_zone_batch_free(malloc_zone_t*, void** to_be_freed, unsigned num)
   {
      for (unsigned i = 0; i < num; i++) h2_override::free(to_be_freed[i]);
   }

   static void* replace_malloc_zone_calloc(malloc_zone_t*, size_t n, size_t size) { return h2_override::calloc(n, size); }
   static bool replace_malloc_zone_check(malloc_zone_t*) { return true; }
   static void replace_malloc_zone_free(malloc_zone_t*, void* ptr) { h2_override::free(ptr); }
   static void replace_malloc_zone_free_definite_size(malloc_zone_t*, void* ptr, size_t) { h2_override::free(ptr); }
   static malloc_zone_t* replace_malloc_zone_from_ptr(const void*) { return replace_malloc_default_zone(); }
   static void replace_malloc_zone_log(malloc_zone_t*, void*) {}
   static void* replace_malloc_zone_malloc(malloc_zone_t*, size_t size) { return h2_override::malloc(size); }
   static void replace_malloc_zone_print(malloc_zone_t*, bool) {}
   static void replace_malloc_zone_print_ptr_info(void*) {}
   static void* replace_malloc_zone_realloc(malloc_zone_t*, void* ptr, size_t size) { return h2_override::realloc(ptr, size); }
   static void replace_malloc_zone_register(malloc_zone_t*) {}
   static void* replace_malloc_zone_memalign(malloc_zone_t*, size_t alignment, size_t size) { return h2_override::aligned_alloc(alignment, size); }
   static void replace_malloc_zone_unregister(malloc_zone_t*) {}
   static void* replace_malloc_zone_valloc(malloc_zone_t*, size_t size) { return h2_override::valloc(size); }
   static void replace__malloc_fork_child() {}   /* Called in the child process after a fork() to resume normal operation.  In the MTASK case we also have to change memory inheritance so that the child does not share memory with the parent. */
   static void replace__malloc_fork_parent() {}  /* Called in the parent process after a fork() to resume normal operation. */
   static void replace__malloc_fork_prepare() {} /* Prepare the malloc module for a fork by insuring that no thread is in a malloc critical section */

   h2_stubs stubs;

   h2_wrapper_specific()
   {
   }

   void overrides()
   {
      stubs.add((void*)_malloc_fork_child, (void*)replace__malloc_fork_child, "_malloc_fork_child", __FILE__, __LINE__);
      stubs.add((void*)_malloc_fork_child, (void*)replace__malloc_fork_child, "_malloc_fork_child", __FILE__, __LINE__);
      stubs.add((void*)_malloc_fork_parent, (void*)replace__malloc_fork_parent, "_malloc_fork_parent", __FILE__, __LINE__);
      stubs.add((void*)_malloc_fork_prepare, (void*)replace__malloc_fork_prepare, "_malloc_fork_prepare", __FILE__, __LINE__);
      stubs.add((void*)malloc_create_zone, (void*)replace_malloc_create_zone, "malloc_create_zone", __FILE__, __LINE__);
      stubs.add((void*)malloc_default_purgeable_zone, (void*)replace_malloc_default_purgeable_zone, "malloc_default_purgeable_zone", __FILE__, __LINE__);
      stubs.add((void*)malloc_default_zone, (void*)replace_malloc_default_zone, "malloc_default_zone", __FILE__, __LINE__);
      stubs.add((void*)malloc_destroy_zone, (void*)replace_malloc_destroy_zone, "malloc_destroy_zone", __FILE__, __LINE__);
      stubs.add((void*)malloc_get_all_zones, (void*)replace_malloc_get_all_zones, "malloc_get_all_zones", __FILE__, __LINE__);
      stubs.add((void*)malloc_get_zone_name, (void*)replace_malloc_get_zone_name, "malloc_get_zone_name", __FILE__, __LINE__);
      stubs.add((void*)malloc_good_size, (void*)replace_malloc_good_size, "malloc_good_size", __FILE__, __LINE__);
      stubs.add((void*)malloc_jumpstart, (void*)replace_malloc_jumpstart, "malloc_jumpstart", __FILE__, __LINE__);
      stubs.add((void*)malloc_printf, (void*)replace_malloc_printf, "malloc_printf", __FILE__, __LINE__);
      stubs.add((void*)malloc_set_zone_name, (void*)replace_malloc_set_zone_name, "malloc_set_zone_name", __FILE__, __LINE__);
      stubs.add((void*)malloc_size, (void*)replace_malloc_usable_size, "malloc_size", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_batch_free, (void*)replace_malloc_zone_batch_free, "malloc_zone_batch_free", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_batch_malloc, (void*)replace_malloc_zone_batch_malloc, "malloc_zone_batch_malloc", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_calloc, (void*)replace_malloc_zone_calloc, "malloc_zone_calloc", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_check, (void*)replace_malloc_zone_check, "malloc_zone_check", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_free, (void*)replace_malloc_zone_free, "malloc_zone_free", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_from_ptr, (void*)replace_malloc_zone_from_ptr, "malloc_zone_from_ptr", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_log, (void*)replace_malloc_zone_log, "malloc_zone_log", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_malloc, (void*)replace_malloc_zone_malloc, "malloc_zone_malloc", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_memalign, (void*)replace_malloc_zone_memalign, "malloc_zone_memalign", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_print, (void*)replace_malloc_zone_print, "malloc_zone_print", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_print_ptr_info, (void*)replace_malloc_zone_print_ptr_info, "malloc_zone_print_ptr_info", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_realloc, (void*)replace_malloc_zone_realloc, "malloc_zone_realloc", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_register, (void*)replace_malloc_zone_register, "malloc_zone_register", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_unregister, (void*)replace_malloc_zone_unregister, "malloc_zone_unregister", __FILE__, __LINE__);
      stubs.add((void*)malloc_zone_valloc, (void*)replace_malloc_zone_valloc, "malloc_zone_valloc", __FILE__, __LINE__);

      stubs.add((void*)reallocf, (void*)h2_override::realloc, "reallocf", __FILE__, __LINE__);
      stubs.add((void*)valloc, (void*)h2_override::valloc, "valloc", __FILE__, __LINE__);
      stubs.add((void*)vfree, (void*)h2_override::free, "vfree", __FILE__, __LINE__);
   }

   void restores()
   {
      stubs.clear();
   }
};
