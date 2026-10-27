// https://github.com/microsoft/Detours/blob/master/src/detours.cpp

#if defined __i386__ || defined _M_IX86
static constexpr int h2_e9_size = 1 + 4;
#elif defined __x86_64__ || defined _M_X64
static constexpr int h2_e9_size = 2 + 8 + 2;
#elif defined __arm64__ || defined __aarch64__
#if defined __APPLE__
// Keep a 4-byte patch so tiny functions do not overwrite adjacent code.
static constexpr int h2_e9_size = 4;
#else
static constexpr int h2_e9_size = 4 + 4 + 8;
#endif
#endif

#if !defined _WIN32
static inline bool h2_e9_set_permission(void* srcfp, int permission)
{
   unsigned long long page_size = (unsigned long long)h2_page_size();
   unsigned long long address = reinterpret_cast<unsigned long long>(srcfp);
   unsigned long long page_start = address & ~(page_size - 1);
   unsigned long long length = address + h2_e9_size - page_start;
   length = ((length + page_size - 1) / page_size) * page_size;
   return mprotect(reinterpret_cast<void*>(page_start), length, permission) == 0;
}
#endif

#if defined __APPLE__
static inline bool h2_e9_set_pointer_permission(void* pointer, int permission)
{
   unsigned long long page_size = (unsigned long long)h2_page_size();
   unsigned long long address = reinterpret_cast<unsigned long long>(pointer);
   unsigned long long page_start = address & ~(page_size - 1);
   return mprotect(reinterpret_cast<void*>(page_start), page_size, permission) == 0;
}
#endif

#if defined __APPLE__
static inline bool h2_e9_in_main_executable(void* srcfp)
{
   const mach_header* header = _dyld_get_image_header(0);
   if (!header || header->magic != MH_MAGIC_64) return false;

   const unsigned long long address = reinterpret_cast<unsigned long long>(srcfp);
   const unsigned long long slide = (unsigned long long)_dyld_get_image_vmaddr_slide(0);
   const char* commands = reinterpret_cast<const char*>(header) + sizeof(mach_header_64);
   const load_command* command = reinterpret_cast<const load_command*>(commands);

   for (uint32_t i = 0; i < header->ncmds; ++i) {
      if (command->cmd == LC_SEGMENT_64) {
         const segment_command_64* segment = reinterpret_cast<const segment_command_64*>(command);
         if (!strncmp(segment->segname, "__TEXT", sizeof(segment->segname))) {
            unsigned long long begin = segment->vmaddr + slide;
            unsigned long long end = begin + segment->vmsize;
            return begin <= address && address < end;
         }
      }
      command = reinterpret_cast<const load_command*>(reinterpret_cast<const char*>(command) + command->cmdsize);
   }
   return false;
}

static inline const void* h2_e9_file_address(const mach_header_64* header, uint32_t offset)
{
   const load_command* command = reinterpret_cast<const load_command*>(header + 1);
   for (uint32_t i = 0; i < header->ncmds; ++i) {
      if (command->cmd == LC_SEGMENT_64) {
         const segment_command_64* segment = reinterpret_cast<const segment_command_64*>(command);
         if (segment->fileoff <= offset && offset < segment->fileoff + segment->filesize) {
            const char* address = reinterpret_cast<const char*>(segment->vmaddr + _dyld_get_image_vmaddr_slide(0));
            return address + offset - segment->fileoff;
         }
      }
      command = reinterpret_cast<const load_command*>(reinterpret_cast<const char*>(command) + command->cmdsize);
   }
   return nullptr;
}

static inline void* h2_e9_import_pointer(const char* symbol)
{
   if (!symbol || !symbol[0]) return nullptr;

   const mach_header_64* header = reinterpret_cast<const mach_header_64*>(_dyld_get_image_header(0));
   if (!header || header->magic != MH_MAGIC_64) return nullptr;

   const symtab_command* symtab = nullptr;
   const dysymtab_command* dysymtab = nullptr;
   const section_64* pointers = nullptr;
   const load_command* command = reinterpret_cast<const load_command*>(header + 1);

   for (uint32_t i = 0; i < header->ncmds; ++i) {
      if (command->cmd == LC_SYMTAB) {
         symtab = reinterpret_cast<const symtab_command*>(command);
      } else if (command->cmd == LC_DYSYMTAB) {
         dysymtab = reinterpret_cast<const dysymtab_command*>(command);
      } else if (command->cmd == LC_SEGMENT_64) {
         const segment_command_64* segment = reinterpret_cast<const segment_command_64*>(command);
         const section_64* section = reinterpret_cast<const section_64*>(segment + 1);
         for (uint32_t j = 0; j < segment->nsects; ++j) {
            if (!strncmp(section[j].sectname, "__got", sizeof(section[j].sectname)) ||
                !strncmp(section[j].sectname, "__la_symbol_ptr", sizeof(section[j].sectname))) {
               pointers = &section[j];
               break;
            }
         }
      }
      command = reinterpret_cast<const load_command*>(reinterpret_cast<const char*>(command) + command->cmdsize);
   }

   if (!symtab || !dysymtab || !pointers || dysymtab->nindirectsyms == 0) return nullptr;

   char imported[1024];
   if (symbol[0] == '_') snprintf(imported, sizeof(imported), "%s", symbol);
   else snprintf(imported, sizeof(imported), "_%s", symbol);

   const char* strings = static_cast<const char*>(h2_e9_file_address(header, symtab->stroff));
   const nlist_64* symbols = static_cast<const nlist_64*>(h2_e9_file_address(header, symtab->symoff));
   const uint32_t* indirect = static_cast<const uint32_t*>(h2_e9_file_address(header, dysymtab->indirectsymoff));
   if (!strings || !symbols || !indirect) return nullptr;
   const uint32_t count = pointers->size / sizeof(void*);

   for (uint32_t i = 0; i < count; ++i) {
      if (pointers->reserved1 + i >= dysymtab->nindirectsyms) break;
      uint32_t symbol_index = indirect[pointers->reserved1 + i];
      if (symbol_index & (INDIRECT_SYMBOL_LOCAL | INDIRECT_SYMBOL_ABS) || symbol_index >= symtab->nsyms) continue;
      const char* name = strings + symbols[symbol_index].n_un.n_strx;
      if (!strcmp(imported, name)) {
         void* address = reinterpret_cast<void*>(pointers->addr + _dyld_get_image_vmaddr_slide(0) + i * sizeof(void*));
         return address;
      }
   }
   return nullptr;
}

static inline bool h2_e9_got_symbol(const char* symbol)
{
   static const char* const symbols[] = {
      "getaddrinfo",
      "freeaddrinfo",
      "gethostbyname",
      "sendto",
      "recvfrom",
      "sendmsg",
      "recvmsg",
      "send",
      "recv",
      "accept",
      "connect",
   };
   for (const char* name : symbols)
      if (!strcmp(name, symbol)) return true;
   return false;
}

static inline void* h2_e9_source_address(void* source_fp, const char* source_name)
{
   if (h2_e9_in_main_executable(source_fp)) return source_fp;
#if defined __x86_64__ || defined __arm64__ || defined __aarch64__
   void* pointer = nullptr;
   if (h2_e9_got_symbol(source_name)) pointer = h2_e9_import_pointer(source_name);
   if (pointer) return pointer;
#endif
   return source_fp;
}
#endif

static inline bool h2_e9_save(void* srcfp, unsigned char* opcode)
{
#if defined _WIN32
   DWORD t;
   if (!VirtualProtect(srcfp, sizeof(void*) + 4, PAGE_EXECUTE_READWRITE, &t)) return false;  // PAGE_EXECUTE_WRITECOPY OR PAGE_WRITECOPY
#elif defined __APPLE__
   // Apple Silicon enforces W^X. Write through an RW mapping, then restore RX.
   if (!h2_e9_set_permission(srcfp, PROT_READ | PROT_WRITE)) return false;
#else
   if (!h2_e9_set_permission(srcfp, PROT_READ | PROT_WRITE | PROT_EXEC)) return false;
#endif
   if (opcode) memcpy(opcode, srcfp, h2_e9_size);
#if defined __APPLE__
   if (!h2_e9_set_permission(srcfp, PROT_READ | PROT_EXEC)) return false;
#endif
   return true;
}

#if defined __APPLE__
static inline bool h2_e9_save_pointer(void* slot, unsigned char* pointer)
{
   if (!h2_e9_set_pointer_permission(slot, PROT_READ | PROT_WRITE)) return false;
   memcpy(pointer, slot, sizeof(void*));
   return h2_e9_set_pointer_permission(slot, PROT_READ);
}

static inline bool h2_e9_set_pointer(void* slot, void* pointer)
{
   if (!h2_e9_set_pointer_permission(slot, PROT_READ | PROT_WRITE)) return false;
   memcpy(slot, &pointer, sizeof(void*));
   return h2_e9_set_pointer_permission(slot, PROT_READ);
}

static inline bool h2_e9_reset_pointer(void* slot, const unsigned char* pointer)
{
   if (!h2_e9_set_pointer_permission(slot, PROT_READ | PROT_WRITE)) return false;
   memcpy(slot, pointer, sizeof(void*));
   return h2_e9_set_pointer_permission(slot, PROT_READ);
}
#endif

static inline bool h2_e9_set(void* srcfp, void* dstfp)
{
   unsigned char* I = static_cast<unsigned char*>(srcfp);

#if defined __APPLE__
   if (!h2_e9_set_permission(srcfp, PROT_READ | PROT_WRITE)) return false;
#endif

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64
   long long delta = (unsigned char*)dstfp - I - 5;
   if (delta < -0x7fffffff || 0x7fffffff < delta) {  //x86_64 asm("movq $dstfp, %rax; jmpq %rax")
      unsigned char C[] = {0x48, 0xB8, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xE0};
      memcpy(C + 2, &dstfp, sizeof(void*));
      memcpy(I, C, sizeof(C));
   } else {  //i386 asm("jmp offset")
      unsigned char C[] = {0xE9, 0, 0, 0, 0};
      *(long*)(&C[1]) = (long)delta;
      memcpy(I, C, sizeof(C));
   }
   // ::FlushInstructionCache(GetCurrentProcess(), srcfp, h2_e9_size);

#elif defined __arm64__ || defined __aarch64__

#if defined __APPLE__
   long long offset = (unsigned char*)dstfp - I;
   if ((offset & 3) || offset < -134217728 || offset >= 134217728) {
      h2_e9_set_permission(srcfp, PROT_READ | PROT_EXEC);
      return false;
   }
   uint32_t branch = 0x14000000u | ((uint32_t)(offset / 4) & 0x03ffffffu);
   memcpy(I, &branch, sizeof(branch));
#else
#   pragma pack(push, 1)
   struct ldr_br_dst {
      unsigned int ldr;
      unsigned int br;
      void* dstfp;
   };
   struct b_offset {
      unsigned int b : 8;
      unsigned int offset : 24;
   };
#   pragma pack(pop)
   long long offset = (unsigned char*)dstfp - I;

   if (1 || offset < -8388607 - 1 || 0x7fffff < offset) {  // signed 2^24
      struct ldr_br_dst* p = static_cast<ldr_br_dst*>(srcfp);
      p->ldr = 0x58000000 | ((8 / 4) << 5) | 17;  // x17 register store dstfp
      p->br = 0xD61F0000 | (17 << 5);             // jmp x17
      p->dstfp = dstfp;
   } else {
      struct b_offset* p = static_cast<b_offset*>(srcfp);
      p->b = offset >= 0 ? 0x14 : 0x17;  //b 14 forward 17 backward
      p->offset = (unsigned long)((offset / 4) & 0xffffff);
   }
#endif

   __builtin___clear_cache((char*)I, (char*)I + h2_e9_size);
#endif

#if defined __APPLE__
   if (!h2_e9_set_permission(srcfp, PROT_READ | PROT_EXEC)) return false;
#endif
   return true;
}

static inline bool h2_e9_reset(void* srcfp, unsigned char* opcode)
{
#if defined __APPLE__
   if (!h2_e9_set_permission(srcfp, PROT_READ | PROT_WRITE)) return false;
#endif
   memcpy(srcfp, opcode, h2_e9_size);
   // ::FlushInstructionCache(GetCurrentProcess(), srcfp, h2_e9_size);
#if defined __arm64__ || defined __aarch64__
   __builtin___clear_cache(static_cast<char*>(srcfp), static_cast<char*>(srcfp) + h2_e9_size);
#endif
#if defined __APPLE__
   if (!h2_e9_set_permission(srcfp, PROT_READ | PROT_EXEC)) return false;
#endif
   return true;
}
