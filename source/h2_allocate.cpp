
struct h2_raw {
   static void* malloc(size_t sz) {
      if (!h2_cfg().memory_check) return ::malloc(sz);
      void* ptr = mmap(nullptr, sz, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      if (ptr == MAP_FAILED) return nullptr;

      uintptr_t* p = (uintptr_t*)ptr;
      *p = (uintptr_t)sz;
      return (void*)(p + 1);
   }

   static void free(void* ptr) {
      if (!h2_cfg().memory_check) return ::free(ptr);
      if (!ptr) return;
      uintptr_t* p = ((uintptr_t*)ptr) - 1;
      munmap((void*)p, (size_t)*p);
   }
};

/* clang-format off */
template <typename T>
class h2_allocator {
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

   pointer allocate(size_type n, const void* = 0) { return (T*)h2_raw::malloc(n * sizeof(T)); }
   void deallocate(void* p, size_type) { h2_raw::free(p); }

   pointer address(reference x) const { return &x; }
   const_pointer address(const_reference x) const { return &x; }
   h2_allocator<T>& operator=(const h2_allocator&) { return *this; }
   void construct(pointer p, const T& val) { new ((T*)p) T(val); }
   void destroy(pointer p) { p->~T(); }

   size_type max_size() const { return size_t(-1); }

   template <typename U> struct rebind { typedef h2_allocator<U> other; };

   template <typename U> h2_allocator(const h2_allocator<U>&) {}

   template <typename U> h2_allocator& operator=(const h2_allocator<U>&) { return *this; }
};

template <typename T> inline bool operator==(const h2_allocator<T>&, const h2_allocator<T>&) { return true; }
template <typename T> inline bool operator!=(const h2_allocator<T>&, const h2_allocator<T>&) { return false; }

template <typename T> using h2_vector = std::vector<T, h2_allocator<T>>;
typedef std::basic_ostringstream<char, std::char_traits<char>, h2_allocator<char>> h2_ostringstream;
/* clang-format on */

template <typename T>
class h2_shared_ptr {
 public:
   h2_shared_ptr() : px(nullptr), pn(nullptr) {}
   explicit h2_shared_ptr(T* p) { acquire(p, nullptr); }
   h2_shared_ptr(const h2_shared_ptr& that) { acquire(that.px, that.pn); }
   ~h2_shared_ptr() { release(); }
   h2_shared_ptr& operator=(h2_shared_ptr that) {
      std::swap(px, that.px);
      std::swap(pn, that.pn);
      return *this;
   }
   operator bool() const { return pn && 0 < *pn; }
   T& operator*() const { return *px; }
   T* operator->() const { return px; }

 private:
   void acquire(T* p, long* n) {
      pn = n;
      if (p) {
         if (!pn)
            pn = new (h2_raw::malloc(sizeof(long))) long(1);
         else
            ++(*pn);
      }
      px = p;
   }
   void release() {
      if (pn && !--(*pn)) {
         delete px;
         h2_raw::free(pn);
      }
   }

   T* px;
   long* pn;

   static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
   static void operator delete(void* ptr) { h2_raw::free(ptr); }
};

class h2_string : public std::basic_string<char, std::char_traits<char>, h2_allocator<char>> {
 public:
   h2_string() : basic_string() {}
   h2_string(const h2_string& __str) : basic_string(__str.c_str()) {}
   h2_string(const char* __s) : basic_string(__s) {}
   h2_string(const std::string& __s) : basic_string(__s.c_str()) {}
   h2_string(size_t __n, char __c) : basic_string(__n, __c) {}

   h2_string& operator=(const h2_string& __str) { return this->assign(__str.c_str()), *this; }
   h2_string& operator=(const char* __s) { return this->assign(__s), *this; }
   h2_string& operator=(const std::string& __str) { return this->assign(__str.c_str()), *this; }
   h2_string& operator=(char __c) { return this->assign(1, __c), *this; }

   h2_string& operator+=(const h2_string& __str) { return this->append(__str.c_str()), *this; }
   h2_string& operator+=(const char* __s) { return this->append(__s), *this; }
   h2_string& operator+=(const std::string& __str) { return this->append(__str.c_str()), *this; }
   h2_string& operator+=(char __c) { return this->push_back(__c), *this; }

   h2_string& printf(const char* format, ...) {
      va_list a;

      va_start(a, format);
#if defined _WIN32
      int length = _vscprintf(format, a);
#else
      int length = vsnprintf(NULL, 0, format, a);
#endif
      va_end(a);

      char* t = (char*)alloca(length + 1);

      va_start(a, format);
      vsprintf(t, format, a);
      va_end(a);

      append(t);

      return *this;
   }

   bool equals(h2_string __str, bool caseless = false) const {
      h2_string a2(this->c_str()), e2(__str);
      if (caseless) {
         for (auto& c : a2) c = tolower(c);
         for (auto& c : e2) c = tolower(c);
      }
      return a2 == e2;
   }

   bool contains(h2_string __substr, bool caseless = false) const {
      h2_string a2(this->c_str()), e2(__substr);
      if (caseless) {
         for (auto& c : a2) c = tolower(c);
         for (auto& c : e2) c = tolower(c);
      }
      return a2.find(e2) != h2_string::npos;
   }

   bool startswith(h2_string __prefix, bool caseless = false) const {
      h2_string a2(this->c_str()), e2(__prefix);
      if (caseless) {
         for (auto& c : a2) c = tolower(c);
         for (auto& c : e2) c = tolower(c);
      }
      return a2.length() >= e2.length() && a2.substr(0, e2.length()) == e2;
   }

   bool endswith(h2_string __suffix, bool caseless = false) const {
      h2_string a2(this->c_str()), e2(__suffix);
      if (caseless) {
         for (auto& c : a2) c = tolower(c);
         for (auto& c : e2) c = tolower(c);
      }
      return a2.length() >= e2.length() && a2.substr(a2.length() - e2.length()) == e2;
   }
};

/* clang-format off */
inline h2_string operator+(const h2_string& lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char* rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const char* lhs, const h2_string& rhs) { h2_string s(lhs); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const std::string& rhs) { h2_string s(lhs); s.append(rhs.c_str()); return s; }
inline h2_string operator+(const std::string& lhs, const h2_string& rhs) { h2_string s(lhs.c_str()); s.append(rhs); return s; }
inline h2_string operator+(const h2_string& lhs, const char rhs) { h2_string s(lhs); s.push_back(rhs); return s; }
inline h2_string operator+(const char lhs, const h2_string& rhs) { h2_string s(1, lhs); s.append(rhs); return s; }
/* clang-format on */
