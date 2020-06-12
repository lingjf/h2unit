
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
   int e_len = strlen(e_expression.c_str());
   int a_len = strlen(a_expression.c_str());
   int len = strlen(expression);
   const char *p = nullptr, *q = nullptr, *comma = nullptr;
   if (memcmp(expression, e_expression.c_str(), e_len) == 0) {
      p = expression + e_len;
      comma = strchr(p, ',');
   }
   if (memcmp(expression + len - a_len, a_expression.c_str(), a_len) == 0) {
      q = expression + len - a_len;
      comma = strrchr(q - 1, ',');
   }
   if (!p || !q) {
      if (!comma) {
         comma = find_outer_comma(expression);
      }
      if (!p && comma) {
         for (p = comma - 1; expression <= p && ::isspace(*p);) p--;
         e_expression.assign(expression, (p + 1) - expression);
      }
      if (!q && comma) {
         for (q = comma + 1; ::isspace(*q);) q++;
         a_expression.assign(q, (expression + len) - q);
      }
   }
}

h2_inline h2_defer_fail::h2_defer_fail(const char* check_type_, const char* e_expression_, const char* a_expression_, const char* expression_, const char* file_, int line_)
  : check_type(check_type_), e_expression(e_expression_), a_expression(a_expression_), expression(expression_), file(file_), line(line_) {}

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
         fail->e_expression = e_expression;
         fail->a_expression = a_expression;
         split_expression(fail->e_expression, fail->a_expression, expression);
      } else {
         fail->e_expression = e_expression;
         fail->a_expression = a_expression;
      }
      fail->user_explain = oss.str().c_str();
      h2_fail_g(fail, false);
   }
}
