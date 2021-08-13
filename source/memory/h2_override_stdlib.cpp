
struct h2_override_stdlib {
   h2_stubs stubs;

   void set()
   {
      stubs.add((void*)::free, (void*)h2_override::free, "free", __FILE__, __LINE__);
      stubs.add((void*)::malloc, (void*)h2_override::malloc, "malloc", __FILE__, __LINE__);
      stubs.add((void*)::realloc, (void*)h2_override::realloc, "realloc", __FILE__, __LINE__);
      stubs.add((void*)::calloc, (void*)h2_override::calloc, "calloc", __FILE__, __LINE__);
#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L
      stubs.add((void*)::posix_memalign, (void*)h2_override::posix_memalign, "posix_memalign", __FILE__, __LINE__);
#endif
#if defined _ISOC11_SOURCE
      stubs.add((void*)::aligned_alloc, (void*)h2_override::aligned_alloc, "aligned_alloc", __FILE__, __LINE__);
#endif
      // valloc pvalloc memalign deprecated
      stubs.add((void*)((void* (*)(std::size_t))::operator new), (void*)((void* (*)(std::size_t))h2_override::operator new), "new", __FILE__, __LINE__);
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new), "new nothrow", __FILE__, __LINE__);
      stubs.add((void*)((void* (*)(std::size_t))::operator new[]), (void*)((void* (*)(std::size_t))h2_override::operator new[]), "new[]", __FILE__, __LINE__);
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new[]), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new[]), "new[] nothrow", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*))::operator delete), (void*)((void (*)(void*))h2_override::operator delete), "delete", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete), "delete nothrow", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*))::operator delete[]), (void*)((void (*)(void*))h2_override::operator delete[]), "delete[]", __FILE__, __LINE__);
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete[]), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete[]), "delete[] nothrow", __FILE__, __LINE__);
   }

   void reset() { stubs.clear(); }
};
