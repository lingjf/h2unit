
struct h2_block_attributes {
   long long limit = LLONG_MAX / 2;
   int alignment = sizeof(void*);
   unsigned char s_fill[32];
   int n_fill = 0;
   bool noleak = false;

   h2_block_attributes(const char* attributes)
   {
      const char* p_noleak = strcasestr(attributes, "noleak");
      if (p_noleak) noleak = true;

      const char* p_limit = strcasestr(attributes, "limit");
      if (p_limit) {
         const char* p = strchr(p_limit, '=');
         if (p) limit = (long long)strtod(p + 1, nullptr);
      }

      const char* p_align = strcasestr(attributes, "align");
      if (p_align) {
         const char* p = strchr(p_align, '=');
         if (p) alignment = (int)strtod(p + 1, nullptr);
      }

      const char* p_fill = strcasestr(attributes, "fill");
      if (p_fill) {
         const char* p = strchr(p_fill, '=');
         if (p) {
            for (p += 1; *p && ::isspace(*p);) p++;  // strip left space

            if (p[0] == '0' && ::tolower(p[1]) == 'x') {
               n_fill = hex_to_bytes(p + 2, s_fill);
            } else {
               long long v = strtoll(p, (char**)NULL, 10);
               if (v <= 0xFFU)
                  n_fill = 1, *((unsigned char*)s_fill) = (unsigned char)v;
               else if (v <= 0xFFFFU)
                  n_fill = 2, *((unsigned short*)s_fill) = (unsigned short)v;
               else if (v <= 0xFFFFFFFFU)
                  n_fill = 4, *((unsigned int*)s_fill) = (unsigned int)v;
               else
                  n_fill = 8, *((unsigned long long*)s_fill) = (unsigned long long)v;
            }
         }
      }
   }
};

struct h2_block : h2_libc {
   h2_list x;
   h2_list pieces;

   h2_block_attributes attributes;
   long long footprint = 0, allocated = 0;
   const char* where;
   const char* file;
   int line;

   h2_block(const char* _attributes, const char* _where, const char* _file, int _line) : attributes(_attributes), where(_where), file(_file), line(_line) {}

   h2_fail* check()
   {
      h2_fail* fails = nullptr;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->violate_times)
            h2_fail::append_subling(fails, p->violate_fail());

      if (fails) return fails;

      h2_leaky leaky;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (!attributes.noleak && !p->free_times)
            leaky.add(p->user_ptr, p->user_size, p->bt_allocate);

      fails = leaky.check(where, file, line);
      if (fails) return fails;

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
      if (attributes.limit < allocated + size) return nullptr;
      allocated += size;
      if (footprint < allocated) footprint = allocated;

      // allocate action alignment is prior to block level alignment
      if (alignment == 0) alignment = attributes.alignment;

      h2_piece* p = new h2_piece(size, alignment, who, bt);

      // allocate action fill is prior to block level fill
      unsigned char* s_fill = attributes.s_fill;
      int n_fill = attributes.n_fill;
      if (fill) {
         s_fill = &c_fill;
         n_fill = 1;
      }
      if (0 < n_fill)
         for (int i = 0, j = 0; i < size; ++i, ++j)
            ((unsigned char*)p->user_ptr)[i] = s_fill[j % n_fill];

      pieces.push_back(p->x);
      return p;
   }

   h2_fail* rel_piece(const char* who, h2_piece* p)
   {
      allocated -= p->user_size;
      return p->free(who);
   }

   h2_piece* get_piece(const void* ptr)
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->user_ptr == ptr) return p;
      return nullptr;
   }

   h2_piece* host_piece(const void* ptr)
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->in_page_range((const unsigned char*)ptr)) return p;
      return nullptr;
   }
};
