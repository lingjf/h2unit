
struct h2_native : h2_libc {
   h2_list x;
   unsigned char saved[32];
   void* origin_fp;
   int reference_count = 0;
   h2_native(void* _origin_fp) : origin_fp(_origin_fp)
   {
      h2_e9::save(origin_fp, saved);
   }
   ~h2_native()
   {
      restore();
   }
   void restore()
   {
      h2_e9::reset(origin_fp, saved);
   }
};

struct h2_natives {
   h2_singleton(h2_natives);
   h2_list natives;

   h2_native* get(void* origin_fp)
   {
      h2_list_for_each_entry (p, natives, h2_native, x)
         if (p->origin_fp == origin_fp)
            return p;
      return nullptr;
   }

   void operator+=(void* origin_fp)
   {
      h2_native* native = get(origin_fp);
      if (!native) {
         native = new h2_native(origin_fp);
         natives.push(native->x);
      }
      native->reference_count++;
   }
   void operator-=(void* origin_fp)
   {
      h2_native* native = get(origin_fp);
      if (native) {
         if (--native->reference_count == 0) {
            native->x.out();
            delete native;
         }
      }
   }
};
