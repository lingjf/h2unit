

static inline long long get_load_text_offset()
{
   h2_symbol* s[16];
   if (h2_nm::get_by_name("main", s, 16) == 0) return 0;
   return (long long)&main - (long long)s[0]->offset;
}

struct h2_vtable_offset_test {
   h2_vtable_offset_test() {}
   virtual ~h2_vtable_offset_test() {}
   virtual void dummy() {}
};

static inline long long get_load_vtable_offset()
{
   char vtable_symbol[256];
   h2_vtable_offset_test t;
   long long absolute_vtable = (long long)*(void***)&t;
   sprintf(vtable_symbol, "_ZTV%s", typeid(h2_vtable_offset_test).name());  // mangled for "vtable for h2::h2_vtable_offset_test"
   long long relative_vtable = (long long)h2_nm::get_mangled(vtable_symbol);
   if (relative_vtable == 0)
      h2_color::prints("yellow", "\nDon't find vtable for h2::h2_vtable_offset_test %s\n", vtable_symbol);
   return absolute_vtable - relative_vtable;
}

h2_inline void* h2_load::vtable_to_addr(unsigned long long offset)
{
   if (I().vtable_offset == -1) I().vtable_offset = get_load_vtable_offset();
   return (void*)(offset + I().vtable_offset);
}

h2_inline void* h2_load::symbol_to_addr(unsigned long long symbol_offset)
{
#if defined _WIN32
   return (void*)symbol_offset;
#else
   if (I().text_offset == -1) I().text_offset = get_load_text_offset();
   return (void*)(symbol_offset + I().text_offset);
#endif
}

h2_inline unsigned long long h2_load::addr_to_symbol(void* addr)
{
#if defined _WIN32
   return (unsigned long long)addr;
#else
   if (I().text_offset == -1) I().text_offset = get_load_text_offset();
   return (unsigned long long)addr - I().text_offset;
#endif
}

h2_inline void h2_load::backtrace_scope(void*& addr, int& size)
{
#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64
   unsigned char* p = (unsigned char*)addr;

   // e8/ff15/ff25 call
   if ((*p == 0xE8) || (*p == 0xFF && (*(p + 1) == 0x15 || *(p + 1) == 0x25))) {
      size = 16;
      return;
   }
   // e9 jmp
   if (*p == 0xE9) {
      addr = (void*)(p + 5 + *(long*)(p + 1));
      backtrace_scope(addr, size);
      return;
   }
   if (size == 0) {
      for (unsigned char* q = p + 1;; q++) {
         // cc      int 3;
         // 5d c3   pop %ebp; ret;
         // 5b c3   pop %ebx; ret;
         // c9 c3   leave; ret;
         if ((*q == 0xCC) ||
             (*q == 0xC3 && ((*(q - 1) == 0x5D) || (*(q - 1) == 0x5B) || (*(q - 1) == 0xC9)))) {
            size = (unsigned long)(q - p);
            return;
         }
      }
   }
#endif
}

h2_inline bool h2_load::in_main(void* addr)
{
   unsigned long long main_addr = (unsigned long long)&main;
   /* main() 52~60 bytes code in linux MAC */
   return main_addr < (unsigned long long)addr && (unsigned long long)addr < main_addr + 128;
}
