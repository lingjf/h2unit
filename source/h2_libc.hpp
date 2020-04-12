
struct h2_libc {
   static void* malloc(size_t sz);
   static void free(void* ptr);
   static ssize_t write(int fd, const void* buf, size_t count);
   static void* operator new(std::size_t sz) { return malloc(sz); }
   static void operator delete(void* ptr) { free(ptr); }
};
