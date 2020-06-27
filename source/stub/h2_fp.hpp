
template <typename T>
void* h2_fp(T p)
{
   void* fp = (void*)p;
   if (std::is_convertible<T, h2::h2_string>::value) {
      fp = (void*)(h2_nm::get((const char*)p) + h2_nm::text_offset());
   }
   return fp;
}
