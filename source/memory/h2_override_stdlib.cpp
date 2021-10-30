struct h2_override_stdlib {
   h2_list stubs;

   void set()
   {
      h2_stubs::add(stubs, (void*)::free, (void*)h2_override::free, "free", H2_FILE);
      h2_stubs::add(stubs, (void*)::malloc, (void*)h2_override::malloc, "malloc", H2_FILE);
      h2_stubs::add(stubs, (void*)::realloc, (void*)h2_override::realloc, "realloc", H2_FILE);
      h2_stubs::add(stubs, (void*)::calloc, (void*)h2_override::calloc, "calloc", H2_FILE);
#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L
      h2_stubs::add(stubs, (void*)::posix_memalign, (void*)h2_override::posix_memalign, "posix_memalign", H2_FILE);
#endif
#if defined _ISOC11_SOURCE
      h2_stubs::add(stubs, (void*)::aligned_alloc, (void*)h2_override::aligned_alloc, "aligned_alloc", H2_FILE);
#endif
      // valloc pvalloc memalign deprecated
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t))::operator new), (void*)((void* (*)(std::size_t))h2_override::operator new), "new", H2_FILE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new), "new nothrow", H2_FILE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t))::operator new[]), (void*)((void* (*)(std::size_t))h2_override::operator new[]), "new[]", H2_FILE);
      h2_stubs::add(stubs, (void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new[]), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new[]), "new[] nothrow", H2_FILE);
      h2_stubs::add(stubs, (void*)((void (*)(void*))::operator delete), (void*)((void (*)(void*))h2_override::operator delete), "delete", H2_FILE);
      h2_stubs::add(stubs, (void*)((void (*)(void*, const std::nothrow_t&))::operator delete), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete), "delete nothrow", H2_FILE);
      h2_stubs::add(stubs, (void*)((void (*)(void*))::operator delete[]), (void*)((void (*)(void*))h2_override::operator delete[]), "delete[]", H2_FILE);
      h2_stubs::add(stubs, (void*)((void (*)(void*, const std::nothrow_t&))::operator delete[]), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete[]), "delete[] nothrow", H2_FILE);
   }

   void reset() { h2_stubs::clear(stubs); }
};
