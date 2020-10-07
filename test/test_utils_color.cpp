#include "../source/h2_unit.cpp"

char* h2__color_tostring(h2::h2__color* color, char* t)
{
   strcpy(t, "");
   for (int i = 0; i < sizeof(color->current) / sizeof(color->current[0]); ++i)
      sprintf(t + strlen(t), "%s,", color->current[i]);
   return t;
}

SUITE(h2_color)
{
   char t[1024];
   h2::h2__color color;

   Case(style2value)
   {
      OK(0, color.style2value("reset"));
      OK(31, color.style2value("red"));
   }

   Case(parse)
   {
      char c1[1024] = "\033{red}";
      color.parse(c1);
      OK("red,,,,,,,,", h2__color_tostring(&color, t));

      char c2[1024] = "\033{bold}";
      color.parse(c2);
      OK("red,bold,,,,,,,", h2__color_tostring(&color, t));

      char c3[1024] = "\033{+bg_green}";
      color.parse(c3);
      OK("red,bold,bg_green,,,,,,", h2__color_tostring(&color, t));

      char c4[1024] = "\033{-bold}";
      color.parse(c4);
      OK("red,,bg_green,,,,,,", h2__color_tostring(&color, t));

      char c0[1024] = "\033{reset}";
      color.parse(c0);
      OK(",,,,,,,,", h2__color_tostring(&color, t));
   }
}
