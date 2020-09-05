
struct h2_once {
   operator bool()
   {
      bool ret = c == 0;
      c = 1;
      return ret;
   }

 private:
   int c = 0;
};

struct h2_with {
   FILE* f;
   int (*c)(FILE*);
   h2_with(FILE* file, int (*close)(FILE*) = ::fclose) : f(file), c(close) {}
   ~h2_with() { (f && c) && c(f); }
};

struct h2_pattern {
   static bool regex_match(const char* pattern, const char* subject, bool caseless = false);
   static bool wildcard_match(const char* pattern, const char* subject, bool caseless = false);
   static bool match(const char* pattern, const char* subject, bool caseless = false);
};

static inline const char* comma_if(bool a, const char* t = ", ", const char* f = "") { return a ? t : f; };

#define h2_singleton(_Class) \
   static _Class& I()        \
   {                         \
      static _Class i;       \
      return i;              \
   }
