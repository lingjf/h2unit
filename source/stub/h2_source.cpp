
struct h2_source : h2_libc {
   h2_list x;
   unsigned char saved_opcode[32];
   void* source_fp;
   int reference_count = 0;
   h2_source(void* _source_fp, const char* fn, const char* file, int line) : source_fp(_source_fp)
   {
      if (!h2_e9_save(source_fp, saved_opcode)) {
         h2_color::prints("yellow", "STUB %s by %s() failed %s:%d\n", fn, O.os == windows ? "VirtualProtect" : "mprotect", file, line);
         if (O.os == macOS) ::printf("try: "), h2_color::prints("green", "printf '\\x07' | dd of=%s bs=1 seek=160 count=1 conv=notrunc\n", O.path);
         if (O.os == Linux) ::printf("try: "), h2_color::prints("green", "objcopy --writable-text %s\n", O.path);
      }
   }
   ~h2_source() { h2_e9_reset(source_fp, saved_opcode); }
   void set(void* dstfp) { h2_e9_set(source_fp, dstfp); }
   void save(unsigned char opcode[32]) { h2_e9_save(source_fp, opcode); }
   void reset(unsigned char opcode[32]) { h2_e9_reset(source_fp, opcode); }
   void reset() { h2_e9_reset(source_fp, saved_opcode); }
};

struct h2_sources {
   h2_singleton(h2_sources);
   h2_list sources;

   h2_source* get(void* source_fp)
   {
      h2_list_for_each_entry (p, sources, h2_source, x)
         if (p->source_fp == source_fp)
            return p;
      return nullptr;
   }

   h2_source* add(void* source_fp, const char* fn, const char* file, int line)
   {
      h2_source* source = get(source_fp);
      if (!source) {
         source = new h2_source(source_fp, fn, file, line);
         sources.push(source->x);
      }
      source->reference_count++;
      return source;
   }

   void del(h2_source* source)
   {
      if (source && --source->reference_count == 0) {
         source->x.out();
         delete source;
      }
   }
};
