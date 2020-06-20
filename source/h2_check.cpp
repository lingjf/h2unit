
static inline const char* find_outer_comma(const char* expression)
{
   char stack[1024] = {'\0'};
   int top = 1;
   for (const char* p = expression; *p; p++) {
      switch (*p) {
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
         if (top == 1) return p;
         break;
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

h2_inline h2_defer_fail::h2_defer_fail(const char* e_expression_, const char* a_expression_, const char* expression_, const char* file_, int line_)
  : e_expression(e_expression_), a_expression(a_expression_), expression(expression_), file(file_), line(line_) {}

h2_inline h2_defer_fail::~h2_defer_fail()
{
   if (fail) {
      fail->file = file;
      fail->line = line;
      fail->check_type = check_type;
      if (!strcmp("OK1", check_type)) {
         fail->e_expression = e_expression;
         fail->a_expression = expression;
      } else if (!strcmp("OK2", check_type)) {
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
