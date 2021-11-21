#include "../source/h2_unit.cpp"

static h2::h2_string h2_color_tostring(h2::h2_color* color)
{
   h2::h2_string out;
   for (int i = 0; i < sizeof(color->current) / sizeof(color->current[0]); ++i)
      out.sprintf("%s,", color->current[i]);
   return out;
}

SUITE(color)
{
   h2::h2_color color;

   Case(parse)
   {
      char c1[1024] = "\033{red}";
      color.parse(c1);
      OK("red,,,,,,,,", h2_color_tostring(&color));

      char c2[1024] = "\033{bold}";
      color.parse(c2);
      OK("red,bold,,,,,,,", h2_color_tostring(&color));

      char c3[1024] = "\033{+bg_green}";
      color.parse(c3);
      OK("red,bold,bg_green,,,,,,", h2_color_tostring(&color));

      char c4[1024] = "\033{-bold}";
      color.parse(c4);
      OK("red,,bg_green,,,,,,", h2_color_tostring(&color));

      char c0[1024] = "\033{reset}";
      color.parse(c0);
      OK(",,,,,,,,", h2_color_tostring(&color));
   }
}
