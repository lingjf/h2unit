struct h2_stub : h2_libc {
   h2_list x;
   unsigned char saved_opcode[32];
   void *srcfp, *dstfp;
   h2_source* source;

   h2_stub(void* srcfp_, const char* srcfn, const char* filine) : srcfp(srcfp_)
   {
      source = h2_sources::I().add(srcfp, srcfn, filine);
      if (source) source->save(saved_opcode);
   }
   ~h2_stub()
   {
      if (source) {
         source->reset(saved_opcode);
         h2_sources::I().del(source);
      }
   }
   void stub(void* dstfp_)
   {
      if (source) source->set((dstfp = dstfp_));
   }
};

static inline h2_stub* h2_stubs_get(h2_list& stubs, void* srcfp)
{
   h2_list_for_each_entry (p, stubs, h2_stub, x)
      if (p->srcfp == srcfp)
         return p;
   return nullptr;
}

h2_inline bool h2_stubs::add(h2_list& stubs, void* srcfp, void* dstfp, const char* srcfn, const char* filine)
{
   h2_stub* stub = h2_stubs_get(stubs, srcfp);
   if (!stub) {
      stub = new h2_stub(srcfp, srcfn, filine);
      stubs.push(stub->x);
   }
   stub->stub(dstfp);
   return true;
}

h2_inline void h2_stubs::clear(h2_list& stubs, void* srcfp)
{
   h2_stub* stub = h2_stubs_get(stubs, srcfp);
   if (stub) {
      stub->x.out();
      delete stub;
   }
}

h2_inline void h2_stubs::clear(h2_list& stubs)
{
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      p->x.out();
      delete p;
   }
}
