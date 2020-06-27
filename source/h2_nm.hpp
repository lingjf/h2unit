
struct h2_nm {
   h2_singleton(h2_nm);
   h2_list symbols;
   h2_nm();
   static unsigned long long get(const char* name);
   static long long text_offset();
   static long long vtable_offset();
   static bool in_main(unsigned long long addr);
};
