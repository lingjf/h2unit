
struct h2_block : h2_libc {
   h2_list x;
   h2_list pieces;

   long long limited;
   size_t align;
   unsigned char s_fill[32];
   int n_fill;
   const char* where;
   const char* file;
   int line;

   h2_block(long long _limited, size_t _align, unsigned char _s_fill[32], int _n_fill, const char* _where, const char* _file, int _line)
     : limited(_limited), align(_align), n_fill(_n_fill), where(_where), file(_file), line(_line) { memcpy(s_fill, _s_fill, _n_fill); }

   h2_fail* check()
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x) {
         h2_fail* fail1 = p->violate_check();
         if (fail1) return fail1;
         h2_fail* fail2 = p->leak_check(where, file, line);
         if (fail2) return fail2;
      }
      /* why not chain fails in subling? report one fail ignore more for clean.
         when fail, memory may be in used, don't free and keep it for robust */
      h2_list_for_each_entry (p, pieces, h2_piece, x) {
         p->x.out();
         delete p;
      }
      return nullptr;
   }

   h2_piece* new_piece(const char* who, size_t size, size_t alignment, unsigned char c_fill, bool fill, h2_backtrace& bt)
   {
      if (limited < size) return nullptr;
      limited -= size;

      // allocate action alignment is prior to block level alignment
      if (alignment == 0)
         alignment = align;

      h2_piece* p = new h2_piece(size, alignment, who, bt);

      // allocate action fill is prior to block level fill
      unsigned char* s_filling = s_fill;
      int n_filling = n_fill;
      if (fill) {
         s_filling = &c_fill;
         n_filling = 1;
      }
      if (0 < n_filling)
         for (int i = 0, j = 0; i < size; ++i, ++j)
            ((unsigned char*)p->user_ptr)[i] = s_filling[j % n_filling];

      pieces.push(p->x);
      return p;
   }

   h2_piece* get_piece(const void* ptr)
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->user_ptr == ptr) return p;
      return nullptr;
   }

   h2_fail* rel_piece(const char* who, h2_piece* p)
   {
      limited += p->user_size;
      return p->free(who);
   }

   h2_piece* host_piece(const void* addr)
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->in_page_range((const unsigned char*)addr)) return p;
      return nullptr;
   }
};
