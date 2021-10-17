#include "../source/h2_unit.cpp"

SUITE(block attributes)
{
   Case("limit=1000, fill=1, align=3")
   {
      h2::h2_block_attributes a("limit=1000, fill=1, align=3,noleak");

      OK(1000, a.limit);
      OK(3, a.alignment);
      OK(1, a.n_fill);
      OK(Me("\01", 1), a.s_fill);
      OK(a.noleak);
   }

   Case("limit=0x1000, fill=0x55")
   {
      h2::h2_block_attributes a("limit =0x1000, fill = 0x55");

      OK(0x1000, a.limit);
      OK(sizeof(void*), a.alignment);
      OK(1, a.n_fill);
      OK(Me("\x55", 1), a.s_fill);
      OK(!a.noleak);
   }

   Case("fill=0x5566")
   {
      h2::h2_block_attributes a("fill=0x5566");

      OK(Ge(0xFFFFFFFFU), a.limit);
      OK(sizeof(void*), a.alignment);
      OK(2, a.n_fill);
      OK(Me("\x55\x66", 2), a.s_fill);
      OK(!a.noleak);
   }

   Case("fill=60000")
   {
      h2::h2_block_attributes a("fill=60000");

      OK(2, a.n_fill);
      OK(Me("\x60\xEA", 2), a.s_fill);
      OK(!a.noleak);
   }
}

SUITE(BLOCK)
{
   void *c1, *c2, *c3, *c4;

   Case(block)
   {
      c1 = malloc(1);
      BLOCK()
      {
         c2 = malloc(3);
         free(c2);
      }
      free(c1);
   }

   Case(nested)
   {
      BLOCK()
      {
         c3 = malloc(5);
         BLOCK()
         {
            c4 = malloc(7);
            free(c4);
         }
         free(c3);
      }
   }

   Case(limited)
   {
      BLOCK(limit = 10)
      {
         c2 = malloc(8);
         OK(NotNull, c2);
         c3 = malloc(8);
         OK(IsNull, c3);
         free(c2);
         c4 = malloc(8);
         OK(NotNull, c4);
         free(c4);
      }
   }

   Case(fill)
   {
      BLOCK(fill = 0xC5)
      {
         c1 = malloc(8);
         OK(Me("C5C5C5C5C5C5C5C5"), c1);
         free(c1);
      }
      BLOCK(fill = 0xC555)
      {
         c1 = malloc(8);
         OK(Me("C555C555C555C555"), c1);
         free(c1);
      }
   }

   Case(align)
   {
      BLOCK(align = 1)
      {
         c1 = malloc(8);
         OK(1, (uintptr_t)c1 & 0x01);
         free(c1);
      }
      BLOCK(align = 2)
      {
         c1 = malloc(8);
         OK(2, (uintptr_t)c1 & 0x03);
         free(c1);
      }
      BLOCK(align = 0x3)
      {
         c1 = malloc(8);
         OK(3, (uintptr_t)c1 & 0x03);
         free(c1);
      }
      BLOCK(align = 4)
      {
         c1 = malloc(8);

#if defined __x86_64__
         OK(4, (uintptr_t)c1 & 0x07);
#elif defined __i386__
         OK(0, (uintptr_t)c1 & 0x07);
#endif

         free(c1);
      }
   }

   Case(noleak)
   {
      BLOCK(noleak)
      {
         c1 = malloc(8);
      }
   }

   Case(unmem)
   {
      BLOCK(unmem)
      {
         c1 = malloc(8);
      }
   }
}
