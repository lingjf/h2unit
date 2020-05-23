
struct h2_libc_malloc {
   h2_singleton(h2_libc_malloc);

   struct buddy {
      size_t size;
      h2_list x;
   };

   int pages;
   h2_list buddies;

   h2_libc_malloc() : pages(0) {}

   void merge()
   {
      buddy* b = nullptr;
      h2_list_for_each_entry(p, &buddies, buddy, x)
      {
         if (b) {
            if (((char*)b) + b->size == (char*)p) {
               b->size += p->size;
               p->x.out();
               continue;
            }
         }
         b = p;
      }
   }

   void insert(buddy* b)
   {
      buddy* n = nullptr;
      h2_list_for_each_entry(p, &buddies, buddy, x)
      {
         if (((char*)b) + b->size <= (char*)p) {
            n = p;
            break;
         }
      }
      if (n)
         n->x.add_before(&b->x);
      else
         buddies.add_tail(&b->x);

      merge();
   }

   void* malloc(size_t size)
   {
      size = (size + 7) / 8 * 8;
      buddy* b = nullptr;
      h2_list_for_each_entry(p, &buddies, buddy, x)
      {
         if (size <= p->size - sizeof(p->size)) {
            b = p;
            p->x.out();
            break;
         }
      }
      if (!b) {
         int pagesize = h2_page_size();
         int pagecount = ::ceil((size + sizeof(b->size)) / (double)pagesize);
         if (pages == 0) pagecount = 1024 * 25;
         pages += pagecount;
#ifdef _WIN32
         PVOID ptr = VirtualAlloc(NULL, pagecount * pagesize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
         if (ptr == NULL) ::printf("VirtualAlloc failed\n at %s:%d", __FILE__, __LINE__), abort();
#else
         void* ptr = ::mmap(nullptr, pagecount * pagesize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
         if (ptr == MAP_FAILED) ::printf("mmap failed\n at %s:%d", __FILE__, __LINE__), abort();
#endif
         b = (buddy*)ptr;
         b->size = pagecount * pagesize;
      }
      size_t bz = b->size;
      b->size = size + sizeof(b->size);
      size_t rz = bz - b->size;
      if (sizeof(buddy) <= rz) {
         buddy* r = (buddy*)(((char*)b) + b->size);
         r->size = rz;
         insert(r);
      } else {
         b->size += rz;
      }

      return (void*)&b->x;
   }

   void free(void* ptr)
   {
      buddy* b = (buddy*)(((char*)ptr) - sizeof(b->size));
      insert(b);
   }
};

h2_inline void* h2_libc::malloc(size_t size)
{
   if (!O.memory_check) {
      return ::malloc(size);
   }

   return h2_libc_malloc::I().malloc(size + 10);
}

h2_inline void h2_libc::free(void* ptr)
{
   if (!O.memory_check) {
      ::free(ptr);
      return;
   }

   if (ptr) h2_libc_malloc::I().free(ptr);
}

h2_inline ssize_t h2_libc::write(int fd, const void* buf, size_t count)
{
#ifdef _WIN32
   return _write(fd, buf, count);
#else
   return ::syscall(SYS_write, fd, buf, count);
#endif
}
