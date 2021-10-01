struct h2_override_stdlib {
   h2_stubs stubs;

   void set()
   {
      stubs.add((void*)::free, (void*)h2_override::free, {__FILE__, __LINE__, "free"});
      stubs.add((void*)::malloc, (void*)h2_override::malloc, {__FILE__, __LINE__, "malloc"});
      stubs.add((void*)::realloc, (void*)h2_override::realloc, {__FILE__, __LINE__, "realloc"});
      stubs.add((void*)::calloc, (void*)h2_override::calloc, {__FILE__, __LINE__, "calloc"});
#if defined _POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L
      stubs.add((void*)::posix_memalign, (void*)h2_override::posix_memalign, {__FILE__, __LINE__, "posix_memalign"});
#endif
#if defined _ISOC11_SOURCE
      stubs.add((void*)::aligned_alloc, (void*)h2_override::aligned_alloc, {__FILE__, __LINE__, "aligned_alloc"});
#endif
      // valloc pvalloc memalign deprecated
      stubs.add((void*)((void* (*)(std::size_t))::operator new), (void*)((void* (*)(std::size_t))h2_override::operator new), {__FILE__, __LINE__, "new"});
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new), {__FILE__, __LINE__, "new nothrow"});
      stubs.add((void*)((void* (*)(std::size_t))::operator new[]), (void*)((void* (*)(std::size_t))h2_override::operator new[]), {__FILE__, __LINE__, "new[]"});
      stubs.add((void*)((void* (*)(std::size_t, const std::nothrow_t&))::operator new[]), (void*)((void* (*)(std::size_t, const std::nothrow_t&))h2_override::operator new[]), {__FILE__, __LINE__, "new[] nothrow"});
      stubs.add((void*)((void (*)(void*))::operator delete), (void*)((void (*)(void*))h2_override::operator delete), {__FILE__, __LINE__, "delete"});
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete), {__FILE__, __LINE__, "delete nothrow"});
      stubs.add((void*)((void (*)(void*))::operator delete[]), (void*)((void (*)(void*))h2_override::operator delete[]), {__FILE__, __LINE__, "delete[]"});
      stubs.add((void*)((void (*)(void*, const std::nothrow_t&))::operator delete[]), (void*)((void (*)(void*, const std::nothrow_t&))h2_override::operator delete[]), {__FILE__, __LINE__, "delete[] nothrow"});
   }

   void reset() { stubs.clear(); }
};
