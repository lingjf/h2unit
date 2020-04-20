// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_thunk {
#if defined __x86_64__
   unsigned char saved_code[sizeof(void*) + 4];
#elif defined __i386__
   unsigned char saved_code[sizeof(void*) + 1];
#endif

   void* save(void* befp) {
      uintptr_t pagesize = (uintptr_t)sysconf(_SC_PAGE_SIZE);
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
      ptrdiff_t delta = (unsigned char*)tofp - (unsigned char*)befp - 5;

#if defined(__i386__) || defined(__x86_64__)
      if (delta < INT_MIN || INT_MAX < delta) {  //x86_64 asm("movq $tofp, %rax; jmpq %rax")
         unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
         memcpy(C + 2, &tofp, sizeof(void*));
         memcpy(I, C, sizeof(C));
      } else {  //i386 asm("jmp offset")
         unsigned char C[] = {0xE9, 0, 0, 0, 0};
         *(int32_t*)(&C[1]) = delta;
         memcpy(I, C, sizeof(C));
      }
#endif
   }

   void reset(void* befp) { memcpy(befp, saved_code, sizeof(saved_code)); }
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
   h2_list natives;
   h2_native* get(void* befp) {
      h2_list_for_each_entry(p, &natives, h2_native, x) if (p->befp == befp) return p;
      return nullptr;
   }
   void dec(void* befp) {
      h2_native* native = get(befp);
      if (native)
         if (--native->refcount == 0) {
            native->x.out();
            h2_libc::free(native);
         }
   }
   void inc(void* befp) {
      h2_native* native = get(befp);
      if (!native) {
         native = new (h2_libc::malloc(sizeof(h2_native))) h2_native(befp);
         natives.push(&native->x);
      }
      native->refcount++;
   }
};

struct h2_stub : h2_thunk, h2_libc {
   h2_list x;
   void *befp, *tofp;
   const char* file;
   int line;

   h2_stub(void* befp_, const char* file_ = nullptr, int line_ = 0) : file(file_), line(line_) {
      h2_natives::I().inc(befp_);
      befp = save(befp_);
   }
   ~h2_stub() { h2_natives::I().dec(befp); }
   void set(void* tofp_) {
      tofp = tofp_;
      h2_thunk::set(befp, tofp);
   }
   void reset() {
      if (befp) h2_thunk::reset(befp);
   }
};

h2_inline bool h2_stubs::add(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line) {
   h2_stub* stub = nullptr;
   h2_list_for_each_entry(p, &stubs, h2_stub, x) if (p->befp == befp) {
      stub = p;
      break;
   }
   if (!stub) {
      stub = new h2_stub(befp, file, line);
      stubs.push(&stub->x);
   }
   stub->set(tofp);
   return true;
}
h2_inline void h2_stubs::clear() {
   h2_list_for_each_entry(p, &stubs, h2_stub, x) {
      p->reset();
      p->x.out();
      delete p;
   }
}

h2_inline h2_stub_temporary_restore::h2_stub_temporary_restore(void* befp_) {
   befp = ((h2_thunk*)current)->save(befp_);
   h2_native* native = h2_natives::I().get(befp);
   if (native) native->restore();
}
h2_inline h2_stub_temporary_restore::~h2_stub_temporary_restore() {
   if (befp) ((h2_thunk*)current)->reset(befp);
}
