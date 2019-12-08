
#ifndef ___H2_LEAK__H___
#define ___H2_LEAK__H___

static unsigned char h2_over_magic[] = {0xbe, 0xaf, 0xca, 0xfe, 0xc0, 0xde, 0xfa, 0xce};

struct h2_mm
{
   void* ptr;
   int size;
   bool escape;
   h2_backtrace bt;
   signed char data[0];

   static void __gen_aligned_ptr(h2_mm* mm, int alignment)
   {
      if (alignment == 0) {
         alignment = 8;
      }
      signed char* p1 = &mm->data[1 + sizeof(h2_over_magic)];
      signed char* p2 = (signed char*)H2_ALIGN_UP((uint64_t)p1, alignment);
      p2[-1 - sizeof(h2_over_magic)] = (signed char)((int64_t)p2 - (int64_t)mm->data);
      mm->ptr = (void*)p2;
   }

   static h2_mm* container_of(void* ptr)
   {
      return (h2_mm*)(((signed char*)ptr) - sizeof(h2_mm) - ((signed char*)ptr)[-1 - sizeof(h2_over_magic)]);
   }

   static h2_mm* allocate(int size, int alignment, h2_backtrace& bt)
   {
      h2_mm* mm = (h2_mm*)h2_alloc::U().malloc(sizeof(h2_mm) + size + alignment + 32 + sizeof(h2_over_magic) * 2);
      if (!mm) {
         return nullptr;
      }

      mm->size = size;
      mm->bt = bt;

      __gen_aligned_ptr(mm, alignment);

      memcpy((unsigned char*)mm->ptr + mm->size, h2_over_magic, sizeof(h2_over_magic));
      memcpy((unsigned char*)mm->ptr - sizeof(h2_over_magic), h2_over_magic, sizeof(h2_over_magic));

      return mm;
   }

   static h2_fail* release(h2_mm* mm)
   {
      h2_fail_memover* fail1 = nullptr;
      h2_fail_memover* fail2 = nullptr;

      /* overflow and under-flow checking */
      if (memcmp((unsigned char*)mm->ptr + mm->size, h2_over_magic, sizeof(h2_over_magic))) {
         fail1 = new h2_fail_memover();
         fail1->add(mm->ptr, mm->size, h2_over_magic, sizeof(h2_over_magic), mm->bt);
      }

      if (memcmp((unsigned char*)mm->ptr - sizeof(h2_over_magic), h2_over_magic, sizeof(h2_over_magic))) {
         fail2 = new h2_fail_memover();
         fail2->add(mm->ptr, -(int)sizeof(h2_over_magic), h2_over_magic, sizeof(h2_over_magic), mm->bt);
      }

      if (fail1) {
         fail1->appendx(fail2);
      }

      h2_alloc::U().free(mm);

      return fail1 ? fail1 : fail2;
   }
};

class h2_leak_stack
{
 private:
   struct block
   {
      std::vector<void*, h2_allocator<void*>> mm_list;

      const char* file;
      int line;
      const char* where;
      long long limited;
      const char* fill;

      block(const char* _file, int _line, const char* _where, long long _limited, const char* _fill)
        : file(_file), line(_line), where(_where), limited(_limited), fill(_fill) {}

      bool escape(h2_backtrace& bt)
      {
         struct
         {
            unsigned char* base;
            int size;
         } exclude_functions[] = {
           {(unsigned char*)sprintf, 300},
           {(unsigned char*)sscanf, 300},
           {(unsigned char*)localtime, 300}};

         for (int i = 0; i < H2_ARRAY_COUNTOF(exclude_functions); ++i) {
            if (bt.has(exclude_functions[i].base, exclude_functions[i].size)) {
               return true;
            }
         }
         return false;
      }

      h2_mm* new_mm(int size, int alignment, const char* fill, h2_backtrace& bt)
      {
         if (limited < size) {
            return nullptr;
         }
         h2_mm* mm = h2_mm::allocate(size, alignment, bt);
         if (!fill) {
            fill = this->fill;
         }
         if (fill) {
            int length = strlen(fill);
            for (int i = 0, j = 0; i < size; ++i, ++j) {
               if (j >= length) j = 0;
               ((char*)mm->ptr)[i] = fill[j];
            }
         }

         mm->escape = escape(bt);
         if (!mm->escape) {
            mm_list.push_back(mm->ptr);
         }
         return mm;
      }

      h2_mm* get_mm(void* ptr)
      {
         for (auto it = mm_list.begin(); it != mm_list.end(); it++) {
            if (*it == ptr) {
               return h2_mm::container_of(ptr);
            }
         }
         return nullptr;
      }

      h2_fail* rel_mm(h2_mm* mm)
      {
         limited += mm->size;
         for (auto it = mm_list.begin(); it != mm_list.end(); it++) {
            if (*it == mm->ptr) {
               mm_list.erase(it);
               return h2_mm::release(mm);
            }
         }

         return nullptr;
      }

      h2_fail* check()
      {
         h2_fail_memleak* fail = nullptr;

         if (!mm_list.empty()) {
            fail = new h2_fail_memleak(file, line, where);
            for (auto it = mm_list.begin(); it != mm_list.end(); it++) {
               h2_mm* mm = h2_mm::container_of(*it);
               fail->add(mm->ptr, mm->size, mm->bt);
            }
         }
         return fail;
      }

      static void* operator new(std::size_t sz)
      {
         return h2_alloc::I().malloc(sz);
      }
      static void operator delete(void* ptr)
      {
         h2_alloc::I().free(ptr);
      }
   };

   block* stack[100];
   int top;

 public:
   h2_leak_stack()
     : top(0) {}

   bool push(const char* file, int line, const char* where, long long limited = 0x7fffffffffffLL, const char* fill = NULL)
   {
      if (top > 64) {
         printf("\nWarning: nested leak block too many! %s:%d\n", file, line);
      }
      stack[top++] = new block(file, line, where, limited, fill);
      return true;
   }

   h2_fail* pop()
   {
      block* b = stack[--top];
      auto fail = b->check();
      delete b;
      return fail;
   }

   h2_mm* new_mm(size_t size, size_t alignment, const char* fill, h2_backtrace& bt)
   {
      if (top == 0) {
         printf("\nWarning: malloc in idle state! %s:%d\n", __FUNCTION__, __LINE__);
         return nullptr;
      }
      return stack[top - 1]->new_mm(size, alignment, fill, bt);
   }

   h2_mm* get_mm(void* ptr)
   {
      for (int i = top - 1; i >= 0; --i) {
         h2_mm* mm = stack[i]->get_mm(ptr);
         if (mm) {
            return mm;
         }
      }

      return nullptr;
   }

   h2_fail* rel_mm(h2_mm* mm)
   {
      if (mm->escape) {
         return nullptr;
      }

      if (top == 0) {
         printf("\nWarning: free in idle state! \n");
         return nullptr;
      }

      for (int i = top - 1; i >= 0; --i) {
         if (stack[i]->get_mm(mm->ptr)) {
            return stack[i]->rel_mm(mm);
         }
      }

      printf("\nWarning: free not found! \n");

      return nullptr;
   }
};

static inline h2_mm* h2_getm_g(void* ptr);
static inline h2_mm* h2_newm_g(size_t size, size_t alignment, const char* fill, h2_backtrace& bt);
static inline void h2_relm_g(h2_mm* mm);

struct h2_leak_inspector
{
   static void free(void* ptr)
   {
      if (ptr) {
         h2_mm* mm = h2_getm_g(ptr);
         if (mm) {
            h2_relm_g(mm);
         }
      }
   }

   static void* malloc(size_t size)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      h2_mm* mm = h2_newm_g(size, 0, nullptr, bt);
      return mm ? mm->ptr : nullptr;
   }

   static void* calloc(size_t count, size_t size)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      h2_mm* mm = h2_newm_g(size * count, 0, "\0", bt);
      return mm ? mm->ptr : nullptr;
   }

   static void* realloc(void* ptr, size_t size)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      if (size == 0) {
         if (ptr) {
            h2_mm* old_mm = h2_getm_g(ptr);
            if (old_mm) {
               h2_relm_g(old_mm);
            }
         }
         return nullptr;
      }

      h2_mm* old_mm = h2_getm_g(ptr);
      if (!old_mm) {
         return nullptr;
      }

      h2_mm* new_mm = h2_newm_g(size, 0, nullptr, bt);
      if (!new_mm) {
         return nullptr;
      }

      memcpy(new_mm->ptr, old_mm->ptr, old_mm->size);
      h2_relm_g(old_mm);

      return new_mm->ptr;
   }

   static int posix_memalign(void** memptr, size_t alignment, size_t size)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      h2_mm* mm = h2_newm_g(size, alignment, nullptr, bt);
      if (mm) {
         *memptr = mm->ptr;
         return 0;
      }
      // #ifndef ENOMEM
      // #define ENOMEM -1
      // #endif
      return ENOMEM;
   }

   static void* aligned_alloc(size_t alignment, size_t size)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      h2_mm* mm = h2_newm_g(size, alignment, nullptr, bt);
      return mm ? mm->ptr : nullptr;
   }

   static char* strdup(const char* s)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      size_t size = strlen(s) + 1;
      h2_mm* mm = h2_newm_g(size, 0, nullptr, bt);
      if (mm) {
         memcpy(mm->ptr, s, size);
      }
      return mm ? (char*)mm->ptr : nullptr;
   }

   static char* strndup(const char* s, size_t n)
   {
      h2_backtrace bt;
      bt.backtrace(2);

      size_t size = strlen(s);
      size = (size > n ? n : size) + 1;
      h2_mm* mm = h2_newm_g(size, 0, nullptr, bt);
      if (mm) {
         memcpy(mm->ptr, s, size - 1);
         ((char*)mm->ptr)[size - 1] = '\0';
      }
      return mm ? (char*)mm->ptr : nullptr;
   }
};

#endif
