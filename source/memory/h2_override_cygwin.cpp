struct h2_override_platform {
   h2_list stubs;

   static char* strndup(char* s, size_t n)
   {
      char* ret = (char*)h2_override::malloc(n + 1);
      if (ret) {
         strncpy(ret, s, n);
         ret[n] = '\0';
      }
      return ret;
   }

   void set()
   {
      h2_stubs::add(stubs, (void*)::strdup, (void*)h2_override_stdlib::strdup, "strdup", H2_FILE);
#if defined __CYGWIN__
      h2_stubs::add(stubs, (void*)::strndup, (void*)strndup, "strndup", H2_FILE);
#endif
   }
   void reset() { h2_stubs::clear(stubs); }
};
