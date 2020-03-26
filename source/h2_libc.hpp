
struct h2_libc {
   static void* malloc(size_t sz);
   static void free(void* ptr);
   static int write(FILE* stream, const void* buf, size_t nbyte);

   static void* operator new(std::size_t sz) { return malloc(sz); }
   static void operator delete(void* ptr) { free(ptr); }
};
