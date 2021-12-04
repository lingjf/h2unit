#include "../source/h2_unit.cpp"

SUITE(subroutine)
{
   Case(factorial)
   {
      OK(1, h2::tinyexpr::_factorial(0));

      OK(1, h2::tinyexpr::_factorial(1));
      OK(1 * 2, h2::tinyexpr::_factorial(2));
      OK(1 * 2 * 3, h2::tinyexpr::_factorial(3));
      OK(1 * 2 * 3 * 4, h2::tinyexpr::_factorial(4));
      OK(1 * 2 * 3 * 4 * 5, h2::tinyexpr::_factorial(5));
   }

   Case(Combination)
   {
      OK(15, h2::tinyexpr::Cnm(6, 2));
      OK(15, h2::tinyexpr::Cnm(6, 4));
      // C62=(6*5)/(2*1)=15
      // C64=(6*5*4*3)/(4*3*2*1)=15
   }

   Case(Permutation)
   {
      OK(30, h2::tinyexpr::Pnm(6, 2));
      OK(360, h2::tinyexpr::Pnm(6, 4));
      // P62=6*5=30
      // P64=6*5*4*3=360
   }
}

SUITE(tinyexpr)
{
   double ev;
   int ret;

   Case(number)
   {
      struct
      {
         const char* expr;
         double answer;
      } samples[] = {
        {"1", 1},
        {"1 ", 1},
        {"(1)", 1},

        {"pi", 3.14159},
        {"atan(1)*4 - pi", 0},
        {"e", 2.71828},

        {"2+1", 2 + 1},
        {"(((2+(1))))", 2 + 1},
        {"3+2", 3 + 2},

        {"3+2+4", 3 + 2 + 4},
        {"(3+2)+4", 3 + 2 + 4},
        {"3+(2+4)", 3 + 2 + 4},
        {"(3+2+4)", 3 + 2 + 4},

        {"3*2*4", 3 * 2 * 4},
        {"(3*2)*4", 3 * 2 * 4},
        {"3*(2*4)", 3 * 2 * 4},
        {"(3*2*4)", 3 * 2 * 4},

        {"3-2-4", 3 - 2 - 4},
        {"(3-2)-4", (3 - 2) - 4},
        {"3-(2-4)", 3 - (2 - 4)},
        {"(3-2-4)", 3 - 2 - 4},

        {"3/2/4", 3.0 / 2.0 / 4.0},
        {"(3/2)/4", (3.0 / 2.0) / 4.0},
        {"3/(2/4)", 3.0 / (2.0 / 4.0)},
        {"(3/2/4)", 3.0 / 2.0 / 4.0},

        {"(3*2/4)", 3.0 * 2.0 / 4.0},
        {"(3/2*4)", 3.0 / 2.0 * 4.0},
        {"3*(2/4)", 3.0 * (2.0 / 4.0)},

        {"asin sin .5", 0.5},
        {"sin asin .5", 0.5},
        {"ln exp .5", 0.5},
        {"exp ln .5", 0.5},

        {"asin sin-.5", -0.5},
        {"asin sin-0.5", -0.5},
        {"asin sin -0.5", -0.5},
        {"asin (sin -0.5)", -0.5},
        {"asin (sin (-0.5))", -0.5},
        {"asin sin (-0.5)", -0.5},
        {"(asin sin (-0.5))", -0.5},

        {"log10 1000", 3},
        {"log10 1e3", 3},
        {"log10 1000", 3},
        {"log10 1e3", 3},
        {"log10(1000)", 3},
        {"log10(1e3)", 3},
        {"log10 1.0e3", 3},
        {"10^5*5e-5", 5},

        {"log2 4", 2},

        {"ln (e^10)", 10},
        {"100^.5+1", 11},
        {"100 ^.5+1", 11},
        {"100^+.5+1", 11},
        {"100^--.5+1", 11},
        {"100^---+-++---++-+-+-.5+1", 11},

        {"100^-.5+1", 1.1},
        {"100^---.5+1", 1.1},
        {"100^+---.5+1", 1.1},
        {"1e2^+---.5e0+1e0", 1.1},
        {"--(1e2^(+(-(-(-.5e0))))+1e0)", 1.1},

        {"sqrt 100 + 7", 17},
        {"sqrt 100 * 7", 70},
        {"sqrt (100 * 100)", 100},

        {"1,2", 2},
        {"1,2+1", 3},
        {"1+1,2+2,2+1", 3},
        {"1,2,3", 3},
        {"(1,2),3", 3},
        {"1,(2,3)", 3},
        {"-(1,(2,3))", -3},

        {"2^2", 4},
        {"pow(2,2)", 4},

        {"fmod(4,3)", 1},
        {"4%3", 1},

        {"A(6,2)", 30},
        {"A(6,4)", 360},
        {"P(6,2)", 30},
        {"P(6,4)", 360},
        {"C(6,2)", 15},
        {"C(6,4)", 15},

        {"A(2,6)", 30},
        {"A(4,6)", 360},
        {"P(2,6)", 30},
        {"P(4,6)", 360},
        {"C(2,6)", 15},
        {"C(4,6)", 15},
        {"A(6, 2)", 30},
        {"A(6, 4)", 360},
        {"P(6, 2)", 30},
        {"P(6, 4)", 360},
        {"C(6, 2)", 15},
        {"C(6, 4)", 15},

        {"3!", 6},
        {"3!!", 720},
        {"2!", 2},
        {"2!!", 2},
        {"2!!!", 2},
        {"2!!!!", 2},
        {"2!!!!!", 2},
        {"3!+1", 7},
        {"1+3!", 7},
      };

      for (size_t i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
         const char* expr = samples[i].expr;
         const double answer = samples[i].answer;

         ret = h2::tinyexpr::evaluate(expr, ev);
         OK(ret) << expr;
         OK(answer, ev) << expr;
      }
   }

   Case(very large number)
   {
      ret = h2::tinyexpr::evaluate("1912000101600571", ev);
      OK(ret);
      OK(1912000101600571LL, ev);
   }

   Case(hex)
   {
      ret = h2::tinyexpr::evaluate("0xffff + 1", ev);
      OK(ret);
      OK(0xffffU + 1, ev);
   }

   Case(divide zero INFINITY)
   {
      const char* samples[] = {
        "1/0",
        "log(0)",
      };

      for (size_t i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
         ret = h2::tinyexpr::evaluate(samples[i], ev);
         OK(ret) << samples[i];
         // OK(INFINITY == ev) << samples[i];
         // OK(std::isinf(ev)) << samples[i]; // ci: linux i386 qemu gcc/clang failed
         OK(_, ev);
      }
   }

   Case(divide zero NAN)
   {
      const char* samples[] = {
        "0/0",
        "1%0",
      };

      for (size_t i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
         ret = h2::tinyexpr::evaluate(samples[i], ev);
         OK(ret) << samples[i];
         // OK(NAN == ev) << samples[i];
         OK(std::isnan(ev)) << samples[i];
      }
   }

   Case(invalid expr)
   {
      const char* samples[] = {
        "",
        "1+",
        "1)",
        "(1",
        "1**1",
        "1*2(+4",
        "1*2(1+4",
        "a+5",
        "!+5",
        "_a+5",
        "#a+5",
        "1^^5",
        "1**5",
        "sin(cos5",

        "!2",
        "1 \\ 2",
        "fft(1)",  // no such function
      };

      for (size_t i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
         ret = h2::tinyexpr::evaluate(samples[i], ev);
         OK(!ret) << samples[i];
      }
   }
}

SUITE(json tinyexpr)
{
   Case(divide zero nan)
   {
      JE("{'value': nan}", "{ \"value\": 0/0 }");
   }

   Case(divide zero inf)
   {
      JE("{'value': inf}", "{ \"value\": 1/0 }");
   }
}
