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
      int pagecount = ::ceil((start + sizeof(saved_code) - pagestart) / (double)pagesize);

      if (mprotect(reinterpret_cast<void*>(pagestart), pagecount * pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         ::printf("STUB: mprotect PROT_READ | PROT_WRITE | PROT_EXEC failed %s\n", strerror(errno));
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

struct h2_native : h2_thunk {
   h2_list x;
   void* befp;
   int refcount;
   h2_native(void* befp_) : befp(befp_), refcount(0) { save(befp); }
   void restore() { reset(befp); }
};

struct h2_natives {
   h2_singleton(h2_natives);

   h2_list s;
   h2_native* get(void* befp) {
      h2_list_for_each_entry(p, &s, h2_native, x) if (p->befp == befp) return p;
      return nullptr;
   }
   void dec(void* befp) {
      h2_native* p = get(befp);
      if (!p) return;
      if (--p->refcount == 0) {
         p->x.out();
         h2_libc::free(p);
      }
   }
   void inc(void* befp) {
      h2_native* p = get(befp);
      if (!p) {
         p = new (h2_libc::malloc(sizeof(h2_native))) h2_native(befp);
         s.push(&p->x);
      }
      p->refcount++;
   }
};

h2_inline h2_stub::h2_stub(void* befp_, const char* file_, int line_) : file(file_), line(line_) {
   h2_natives::I().inc(befp_);
   befp = ((h2_thunk*)thunk)->save(befp_);
}
h2_inline h2_stub::~h2_stub() {
   h2_natives::I().dec(befp);
}

h2_inline void h2_stub::set(void* tofp_) { ((h2_thunk*)thunk)->set(befp, tofp = tofp_); }
h2_inline void h2_stub::reset() { befp && ((h2_thunk*)thunk)->reset(befp); }
h2_inline void h2_stub::restore() { h2_natives::I().get(befp)->restore(); }

h2_inline bool h2_stubs::add(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line) {
   h2_stub* stub = nullptr;
   h2_list_for_each_entry(p, &s, h2_stub, x) h2_if_find_break(p->befp == befp, p, stub);

   if (!tofp) { /* unstub */
      if (stub) {
         stub->reset();
         stub->x.out();
         delete stub;
      }
      return true;
   }

   if (!stub) {
      stub = new h2_stub(befp, file, line);
      s.push(&stub->x);
   }
   stub->set(tofp);
   return true;
}

h2_inline void h2_stubs::clear() {
   h2_list_for_each_entry(p, &s, h2_stub, x) {
      p->reset();
      p->x.out();
      delete p;
   }
}
