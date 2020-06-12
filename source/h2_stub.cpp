// https://www.codeproject.com/Articles/25198/Generic-Thunk-with-5-combinations-of-Calling-Conve

struct h2_thunk {
#if defined __x86_64__ || defined _M_X64
   unsigned char saved_code[sizeof(void*) + 4];
#elif defined __i386__ || defined _M_IX86
   unsigned char saved_code[sizeof(void*) + 1];
#else

#endif

   void* save(void* origin_fp)
   {
#ifdef _WIN32
      DWORD t;
      if (!VirtualProtect(origin_fp, sizeof(void*) + 4, PAGE_EXECUTE_READWRITE, &t)) {  // PAGE_EXECUTE_WRITECOPY OR PAGE_WRITECOPY
         ::printf("STUB: VirtualProtect PAGE_EXECUTE_READWRITE failed %d\n", GetLastError());
         return nullptr;
      }
#else
      long long pagesize = (long long)h2_page_size();
      long long start = reinterpret_cast<long long>(origin_fp);
      long long pagestart = start & (~(pagesize - 1));
      int pagecount = ::ceil((start + sizeof(saved_code) - pagestart) / (double)pagesize);

      if (mprotect(reinterpret_cast<void*>(pagestart), pagecount * pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
         ::printf("STUB: mprotect PROT_READ | PROT_WRITE | PROT_EXEC failed %s\n", strerror(errno));
         return nullptr;
      }
#endif
      memcpy(saved_code, origin_fp, sizeof(saved_code));
      return origin_fp;
   }

   void set(void* origin_fp, void* substitute_fp)
   {
      unsigned char* I = reinterpret_cast<unsigned char*>(origin_fp);
      ptrdiff_t delta = (unsigned char*)substitute_fp - (unsigned char*)origin_fp - 5;

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64
      if (delta < INT_MIN || INT_MAX < delta) {  //x86_64 asm("movq $substitute_fp, %rax; jmpq %rax")
         unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
         memcpy(C + 2, &substitute_fp, sizeof(void*));
         memcpy(I, C, sizeof(C));
      } else {  //i386 asm("jmp offset")
         unsigned char C[] = {0xE9, 0, 0, 0, 0};
         *(int32_t*)(&C[1]) = delta;
         memcpy(I, C, sizeof(C));
      }
#endif
   }

   void reset(void* origin_fp)
   {
      memcpy(origin_fp, saved_code, sizeof(saved_code));
   }
};

struct h2_native : h2_thunk {
   h2_list x;
   void* origin_fp;
   int refcount = 0;
   h2_native(void* origin_fp_) : origin_fp(origin_fp_) { save(origin_fp); }
   void restore() { reset(origin_fp); }
};

struct h2_natives {
   h2_singleton(h2_natives);
   h2_list natives;
   h2_native* get(void* origin_fp)
   {
      h2_list_for_each_entry (p, natives, h2_native, x)
         if (p->origin_fp == origin_fp) return p;
      return nullptr;
   }
   void dec(void* origin_fp)
   {
      h2_native* native = get(origin_fp);
      if (native) {
         if (--native->refcount == 0) {
            native->x.out();
            h2_libc::free(native);
         }
      }
   }
   void inc(void* origin_fp)
   {
      h2_native* native = get(origin_fp);
      if (!native) {
         native = new (h2_libc::malloc(sizeof(h2_native))) h2_native(origin_fp);
         natives.push(native->x);
      }
      native->refcount++;
   }
};

struct h2_stub : h2_thunk, h2_libc {
   h2_list x;
   void *origin_fp, *substitute_fp;
   const char* file;
   int line;

   h2_stub(void* origin_fp_, const char* file_ = nullptr, int line_ = 0) : file(file_), line(line_)
   {
      h2_natives::I().inc(origin_fp_);
      origin_fp = save(origin_fp_);
   }
   ~h2_stub() { h2_natives::I().dec(origin_fp); }
   void set(void* substitute_fp_)
   {
      substitute_fp = substitute_fp_;
      h2_thunk::set(origin_fp, substitute_fp);
   }
   void reset()
   {
      if (origin_fp) h2_thunk::reset(origin_fp);
   }
};

h2_inline bool h2_stubs::add(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* substitute_fn, const char* file, int line)
{
   h2_stub* stub = nullptr;
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      if (p->origin_fp == origin_fp) {
         stub = p;
         break;
      }
   }
   if (!stub) {
      stub = new h2_stub(origin_fp, file, line);
      stubs.push(stub->x);
   }
   stub->set(substitute_fp);
   return true;
}
h2_inline void h2_stubs::clear()
{
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      p->reset();
      p->x.out();
      delete p;
   }
}

h2_inline h2_stub_temporary_restore::h2_stub_temporary_restore(void* origin_fp_)
{
   origin_fp = ((h2_thunk*)current)->save(origin_fp_);
   h2_native* native = h2_natives::I().get(origin_fp);
   if (native) native->restore();
}
h2_inline h2_stub_temporary_restore::~h2_stub_temporary_restore()
{
   if (origin_fp) ((h2_thunk*)current)->reset(origin_fp);
}
