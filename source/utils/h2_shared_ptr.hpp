template <typename T>
struct h2_shared_ptr {
   h2_shared_ptr() = default;
   explicit h2_shared_ptr(T* p) { acquire(p, nullptr); }
   h2_shared_ptr(const h2_shared_ptr& that) { acquire(that.px, that.pn); }
   ~h2_shared_ptr() { release(); }
   h2_shared_ptr& operator=(h2_shared_ptr that)
   {
      std::swap(px, that.px);
      std::swap(pn, that.pn);
      return *this;
   }
   void reset() { release(); }
   operator bool() const { return pn && 0 < *pn; }
   T& operator*() const { return *px; }
   T* operator->() const { return px; }

   void acquire(T* p, long* n)
   {
      pn = n;
      if (p) pn ? ++*pn : *(pn = (long*)h2_libc::malloc(sizeof(long))) = 1;
      px = p;
   }
   void release()
   {
      if (pn && !--*pn) {
         delete px;
         h2_libc::free(pn);
      }
      px = nullptr;
      pn = nullptr;
   }

   T* px = nullptr;
   long* pn = nullptr;
};
