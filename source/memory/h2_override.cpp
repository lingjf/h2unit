struct h2_override {
   static size_t size(const void* ptr)
   {
      h2_piece* p = h2_stack::I().get_piece(ptr);
      return p ? p->user_size : 0;
   }
   static void free(void* ptr)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("free", ptr));
   }
   static void* malloc(size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("malloc", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* calloc(size_t count, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("calloc", size * count, 0, "\0");
      return p ? p->user_ptr : nullptr;
   }
   static void* realloc(void* ptr, size_t size)
   {
      h2_piece *old_p = nullptr, *new_p = nullptr;
      if (size == 0 && O.os == 'm') size = 1;
      if (ptr) old_p = h2_stack::I().get_piece(ptr);
      if (size) new_p = h2_stack::I().new_piece("realloc", size, 0, nullptr);
      if (old_p && new_p) memcpy(new_p->user_ptr, old_p->user_ptr, std::min(old_p->user_size, size));
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("free", ptr));
      return new_p ? new_p->user_ptr : nullptr;
   }
   static int posix_memalign(void** memptr, size_t alignment, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("posix_memalign", size, alignment, nullptr);
      return p ? (*memptr = p->user_ptr, 0) : ENOMEM;
   }
   static void* aligned_alloc(size_t alignment, size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("aligned_alloc", size, alignment, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new(std::size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("new", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new(std::size_t size, const std::nothrow_t&)
   {
      h2_piece* p = h2_stack::I().new_piece("new nothrow", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new[](std::size_t size)
   {
      h2_piece* p = h2_stack::I().new_piece("new[]", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void* operator new[](std::size_t size, const std::nothrow_t&)
   {
      h2_piece* p = h2_stack::I().new_piece("new[] nothrow", size, 0, nullptr);
      return p ? p->user_ptr : nullptr;
   }
   static void operator delete(void* ptr)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete", ptr));
   }
   static void operator delete(void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete nothrow", ptr));
   }
   static void operator delete[](void* ptr)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete[]", ptr));
   }
   static void operator delete[](void* ptr, const std::nothrow_t&)
   {
      if (ptr) h2_runner::failing(h2_stack::I().rel_piece("delete[] nothrow", ptr));
   }
};

struct h2_override_stdlib {
   h2_list stubs;

   static char* strdup(char* s)
   {
      char* ret = (char*)h2_override::malloc(strlen(s) + 1);
      if (ret) strcpy(ret, s);
      return ret;
   }

   void set()
   {
      h2_stubs::add(stubs, (void*)::free, (void*)h2_override::free, "free", H2_FILINE);
      h2_stubs::add(stubs, (void*)::malloc, (void*)h2_override::malloc, "malloc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::realloc, (void*)h2_override::realloc, "realloc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::calloc, (void*)h2_override::calloc, "calloc", H2_FILINE);
#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L
      h2_stubs::add(stubs, (void*)::posix_memalign, (void*)h2_override::posix_memalign, "posix_memalign", H2_FILINE);
#endif
#if defined _ISOC11_SOURCE
      h2_stubs::add(stubs, (void*)::aligned_alloc, (void*)h2_override::aligned_alloc, "aligned_alloc", H2_FILINE);
#endif
      // valloc pvalloc memalign deprecated
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t))::operator new), (void*)((void* (*)(std::size_t))h2_override::operator new), "new", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new), "new nothrow", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t))::operator new[]), (void*)((void* (*)(std::size_t))h2_override::operator new[]), "new[]", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new[]), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new[]), "new[] nothrow", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*))::operator delete), (void*)((void (*)(void*))h2_override::operator delete), "delete", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*, const std::nothrow_t&))::operator delete), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete), "delete nothrow", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*))::operator delete[]), (void*)((void (*)(void*))h2_override::operator delete[]), "delete[]", H2_FILINE);
      h2_stubs::add(stubs, (void*)((void (*)(void*, const std::nothrow_t&))::operator delete[]), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete[]), "delete[] nothrow", H2_FILINE);
   }

   void reset() { h2_stubs::clear(stubs); }
};
