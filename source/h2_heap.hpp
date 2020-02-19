#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
extern "C" {
extern malloc_zone_t* malloc_default_purgeable_zone(void) WEAK_IMPORT_ATTRIBUTE;
}
#endif

static inline void h2_fail_g(void* fail);

static const unsigned char forbidden_zone[] = {0xbe, 0xaf, 0xca, 0xfe, 0xc0, 0xde, 0xfa, 0xce};

struct h2_piece {
   void *ptr, *page;
   h2_list x;
   int size, pagesize, pagecount;
   unsigned escape : 1, freed : 1;
   h2_backtrace bt;

   h2_piece(void* ptr_, void* page_, int size_, int pagesize_, int pagecount_, h2_backtrace& bt_)
     : ptr(ptr_), page(page_), size(size_), pagesize(pagesize_), pagecount(pagecount_), escape(0), freed(0), bt(bt_) {}

   bool in_range(const void* p) {
      const unsigned char* p0 = (const unsigned char*)page;
      const unsigned char* p2 = p0 + pagesize * (pagecount + 1);
      return p0 <= (const unsigned char*)p && (const unsigned char*)p < p2;
   }

   static h2_piece* allocate(int size, int alignment, h2_backtrace& bt) {
      static int pagesize = sysconf(_SC_PAGE_SIZE);
      int pagecount = H2_DIV_ROUND_UP(size + (alignment ? alignment : 8) + sizeof(forbidden_zone), pagesize);
      unsigned char* p = (unsigned char*)mmap(nullptr, pagesize * (pagecount + 1), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (p == MAP_FAILED) return nullptr;
      unsigned char* ptr = p + pagesize * pagecount - size;
      ptr = (unsigned char*)H2_ALIGN_DOWN((intptr_t)ptr, alignment ? alignment : 8);

      h2_piece* m = new (h2_raw::malloc(sizeof(h2_piece))) h2_piece((void*)ptr, p, size, pagesize, pagecount, bt);

      unsigned char* p1 = ptr - sizeof(forbidden_zone);
      memcpy(p1, forbidden_zone, sizeof(forbidden_zone));

      unsigned char* p2 = p1 - sizeof(void*);
      *(void**)p2 = (void*)m;

      memcpy(ptr + size, forbidden_zone, sizeof(forbidden_zone));

      if (mprotect((void*)(p + pagesize * pagecount), pagesize, PROT_READ) != 0)
         printf("mprotect failed %s\n", strerror(errno));

      return m;
   }

   static h2_fail* prefree(h2_piece* m) {
      if (m->freed++) return new h2_fail_doublefree(m->ptr, m->bt, h2_backtrace(O().isMAC() ? 5 : 4));

      h2_fail* fail = nullptr;

      if (memcmp((unsigned char*)m->ptr + m->size, forbidden_zone, sizeof(forbidden_zone)))
         h2_append_x_fail(fail, new h2_fail_memoverflow(m->ptr, m->size, forbidden_zone, sizeof(forbidden_zone), m->bt, h2_backtrace()));

      if (memcmp((unsigned char*)m->ptr - sizeof(forbidden_zone), forbidden_zone, sizeof(forbidden_zone)))
         h2_append_x_fail(fail, new h2_fail_memoverflow(m->ptr, -(int)sizeof(forbidden_zone), forbidden_zone, sizeof(forbidden_zone), m->bt, h2_backtrace()));

      if (mprotect(m->page, m->pagesize * (m->pagecount + 1), PROT_READ) != 0)
         printf("mprotect failed %s\n", strerror(errno));

      return fail;
   }

   static void release(h2_piece* m) {
      munmap(m->page, m->pagesize * (m->pagecount + 1));
      h2_raw::free(m);
   }
};

struct h2_block {
   h2_list x;

   h2_list using_list, freed_list;

   const char* file;
   int line;
   const char* where;
   long long limited;
   const char* fill;

   h2_block(const char* file_, int line_, const char* where_, long long limited_, const char* fill_)
     : file(file_), line(line_), where(where_), limited(limited_), fill(fill_) {}

   h2_fail* leak_check() {
      h2_fail_memleak* fail = nullptr;

      if (!using_list.empty()) {
         fail = new h2_fail_memleak(file, line, where);
         h2_list_for_each_entry(p, &using_list, h2_piece, x) fail->add(p->ptr, p->size, p->bt);
      }

      h2_list_for_each_entry(p, &freed_list, h2_piece, x) p->x.out(), h2_piece::release(p);

      return fail;
   }

   h2_piece* newm(int size, int alignment, const char* fill_, h2_backtrace& bt) {
      if (limited < size) return nullptr;

      h2_piece* m = h2_piece::allocate(size, alignment, bt);

      if (fill_ ? fill_ : (fill_ = fill))
         for (int i = 0, j = 0, l = strlen(fill_); i < size; ++i, ++j)
            ((char*)m->ptr)[i] = fill_[j % (l ? l : 1)];

      using_list.push(&m->x);
      return m;
   }

   h2_piece* getm(const void* ptr) {
      h2_list_for_each_entry(p, &using_list, h2_piece, x) if (p->ptr == ptr) return p;
      h2_list_for_each_entry(p, &freed_list, h2_piece, x) if (p->ptr == ptr) return p;
      return nullptr;
   }

   h2_fail* relm(h2_piece* m) {
      limited += m->size;

      m->x.out();
      freed_list.push(&m->x);
      return h2_piece::prefree(m);
   }

   h2_piece* whom(const void* addr) {
      h2_list_for_each_entry(p, &using_list, h2_piece, x) if (p->in_range(addr)) return p;
      h2_list_for_each_entry(p, &freed_list, h2_piece, x) if (p->in_range(addr)) return p;
      return nullptr;
   }
};

struct h2_stack {
   h2_list blocks;

   bool escape(h2_backtrace& bt) {
      static struct {
         unsigned char* base;
         int size;
      } exclude_functions[] = {
        {(unsigned char*)printf, 300},
        {(unsigned char*)sprintf, 300},
        {(unsigned char*)vsnprintf, 300},
        {(unsigned char*)sscanf, 300},
        {(unsigned char*)localtime, 300}};

      for (size_t i = 0; i < sizeof(exclude_functions) / sizeof(exclude_functions[0]); ++i)
         if (bt.has(exclude_functions[i].base, exclude_functions[i].size))
            return true;

      return false;
   }

   bool push(const char* file, int line, const char* where, long long limited = 0x7fffffffffffLL, const char* fill = nullptr) {
      h2_block* b = new (h2_raw::malloc(sizeof(h2_block))) h2_block(file, line, where, limited, fill);
      blocks.push(&b->x);
      return true;
   }

   h2_fail* pop() {
      h2_block* b = h2_list_pop_entry(&blocks, h2_block, x);
      h2_fail* fail = b->leak_check();
      h2_raw::free(b);
      return fail;
   }

   h2_piece* newm(size_t size, size_t alignment, const char* fill) {
      h2_backtrace bt(O().isMAC() ? 6 : 2);
      h2_block* b = escape(bt) ? h2_list_bottom_entry(&blocks, h2_block, x) : h2_list_top_entry(&blocks, h2_block, x);
      return b ? b->newm(size, alignment, fill, bt) : nullptr;
   }

   h2_piece* getm(const void* ptr) {
      h2_list_for_each_entry(p, &blocks, h2_block, x) {
         h2_piece* m = p->getm(ptr);
         if (m) return m;
      }
      return nullptr;
   }

   h2_fail* relm(void* ptr) {
      h2_list_for_each_entry(p, &blocks, h2_block, x) {
         h2_piece* m = p->getm(ptr);
         if (m) return p->relm(m);
      }

      h2_debug("Warning: free not found!");
      return nullptr;
   }

   h2_piece* whom(const void* addr) {
      h2_list_for_each_entry(p, &blocks, h2_block, x) {
         h2_piece* m = p->whom(addr);
         if (m) return m;
      }
      return nullptr;
   }

   /* clang-format off */
   static h2_stack& G() { static h2_stack __; return __; }
   /* clang-format on */

   struct A {
      int count;

      A(const char* file, int line, long long limited = 0x7fffffffffffLL, const char* fill = nullptr)
        : count(0) { h2_stack::G().push(file, line, "block", limited, fill); }
      ~A() { h2_fail_g(h2_stack::G().pop()); }

      operator bool() { return 0 == count++; }
   };
};

struct h2_hook {
   static void free(void* ptr) {
      if (ptr) h2_fail_g(h2_stack::G().relm(ptr)); /* overflow or double free */
   }

   static void* malloc(size_t size) {
      h2_piece* m = h2_stack::G().newm(size, 0, nullptr);
      return m ? m->ptr : nullptr;
   }

   static void* calloc(size_t count, size_t size) {
      h2_piece* m = h2_stack::G().newm(size * count, 0, "\0");
      return m ? m->ptr : nullptr;
   }

   static void* realloc(void* ptr, size_t size) {
      if (size == 0) {
         if (ptr) h2_fail_g(h2_stack::G().relm(ptr));
         return nullptr;
      }

      h2_piece* old_m = h2_stack::G().getm(ptr);
      if (!old_m) return nullptr;

      h2_piece* new_m = h2_stack::G().newm(size, 0, nullptr);
      if (!new_m) return nullptr;

      memcpy(new_m->ptr, old_m->ptr, old_m->size);
      h2_fail_g(h2_stack::G().relm(ptr));

      return new_m->ptr;
   }

   static int posix_memalign(void** memptr, size_t alignment, size_t size) {
      h2_piece* m = h2_stack::G().newm(size, alignment, nullptr);
      return m ? (*memptr = m->ptr, 0) : ENOMEM;
   }

   static void* aligned_alloc(size_t alignment, size_t size) {
      h2_piece* m = h2_stack::G().newm(size, alignment, nullptr);
      return m ? m->ptr : nullptr;
   }

#if defined __GLIBC__

   static void free_hook(void* __ptr, const void* caller) { free(__ptr); }
   static void* malloc_hook(size_t __size, const void* caller) { return malloc(__size); }
   static void* realloc_hook(void* __ptr, size_t __size, const void* caller) { return realloc(__ptr, __size); }
   static void* memalign_hook(size_t __alignment, size_t __size, const void* caller) { return aligned_alloc(__alignment, __size); }

   void (*sys__free_hook)(void*, const void*);
   void* (*sys__malloc_hook)(size_t, const void*);
   void* (*sys__realloc_hook)(void*, size_t, const void*);
   void* (*sys__memalign_hook)(size_t, size_t, const void*);

#elif defined __APPLE__
   static size_t mz_size(malloc_zone_t* zone, const void* ptr) {
      h2_piece* m = h2_stack::G().getm(ptr);
      return m ? m->size : 0;
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

   static malloc_zone_t* get_default_zone() {
      malloc_zone_t** zones = nullptr;
      unsigned int num_zones = 0;

      if (KERN_SUCCESS != malloc_get_all_zones(0, nullptr, (vm_address_t**)&zones, &num_zones)) num_zones = 0;
      if (num_zones) return zones[0];
      return malloc_default_zone();
   }

   malloc_introspection_t mi;
   malloc_zone_t mz;

#else

#endif

   h2_stub free_stub;
   h2_stub malloc_stub;
   h2_stub calloc_stub;
   h2_stub realloc_stub;
   h2_stub posix_memalign_stub;

   h2_hook()
     : free_stub((void*)::free), malloc_stub((void*)::malloc), calloc_stub((void*)::calloc), realloc_stub((void*)::realloc), posix_memalign_stub((void*)::posix_memalign) {
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

#else
#endif
   }

   /* clang-format off */
   static h2_hook& I() { static h2_hook __; return __; }
   /* clang-format on */

   void dohook() {
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

#else
      free_stub.replace((void*)hook::free);
      malloc_stub.replace((void*)hook::malloc);
      calloc_stub.replace((void*)hook::calloc);
      realloc_stub.replace((void*)hook::realloc);
      posix_memalign_stub.replace((void*)hook::posix_memalign);
#endif
   }

   void unhook() {
#if defined __GLIBC__
      __free_hook = sys__free_hook;
      __malloc_hook = sys__malloc_hook;
      __realloc_hook = sys__realloc_hook;
      __memalign_hook = sys__memalign_hook;

#elif defined __APPLE__

      malloc_zone_unregister(&mz);

#else
      free_stub.restore();
      malloc_stub.restore();
      calloc_stub.restore();
      realloc_stub.restore();
      posix_memalign_stub.restore();
#endif
   }

   static void overflow_handler(int sig, siginfo_t* si, void* unused) {
      h2_piece* m = h2_stack::G().whom(si->si_addr);
      if (m) h2_fail_g(new h2_fail_memoverflow(m->ptr, (intptr_t)si->si_addr - (intptr_t)m->ptr, nullptr, 0, m->bt, h2_backtrace(O().isMAC() ? 5 : 4)));
      h2_debug();
      exit(1);
   }
};

static inline void h2_signal_g() {
   if (!O().memory_check) return;
   struct sigaction act;
   act.sa_sigaction = h2_hook::overflow_handler;
   sigemptyset(&act.sa_mask);
   act.sa_flags = SA_SIGINFO;
   if (sigaction(SIGSEGV, &act, nullptr) == -1)
      perror("Register SIGSEGV handler failed");
   if (sigaction(SIGBUS, &act, nullptr) == -1)
      perror("Register SIGBUS handler failed");
}

static inline void h2_dohook_g() {
   if (O().memory_check) h2_hook::I().dohook();
}
static inline void h2_unhook_g() {
   if (O().memory_check) h2_hook::I().unhook();
}

// https://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Hooks-for-Malloc.html
// https://github.com/gperftools/gperftools/blob/master/src/libc_override.h
