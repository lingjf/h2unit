
struct h2_libc_malloc {
   h2_singleton(h2_libc_malloc);

   struct blob {
      size_t size;
      h2_list link;
   };

   char buffer[1024 * 1024 * 100];
   h2_list blobs;

   h2_libc_malloc()
   {
      blob* b = (blob*)buffer;
      b->size = sizeof(buffer);
      blobs.add_tail(&b->link);
   }

   void merge()
   {
      blob* b = nullptr;
      h2_list_for_each_entry(p, &blobs, blob, link)
      {
         if (b) {
            if (((char*)b) + b->size == (char*)p) {
               b->size += p->size;
               p->link.out();
               continue;
            }
         }
         b = p;
      }
   }

   void insert(blob* b)
   {
      blob* n = nullptr;
      h2_list_for_each_entry(p, &blobs, blob, link)
      {
         if (((char*)b) + b->size <= (char*)p) {
            n = p;
            break;
         }
      }
      if (n)
         n->link.add_before(&b->link);
      else
         blobs.add_tail(&b->link);

      merge();
   }

   void* malloc(size_t size)
   {
      size = (size + 7) / 8 * 8;
      blob* b = nullptr;
      h2_list_for_each_entry(p, &blobs, blob, link)
      {
         if (size <= p->size - sizeof(p->size)) {
            b = p;
            p->link.out();
            break;
         }
      }
      if (!b) {
         int pagesize = h2_page_size();
         int pagecount = ::ceil((size + sizeof(b->size)) / (double)pagesize);

#ifdef _WIN32
         PVOID ptr = VirtualAlloc(NULL, pagecount * pagesize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
         if (ptr == NULL) return nullptr;
#else
         void* ptr = ::mmap(nullptr, pagecount * pagesize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
         if (ptr == MAP_FAILED) return nullptr;
#endif
         b = (blob*)ptr;
         b->size = pagecount * pagesize;
      }
      size_t bz = b->size;
      b->size = size + sizeof(b->size);
      size_t rz = bz - b->size;
      if (sizeof(blob) <= rz) {
         blob* r = (blob*)(((char*)b) + b->size);
         r->size = rz;
         insert(r);
      } else {
         b->size += rz;
      }

      return (void*)&b->link;
   }

   void free(void* ptr)
   {
      blob* b = (blob*)(((char*)ptr) - sizeof(b->size));
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
