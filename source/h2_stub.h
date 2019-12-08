
#ifndef ___H2_STUB__H___
#define ___H2_STUB__H___

// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_stub
{
   void* be_fp;
#if defined(__x86_64__) || defined(_M_X64)
   unsigned char saved_code[sizeof(void*) + 4];
#elif defined(__i386__) || defined(_M_IX86)
   unsigned char saved_code[sizeof(void*) + 1];
#else
   unsigned char saved_code[1];
#endif

   static h2_stub* acq()
   {
      h2_stub* stub = (h2_stub*)h2_alloc::I().malloc(sizeof(h2_stub));
      return stub;
   }
   static void rel(h2_stub* stub)
   {
      h2_alloc::I().free(stub);
   }

   bool save(void* be_fp)
   {
#ifdef _WIN32
      DWORD saved;
      if (!VirtualProtect(be_fp, sizeof(void*) + 4, PAGE_EXECUTE_READWRITE, &saved)) {  // PAGE_EXECUTE_WRITECOPY OR PAGE_WRITECOPY
         return false;
      }
#else
      long long pagesize = (long long)sysconf(_SC_PAGE_SIZE);
      if (mprotect((void*)((unsigned long long)be_fp & (~(pagesize - 1))), pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         printf("STUB failed %s\n", strerror(errno));
         return false;
      }
      if (mprotect((void*)(((unsigned long long)be_fp + sizeof(saved_code)) & (~(pagesize - 1))), pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         printf("STUB failed %s\n", strerror(errno));
         return false;
      }
#endif

      this->be_fp = be_fp;
      memcpy(saved_code, be_fp, sizeof(saved_code));
      return true;
   }
   void set(void* to_fp)
   {
      unsigned char* I = (unsigned char*)be_fp;

      //x86 __asm("jmp $to_fp") : 0xE9 {offset=to_fp-be_fp-5}
      //x86 __asm("movl $to_fp, %eax; jmpl %eax") : 0xB8 {to_fp} 0xFF 0xE0
      //x86_64 __asm("movq $to_fp, %rax; jmpq %rax") : 0x48 0xB8 {to_fp} 0xFF 0xE0
#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)
      long long delta = (long long)to_fp - (long long)be_fp;
#   if defined(__x86_64__) || defined(_M_X64)
      if (delta < (int)(-2147483647 - 1) || (int)2147483647 < delta) {
         *I++ = 0x48;
         *I++ = 0xB8;
         memcpy(I, &to_fp, sizeof(void*));
         I += sizeof(void*);
         *I++ = 0xFF;
         *I++ = 0xE0;
         return;
      }
#   endif

      int offset = delta - 5;
      *I++ = 0xE9;
      memcpy(I, (void*)&offset, sizeof(offset));

#elif defined(__powerpc__)
#else
#endif
   }
   void restore()
   {
      memcpy(be_fp, saved_code, sizeof(saved_code));
   }
};

#endif
