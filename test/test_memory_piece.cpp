#include "../source/h2_unit.cpp"

SUITE(piece)
{
   h2::h2_backtrace bt;
   h2::h2_piece* m = nullptr;
   unsigned page_size = h2::h2_page_size();

#define Pu (((uintptr_t)m->user_ptr) & (page_size - 1))

   Cleanup()
   {
      if (m) delete m;
   }
   Case(new delete)
   {
      delete new h2::h2_piece(100, 8, "malloc", bt);
      delete new h2::h2_piece(100, 0, "malloc", bt);
   }

   Case(align 1 / 8)
   {
      m = new h2::h2_piece(8, 1, "malloc", bt);
      OK(1, Pu & 0x1);
      OK(7, page_size - (Pu + 8));
   }
   Case(align 2 / 8)
   {
      m = new h2::h2_piece(8, 2, "malloc", bt);
      OK(2, Pu & 0x3);
      OK(6, page_size - (Pu + 8));
   }
   Case(align 3 / 8)
   {
      m = new h2::h2_piece(8, 3, "malloc", bt);
      OK(3, Pu & 0x3);
      OK(5, page_size - (Pu + 8));
   }
   Case(align 4 / 8)
   {
      m = new h2::h2_piece(8, 4, "malloc", bt);
      OK(4, Pu & 0x7);
      OK(4, page_size - (Pu + 8));
   }
   Case(align 5 / 8)
   {
      m = new h2::h2_piece(8, 5, "malloc", bt);
      OK(5, Pu & 0x7);
      OK(3, page_size - (Pu + 8));
   }
   Case(align 6 / 8)
   {
      m = new h2::h2_piece(8, 6, "malloc", bt);
      OK(6, Pu & 0x7);
      OK(2, page_size - (Pu + 8));
   }
   Case(align 7 / 8)
   {
      m = new h2::h2_piece(8, 7, "malloc", bt);
      OK(7, Pu & 0x7);
      OK(1, page_size - (Pu + 8));
   }
   Case(align 8 / 8)
   {
      m = new h2::h2_piece(8, 8, "malloc", bt);
      OK(8, Pu & 0xF);
      OK(0, page_size - (Pu + 8));
   }

   Case(align 1 / 5)
   {
      m = new h2::h2_piece(5, 1, "malloc", bt);
      OK(1, Pu & 0x1);
      // OK(7, page_size - (Pu + 5));
   }
   Case(align 2 / 5)
   {
      m = new h2::h2_piece(5, 2, "malloc", bt);
      OK(2, Pu & 0x3);
      // OK(6, page_size - (Pu + 5));
   }
   Case(align 3 / 5)
   {
      m = new h2::h2_piece(5, 3, "malloc", bt);
      OK(3, Pu & 0x3);
      // OK(5, page_size - (Pu + 5));
   }
   Case(align 4 / 5)
   {
      m = new h2::h2_piece(5, 4, "malloc", bt);
      OK(4, Pu & 0x7);
      // OK(4, page_size - (Pu + 5));
   }
   Case(align 5 / 5)
   {
      m = new h2::h2_piece(5, 5, "malloc", bt);
      OK(5, Pu & 0x7);
      // OK(3, page_size - (Pu + 5));
   }
   Case(align 6 / 5)
   {
      m = new h2::h2_piece(5, 6, "malloc", bt);
      OK(6, Pu & 0x7);
      // OK(2, page_size - (Pu + 5));
   }
   Case(align 7 / 5)
   {
      m = new h2::h2_piece(5, 7, "malloc", bt);
      OK(7, Pu & 0x7);
      // OK(1, page_size - (Pu + 5));
   }
   Case(align 8 / 5)
   {
      m = new h2::h2_piece(5, 8, "malloc", bt);
      OK(8, Pu & 0xF);
      // OK(0, page_size - (Pu + 5));
   }
}
