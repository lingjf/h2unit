
#define H2UNIT_IMPORT_MAIN

#if H2UNIT == 1
#include "../h2unit.h"
#endif

#if H2UNIT == 2
#include "../build/h2unit.hpp"
#endif

SUITE(1v2)
{
   Cleanup()
   {
   }

   Case(strcmp 10k chars)
   {
      char* es = (char*)malloc(1024 * 1024);
      char* as = (char*)malloc(1024 * 1024);

      FILE* ef = fopen("./benchmark/e.txt", "r");
      FILE* af = fopen("./benchmark/a.txt", "r");

      auto el = fread(es, 1, 1024 * 1024, ef);
      auto al = fread(as, 1, 1024 * 1024, af);
      es[el] = '\0';
      as[al] = '\0';

#ifndef _WIN32
      OK(es, as);
#endif

      fclose(ef);
      fclose(af);
      free(es);
      free(as);
   }

   Todo(2)
   {
   }
}
