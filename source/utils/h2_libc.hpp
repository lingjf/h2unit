struct h2_libc {
   static void* malloc(size_t size);
   static void free(void* ptr);
   static void* operator new(std::size_t size) { return malloc(size); }
   static void operator delete(void* ptr) { free(ptr); }

   static int write(int fd, const void* buf, size_t count);
};
