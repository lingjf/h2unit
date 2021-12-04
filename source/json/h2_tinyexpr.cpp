// TINYEXPR - Tiny recursive descent parser and evaluation engine in C; Copyright (c) 2015-2018 Lewis Van Winkle
struct tinyexpr {
   static bool evaluate(const char* expression, double& value)
   {
      syntax_tree* st = compile(expression);
      if (!st) return false;
      value = st->evaluate();
      delete st;
      return true;
   }

   struct function_spec {
      int parameters;
      const char* fn;
      const void* fp;
   };

   struct syntax_tree : h2_libc {
      double value = 0;
      const function_spec* function = nullptr;
      syntax_tree* parameters[4]{nullptr};
      syntax_tree(double value_) : value(value_) {}
      syntax_tree(const function_spec* function_, syntax_tree* _0 = nullptr, syntax_tree* _1 = nullptr, syntax_tree* _2 = nullptr, syntax_tree* _3 = nullptr) : function(function_), parameters{_0, _1, _2, _3} {}
      ~syntax_tree()
      {
         for (int i = 0; i < 4; ++i)
            if (parameters[i]) delete parameters[i];
      }
      double evaluate()
      {
         if (!function) return value;
         switch (function->parameters) {
            case 0: return ((double (*)())function->fp)();
            case 1: return ((double (*)(double))function->fp)(parameters[0]->evaluate());
            case 2: return ((double (*)(double, double))function->fp)(parameters[0]->evaluate(), parameters[1]->evaluate());
            case 3: return ((double (*)(double, double, double))function->fp)(parameters[0]->evaluate(), parameters[1]->evaluate(), parameters[2]->evaluate());
            case 4: return ((double (*)(double, double, double, double))function->fp)(parameters[0]->evaluate(), parameters[1]->evaluate(), parameters[2]->evaluate(), parameters[3]->evaluate());
            default: return NAN;
         }
      }
   };

   static constexpr int TOK_NULL = 0, TOK_ERROR = 1, TOK_END = 2, TOK_COMMA = 3, TOK_OPEN = 4, TOK_CLOSE = 5, TOK_NUMBER = 6, TOK_FUNCTION = 7, TOK_ADD = 8, TOK_SUB = 9, TOK_MMD = 10, TOK_POW = 11, TOK_FAC = 12;
   struct parser_state {
      const char *start, *next;
      int type;
      double value;
      const function_spec* function;
   };

   static double _add(double a, double b) { return a + b; }
   static double _sub(double a, double b) { return a - b; }
   static double _mul(double a, double b) { return a * b; }
   static double _div(double a, double b) { return a / b; }
   static double _negate(double a) { return -a; }
   static double _comma(double, double b) { return b; }
   static double __pi() { return 3.14159265358979323846; }
   static double __e() { return 2.71828182845904523536; }
   static double __nan() { return NAN; }
   static double __inf() { return INFINITY; }

   static double _factorial(double n)
   {  // n! = 1*2*3...*n , 0!=1
      if (n < 0.0) return NAN;
      unsigned long result = 1;
      for (unsigned long i = 1; i <= (unsigned long)(n); ++i) result *= i;
      return (double)result;
   }
   static double Pnm(double n, double m)
   {  // P(n,m) = n(n-1)(n-2)...(n-m+1)
      if (n < 0.0 || m < 0.0) return NAN;
      unsigned long result = 1;
      for (unsigned long i = std::max(n, m) - std::min(n, m) + 1; i <= std::max(n, m); ++i) result *= i;
      return result;
   }
   static double Cnm(double n, double m)
   {  // C(n,m) = n(n-1)(n-2)...(n-m+1)/m!
      return Pnm(n, m) / _factorial(std::min(n, m));
   }

#ifdef _MSC_VER
#pragma function(ceil)
#pragma function(floor)
#endif

   static const function_spec* get_function(const char* name, int len = 0)
   {
      static const function_spec functions[] = {
        {2, "+", (const void*)_add},
        {2, "-", (const void*)_sub},
        {2, "*", (const void*)_mul},
        {2, "/", (const void*)_div},
        {1, "negate", (const void*)_negate},
        {2, "comma", (const void*)_comma},
        {0, "pi", (const void*)__pi},
        {0, "e", (const void*)__e},
        {0, "nan", (const void*)__nan},
        {0, "inf", (const void*)__inf},
        {1, "factorial", (const void*)_factorial},
        {2, "C", (const void*)Cnm},
        {2, "P", (const void*)Pnm},
        {2, "A", (const void*)Pnm},
        {1, "abs", (const void*)(double (*)(double))::fabs},
        {1, "sqrt", (const void*)(double (*)(double))::sqrt},
        {1, "ln", (const void*)(double (*)(double))::log},
        {1, "log", (const void*)(double (*)(double))::log},
        {1, "log10", (const void*)(double (*)(double))::log10},
        {1, "log2", (const void*)(double (*)(double))::log2},
        {1, "floor", (const void*)(double (*)(double))::floor},
        {1, "ceil", (const void*)(double (*)(double))::ceil},
        {2, "fmod", (const void*)(double (*)(double, double))::fmod},
        {2, "pow", (const void*)(double (*)(double, double))::pow},
        {1, "exp", (const void*)(double (*)(double))::exp},
        {1, "cos", (const void*)(double (*)(double))::cos},
        {1, "acos", (const void*)(double (*)(double))::acos},
        {1, "cosh", (const void*)(double (*)(double))::cosh},
        {1, "sin", (const void*)(double (*)(double))::sin},
        {1, "asin", (const void*)(double (*)(double))::asin},
        {1, "sinh", (const void*)(double (*)(double))::sinh},
        {1, "tan", (const void*)(double (*)(double))::tan},
        {1, "atan", (const void*)(double (*)(double))::atan},
        {2, "atan2", (const void*)(double (*)(double, double))::atan2},
        {1, "tanh", (const void*)(double (*)(double))::tanh},
      };
      if (len == 0) len = strlen(name);
      for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i)
         if (!strncmp(name, functions[i].fn, len) && '\0' == functions[i].fn[len])
            return &functions[i];
      return nullptr;
   }

   static void lexical_token(parser_state* ps)
   {
      ps->type = TOK_NULL;
      do {
         if (!ps->next[0]) {
            ps->type = TOK_END;
         } else if (::isdigit(ps->next[0]) || ps->next[0] == '.') { /* Reading a number. */
            ps->value = strtod(ps->next, (char**)&ps->next);
            ps->type = TOK_NUMBER;
         } else {
            if (::isalpha(ps->next[0])) { /* Look for a builtin function call. */
               const char* fn = ps->next;
               while (::isalpha(ps->next[0]) || ::isdigit(ps->next[0]) || (ps->next[0] == '_')) ps->next++;
               ps->function = get_function(fn, (int)(ps->next - fn));
               ps->type = TOK_FUNCTION;
               if (!ps->function) ps->type = TOK_ERROR;
            } else { /* Look for an operator or special character. */
               switch (ps->next++[0]) {
                  case '+': ps->type = TOK_ADD, ps->function = get_function("+"); break;
                  case '-': ps->type = TOK_SUB, ps->function = get_function("-"); break;
                  case '*': ps->type = TOK_MMD, ps->function = get_function("*"); break;
                  case '/': ps->type = TOK_MMD, ps->function = get_function("/"); break;
                  case '%': ps->type = TOK_MMD, ps->function = get_function("fmod"); break;
                  case '^': ps->type = TOK_POW, ps->function = get_function("pow"); break;
                  case '!': ps->type = TOK_FAC, ps->function = get_function("factorial"); break;
                  case '(': ps->type = TOK_OPEN; break;
                  case ')': ps->type = TOK_CLOSE; break;
                  case ',': ps->type = TOK_COMMA; break;
                  case ' ':
                  case '\t': break;  //  case '\n': case '\r':
                  default: ps->type = TOK_ERROR; break;
               }
            }
         }
      } while (ps->type == TOK_NULL);
   }

   static syntax_tree* base(parser_state* ps)
   { /* <base> = <constant> | <function-0> {"(" ")"} | <function-1> <power> | <function-X> "(" <expr> {"," <expr>} ")" | "(" <list> ")" */
      syntax_tree* st;
      if (ps->type == TOK_NUMBER) {
         st = new syntax_tree(ps->value);
         lexical_token(ps);
      } else if (ps->type == TOK_OPEN) {
         lexical_token(ps);
         st = list(ps);
         if (ps->type != TOK_CLOSE) {
            ps->type = TOK_ERROR;
         } else {
            lexical_token(ps);
         }
      } else if (ps->type == TOK_FUNCTION) {
         st = new syntax_tree(ps->function);
         lexical_token(ps);
         if (st->function->parameters == 0) {
            if (ps->type == TOK_OPEN) {
               lexical_token(ps);
               if (ps->type != TOK_CLOSE) {
                  ps->type = TOK_ERROR;
               } else {
                  lexical_token(ps);
               }
            }
         } else if (st->function->parameters == 1) {
            st->parameters[0] = power(ps);
         } else {
            if (ps->type != TOK_OPEN) {
               ps->type = TOK_ERROR;
            } else {
               int i = 0;
               while (i < 4) {
                  lexical_token(ps);
                  st->parameters[i++] = expr(ps);
                  if (ps->type != TOK_COMMA) break;
               }
               if (ps->type != TOK_CLOSE || i != st->function->parameters) {
                  ps->type = TOK_ERROR;
               } else {
                  lexical_token(ps);
               }
            }
         }
      } else {
         st = new syntax_tree(NAN);
         ps->type = TOK_ERROR;
      }
      return st;
   }

   static syntax_tree* factorial(parser_state* ps)
   { /* <factorial> = <base> {"!"} */
      syntax_tree* st = base(ps);
      while (ps->type == TOK_FAC) {
         st = new syntax_tree(get_function("factorial"), st);
         lexical_token(ps);
      }
      return st;
   }

   static syntax_tree* power(parser_state* ps)
   { /* <power> = {("-" | "+")} <factorial> */
      int sign = 1;
      while (ps->type == TOK_ADD || ps->type == TOK_SUB) {
         if (ps->type == TOK_SUB) sign = -sign;
         lexical_token(ps);
      }
      syntax_tree* st = factorial(ps);
      if (sign == -1) st = new syntax_tree(get_function("negate"), st);
      return st;
   }

   static syntax_tree* factor(parser_state* ps)
   { /* <factor> = <power> {"^" <power>} */
      syntax_tree* st = power(ps);
      while (ps->type == TOK_POW) {
         auto t = ps->function;
         lexical_token(ps);
         st = new syntax_tree(t, st, power(ps));
      }
      return st;
   }

   static syntax_tree* term(parser_state* ps)
   { /* <term> = <factor> {("*" | "/" | "%") <factor>} */
      syntax_tree* st = factor(ps);
      while (ps->type == TOK_MMD) {
         auto t = ps->function;
         lexical_token(ps);
         st = new syntax_tree(t, st, factor(ps));
      }
      return st;
   }

   static syntax_tree* expr(parser_state* ps)
   { /* <expr> = <term> {("+" | "-") <term>} */
      syntax_tree* st = term(ps);
      while (ps->type == TOK_ADD || ps->type == TOK_SUB) {
         auto t = ps->function;
         lexical_token(ps);
         st = new syntax_tree(t, st, term(ps));
      }
      return st;
   }

   static syntax_tree* list(parser_state* ps)
   { /* <list> = <expr> {"," <expr>} */
      syntax_tree* st = expr(ps);
      while (ps->type == TOK_COMMA) {
         lexical_token(ps);
         st = new syntax_tree(get_function("comma"), st, expr(ps));
      }
      return st;
   }

   static syntax_tree* compile(const char* expression)
   {
      parser_state ps;
      ps.start = ps.next = expression;
      lexical_token(&ps);
      syntax_tree* st = list(&ps);
      if (ps.type != TOK_END) {
         delete st;
         return nullptr;
      }
      return st;
   }
};
