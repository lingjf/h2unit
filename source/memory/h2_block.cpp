struct h2_block_attributes {
   unsigned long long limit = 0xffffffffffffull;
   size_t alignment = sizeof(void*);
   size_t n_fill = 0;
   unsigned char s_fill[32];
   bool noleak = false;

   size_t parse_fill(const char* p, unsigned char bytes[])
   {
      if (p[0] == '0' && ::tolower(p[1]) == 'x') {
         return hex2bytes(p + 2, bytes);
      } else {
         unsigned long long v = strtoull(p, nullptr, 10);
         if (v <= 0xFFULL) return *((unsigned char*)bytes) = (unsigned char)v, 1;
         else if (v <= 0xFFFFULL) return *((unsigned short*)bytes) = (unsigned short)v, 2;
         else if (v <= 0xFFFFFFFFULL) return *((unsigned int*)bytes) = (unsigned int)v, 4;
         else return *((unsigned long long*)bytes) = (unsigned long long)v, 8;
      }
   }

   h2_block_attributes(const char* attributes)
   {
      const char* p;
      if (get_keyvalue(attributes, "noleak")) noleak = true;
      if ((p = get_keyvalue(attributes, "limit"))) limit = strtod(p, nullptr);
      if ((p = get_keyvalue(attributes, "align"))) alignment = strtod(p, nullptr);
      if ((p = get_keyvalue(attributes, "fill"))) n_fill = parse_fill(p, s_fill);
   }
};

struct h2_block : h2_libc {
   h2_list x;
   h2_list pieces;

   h2_block_attributes attributes;
   unsigned long long footprint = 0, allocated = 0;
   const char* where;
   const char* filine;

   h2_block(const char* attributes_, const char* where_, const char* filine_) : attributes(attributes_), where(where_), filine(filine_) {}

   h2_fail* check()
   {
      h2_fail* fails = nullptr;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->violate_times)
            h2_fail::append_subling(fails, p->violate_fail());

      if (fails) {
         if (O.as_waring_memory_violate) fails->warning = true;
         return fails;
      }
      h2_leaky leaky;
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (!attributes.noleak && !p->free_times)
            leaky.add(p->user_ptr, p->user_size, p->bt_allocate);

      fails = leaky.check(where, filine);
      if (fails) {
         if (O.as_waring_memory_leak) fails->warning = true;
         return fails;
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
      if (attributes.limit < allocated + size) return nullptr;
      allocated += size;
      if (footprint < allocated) footprint = allocated;

      // allocate action alignment is prior to block level alignment
      if (alignment == 0) alignment = attributes.alignment;

      h2_piece* p = new h2_piece(size, alignment, who, bt);

      // allocate action fill is prior to block level fill
      unsigned char* s_fill = attributes.s_fill;
      size_t n_fill = attributes.n_fill;
      if (fill) {
         s_fill = &c_fill;
         n_fill = 1;
      }
      if (0 < n_fill)
         for (size_t i = 0, j = 0; i < size; ++i, ++j)
            ((unsigned char*)p->user_ptr)[i] = s_fill[j % n_fill];

      pieces.push_back(p->x);
      return p;
   }

   h2_fail* rel_piece(const char* who, h2_piece* p)
   {
      allocated -= p->user_size;
      return p->free(who);
   }

   h2_piece* get_piece(const void* ptr) const
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->user_ptr == ptr) return p;
      return nullptr;
   }

   h2_piece* host_piece(const void* ptr) const
   {
      h2_list_for_each_entry (p, pieces, h2_piece, x)
         if (p->in_page_range((const unsigned char*)ptr)) return p;
      return nullptr;
   }
};
