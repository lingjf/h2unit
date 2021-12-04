static inline const char* __find_compare(const char* expression, const char* op)
{
   bool quote = false;
   if (strlen(op) == 2) {
      for (const char* p = expression; *p; ++p) {
         if (*p == '\"') quote = !quote;
         if (!quote && !strncmp(op, p, 2)) return p;
      }
   } else {
      int stacks = 0;
      if (*op == '>')
         for (const char* p = expression; *p; ++p) {
            if (*p == '\"') quote = !quote;
            if (!quote && *p == '<') ++stacks;
            if (!quote && *p == '>' && 0 == stacks--) return p;
         }
      if (*op == '<')
         for (const char* p = expression + strlen(expression); expression <= p; --p) {
            if (*p == '\"') quote = !quote;
            if (!quote && *p == '>') ++stacks;
            if (!quote && *p == '<' && 0 == stacks--) return p;
         }
   }
   return nullptr;
}

static inline void __split_compare(const char* expression, const char* op, h2_string& e_expression, h2_string& a_expression)
{
   const char* p = __find_compare(expression, op);
   if (p) {
      e_expression.assign(expression, strip_right(expression, p) - expression);
      a_expression = strip_left(p + strlen(op));
   }
}

h2_inline h2_ostringstream& h2_assert::stash(h2_fail* fail, const char* assert_type, const char* assert_op)
{
   h2_runner::asserts();
   fails = fail;
   if (fail && fail->subling_next) {
      fails = h2_fail::new_unexpect();
      h2_fail::append_child(fails, fail);
   }
   if (fails) {
      fails->assert_type = assert_type;
      fails->assert_op = assert_op;
   }
   return oss;
}

h2_inline void h2_assert::failing(const char* e_expression, const char* a_expression, const char* filine)
{
   if (fails) {
      fails->user_explain = oss.str().c_str();
      fails->filine = filine;
      fails->e_expression = e_expression;
      fails->a_expression = a_expression;
      if (*fails->assert_op != ',') __split_compare(a_expression, fails->assert_op, fails->e_expression, fails->a_expression);
   }
   h2_runner::failing(fails);
}
