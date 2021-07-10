
template <typename T>
void* h2_fp(T p)
{
   void* fp = (void*)p;
   if (std::is_convertible<T, h2::h2_string>::value) {
      h2_symbol* res[16];
      int n = h2_nm::get_by_name((const char*)p, res, 16);
      if (n != 1) {
         h2_color::prints("yellow", n ? "\nFind multiple %s :\n" : "\nDon't find %s\n", (const char*)p);
         for (int i = 0; i < n; ++i)
            h2_color::prints("yellow", "  %d. %s \n", i + 1, res[i]->name);
         return nullptr;
      }
      fp = h2_load::addr_to_ptr(res[0]->addr);
   }
   return fp;
}
