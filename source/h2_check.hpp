
struct h2_defer_fail : h2_once {
   const char* check_type;
   const char *e_expression, *a_expression, *expression;
   const char* file;
   int line;
   h2_fail* fail{nullptr};
   h2_ostringstream oss;

   h2_defer_fail(const char* e_expression_, const char* a_expression_, const char* expression_, const char* file_, int line_);
   ~h2_defer_fail();
};

static inline h2_ostringstream& h2_OK(h2_defer_fail* d, bool a)
{
   d->check_type = "OK1";
   if (!a) d->fail = h2_fail::new_unexpect("true", "false");
   h2_check_g();
   return d->oss;
}

template <typename E, typename A>
static inline h2_ostringstream& h2_OK(h2_defer_fail* d, E e, A a, int n = 0)
{
   d->check_type = "OK2";
   h2::h2_matcher<typename h2_decay<A>::type> m = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e);
   h2_fail* fail = m.matches((typename h2_decay<A>::type)a, n);
   d->fail = fail;
   if (fail && fail->subling_next) {
      d->fail = h2_fail::new_unexpect();
      h2_fail::append_child(d->fail, fail);
   }
   h2_check_g();
   return d->oss;
}

static inline h2_ostringstream& h2_JE(h2_defer_fail* d, h2_string e, h2_string a)
{
   d->check_type = "JE";
   h2::h2_matcher<h2_string> m = Je(e);
   d->fail = m.matches(a);
   h2_check_g();
   return d->oss;
}

#define __H2OK(Qt, expression, ...) \
   for (h2::h2_defer_fail Qt("", "", expression, __FILE__, __LINE__); Qt;) h2::h2_OK(&Qt, __VA_ARGS__)

#define __H2JE(Qt, expect, actual) \
   for (h2::h2_defer_fail Qt(#expect, #actual, "", __FILE__, __LINE__); Qt;) h2::h2_JE(&Qt, expect, actual)

#define H2OK(...) __H2OK(H2Q(t_defer_fail), (#__VA_ARGS__), __VA_ARGS__)

#define H2JE(expect, actual) __H2JE(H2Q(t_defer_fail), expect, actual)
