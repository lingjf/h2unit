
struct h2_load {
   h2_singleton(h2_load);
   long long text_offset = -1;
   long long vtable_offset = -1;
   static void* symbol_to_addr(unsigned long long symbol_offset);
   static unsigned long long addr_to_symbol(void* addr);
   static void* vtable_to_addr(unsigned long long offset);
};
