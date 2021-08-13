#include "../source/h2_unit.cpp"

void parse_something1()
{
   static char* buffer = NULL;
   if (buffer == NULL) { buffer = (char*)malloc(1000); }
}

void parse_something2()
{
   static char* buffer = NULL;
   if (buffer == NULL) { buffer = (char*)malloc(2000); }
}

SUITE(leak)
{
   void* p = malloc(32 * 864);

   Cleanup() { free(p); }

   Case(malloc in setup and free in teardown)
   {
      strcpy((char*)p, "hello world");
      OK("hello world", (char*)p);
   }

   Case(UNMEM by function pointer)
   {
      UNMEM(parse_something1);

      parse_something1();
   }

   Case(UNMEM by function name)
   {
      UNMEM("parse_something2");

      parse_something2();
   }
}
