
struct h2_stub : h2_libc {
   h2_list x;
   unsigned char saved_opcode[32];
   void *srcfp, *dstfp;

   h2_stub(void* _srcfp, const char* srcfn, const char* file, int line) : srcfp(_srcfp)
   {
      h2_source* source = h2_sources::I().add(srcfp, srcfn, file, line);
      if (source) source->save(saved_opcode);
   }
   ~h2_stub()
   {
      h2_source* source = h2_sources::I().get(srcfp);
      if (source) {
         source->reset(saved_opcode);
         h2_sources::I().del(source);
      }
   }
   void stub(void* _dstfp)
   {
      h2_source* source = h2_sources::I().get(srcfp);
      if (source) source->set((dstfp = _dstfp));
   }
};

static inline h2_stub* h2_stubs_get(h2_stubs* stubs, void* srcfp)
{
   h2_list_for_each_entry (p, stubs->stubs, h2_stub, x)
      if (p->srcfp == srcfp)
         return p;
   return nullptr;
}

h2_inline bool h2_stubs::add(void* srcfp, void* dstfp, const char* srcfn, const char* file, int line)
{
   h2_stub* stub = h2_stubs_get(this, srcfp);
   if (!stub) {
      stub = new h2_stub(srcfp, srcfn, file, line);
      stubs.push(stub->x);
   }
   stub->stub(dstfp);
   return true;
}

h2_inline void h2_stubs::clear(void* srcfp)
{
   h2_stub* stub = h2_stubs_get(this, srcfp);
   if (stub) {
      stub->x.out();
      delete stub;
   }
}

h2_inline void h2_stubs::clear()
{
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      p->x.out();
      delete p;
   }
}

h2_inline h2_stub_temporary_restore::h2_stub_temporary_restore(void* _srcfp) : srcfp(_srcfp)
{
   h2_source* source = h2_sources::I().get(srcfp);
   if (source) {
      source->save(saved_opcode);
      source->reset();
   }
}

h2_inline h2_stub_temporary_restore::~h2_stub_temporary_restore()
{
   h2_source* source = h2_sources::I().get(srcfp);
   if (source) source->reset(saved_opcode);
}
