

static inline long long get_load_text_offset()
{
   h2_symbol* s[16];
   if (h2_nm::get_by_name("main", s, 16) == 0) return 0;
   return (long long)&main - (long long)s[0]->addr;
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
   long long relative_vtable = (long long)h2_nm::get_mangle(vtable_symbol);
   if (relative_vtable == 0)
      h2_color::prints("yellow", "\nDon't find vtable for h2::h2_vtable_offset_test %s\n", vtable_symbol);
   return absolute_vtable - relative_vtable;
}

h2_inline void* h2_load::vtable_to_ptr(unsigned long long addr)
{
   if (I().vtable_offset == -1) I().vtable_offset = get_load_vtable_offset();
   return (void*)(addr + I().vtable_offset);
}

h2_inline void* h2_load::addr_to_ptr(unsigned long long addr)
{
#if defined _WIN32
   return (void*)addr;
#else
   if (I().text_offset == -1) I().text_offset = get_load_text_offset();
   return (void*)(addr + I().text_offset);
#endif
}

h2_inline unsigned long long h2_load::ptr_to_addr(void* ptr)
{
#if defined _WIN32
   return (unsigned long long)ptr;
#else
   if (I().text_offset == -1) I().text_offset = get_load_text_offset();
   return (unsigned long long)ptr - I().text_offset;
#endif
}

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64

static inline void* follow_jmp(void* pc)
{
   unsigned char* p = (unsigned char*)pc;
   if (!p) return NULL;

#   if 0
   if (p[0] == 0xff && p[1] == 0x25) {  // jmp [imm32]
      unsigned char* p1 = sizeof(void*) == 8 ? p + 6 + *(long*)(p + 2) : *(unsigned char**)(p + 2);
      unsigned char* p2 = *(unsigned char**)p1;
      // ::printf("%p->%p: skipped over import table.\n", p, p2);  // skip over the import vector
      return follow_jmp(p2);
   }

   if (p[0] == 0xeb) {  // jmp +imm8
      unsigned char* p1 = p + 2 + *(char*)(p + 1);
      // ::printf("%p->%p: skipped over short jump.\n", p, p1);  // skip over a patch jump
      return follow_jmp(p1);
   }
#   endif

   if (p[0] == 0xe9) {  // jmp +imm32
      unsigned char* p1 = p + 5 + *(long*)(p + 1);
      // ::printf("%p->%p: skipped over long jump.\n", p, p1);  // skip over a long jump if it is the target of the patch jump.
      return follow_jmp(p1);
   }
   return (void*)p;
}

#elif defined __arm__ || defined __arm64__ || defined __aarch64__

static inline void* follow_jmp(void* pc)
{
   return pc;
}

#endif
