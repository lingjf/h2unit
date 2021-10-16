struct h2_cout : h2_once {
   h2_fs fs;
   h2_matcher<const char*> m;
   const char *e, *type;
   h2_cout(h2_matcher<const char*> m, const char* e, const char* type, const h2_fs& fs);
   ~h2_cout();
};

#define __H2COUT(Matcher_, expression, type, Q) for (h2::h2_cout Q(Matcher_, expression, h2::ss(type), {__FILE__, __LINE__}); Q;)
#define H2COUT(Matcher_, ...) __H2COUT(Matcher_, #Matcher_, #__VA_ARGS__, H2PP_UNIQUE())
