
struct h2_libc {
   static void* malloc(size_t sz);
   static void free(void* ptr);
   static int write(FILE* stream, const void* buf, size_t nbyte);
};
