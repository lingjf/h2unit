
struct h2_describe {
   const char* desc;
   const char* name = "";
   const char* tags[64]{nullptr};
   char nbuf[512], tbuf[512];

   h2_describe(const char* describe);
   void split();
   bool has_tag(const char* pattern);
};
