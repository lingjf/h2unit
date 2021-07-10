
struct h2_exempt {
   h2_singleton(h2_exempt);
   void* fps[10000];
   int nfp = 0;
   static void setup();
   static void add_by_fp(void* fp);
   static void add_by_name(const char* fn);
};

template <typename T>
inline void h2_unmem(T f) { h2_exempt::add_by_fp((void*)f); }
template <>
inline void h2_unmem(const char* f) { h2_exempt::add_by_name(f); }

#define H2UNMEM(f) h2::h2_unmem(f)
