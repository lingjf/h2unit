
h2_inline void* h2_libc::malloc(size_t sz) {
   if (!O.memory_check) {
      return ::malloc(sz);
   }
   void* ptr = mmap(nullptr, sz, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
   if (ptr == MAP_FAILED) return nullptr;

   uintptr_t* p = (uintptr_t*)ptr;
   *p = (uintptr_t)sz;
   return (void*)(p + 1);
}

h2_inline void h2_libc::free(void* ptr) {
   if (!O.memory_check) {
      ::free(ptr);
      return;
   }
   if (!ptr) return;
   uintptr_t* p = ((uintptr_t*)ptr) - 1;
   munmap((void*)p, (size_t)*p);
}

h2_inline ssize_t h2_libc::write(int fd, const void* buf, size_t count) {
   return syscall(SYS_write, fd, buf, count);
}
