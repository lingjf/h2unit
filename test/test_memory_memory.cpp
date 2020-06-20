#include "../source/h2_unit.cpp"
#include <sys/time.h>

SUITE(parse_block_attributes)
{
   long long n_limit;
   int n_align;
   unsigned char s_fill[32];
   int n_fill;

   Case("limit=1000, fill=1, align=3")
   {
      const char* x = "limit=1000, fill=1, align=3";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill);
      OK(1000, n_limit);
      OK(3, n_align);
      OK(1, n_fill);
      OK(Me("\01", 1), s_fill);
   }
   Case("limit=0x1000, fill=0x55")
   {
      const char* x = "limit =0x1000, fill = 0x55";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill);
      OK(0x1000, n_limit);
      OK(8, n_align);
      OK(1, n_fill);
      OK(Me("\x55", 1), s_fill);
   }
   Case("fill=0x5566")
   {
      const char* x = "fill=0x5566";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill);
      OK(Ge(0xFFFFFFFFU), n_limit);
      OK(8, n_align);
      OK(2, n_fill);
      OK(Me("\x55\x66", 2), s_fill);
   }
   Case("fill=60000")
   {
      const char* x = "fill=60000";
      h2::parse_block_attributes(x, n_limit, n_align, s_fill, n_fill);
      OK(2, n_fill);
      OK(Me("\x60\xEA", 2), s_fill);
   }
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
}
