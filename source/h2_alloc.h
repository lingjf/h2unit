
#ifndef ___H2_ALLOC__H___
#define ___H2_ALLOC__H___

struct h2_alloc
{
   static h2_alloc& I()
   {
      static h2_alloc I;
      return I;
   }
   static h2_alloc& U()
   {
      static h2_alloc U;
      return U;
   }

   h2_alloc() {}

   struct X
   {
      uint32_t size;
   };

   void* malloc(size_t size)
   {
      if (h2_cfg::I().memory_check) {
         void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

         if (ptr == MAP_FAILED) {
            return NULL;
         }

         X* p = (X*)ptr;
         p->size = size;
         return (void*)(p + 1);
      }
      else {
         return ::malloc(size);
      }
   }

   void free(void* ptr)
   {
      if (h2_cfg::I().memory_check) {
         X* p = (X*)ptr;
         p = p - 1;
         munmap(p, p->size);
      }
      else {
         ::free(ptr);
      }
   }
};

template <typename T>
class h2_allocator
{
 public:
   typedef size_t size_type;
   typedef ptrdiff_t difference_type;
   typedef T* pointer;
   typedef const T* const_pointer;
   typedef T& reference;
   typedef const T& const_reference;
   typedef T value_type;

   h2_allocator() {}
   h2_allocator(const h2_allocator&) {}

   pointer allocate(size_type n, const void* = 0)
   {
      T* t = (T*)h2_alloc::I().malloc(n * sizeof(T));
      return t;
   }

   void deallocate(void* p, size_type)
   {
      if (p) {
         h2_alloc::I().free(p);
      }
   }

   pointer address(reference x) const
   {
      return &x;
   }
   const_pointer address(const_reference x) const
   {
      return &x;
   }
   h2_allocator<T>& operator=(const h2_allocator&)
   {
      return *this;
   }
   void construct(pointer p, const T& val)
   {
      new ((T*)p) T(val);
   }
   void destroy(pointer p)
   {
      p->~T();
   }

   size_type max_size() const
   {
      return size_t(-1);
   }

   template <typename U>
   struct rebind
   {
      typedef h2_allocator<U> other;
   };

   template <typename U>
   h2_allocator(const h2_allocator<U>&) {}

   template <typename U>
   h2_allocator& operator=(const h2_allocator<U>&)
   {
      return *this;
   }
};

template <typename T>
inline bool
operator==(const h2_allocator<T>&, const h2_allocator<T>&)
{
   return true;
}

template <typename T>
inline bool
operator!=(const h2_allocator<T>&, const h2_allocator<T>&)
{
   return false;
}

typedef std::basic_string<char, std::char_traits<char>, h2_allocator<char>> h2_string;
typedef std::basic_ostringstream<char, std::char_traits<char>, h2_allocator<char>> h2_ostringstream;

#endif
