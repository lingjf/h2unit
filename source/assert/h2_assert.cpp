
h2_inline h2_defer_failure::~h2_defer_failure()
{
   if (fails) {
      fails->file = file;
      fails->line = line;
      fails->assert_type = assert_type;
      fails->e_expression = e_expression;
      fails->a_expression = a_expression;
      fails->user_explain = oss.str().c_str();
      h2_fail_g(fails);
   }
}
