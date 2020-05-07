
struct h2_defer_fail : h2_once {
   int w_type;
   const char *e_expr, *a_expr;
   const char* file;
   int line;
   h2_fail* fail;
   h2_ostringstream oss;

   h2_defer_fail(int w_type_, const char* e_expr_, const char* a_expr_, const char* file_, int line_)
     : w_type(w_type_), e_expr(e_expr_), a_expr(a_expr_), file(file_), line(line_), fail(nullptr) {}

   ~h2_defer_fail()
   {
      if (fail) {
         fail->set_w_type(w_type);
         fail->set_e_expr(e_expr);
         fail->set_a_expr(a_expr);
         fail->uprintf("%s", oss.str().c_str());
         fail->locate(file, line);
         h2_fail_g(fail);
      }
   }
};

static inline h2_ostringstream& h2_OK1(bool a, h2_defer_fail* d)
{
   if (!a) {
      h2_fail_unexpect* fail = new h2_fail_unexpect();
      fail->aprintf("false");
      d->fail = fail;
   }
   return d->oss;
}

template <typename E, typename A>
static inline h2_ostringstream& h2_OK2(E e, A a, h2_defer_fail* d)
{
   h2::h2_matcher<typename h2_decay<A>::type> m = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e);
   d->fail = m.matches((typename h2_decay<A>::type)a);
   return d->oss;
}

static inline h2_ostringstream& h2_JE(h2_string e, h2_string a, h2_defer_fail* d)
{
   h2::h2_matcher<h2_string> m = Je(e);
   d->fail = m.matches(a);
   return d->oss;
}

#define __H2OK2(condition, Qt) \
   for (h2::h2_defer_fail Qt(1, "", #condition, __FILE__, __LINE__); Qt;) h2::h2_OK1(condition, &Qt)

#define __H2OK3(expect, actual, Qt) \
   for (h2::h2_defer_fail Qt(2, #expect, #actual, __FILE__, __LINE__); Qt;) h2::h2_OK2(expect, actual, &Qt)

#define __H2JE(expect, actual, Qt) \
   for (h2::h2_defer_fail Qt(3, #expect, #actual, __FILE__, __LINE__); Qt;) h2::h2_JE(expect, actual, &Qt)

#define H2OK(...) H2PP_VARIADIC_CALL(__H2OK, __VA_ARGS__, H2Q(t_defer_fail))

#define H2JE(expect, actual) __H2JE(expect, actual, H2Q(t_defer_fail))
