struct h2_source : h2_libc {
   h2_list x;
   unsigned char origin_opcode[32];
   void* source_fp;
   bool patchable = true;
   bool patch_pointer = false;
   int reference_count = 0;
   h2_source(void* source_fp_, const char* srcfn, const char* filine)
      :
#if defined __APPLE__
        source_fp(h2_e9_source_address(source_fp_, srcfn))
#else
        source_fp(source_fp_)
#endif
   {
#if defined __APPLE__
      // dyld shared-cache code pages are immutable. Imported DNS functions are
      // redirected through the main executable's GOT; other unsupported probes
      // are skipped instead of calling mprotect on the shared cache.
      patchable = h2_e9_in_main_executable(source_fp_) || source_fp != source_fp_;
      patch_pointer = source_fp != source_fp_;
#endif
      if (patchable) {
         bool saved = true;
#if defined __APPLE__
         if (patch_pointer) saved = h2_e9_save_pointer(source_fp, origin_opcode);
         else
#endif
            saved = h2_e9_save(source_fp, origin_opcode);
         if (!saved) {
            patchable = false;
            h2_console::prints("yellow", "STUB %s by %s() failed %s\n", srcfn, O.os == OsWindows ? "VirtualProtect" : "mprotect", filine);
            if (O.os == OsMacOS) h2_console::prints("", "try: "), h2_console::prints("green", "../h2fix.sh %s\n", O.path);
            if (O.os == OsLinux) h2_console::prints("", "try: "), h2_console::prints("green", "objcopy --writable-text %s\n", O.path);
         }
      }
   }
   ~h2_source()
   {
      if (!patchable) return;
#if defined __APPLE__
      if (patch_pointer) h2_e9_reset_pointer(source_fp, origin_opcode);
      else
#endif
         h2_e9_reset(source_fp, origin_opcode);
   }
   bool set(void* dstfp)
   {
      if (!patchable) return false;
#if defined __APPLE__
      if (patch_pointer) return h2_e9_set_pointer(source_fp, dstfp);
#endif
      return h2_e9_set(source_fp, dstfp);
   }
   bool save(unsigned char opcode[32])
   {
      if (!patchable) return false;
#if defined __APPLE__
      if (patch_pointer) return h2_e9_save_pointer(source_fp, opcode);
#endif
      return h2_e9_save(source_fp, opcode);
   }
   bool reset(unsigned char opcode[32])
   {
      if (!patchable) return false;
#if defined __APPLE__
      if (patch_pointer) return h2_e9_reset_pointer(source_fp, opcode);
#endif
      return h2_e9_reset(source_fp, opcode);
   }
   bool reset()
   {
      if (!patchable) return false;
#if defined __APPLE__
      if (patch_pointer) return h2_e9_reset_pointer(source_fp, origin_opcode);
#endif
      return h2_e9_reset(source_fp, origin_opcode);
   }
};

struct h2_sources {
   h2_singleton(h2_sources);
   h2_list sources;

   h2_source* __find(void* fp)
   {
      h2_list_for_each_entry (p, sources, h2_source, x)
         if (p->source_fp == fp)
            return p;
      return nullptr;
   }

   void* __follow(void* fp)
   {
#if defined __arm64__ || defined __aarch64__
#else
      for (int i = 0; i < 1; ++i) {  // follow PLT(Linux) or ILT (Incremental Link Table /Windows)
         if (__find(fp)) break;
         void* next = h2_cxa::follow_jmp(fp, 1);
         if (next == fp) break;
         fp = next;
      }
#endif
      return fp;
   }

   h2_source* get(void* fp) { return __find(__follow(fp)); }

   h2_source* add(void* fp, const char* srcfn, const char* filine)
   {
      void* source_fp = __follow(fp);
      h2_source* source = __find(source_fp);
      if (!source) {
         source = new h2_source(source_fp, srcfn, filine);
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
