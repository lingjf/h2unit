#include "../source/h2_unit.cpp"

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
         OK(Me("\xC5\xC5\xC5\xC5\xC5\xC5\xC5\xC5", 8), c1);
         free(c1);
      }
      BLOCK(fill = 0xC555)
      {
         c1 = malloc(8);
         OK(Me("\xC5\x55\xC5\x55\xC5\x55\xC5\x55", 8), c1);
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
      BLOCK(align = 3)
      {
         c1 = malloc(8);
         OK(3, (uintptr_t)c1 & 0x03);
         free(c1);
      }
      BLOCK(align = 4)
      {
         c1 = malloc(8);
         OK(4, (uintptr_t)c1 & 0x07);
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
}
