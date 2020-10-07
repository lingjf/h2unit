#include "../source/h2_unit.cpp"

char* h2_piece_tojson(h2::h2_piece* piece, char* b)
{
   int l = 0;
   l += sprintf(b + l, "{");

   l += sprintf(b + l, "\"user_size\": %zu", piece->user_size);
   l += sprintf(b + l, ",\"page_size\": %zu", piece->page_size);
   l += sprintf(b + l, ",\"page_count\": %zu", piece->page_count);
   l += sprintf(b + l, ",\"user_ptr\": \"%p\"", piece->user_ptr);
   l += sprintf(b + l, ",\"page_ptr\": \"%p\"", piece->page_ptr);
   l += sprintf(b + l, ",\"who_allocate\": \"%s\"", piece->who_allocate);
   l += sprintf(b + l, ",\"free_times\": %d", piece->free_times);
   l += sprintf(b + l, ",\"snow_flower\": \"%x\"", piece->snow_flower);
   l += sprintf(b + l, ",\"forbidden_page\": \"%p\"", piece->forbidden_page);
   l += sprintf(b + l, ",\"forbidden_size\": %zu", piece->forbidden_size);
   l += sprintf(b + l, ",\"violate_address\": \"%p\"", piece->violate_address);
   l += sprintf(b + l, ",\"violate_action\": \"%s\"", piece->violate_action);
   l += sprintf(b + l, ",\"violate_times\": %d", piece->violate_times);
   l += sprintf(b + l, ",\"violate_after_free\": %s", piece->violate_after_free ? "true" : "false");

   l += sprintf(b + l, "}");

   return b;
}

SUITE(piece)
{
   char t1[1024 * 8];
   h2::h2_piece* m = nullptr;

   Case(new piece)
   {
      h2::h2_backtrace bt;
      m = new h2::h2_piece(100, 8, "malloc", bt);
      JE("{                               \
            'user_size': 100,             \
            'page_size': 4096,            \
            'page_count': 1,              \
            'who_allocate': 'malloc',     \
            'free_times': 0 ,             \
            'forbidden_size': 4096        \
         }",
         h2_piece_tojson(m, t1));

      OK(IsNull, m->free("free"));

      JE("{                               \
            'user_size': 100,             \
            'page_size': 4096,            \
            'page_count': 1,              \
            'who_allocate': 'malloc',     \
            'free_times': 1 ,             \
            'forbidden_size': 4096 * 2    \
         }",
         h2_piece_tojson(m, t1));

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
      ((unsigned char*)m->user_ptr)[101] = '1';
      auto fail = m->free("free");
      OK(NotNull, fail);
      OK(typeid(h2::h2_fail_overflow).name(), typeid(*fail).name());
      delete m;
   }

   Case(use after free)
   {
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
