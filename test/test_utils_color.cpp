#include "../source/h2_unit.cpp"

static h2::h2_string h2_shell_tostring(h2::h2_shell* shell)
{
   h2::h2_string out;
   for (int i = 0; i < sizeof(shell->current) / sizeof(shell->current[0]); ++i)
      out.sprintf("%s,", shell->current[i]);
   return out;
}

SUITE(h2_shell)
{
   h2::h2_shell shell;

   Case(parse)
   {
      char c1[1024] = "\033{red}";
      shell.parse(c1);
      OK("red,,,,,,,,", h2_shell_tostring(&shell));

      char c2[1024] = "\033{bold}";
      shell.parse(c2);
      OK("red,bold,,,,,,,", h2_shell_tostring(&shell));

      char c3[1024] = "\033{+bg_green}";
      shell.parse(c3);
      OK("red,bold,bg_green,,,,,,", h2_shell_tostring(&shell));

      char c4[1024] = "\033{-bold}";
      shell.parse(c4);
      OK("red,,bg_green,,,,,,", h2_shell_tostring(&shell));

      char c0[1024] = "\033{reset}";
      shell.parse(c0);
      OK(",,,,,,,,", h2_shell_tostring(&shell));
   }
}
