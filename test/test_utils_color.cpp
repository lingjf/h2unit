#include "../source/h2_unit.cpp"

char* h2_colorful_tostring(h2::h2_colorful* colorful, char* t)
{
   strcpy(t, "");
   for (int i = 0; i < sizeof(colorful->current) / sizeof(colorful->current[0]); ++i)
      sprintf(t + strlen(t), "%s,", colorful->current[i]);
   return t;
}

SUITE(h2_colorful)
{
   char t[1024];
   h2::h2_colorful colorful;

   Case(parse)
   {
      char c1[1024] = "\033{red}";
      colorful.parse(c1);
      OK("red,,,,,,,,", h2_colorful_tostring(&colorful, t));

      char c2[1024] = "\033{bold}";
      colorful.parse(c2);
      OK("red,bold,,,,,,,", h2_colorful_tostring(&colorful, t));

      char c3[1024] = "\033{+bg_green}";
      colorful.parse(c3);
      OK("red,bold,bg_green,,,,,,", h2_colorful_tostring(&colorful, t));

      char c4[1024] = "\033{-bold}";
      colorful.parse(c4);
      OK("red,,bg_green,,,,,,", h2_colorful_tostring(&colorful, t));

      char c0[1024] = "\033{reset}";
      colorful.parse(c0);
      OK(",,,,,,,,", h2_colorful_tostring(&colorful, t));
   }
}
