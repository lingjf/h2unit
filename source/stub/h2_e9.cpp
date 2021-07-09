// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_e9 {
#if defined __i386__ || defined _M_IX86
   static constexpr int size = 1 + 4;
#elif defined __x86_64__ || defined _M_X64
   static constexpr int size = 2 + 8 + 2;
#elif defined __arm__ || defined __arm64__ || defined __aarch64__
   static constexpr int size = 4 + 4 + 8;
#endif

   static bool save(void* origin_fp, unsigned char* saved)
   {
#if defined _WIN32
      DWORD t;
      if (!VirtualProtect(origin_fp, sizeof(void*) + 4, PAGE_EXECUTE_READWRITE, &t))  // PAGE_EXECUTE_WRITECOPY OR PAGE_WRITECOPY
         return false;
#else
      /* uintptr_t is favourite, but is optional type in c++ std, using unsigned long long for portable */
      unsigned long long page_size = (unsigned long long)h2_page_size();
      unsigned long long origin_start = reinterpret_cast<unsigned long long>(origin_fp);
      unsigned long long page_start = origin_start & ~(page_size - 1);
      int page_count = ::ceil((origin_start + size - page_start) / (double)page_size);

      if (mprotect(reinterpret_cast<void*>(page_start), page_count * page_size, PROT_READ | PROT_WRITE | PROT_EXEC) != 0)
         return false;
#endif
      if (saved) memcpy(saved, origin_fp, size);
      return true;
   }

   static void set(void* origin_fp, void* substitute_fp)
   {
      unsigned char* I = reinterpret_cast<unsigned char*>(origin_fp);
      long long delta = (unsigned char*)substitute_fp - (unsigned char*)origin_fp - 5;

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64
      if (delta < INT_MIN || INT_MAX < delta) {  //x86_64 asm("movq $substitute_fp, %rax; jmpq %rax")
         unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
         memcpy(C + 2, &substitute_fp, sizeof(void*));
         memcpy(I, C, sizeof(C));
      } else {  //i386 asm("jmp offset")
         unsigned char C[] = {0xE9, 0, 0, 0, 0};
         *(long*)(&C[1]) = delta;
         memcpy(I, C, sizeof(C));
      }
#elif defined __arm__ || defined __arm64__ || defined __aarch64__

#   pragma pack(push, 1)
      struct ldr_br_dst {
         unsigned int ldr;
         unsigned int br;
         void* dstfp;
      };
      struct b_offset {
         unsigned int b : 8;
         unsigned int offset : 24;
      };
#   pragma pack(pop)
      long long offset = (unsigned char*)substitute_fp - I;

      if (1 || offset < -8388607 - 1 || 0x7fffff < offset) {  // signed 2^24
         struct ldr_br_dst* p = static_cast<ldr_br_dst*>(origin_fp);
         p->ldr = 0x58000000 | ((8 / 4) << 5) | 17;  // x17 register store dstfp
         p->br = 0xD61F0000 | (17 << 5);             // jmp x17
         p->dstfp = substitute_fp;
      } else {
         struct b_offset* p = static_cast<b_offset*>(origin_fp);
         p->b = offset >= 0 ? 0x14 : 0x17;  //b 14 forward 17 backward
         p->offset = (unsigned long)((offset / 4) & 0xffffff);
      }

      __builtin___clear_cache((char*)I, (char*)I + size);
#endif
   }

   static void reset(void* origin_fp, unsigned char* saved)
   {
      memcpy(origin_fp, saved, size);
#if defined __arm__ || defined __arm64__ || defined __aarch64__
      __builtin___clear_cache(static_cast<char*>(origin_fp), static_cast<char*>(origin_fp) + size);
#endif
   }
};
