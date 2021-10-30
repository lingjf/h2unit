struct h2_override_platform {
   h2_list stubs;

   static char* strdup(char* s)
   {
      char* ret = (char*)h2_override::malloc(strlen(s) + 1);
      return ret && strcpy(ret, s), ret;
   }
   static char* strndup(char* s, size_t n)
   {
      char* ret = (char*)h2_override::malloc(n + 1);
      return ret && strncpy(ret, s, n), ret;
   }

   void set()
   {
      h2_stubs::add(stubs, (void*)::strdup, (void*)strdup, "strdup", H2_FILE);
#if defined __CYGWIN__
      h2_stubs::add(stubs, (void*)::strndup, (void*)strndup, "strndup", H2_FILE);
#endif
   }
   void reset() { h2_stubs::clear(stubs); }
};
