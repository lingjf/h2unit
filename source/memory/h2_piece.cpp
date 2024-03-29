struct h2_piece : h2_libc {
   h2_list x;
   unsigned char *user_ptr, *page_ptr;
   size_t user_size, page_size, page_count;
   // free
   const char* who_allocate;
   h2_backtrace bt_allocate, bt_release;
   int free_times = 0;
   // snowfield
   unsigned char snow_flower;
   // forbidden
   static constexpr const unsigned readable = 1, writable = 1 << 1;
   void* forbidden_page{nullptr};
   size_t forbidden_size = 0;
   int violate_times = 0;
   void* violate_ptr{nullptr};
   const char* violate_action = "";
   bool violate_after_free = false;
   h2_backtrace violate_backtrace;

   h2_piece(size_t size, size_t alignment, const char* who, h2_backtrace& bt) : user_size(size), page_size(h2_page_size()), who_allocate(who), bt_allocate(bt)
   {
      size_t alignment_2n = alignment;
      if (not2n((unsigned)alignment)) alignment_2n = (size_t)mask2n((unsigned)alignment) + 1;
      if (alignment_2n < sizeof(void*)) alignment_2n = sizeof(void*);

      size_t user_size_plus = (user_size + alignment_2n - 1 + alignment_2n) & ~(alignment_2n - 1);
      page_count = (size_t)::ceil(user_size_plus / (double)page_size);

#if defined _WIN32
      page_ptr = (unsigned char*)VirtualAlloc(NULL, page_size * (page_count + 1), MEM_COMMIT, PAGE_READWRITE);
      if (page_ptr == NULL) h2_console::prints("yellow", "VirtualAlloc failed at %s:%d\n", __FILE__, __LINE__), abort();
#else
      page_ptr = (unsigned char*)::mmap(nullptr, page_size * (page_count + 1), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (page_ptr == MAP_FAILED) h2_console::prints("yellow", "mmap failed at %s:%d\n", __FILE__, __LINE__), abort();
#endif

      user_ptr = page_ptr + page_size * page_count - user_size_plus + alignment;

      mark_snowfield();
   }

   ~h2_piece()
   {
#if defined _WIN32
      VirtualFree(page_ptr, 0, MEM_RELEASE);
#else
      ::munmap(page_ptr, page_size * (page_count + 1));
#endif
   }

   void set_forbidden(unsigned permission, void* page = 0, size_t size = 0)
   {
      if (page) forbidden_page = page;
      if (size) forbidden_size = size;

#if defined _WIN32
      DWORD old_permission, new_permission;
      new_permission = PAGE_NOACCESS;
      if (permission & readable) new_permission = PAGE_READONLY;
      if (permission & writable) new_permission = PAGE_READWRITE;
      if (!VirtualProtect(forbidden_page, forbidden_size, new_permission, &old_permission)) h2_console::prints("yellow", "VirtualProtect failed %lu\n", GetLastError());
#else
      int new_permission = PROT_NONE;
      if (permission & readable) new_permission = PROT_READ;
      if (permission & writable) new_permission = PROT_READ | PROT_WRITE;
      if (::mprotect(forbidden_page, forbidden_size, new_permission) != 0) h2_console::prints("yellow", "mprotect failed %s\n", strerror(errno));
#endif
   }

#if defined _WIN32
   void violate_forbidden(void* ptr, const char* type)
   {
      if (!violate_times++) { /* 只记录第一犯罪现场 */
         violate_backtrace = h2_backtrace::dump(3);
         violate_ptr = ptr;
         violate_action = type;
         violate_after_free = 0 < free_times;
      }
      set_forbidden(readable | writable);
   }
#else
   void violate_forbidden(void* ptr)
   {
      /* 区分读写犯罪方法(一次或二次进入 segment fault):
         1) 设区域为不可读不可写
         2) 读或写行为触发 segment fault, 并进入handler
         3) 设区域为只可读不可写, 先认为犯罪为读
         4) 重新执行代码, 如果是读行为, 则结束(犯罪已经记录为读)
         5) 写行为再次触发 segment fault, 并再次进入handler
         6) 设区域为可读可写, 修正犯罪为写
         7) 恢复执行代码
       */
      auto bt = h2_backtrace::dump(4);
      if (!violate_times++) { /* 只记录第一犯罪现场 */
         set_forbidden(readable);
         violate_backtrace = bt;
         violate_ptr = ptr;
         violate_action = "read";
         violate_after_free = 0 < free_times;
      } else {
         set_forbidden(readable | writable);
         if (bt == violate_backtrace) /* 是第一犯罪现场 */
            violate_action = "write";
      }
   }
#endif

   void mark_snowfield()
   {
      static unsigned char s_snow_flower = (unsigned char)::rand();
      snow_flower = ++s_snow_flower;
      memset(page_ptr, snow_flower, user_ptr - page_ptr);
      memset(user_ptr + user_size, snow_flower, (page_ptr + page_size * page_count) - (user_ptr + user_size));
      set_forbidden(readable, page_ptr + page_size * page_count, page_size);
   }

   h2_fail* check_snowfield(const unsigned char* start, const unsigned char* end)
   {
      for (const unsigned char* p = start; p < end; ++p) {
         if (*p == snow_flower) continue;
         int n = std::min((int)(end - p), 8);
         for (; 0 < n; --n)
            if (p[n - 1] != snow_flower) break;
         h2_vector<unsigned char> spot(p, p + n);
         return h2_fail::new_overflow(user_ptr, user_size, p, "write", spot, bt_allocate, h2_backtrace());
      }
      return nullptr;
   }

   h2_fail* check_snowfield()
   {
      h2_fail* fail = nullptr;
      fail = check_snowfield(user_ptr + user_size, page_ptr + page_size * page_count);
      if (!fail) fail = check_snowfield(page_ptr, user_ptr);
      if (fail && O.as_waring_memory_violate) fail->warning = true;
      return fail;
   }

   h2_fail* violate_fail()
   {
      if (violate_after_free) return h2_fail::new_use_after_free(user_ptr, violate_ptr, violate_action, bt_allocate, bt_release, violate_backtrace);
      else return h2_fail::new_overflow(user_ptr, user_size, violate_ptr, violate_action, h2_vector<unsigned char>(), bt_allocate, violate_backtrace);
   }

   h2_fail* check_asymmetric_free(const char* who_release)
   {
      if (h2_in(who_allocate, 5, "malloc", "calloc", "realloc", "posix_memalign", "aligned_alloc") && h2_in(who_release, 1, "free")) return nullptr;
      // if (h2_in(who_allocate, 2, "new", "new nothrow") && h2_in(who_release, 2, "delete", "delete nothrow")) return nullptr;
      // if (h2_in(who_allocate, 2, "new[]", "new[] nothrow") && h2_in(who_release, 2, "delete[]", "delete[] nothrow")) return nullptr;
      // some compiler new but delete[]
      if (h2_in(who_allocate, 4, "new", "new nothrow", "new[]", "new[] nothrow") && h2_in(who_release, 4, "delete", "delete nothrow", "delete[]", "delete[] nothrow")) return nullptr;

      if (bt_allocate.in(h2_exempt::I().fps)) return nullptr;
      auto fail = h2_fail::new_asymmetric_free(user_ptr, who_allocate, who_release, bt_allocate, bt_release);
      if (O.as_waring_memory_asymmetric_free) fail->warning = true;
      return fail;
   }

   h2_fail* check_double_free(h2_backtrace& bt)
   {
      if (!free_times++) {
         bt_release = bt;
         return nullptr;
      }
      auto fail = h2_fail::new_double_free(user_ptr, bt_allocate, bt_release, bt);
      if (O.as_waring_memory_double_free) fail->warning = true;
      return fail;
   }

   h2_fail* free(const char* who_release)
   {
      h2_fail* fail = check_double_free(h2_backtrace::dump(4));
      if (!fail && who_release) fail = check_asymmetric_free(who_release);
      if (!fail) fail = check_snowfield();
      if (!fail) set_forbidden(0, page_ptr, page_size * (page_count + 1));
      return fail;
   }

   bool in_page_range(const unsigned char* p) const
   {
      return page_ptr <= p && p < page_ptr + page_size * (page_count + 1);
   }
};
