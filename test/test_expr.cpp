#include "../source/h2_unit.h"

SUITE(expr)
{
   struct
   {
      const char* expr;
      double answer;
   } samples [] = {
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
   };

   Case(number)
   {
      for (size_t i = 0; i < sizeof(samples) / sizeof(samples[0]); ++i) {
         const char* expr = samples[i].expr;
         const double answer = samples[i].answer;

         int err;
         const double ev = tinyexpr::te_interp(expr, &err);
         OK(0, err);
         OK(answer, ev);
      }
   };
}