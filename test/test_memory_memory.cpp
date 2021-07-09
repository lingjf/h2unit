#include "../source/h2_unit.cpp"

SUITE(parse_block_attributes)
{
   long long n_limit;
   int n_align;
   unsigned char s_fill[32];
   int n_fill;
   bool noleak;

   Case("limit=1000, fill=1, align=3")
   {
      const char* x = "limit=1000, fill=1, align=3,noleak";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill, noleak);
      OK(1000, n_limit);
      OK(3, n_align);
      OK(1, n_fill);
      OK(Me("\01", 1), s_fill);
      OK(noleak);
   }

   Case("limit=0x1000, fill=0x55")
   {
      const char* x = "limit =0x1000, fill = 0x55";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill, noleak);
      OK(0x1000, n_limit);
      OK(sizeof(void*), n_align);
      OK(1, n_fill);
      OK(Me("\x55", 1), s_fill);
      OK(!noleak);
   }

   Case("fill=0x5566")
   {
      const char* x = "fill=0x5566";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill, noleak);
      OK(Ge(0xFFFFFFFFU), n_limit);
      OK(sizeof(void*), n_align);
      OK(2, n_fill);
      OK(Me("\x55\x66", 2), s_fill);
      OK(!noleak);
   }

   Case("fill=60000")
   {
      const char* x = "fill=60000";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill, noleak);
      OK(2, n_fill);
      OK(Me("\x60\xEA", 2), s_fill);
      OK(!noleak);
   }
}

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
