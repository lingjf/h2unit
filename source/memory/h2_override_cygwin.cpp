struct h2_override_platform {
   h2_stubs stubs;

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
      stubs.add((void*)::strdup, (void*)strdup, {__FILE__, __LINE__, "strdup"});
#if defined __CYGWIN__
      stubs.add((void*)::strndup, (void*)strndup, {__FILE__, __LINE__, "strndup"});
#endif
   }
   void reset() { stubs.clear(); }
};
