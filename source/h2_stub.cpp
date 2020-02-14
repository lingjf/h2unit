// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_thunk {
#if defined __x86_64__
   unsigned char saved_code[sizeof(void*) + 4];
#elif defined __i386__
   unsigned char saved_code[sizeof(void*) + 1];
#endif

   bool save(void* befp) {
      static uintptr_t pagesize = (uintptr_t)sysconf(_SC_PAGE_SIZE);
      uintptr_t start = reinterpret_cast<uintptr_t>(befp);
      uintptr_t pagestart = start & (~(pagesize - 1));

      if (mprotect(reinterpret_cast<void*>(pagestart), H2_ALIGN_UP(start + sizeof(saved_code) - pagestart, pagesize), PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         printf("STUB failed %s\n", strerror(errno));
         return false;
      }

      memcpy(saved_code, befp, sizeof(saved_code));
      return true;
   }

   void set(void* befp, void* tofp) {
      unsigned char* I = reinterpret_cast<unsigned char*>(befp);

      //x86 __asm("jmp $tofp") : 0xE9 {offset=tofp-befp-5}
      //x86 __asm("movl $tofp, %eax; jmpl %eax") : 0xB8 {tofp} 0xFF 0xE0
      //x86_64 __asm("movq $tofp, %rax; jmpq %rax") : 0x48 0xB8 {tofp} 0xFF 0xE0
#if defined(__i386__) || defined(__x86_64__)
      ptrdiff_t delta = (unsigned char*)tofp - (unsigned char*)befp;
#   if defined __x86_64__
      if (delta < INT_MIN || INT_MAX < delta) {
         *I++ = 0x48;
         *I++ = 0xB8;
         memcpy(I, &tofp, sizeof(void*));
         I += sizeof(void*);
         *I++ = 0xFF;
         *I++ = 0xE0;
         return;
      }
#   endif

      int32_t offset = delta - 5;
      *I++ = 0xE9;
      memcpy(I, (void*)&offset, sizeof(offset));
#endif
   }

   void reset(void* befp) { memcpy(befp, saved_code, sizeof(saved_code)); }
};

struct h2_stub : protected h2_thunk {
   h2_list x;
   void* befp;
   const char* file;
   int line;

   h2_stub(void* befp_, const char* file_ = nullptr, int line_ = 0) : befp(befp_), file(file_), line(line_) {
      if (!save(befp_)) befp = nullptr;
   }

   void replace(void* tofp) { set(befp, tofp); }

   void restore() {
      if (befp) reset(befp);
   }

   static void* operator new(std::size_t sz) { return h2_raw::malloc(sz); }
   static void operator delete(void* ptr) { h2_raw::free(ptr); }
};
