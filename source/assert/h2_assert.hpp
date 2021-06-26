
struct h2_defer_failure : h2_once {
   const char* assert_type;
   const char *e_expression, *a_expression, *expression;
   const char* file;
   int line;
   h2_fail* fails{nullptr};
   h2_oss oss;

   h2_defer_failure(const char* e_expression_, const char* a_expression_, const char* expression_, const char* file_, int line_) : e_expression(e_expression_), a_expression(a_expression_), expression(expression_), file(file_), line(line_) {}
   ~h2_defer_failure();
};

static inline h2_oss& h2_OK(h2_defer_failure* d, bool a)
{
   d->assert_type = "OK1";
   if (!a) d->fails = h2_fail::new_unexpect("true", "false");
   h2_assert_g();
   return d->oss;
}

template <typename E, typename A>
static inline h2_oss& h2_OK(h2_defer_failure* d, E e, A a, int n = 0)
{
   d->assert_type = "OK2";
   h2::h2_matcher<typename h2_decay<A>::type> m = h2::h2_matcher_cast<typename h2_decay<A>::type>((typename h2_decay<E>::type)e);
   h2_fail* fail = m.matches((typename h2_decay<A>::type)a, n);
   d->fails = fail;
   if (fail && fail->subling_next) {
      d->fails = h2_fail::new_unexpect();
      h2_fail::append_child(d->fails, fail);
   }
   h2_assert_g();
   return d->oss;
}

static inline h2_oss& h2_JE(h2_defer_failure* d, h2_string e, h2_string a, h2_string selector)
{
   d->assert_type = "JE";
   h2::h2_matcher<h2_string> m = Je(e, selector);
   d->fails = m.matches(a);
   h2_assert_g();
   return d->oss;
}
