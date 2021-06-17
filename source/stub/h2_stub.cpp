
struct h2_stub : h2_libc {
   h2_list x;
   unsigned char saved[32];
   void *origin_fp, *substitute_fp;

   h2_stub(void* _origin_fp) : origin_fp(_origin_fp)
   {
      h2_natives::I() += origin_fp;
      h2_e9::save(origin_fp, saved);
   }
   ~h2_stub()
   {
      h2_e9::reset(origin_fp, saved);
      h2_natives::I() -= origin_fp;
   }
   void stub(void* _substitute_fp)
   {
      substitute_fp = _substitute_fp;
      h2_e9::set(origin_fp, substitute_fp);
   }
};

h2_inline bool h2_stubs::add(void* origin_fp, void* substitute_fp, const char* origin_fn, const char* file, int line)
{
   h2_stub* stub = nullptr;
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      if (p->origin_fp == origin_fp) {
         stub = p;
         break;
      }
   }
   if (!stub) {
      if (!h2_e9::save(origin_fp, nullptr)) {
         h2_color::prints("yellow", "STUB failed: %s %s:%d\n", origin_fn, file, line);
         return false;
      }
      stub = new h2_stub(origin_fp);
      stubs.push(stub->x);
   }
   stub->stub(substitute_fp);
   return true;
}

h2_inline void h2_stubs::clear(void* origin_fp)
{
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      if (p->origin_fp == origin_fp) {
         p->x.out();
         delete p;
      }
   }
}

h2_inline void h2_stubs::clear()
{
   h2_list_for_each_entry (p, stubs, h2_stub, x) {
      p->x.out();
      delete p;
   }
}

h2_inline h2_stub_temporary_restore::h2_stub_temporary_restore(void* _origin_fp) : origin_fp(_origin_fp)
{
   h2_native* native = h2_natives::I().get(origin_fp);
   if (native) {
      h2_e9::save(origin_fp, saved);
      native->restore();
   }
}

h2_inline h2_stub_temporary_restore::~h2_stub_temporary_restore()
{
   h2_native* native = h2_natives::I().get(origin_fp);
   if (native) {
      h2_e9::reset(origin_fp, saved);
   }
}
