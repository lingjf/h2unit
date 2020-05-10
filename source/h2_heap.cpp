
static const unsigned char snowfield[] = {0xbe, 0xaf, 0xca, 0xfe, 0xc0, 0xde, 0xfa, 0xce};

struct h2_piece : h2_libc {
   unsigned char *ptr, *page;
   h2_list x;
   int size, page_size, page_count, free_times;
   const char* who_allocate;
   h2_backtrace bt_allocate, bt_release;

   h2_piece(int size_, int alignment, const char* who, h2_backtrace& bt) : size(size_), free_times(0), who_allocate(who), bt_allocate(bt)
   {
      page_size = h2_page_size();
      if (alignment <= 0) alignment = 8;
      page_count = ::ceil((size + alignment + sizeof(snowfield)) / (double)page_size);

#ifdef _WIN32
      page = (unsigned char*)VirtualAlloc(NULL, page_size * (page_count + 1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
      assert(page);
#else
      page = (unsigned char*)::mmap(nullptr, page_size * (page_count + 1), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      assert(page != MAP_FAILED);
#endif

      ptr = page + page_size * page_count - size;
      ptr = (unsigned char*)(((intptr_t)ptr / alignment) * alignment);

      h2_piece** backward = (h2_piece**)(ptr - sizeof(snowfield) - sizeof(void*));
      *backward = this;

      mark_snowfield();
   }

   ~h2_piece()
   {
#ifdef _WIN32
      VirtualFree(page, 0, MEM_DECOMMIT | MEM_RELEASE);
#else
      ::munmap(page, page_size * (page_count + 1));
#endif
   }

   void mark_snowfield()
   {
      memcpy(ptr - sizeof(snowfield), snowfield, sizeof(snowfield));
      memcpy(ptr + size, snowfield, sizeof(snowfield));
#ifdef _WIN32
      DWORD old;
      if (!VirtualProtect(page + page_size * page_count, page_size, PAGE_READONLY, &old))
         ::printf("VirtualProtect PAGE_READONLY failed %d\n", GetLastError());
#else
      if (::mprotect(page + page_size * page_count, page_size, PROT_READ) != 0)
         ::printf("mprotect PROT_READ failed %s\n", strerror(errno));
#endif
   }

   h2_fail* check_snowfield()
   {
      h2_fail* fail = nullptr;
      if (memcmp(ptr + size, snowfield, sizeof(snowfield)))
         h2_fail::append_x(fail, new h2_fail_memoverflow(ptr, size, snowfield, sizeof(snowfield), bt_allocate, h2_backtrace()));
      if (memcmp(ptr - sizeof(snowfield), snowfield, sizeof(snowfield)))
         h2_fail::append_x(fail, new h2_fail_memoverflow(ptr, -(int)sizeof(snowfield), snowfield, sizeof(snowfield), bt_allocate, h2_backtrace()));

#ifdef _WIN32
      DWORD old;
      if (!VirtualProtect(page, page_size * (page_count + 1), PAGE_NOACCESS, &old))
         ::printf("VirtualProtect PAGE_NOACCESS failed %d\n", GetLastError());
#else
      if (::mprotect(page, page_size * (page_count + 1), PROT_NONE) != 0)
         ::printf("mprotect PROT_NONE failed %s\n", strerror(errno));
#endif
      return fail;
   }

   h2_fail* check_symmetric_free(const char* who_release)
   {
      static const char* free_a[] = {"malloc", "calloc", "realloc", "reallocf", "posix_memalign", "memalign", "aligned_alloc", "valloc", "pvalloc", nullptr};
      static const char* free_r[] = {"free", nullptr};
      static const char* new_a[] = {"new", "new nothrow", nullptr};
      static const char* new_r[] = {"delete", nullptr};
      static const char* news_a[] = {"new[]", "new[] nothrow", nullptr};
      static const char* news_r[] = {"delete[]", nullptr};
      static const char* _aligned_a[] = {"_aligned_malloc", "_aligned_realloc", "_aligned_recalloc", "_aligned_offset_malloc", "_aligned_offset_realloc", "_aligned_offset_recalloc", nullptr};
      static const char* _aligned_r[] = {"_aligned_free", nullptr};
      static const char* HeapAlloc_a[] = {"HeapAlloc", nullptr};
      static const char* HeapFree_r[] = {"HeapFree", nullptr};
      static const char* VirtualAlloc_a[] = {"VirtualAlloc", nullptr};
      static const char* VirtualFree_r[] = {"VirtualFree", nullptr};
      static struct {
         const char **a, **r;
      } S[] = {{free_a, free_r}, {new_a, new_r}, {news_a, news_r}, {_aligned_a, _aligned_r}, {HeapAlloc_a, HeapFree_r}, {VirtualAlloc_a, VirtualFree_r}};

      for (int i = 0; i < sizeof(S) / sizeof(S[0]); ++i)
         if (h2_in(who_allocate, S[i].a) && h2_in(who_release, S[i].r))
            return nullptr;

      h2_backtrace bt_release(O.isMAC() ? 6 : 5);
      return new h2_fail_symmetric_free(ptr, bt_allocate, bt_release, "allocate with %s, release by %s", SF("bold,red", "%s", who_allocate), SF("bold,red", "%s", who_release));
   }

   h2_fail* check_double_free()
   {
      h2_fail* fail = nullptr;
      h2_backtrace bt(O.isMAC() ? 6 : 5);
      if (free_times++ == 0)
         bt_release = bt;
      else
         fail = (h2_fail*)new h2_fail_double_free(ptr, bt_allocate, bt_release, bt);
      return fail;
   }

   h2_fail* free(const char* who_release)
   {
      h2_fail* fail = nullptr;
      if (!fail)
         fail = check_double_free();
      if (!fail)
         fail = check_symmetric_free(who_release);
      if (!fail)
         fail = check_snowfield();
      return fail;
   }

   bool in_range(const void* p)
   {
      const unsigned char* p0 = page;
      const unsigned char* p2 = p0 + page_size * (page_count + 1);
      return p0 <= (const unsigned char*)p && (const unsigned char*)p < p2;
   }
};

struct h2_block : h2_libc {
   h2_list x;
   h2_list using_list, freed_list;

   const char* file;
   int line;
   const char* where;
   long long limited;
   const char* fill;

   h2_block(const char* file_, int line_, const char* where_, long long limited_, const char* fill_)
     : file(file_), line(line_), where(where_), limited(limited_), fill(fill_) {}

   h2_fail* leak_check()
   {
      h2_fail_memleak* fail = nullptr;
      if (!using_list.empty()) {
         fail = new h2_fail_memleak(file, line, where);
         h2_list_for_each_entry(p, &using_list, h2_piece, x) fail->add(p->ptr, p->size, p->bt_allocate);
      }
      h2_list_for_each_entry(p, &freed_list, h2_piece, x)
      {
         p->x.out();
         delete p;
      }
      return fail;
   }

   h2_piece* new_piece(const char* who, int size, int alignment, const char* fill_, h2_backtrace& bt)
   {
      if (limited < size) return nullptr;
      limited -= size;

      h2_piece* p = new h2_piece(size, alignment, who, bt);

      if (fill_ ? fill_ : (fill_ = fill))
         for (int i = 0, j = 0, l = strlen(fill_); i < size; ++i, ++j)
            ((char*)p->ptr)[i] = fill_[j % (l ? l : 1)];

      using_list.push(&p->x);
      return p;
   }

   h2_piece* get_piece(const void* ptr)
   {
      h2_list_for_each_entry(p, &using_list, h2_piece, x) if (p->ptr == ptr) return p;
      h2_list_for_each_entry(p, &freed_list, h2_piece, x) if (p->ptr == ptr) return p;
      return nullptr;
   }

   h2_fail* rel_piece(const char* who, h2_piece* p)
   {
      limited += p->size;

      p->x.out();
      freed_list.push(&p->x);
      return p->free(who);
   }

   h2_piece* host_piece(const void* addr)
   {
      h2_list_for_each_entry(p, &using_list, h2_piece, x) if (p->in_range(addr)) return p;
      h2_list_for_each_entry(p, &freed_list, h2_piece, x) if (p->in_range(addr)) return p;
      return nullptr;
   }
};

struct h2_stack {
   h2_singleton(h2_stack);

   h2_list blocks;

   bool escape(h2_backtrace& bt)
   {
      static struct {
         void* base;
         int size;
      } escape_functions[] = {
        {(void*)sprintf, 300},
        {(void*)vsnprintf, 300},  //  {(void*)sscanf, 300},
        {(void*)localtime, 300},
#ifndef _WIN32
        {(void*)tzset, 300},
#endif
      };

      for (auto& x : escape_functions)
         if (bt.has(x.base, x.size))
            return true;

      return false;
   }

   void push(const char* file, int line, const char* where, long long limited, const char* fill)
   {
      h2_block* b = new h2_block(file, line, where, limited, fill);
      blocks.push(&b->x);
   }

   h2_fail* pop()
   {
      h2_block* b = h2_list_pop_entry(&blocks, h2_block, x);
      h2_fail* fail = b->leak_check();
      delete b;
      return fail;
   }

   h2_piece* new_piece(const char* who, size_t size, size_t alignment, const char* fill)
   {
      h2_backtrace bt(O.isMAC() ? 3 : 2);
      h2_block* b = escape(bt) ? h2_list_bottom_entry(&blocks, h2_block, x) : h2_list_top_entry(&blocks, h2_block, x);
      return b ? b->new_piece(who, size, alignment, fill, bt) : nullptr;
   }

   h2_piece* get_piece(const void* ptr)
   {
      h2_list_for_each_entry(p, &blocks, h2_block, x)
      {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return piece;
      }
      return nullptr;
   }

   h2_fail* rel_piece(const char* who, void* ptr)
   {
      h2_list_for_each_entry(p, &blocks, h2_block, x)
      {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return p->rel_piece(who, piece);
      }
      h2_debug("Warning: free %p not found!", ptr);
      return nullptr;
   }

   h2_piece* host_piece(const void* addr)
   {
      h2_list_for_each_entry(p, &blocks, h2_block, x)
      {
         h2_piece* piece = p->host_piece(addr);
         if (piece) return piece;
      }
      return nullptr;
   }
};

// https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html
// https://github.com/gperftools/gperftools/blob/master/src/libc_override.h
// https://github.com/google/tcmalloc/blob/master/tcmalloc/libc_override.h

#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
extern "C" {
extern malloc_zone_t* malloc_default_purgeable_zone(void) WEAK_IMPORT_ATTRIBUTE;
}
#endif

struct h2_hook {
   h2_singleton(h2_hook);

   static void free(void* ptr)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("free", ptr)); /* overflow or double free */
   }
   static void* malloc(size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("malloc", size, 0, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void* calloc(size_t count, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("calloc", size * count, 0, "\0");
      return p ? p->ptr : nullptr;
   }
   static void* realloc(void* ptr, size_t size)
   {
      if (size == 0) {
         free(ptr);
         return nullptr;
      }

      h2_piece* old_p = h2_stack::I().get_piece(ptr);
      if (!old_p) return nullptr;

      h2_piece* new_p = h2_stack::I().new_piece("realloc", size, 0, nullptr);
      if (!new_p) return nullptr;

      memcpy(new_p->ptr, old_p->ptr, old_p->size);
      h2_fail_g(h2_stack::I().rel_piece("free", ptr));

      return new_p->ptr;
   }
   static int posix_memalign(void** memptr, size_t alignment, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("posix_memalign", size, alignment, nullptr);
      return p ? (*memptr = p->ptr, 0) : ENOMEM;
   }
   static void* aligned_alloc(size_t alignment, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("aligned_alloc", size, alignment, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void* _aligned_malloc(size_t size, size_t alignment)
   {
      h2_piece* p = h2_stack::I().new_piece("_aligned_malloc", size, alignment, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void _aligned_free(void* memblock)
   {
      if (memblock) h2_fail_g(h2_stack::I().rel_piece("_aligned_free", memblock));
   }
   static void* operator new(std::size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("new", size, 0, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void* operator new(std::size_t size, const std::nothrow_t&)
   {
      h2_piece* p = h2_stack::I().new_piece("new nothrow", size, 0, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void* operator new[](std::size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("new[]", size, 0, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void* operator new[](std::size_t size, const std::nothrow_t&)
   {
      h2_piece* p = h2_stack::I().new_piece("new[] nothrow", size, 0, nullptr);
      return p ? p->ptr : nullptr;
   }
   static void operator delete(void* ptr)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete", ptr));
   }
   static void operator delete(void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete", ptr));
   }
   static void operator delete[](void* ptr)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete[]", ptr));
   }
   static void operator delete[](void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_fail_g(h2_stack::I().rel_piece("delete[]", ptr));
   }

#if defined __GLIBC__

   static void free_hook(void* __ptr, const void* caller)
   {
      free(__ptr);
   }
   static void* malloc_hook(size_t __size, const void* caller) { return malloc(__size); }
   static void* realloc_hook(void* __ptr, size_t __size, const void* caller) { return realloc(__ptr, __size); }
   static void* memalign_hook(size_t __alignment, size_t __size, const void* caller) { return aligned_alloc(__alignment, __size); }

   void (*sys__free_hook)(void*, const void*);
   void* (*sys__malloc_hook)(size_t, const void*);
   void* (*sys__realloc_hook)(void*, size_t, const void*);
   void* (*sys__memalign_hook)(size_t, size_t, const void*);

#elif defined __APPLE__
   static size_t mz_size(malloc_zone_t* zone, const void* ptr)
   {
      h2_piece* p = h2_stack::I().get_piece(ptr);
      return p ? p->size : 0;
   }

   static void* mz_malloc(malloc_zone_t* zone, size_t size) { return malloc(size); }
   static void* mz_calloc(malloc_zone_t* zone, size_t num_items, size_t size) { return calloc(num_items, size); }
   static void* mz_valloc(malloc_zone_t* zone, size_t size) { return malloc(size); }
   static void mz_free(malloc_zone_t* zone, void* ptr) { free(ptr); }
   static void mz_free_sized(malloc_zone_t* zone, void* ptr, size_t size) { free(ptr); }
   static void* mz_realloc(malloc_zone_t* zone, void* ptr, size_t size) { return realloc(ptr, size); }
   static void* mz_memalign(malloc_zone_t* zone, size_t align, size_t size) { return aligned_alloc(align, size); }
   static void mz_destroy(malloc_zone_t* zone) {}

   static kern_return_t mi_enumerator(task_t task, void*, unsigned type_mask, vm_address_t zone_address, memory_reader_t reader, vm_range_recorder_t recorder) { return KERN_FAILURE; }
   static size_t mi_good_size(malloc_zone_t* zone, size_t size) { return size; }
   static boolean_t mi_check(malloc_zone_t* zone) { return true; }
   static void mi_print(malloc_zone_t* zone, boolean_t verbose) {}
   static void mi_log(malloc_zone_t* zone, void* address) {}
   static void mi_force_lock(malloc_zone_t* zone) {}
   static void mi_force_unlock(malloc_zone_t* zone) {}

   static boolean_t mi_zone_locked(malloc_zone_t* zone) { return false; }  // Hopefully unneeded by us!

   static malloc_zone_t* get_default_zone()
   {
      malloc_zone_t** zones = nullptr;
      unsigned int num_zones = 0;

      if (KERN_SUCCESS != malloc_get_all_zones(0, nullptr, (vm_address_t**)&zones, &num_zones)) num_zones = 0;
      if (num_zones) return zones[0];
      return malloc_default_zone();
   }

   malloc_introspection_t mi;
   malloc_zone_t mz;

#elif defined _WIN32

#else

#endif

   h2_stubs stubs;

   h2_hook()
   {
#if defined __GLIBC__
      sys__free_hook = __free_hook;
      sys__malloc_hook = __malloc_hook;
      sys__realloc_hook = __realloc_hook;
      sys__memalign_hook = __memalign_hook;

#elif defined __APPLE__

      memset(&mi, 0, sizeof(mi));
      mi.enumerator = &mi_enumerator;
      mi.good_size = &mi_good_size;
      mi.check = &mi_check;
      mi.print = &mi_print;
      mi.log = &mi_log;
      mi.force_lock = &mi_force_lock;
      mi.force_unlock = &mi_force_unlock;

      memset(&mz, 0, sizeof(malloc_zone_t));
      mz.version = 4;
      mz.zone_name = "h2unit";
      mz.size = &mz_size;
      mz.malloc = &mz_malloc;
      mz.calloc = &mz_calloc;
      mz.valloc = &mz_valloc;
      mz.free = &mz_free;
      mz.realloc = &mz_realloc;
      mz.destroy = &mz_destroy;
      mz.batch_malloc = nullptr;
      mz.batch_free = nullptr;
      mz.introspect = &mi;

#   if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
      mz.version = 6;
      mz.free_definite_size = &mz_free_sized;
      mz.memalign = &mz_memalign;
      mi.zone_locked = &mi_zone_locked;

      if (malloc_default_purgeable_zone) malloc_default_purgeable_zone();

#   endif

#elif defined _WIN32
      //TODO
#else
#endif
   }

   void dohook()
   {
#if defined __GLIBC__
      __free_hook = free_hook;
      __malloc_hook = malloc_hook;
      __realloc_hook = realloc_hook;
      __memalign_hook = memalign_hook;
#elif defined __APPLE__
      malloc_zone_register(&mz);
      malloc_zone_t* default_zone = get_default_zone();
      malloc_zone_unregister(default_zone);
      malloc_zone_register(default_zone);
#elif defined _WIN32
      stubs.add((void*)::free, (void*)free);
      stubs.add((void*)::malloc, (void*)malloc);
      stubs.add((void*)::realloc, (void*)realloc);
      stubs.add((void*)::calloc, (void*)calloc);
      stubs.add((void*)::_aligned_malloc, (void*)_aligned_malloc);
#else
#endif
#if defined _ISOC11_SOURCE
      stubs.add((void*)::aligned_alloc, (void*)aligned_alloc);
#endif
      stubs.add((void*)((void* (*)(std::size_t))::operator new), (void*)((void* (*)(std::size_t)) operator new));
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new), (void*)((void* (*)(std::size_t, const std::nothrow_t&)) operator new));
      stubs.add((void*)((void* (*)(std::size_t))::operator new[]), (void*)((void* (*)(std::size_t)) operator new[]));
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new[]), (void*)((void* (*)(std::size_t, const std::nothrow_t&)) operator new[]));
      stubs.add((void*)((void (*)(void*))::operator delete), (void*)((void (*)(void*)) operator delete));
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete), (void*)((void (*)(void*, const std::nothrow_t&)) operator delete));
      stubs.add((void*)((void (*)(void*))::operator delete[]), (void*)((void (*)(void*)) operator delete[]));
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete[]), (void*)((void (*)(void*, const std::nothrow_t&)) operator delete[]));
   }

   void unhook()
   {
#if defined __GLIBC__
      __free_hook = sys__free_hook;
      __malloc_hook = sys__malloc_hook;
      __realloc_hook = sys__realloc_hook;
      __memalign_hook = sys__memalign_hook;
#elif defined __APPLE__
      malloc_zone_unregister(&mz);
#elif defined _WIN32
#else
#endif
      stubs.clear();
   }

#ifndef _WIN32
   static void segment_fault_handler(int sig, siginfo_t* si, void* unused)
   {
      h2_piece* m = h2_stack::I().host_piece(si->si_addr);
      if (m) h2_fail_g(new h2_fail_memoverflow(m->ptr, (intptr_t)si->si_addr - (intptr_t)m->ptr, nullptr, 0, m->bt_allocate, h2_backtrace(O.isMAC() ? 5 : 4)));
      h2_debug();
      exit(1);
   }
   void install_segment_fault_handler()
   {
      struct sigaction act;
      act.sa_sigaction = segment_fault_handler;
      act.sa_flags = SA_SIGINFO;
      sigemptyset(&act.sa_mask);
      if (sigaction(SIGSEGV, &act, nullptr) == -1) perror("Register SIGSEGV handler failed");
      if (sigaction(SIGBUS, &act, nullptr) == -1) perror("Register SIGBUS handler failed");
   }
#endif
};

h2_inline void h2_heap::initialize()
{
#ifndef _WIN32
   if (O.memory_check && !O.debug) h2_hook::I().install_segment_fault_handler();
#endif
   stack::root();
}
h2_inline void h2_heap::dohook()
{
   if (O.memory_check) h2_hook::I().dohook();
}
h2_inline void h2_heap::unhook()
{
   if (O.memory_check) h2_hook::I().unhook();
}
h2_inline void h2_heap::stack_push_block(const char* file, int line, const char* where, long long limited, const char* fill)
{
   h2_stack::I().push(file, line, where, limited, fill);
}
h2_inline h2_fail* h2_heap::stack_pop_block()
{
   return h2_stack::I().pop();
}
