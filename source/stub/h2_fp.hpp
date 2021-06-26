
template <typename T>
void* h2_fp(T p)
{
   void* fp = (void*)p;
   if (std::is_convertible<T, h2::h2_string>::value) {
#if defined _WIN32
      fp = (void*)h2_nm::get((const char*)p);
      if (!fp)
         h2_color::prints("yellow", "\nDon't find %s\n", (const char*)p);
#else
      h2_symbol* res[100];
      int n = h2_nm::find((const char*)p, res, 100);
      if (n != 1) {
         if (n == 0) {
            h2_color::prints("yellow", "\nDon't find %s\n", (const char*)p);
         } else {
            h2_color::prints("yellow", "\nFind multiple %s :\n", (const char*)p);
            for (int i = 0; i < n; ++i)
               h2_color::prints("yellow", "  %d. %s \n", i + 1, res[i]->name.c_str());
         }
         return nullptr;
      }
      fp = (void*)(res[0]->addr + h2_nm::text_offset());
#endif
   }
   return fp;
}
