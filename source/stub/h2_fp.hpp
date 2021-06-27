
template <typename T>
void* h2_fp(T p)
{
   void* fp = (void*)p;
   if (std::is_convertible<T, h2::h2_string>::value) {
      h2_symbol* res[16];
      int n = h2_nm::get_by_name((const char*)p, res, 16);
      if (n != 1) {
         if (n == 0) {
            h2_color::prints("yellow", "\nDon't find %s\n", (const char*)p);
         } else {
            h2_color::prints("yellow", "\nFind multiple %s :\n", (const char*)p);
            for (int i = 0; i < n; ++i)
               h2_color::prints("yellow", "  %d. %s \n", i + 1, res[i]->name);
         }
         return nullptr;
      }
      fp = h2_load::symbol_to_addr(res[0]->offset);
   }
   return fp;
}
