#include "../source/h2_unit.cpp"

char* h2_shell_tostring(h2::h2_shell* shell, char* t)
{
   strcpy(t, "");
   for (int i = 0; i < sizeof(shell->current) / sizeof(shell->current[0]); ++i)
      sprintf(t + strlen(t), "%s,", shell->current[i]);
   return t;
}

SUITE(h2_shell)
{
   char t[1024];
   h2::h2_shell shell;

   Case(parse)
   {
      char c1[1024] = "\033{red}";
      shell.parse(c1);
      OK("red,,,,,,,,", h2_shell_tostring(&shell, t));

      char c2[1024] = "\033{bold}";
      shell.parse(c2);
      OK("red,bold,,,,,,,", h2_shell_tostring(&shell, t));

      char c3[1024] = "\033{+bg_green}";
      shell.parse(c3);
      OK("red,bold,bg_green,,,,,,", h2_shell_tostring(&shell, t));

      char c4[1024] = "\033{-bold}";
      shell.parse(c4);
      OK("red,,bg_green,,,,,,", h2_shell_tostring(&shell, t));

      char c0[1024] = "\033{reset}";
      shell.parse(c0);
      OK(",,,,,,,,", h2_shell_tostring(&shell, t));
   }
}
