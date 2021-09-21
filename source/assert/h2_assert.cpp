static inline const char* find_outer_op(const char* src, const char* op)
{
   auto p1 = strstr(src, op);
   if (!p1) return nullptr;
   auto p2 = strstr(p1 + 1, op);
   if (p2) return nullptr;
   return p1;
}

h2_inline h2_defer_failure::~h2_defer_failure()
{
   if (fails) {
      fails->file = file;
      fails->line = line;
      fails->assert_type = assert_type;
      fails->assert_op = assert_op;
      fails->e_expression = e_expression;
      fails->a_expression = a_expression;
      fails->user_explain = oss.str().c_str();

      if (!strcmp("Ok2", assert_type) && strcmp(",", assert_op)) {
         const char* pop = find_outer_op(a_expression, assert_op);
         if (pop) {
            const char *p, *q;
            for (p = pop - 1; a_expression <= p && ::isspace(*p);) p--;
            fails->e_expression.assign(a_expression, (p + 1) - a_expression);
            for (q = pop + strlen(assert_op); ::isspace(*q);) q++;
            fails->a_expression.assign(q, (a_expression + strlen(a_expression)) - q);
            fails->assert_type = "OK2";
         }
      }
      h2_fail_g(fails);
   }
}
