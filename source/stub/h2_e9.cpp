// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_e9 {
   static constexpr int size = sizeof(void*) == 8 ? 2 + 8 + 2 : 5;

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
#endif
   }

   static void reset(void* origin_fp, unsigned char* saved)
   {
      memcpy(origin_fp, saved, size);
   }
};
