
struct h2_cout : h2_once {
   const char* file;
   int line;
   h2_matcher<const char*> m;
   const char *e, *type;
   h2_cout(h2_matcher<const char*> m, const char* e, const char* type, const char* file, int line);
   ~h2_cout();
   static size_t length();
};

#define __H2COUT(m, e, type, Q) for (h2::h2_cout Q(m, e, type, __FILE__, __LINE__); Q;)
#define H2COUT(m, ...) __H2COUT(m, #m, #__VA_ARGS__, H2PP_UNIQUE(t_cout))
