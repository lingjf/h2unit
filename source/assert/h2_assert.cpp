static inline const char* find_op(const char* src, const char* op)
{
   bool quote = false;
   if (strlen(op) == 2) {
      for (const char* p = src; *p; p++) {
         if (*p == '\"') quote = !quote;
         if (!quote && !strncmp(op, p, 2)) return p;
      }
   } else {
      int stacks = 0;
      if (*op == '>')
         for (const char* p = src; *p; p++) {
            if (*p == '\"') quote = !quote;
            if (!quote && *p == '<') ++stacks;
            if (!quote && *p == '>' && 0 == stacks--) return p;
         }
      if (*op == '<')
         for (const char* p = src + strlen(src); src <= p; p--) {
            if (*p == '\"') quote = !quote;
            if (!quote && *p == '>') ++stacks;
            if (!quote && *p == '<' && 0 == stacks--) return p;
         }
   }
   return nullptr;
}

h2_inline h2_defer_failure::~h2_defer_failure()
{
   if (fails) {
      fails->filine = filine;
      fails->assert_type = assert_type;
      fails->assert_op = assert_op;
      fails->e_expression = e_expression;
      fails->a_expression = a_expression;
      fails->user_explain = oss.str().c_str();

      if (!strcmp("CP", assert_type) && strcmp(",", assert_op)) {
         const char* p_op = find_op(a_expression, assert_op);
         if (p_op) {
            const char *p, *q;
            for (p = p_op - 1; a_expression <= p && ::isspace(*p);) p--;
            fails->e_expression.assign(a_expression, (p + 1) - a_expression);
            for (q = p_op + strlen(assert_op); ::isspace(*q);) q++;
            fails->a_expression.assign(q, (a_expression + strlen(a_expression)) - q);
         }
      }
      h2_runner::failing(fails);
   }
}
