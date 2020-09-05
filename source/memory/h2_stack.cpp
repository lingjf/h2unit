
struct h2_stack {
   h2_singleton(h2_stack);
   h2_list blocks;

   void push(long long limit, size_t align, unsigned char s_fill[32], int n_fill, bool noleak, const char* where, const char* file, int lino)
   {
      h2_block* b = new h2_block(limit, align, s_fill, n_fill, noleak, where, file, lino);
      blocks.push(b->x);
   }

   h2_fail* pop()
   {
      h2_block* b = h2_list_pop_entry(blocks, h2_block, x);
      h2_fail* fail = b->check();
      delete b;
      return fail;
   }

   h2_block* top()
   {
      return h2_list_top_entry(blocks, h2_block, x);
   }

   h2_piece* new_piece(const char* who, size_t size, size_t alignment, const char* fill)
   {
      h2_backtrace bt(strcmp("macos", O.os) ? 2 : 3);
      h2_block* b = h2_patch::exempt(bt) ? h2_list_bottom_entry(blocks, h2_block, x) : h2_list_top_entry(blocks, h2_block, x);
      return b ? b->new_piece(who, size, alignment, fill ? *fill : 0, fill, bt) : nullptr;
   }

   h2_fail* rel_piece(const char* who, void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return p->rel_piece(who, piece);
      }
      h2_debug("Warning: free %p not found!", ptr);
      return nullptr;
   }

   h2_piece* get_piece(const void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->get_piece(ptr);
         if (piece) return piece;
      }
      return nullptr;
   }

   h2_piece* host_piece(const void* addr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {
         h2_piece* piece = p->host_piece(addr);
         if (piece) return piece;
      }
      return nullptr;
   }
};
