h2_inline bool h2_sz::empty() const
{
   return !file || file[0] == '\0';
}
h2_inline const char* h2_sz::basefile() const
{
   if (file)
      for (const char* p = file + strlen(file); file <= p; --p)
         if (*p == '/' || *p == '\\')
            return p + 1;
   return file;
}
