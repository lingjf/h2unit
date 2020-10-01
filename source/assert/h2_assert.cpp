
static inline const char* find_outer_comma(const char* expression)
{
   char stack[1024] = {'\0'};
   int top = 1;
   int len = strlen(expression);
   for (int i = 0; i < len; ++i) {
      switch (expression[i]) {
      case '\\':
         if (expression[i + 1]) ++i;
         break;
      case '\"':
         if (stack[top - 1] == '\"')
            top--;
         else
            stack[top++] = '\"';
         break;
      case '\'':
         if (stack[top - 1] != '\"') {
            if (stack[top - 1] == '\'')
               top--;
            else
               stack[top++] = '\'';
         }
         break;
      case '(':
         if (stack[top - 1] != '\"') {
            stack[top++] = '(';
         }
         break;
      case ')':
         if (stack[top - 1] != '\"') {
            if (stack[top - 1] != '(') return nullptr;
            top--;
         }
         break;
      case '<':
         if (stack[top - 1] != '\"') {
            stack[top++] = '<';
         }
         break;
      case '>':
         if (stack[top - 1] != '\"') {
            if (stack[top - 1] != '<') return nullptr;
            top--;
         }
         break;
      case '{':
         if (stack[top - 1] != '\"') {
            stack[top++] = '{';
         }
         break;
      case '}':
         if (stack[top - 1] != '\"') {
            if (stack[top - 1] != '{') return nullptr;
            top--;
         }
         break;
      case ',':
         if (top == 1) return expression + i;
         break;
      default: break;
      }
   }
   return nullptr;
}

static inline void split_expression(h2_string& e_expression, h2_string& a_expression, const char* expression)
{
   const char *p = nullptr, *q = nullptr, *comma = nullptr;

   comma = find_outer_comma(expression);
   if (comma) {
      for (p = comma - 1; expression <= p && ::isspace(*p);) p--;
      e_expression.assign(expression, (p + 1) - expression);

      for (q = comma + 1; ::isspace(*q);) q++;
      a_expression.assign(q, (expression + strlen(expression)) - q);
   }
}

h2_inline h2_defer_fail::h2_defer_fail(const char* e_expression_, const char* a_expression_, const char* expression_, const char* file_, int lino_)
  : e_expression(e_expression_), a_expression(a_expression_), expression(expression_), file(file_), lino(lino_) {}

h2_inline h2_defer_fail::~h2_defer_fail()
{
   if (fail) {
      fail->file = file;
      fail->lino = lino;
      fail->assert_type = assert_type;
      if (!strcmp("OK1", assert_type)) {
         fail->e_expression = e_expression;
         fail->a_expression = expression;
      } else if (!strcmp("OK2", assert_type)) {
         const char* comma = find_outer_comma(expression);
         if (comma) {
            const char *p, *q;
            for (p = comma - 1; expression <= p && ::isspace(*p);) p--;
            fail->e_expression.assign(expression, (p + 1) - expression);

            for (q = comma + 1; ::isspace(*q);) q++;
            fail->a_expression.assign(q, (expression + strlen(expression)) - q);
         } else {
            fail->e_expression = e_expression;
            fail->a_expression = a_expression;
         }
      } else {
         fail->e_expression = e_expression;
         fail->a_expression = a_expression;
      }
      fail->user_explain = oss.str().c_str();
      h2_fail_g(fail, false);
   }
}
