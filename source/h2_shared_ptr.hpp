
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
