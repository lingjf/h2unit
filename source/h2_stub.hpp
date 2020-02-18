// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_thunk {
#if defined __x86_64__
   unsigned char saved_code[sizeof(void*) + 4];
#elif defined __i386__
   unsigned char saved_code[sizeof(void*) + 1];
#endif

   void* save(void* befp) {
      static uintptr_t pagesize = (uintptr_t)sysconf(_SC_PAGE_SIZE);
      uintptr_t start = reinterpret_cast<uintptr_t>(befp);
      uintptr_t pagestart = start & (~(pagesize - 1));

      if (mprotect(reinterpret_cast<void*>(pagestart), H2_ALIGN_UP(start + sizeof(saved_code) - pagestart, pagesize), PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         printf("STUB failed %s\n", strerror(errno));
         return nullptr;
      }

      memcpy(saved_code, befp, sizeof(saved_code));
      return befp;
   }

   void set(void* befp, void* tofp) {
      unsigned char* I = reinterpret_cast<unsigned char*>(befp);
      ptrdiff_t delta = (unsigned char*)tofp - (unsigned char*)befp;

#if defined(__i386__) || defined(__x86_64__)
      //x86 __asm("jmp $tofp") : 0xE9 {offset=tofp-befp-5}
      //x86 __asm("movl $tofp, %eax; jmpl %eax") : 0xB8 {tofp} 0xFF 0xE0
      //x86_64 __asm("movq $tofp, %rax; jmpq %rax") : 0x48 0xB8 {tofp} 0xFF 0xE0
      if (delta < INT_MIN || INT_MAX < delta) {
         unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
         memcpy(C + 2, &tofp, sizeof(void*));
         memcpy(I, C, sizeof(C));
      } else {
         *I++ = 0xE9;
         *(int32_t*)I = delta - 5;
      }
#endif
   }

   void* reset(void* befp) { return memcpy(befp, saved_code, sizeof(saved_code)); }
};

struct h2_stub : protected h2_thunk {
   h2_list x;
   void* befp;
   const char* file;
   int line;

   h2_stub(void* befp_, const char* file_ = nullptr, int line_ = 0) : file(file_), line(line_) { befp = save(befp_); }

   void replace(void* tofp) { set(befp, tofp); }
   void restore() { befp&& reset(befp); }

   static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
   static void operator delete(void* ptr) { h2_raw::free(ptr); }
};
