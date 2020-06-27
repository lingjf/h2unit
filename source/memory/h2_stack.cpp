
struct h2_stack {
   h2_singleton(h2_stack);
   h2_list blocks;

   void push(long long limit, size_t align, unsigned char s_fill[32], int n_fill, bool noleak, const char* where, const char* file, int line)
   {
      h2_block* b = new h2_block(limit, align, s_fill, n_fill, noleak, where, file, line);
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
      h2_backtrace bt(O.isMAC() ? 3 : 2);
      h2_block* b = h2_patch::exempt(bt) ? h2_list_bottom_entry(blocks, h2_block, x) : h2_list_top_entry(blocks, h2_block, x);
      h2_list_for_each_reverse_entry (p, blocks, h2_block, x) {  // from bottom to current
         if (p->limited(size)) return nullptr;
         if (p == b) break;
      }
      h2_list_for_each_reverse_entry (p, blocks, h2_block, x) {  // from bottom to current
         p->balance(size);
         if (p == b) break;
      }
      return b ? b->new_piece(who, size, alignment, fill ? *fill : 0, fill, bt) : nullptr;
   }

   h2_fail* rel_piece(const char* who, void* ptr)
   {
      h2_list_for_each_entry (p, blocks, h2_block, x) {  // from top to bottom
         h2_piece* piece = p->get_piece(ptr);
         if (piece) {
            h2_list_for_each_reverse_entry (q, blocks, h2_block, x) {  // from bottom to current
               q->balance(-(int)piece->user_size);
               if (q == p) break;
            }
            return p->rel_piece(who, piece);
         }
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
