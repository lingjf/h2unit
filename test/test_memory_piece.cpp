#include "../source/h2_unit.cpp"

static h2::h2_string h2_piece_tojson(h2::h2_piece* piece)
{
   h2::h2_string out;
   out.sprintf("{");

   out.sprintf("\"user_size\": %zu", piece->user_size);
   out.sprintf(",\"page_size\": %zu", piece->page_size);
   out.sprintf(",\"page_count\": %zu", piece->page_count);
   out.sprintf(",\"user_ptr\": \"%p\"", piece->user_ptr);
   out.sprintf(",\"page_ptr\": \"%p\"", piece->page_ptr);
   out.sprintf(",\"who_allocate\": \"%s\"", piece->who_allocate);
   out.sprintf(",\"free_times\": %d", piece->free_times);
   out.sprintf(",\"snow_flower\": \"%x\"", piece->snow_flower);
   out.sprintf(",\"forbidden_page\": \"%p\"", piece->forbidden_page);
   out.sprintf(",\"forbidden_size\": %zu", piece->forbidden_size);
   out.sprintf(",\"violate_ptr\": \"%p\"", piece->violate_ptr);
   out.sprintf(",\"violate_action\": \"%s\"", piece->violate_action);
   out.sprintf(",\"violate_times\": %d", piece->violate_times);
   out.sprintf(",\"violate_after_free\": %s", piece->violate_after_free ? "true" : "false");

   out.sprintf("}");
   return out;
}

SUITE(piece)
{
   h2::h2_piece* m = nullptr;

   Case(new piece)
   {
      h2::h2_backtrace bt;
      m = new h2::h2_piece(100, 8, "malloc", bt);
      JE("{                               \
            'user_size': 100,             \
            'page_count': 1,              \
            'who_allocate': 'malloc',     \
            'free_times': 0               \
         }",
         h2_piece_tojson(m));

      OK(IsNull, m->free("free"));

      JE("{                               \
            'user_size': 100,             \
            'page_count': 1,              \
            'who_allocate': 'malloc',     \
            'free_times': 1               \
         }",
         h2_piece_tojson(m));

      delete m;
   }

   Case(zero size)
   {
      h2::h2_backtrace bt;
      m = new h2::h2_piece(0, 0, "malloc", bt);
      OK(NotNull, m);
      OK(NotNull, m->user_ptr);
      OK(0, m->user_size);
      OK(IsNull, m->free("free"));
      delete m;
   }

   Case(double free)
   {
      h2::h2_backtrace bt;
      m = new h2::h2_piece(100, 8, "malloc", bt);
      OK(IsNull, m->free("free"));
      auto fail = m->free("free");
      OK(NotNull, fail);
      OK(typeid(h2::h2_fail_double_free).name(), typeid(*fail).name());
      delete m;
   }

   Case(asymmetric free)
   {
      h2::h2_backtrace bt;
      m = new h2::h2_piece(100, 8, "malloc", bt);
      auto fail = m->free("delete");
      OK(NotNull, fail);
      OK(typeid(h2::h2_fail_asymmetric_free).name(), typeid(*fail).name());
      delete m;
   }

   Case(snowfield)
   {
      h2::h2_backtrace bt;
      m = new h2::h2_piece(100, 8, "malloc", bt);
      ((unsigned short*)m->user_ptr)[50] = 0xCC33;
      auto fail = m->free("free");
      OK(NotNull, fail);
      OK(typeid(h2::h2_fail_overflow).name(), typeid(*fail).name());
      delete m;
   }

   Case(use after free)
   {
      // reference to print project
   }
}

SUITE(piece align)
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
#if defined __x86_64__
      OK(1, Pu & 0x1);
#elif defined __i386__
      OK(1, Pu & 0x1);
#endif
   }

   Case(align 2 / 8)
   {
      m = new h2::h2_piece(8, 2, "malloc", bt);
#if defined __x86_64__
      OK(2, Pu & 0x3);
#elif defined __i386__
      OK(2, Pu & 0x3);
#endif
   }

   Case(align 3 / 8)
   {
      m = new h2::h2_piece(8, 3, "malloc", bt);
#if defined __x86_64__
      OK(3, Pu & 0x3);
#elif defined __i386__
      OK(3, Pu & 0x3);
#endif
   }

   Case(align 4 / 8)
   {
      m = new h2::h2_piece(8, 4, "malloc", bt);
#if defined __x86_64__
      OK(4, Pu & 0x7);
#elif defined __i386__
      OK(0, Pu & 0x7);
#endif
   }

   Case(align 5 / 8)
   {
      m = new h2::h2_piece(8, 5, "malloc", bt);
#if defined __x86_64__
      OK(5, Pu & 0x7);
#elif defined __i386__
      OK(5, Pu & 0x7);
#endif
   }

   Case(align 6 / 8)
   {
      m = new h2::h2_piece(8, 6, "malloc", bt);
      OK(6, Pu & 0x7);
   }

   Case(align 7 / 8)
   {
      m = new h2::h2_piece(8, 7, "malloc", bt);
      OK(7, Pu & 0x7);
   }

   Case(align 8 / 8)
   {
      m = new h2::h2_piece(8, 8, "malloc", bt);
      OK(8, Pu & 0xF);
   }

   Case(align 1 / 5)
   {
      m = new h2::h2_piece(5, 1, "malloc", bt);
      OK(1, Pu & 0x1);
   }

   Case(align 2 / 5)
   {
      m = new h2::h2_piece(5, 2, "malloc", bt);
      OK(2, Pu & 0x3);
   }

   Case(align 3 / 5)
   {
      m = new h2::h2_piece(5, 3, "malloc", bt);
      OK(3, Pu & 0x3);
   }

   Case(align 4 / 5)
   {
      m = new h2::h2_piece(5, 4, "malloc", bt);
#if defined __x86_64__
      OK(4, Pu & 0x7);
#elif defined __i386__
      OK(0, Pu & 0x7);
#endif
   }

   Case(align 5 / 5)
   {
      m = new h2::h2_piece(5, 5, "malloc", bt);
      OK(5, Pu & 0x7);
   }

   Case(align 6 / 5)
   {
      m = new h2::h2_piece(5, 6, "malloc", bt);
      OK(6, Pu & 0x7);
   }

   Case(align 7 / 5)
   {
      m = new h2::h2_piece(5, 7, "malloc", bt);
      OK(7, Pu & 0x7);
   }

   Case(align 8 / 5)
   {
      m = new h2::h2_piece(5, 8, "malloc", bt);
      OK(8, Pu & 0xF);
   }
}
