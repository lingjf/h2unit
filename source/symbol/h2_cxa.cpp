
h2_inline char* h2_cxa::demangle(const char* mangle_name, char* demangle_name, size_t length)
{
#if defined _MSC_VER
   return (char*)mangle_name;
#else
   int status = -1;
   if (strlen(mangle_name)) abi::__cxa_demangle(mangle_name, demangle_name, &length, &status);
   return status == 0 ? demangle_name : (char*)mangle_name;
#endif
}

#if defined __i386__ || defined __x86_64__ || defined _M_IX86 || defined _M_X64

static inline unsigned char* follow_JMP32REL(unsigned char* target)
{
   int relative_offset;
   memcpy(reinterpret_cast<void*>(&relative_offset), reinterpret_cast<void*>(target + 1), 4);
   return target + 5 + relative_offset;
}
static inline unsigned char* follow_JMP8REL(unsigned char* target)
{
   signed char relative_offset;
   memcpy(reinterpret_cast<void*>(&relative_offset), reinterpret_cast<void*>(target + 1), 1);
   return target + 2 + relative_offset;
}
static inline unsigned char* follow_JMP32ABS(unsigned char* target)
{
   void** new_target_p;
   if (sizeof(void*) == 8) {  // In 64-bit mode JMPs are RIP-relative, not absolute
      int target_offset;
      memcpy(reinterpret_cast<void*>(&target_offset), reinterpret_cast<void*>(target + 2), 4);
      new_target_p = reinterpret_cast<void**>(target + target_offset + 6);
   } else {
      memcpy(&new_target_p, reinterpret_cast<void*>(target + 2), 4);
   }
   return reinterpret_cast<unsigned char*>(*new_target_p);
}

h2_inline void* h2_cxa::follow_jmp(void* fp, int n)
{
   unsigned char* p = (unsigned char*)fp;
   while (n--) {
      if (p[0] == 0xE9) {  // ASM_JMP32REL
         p = follow_JMP32REL(p);
      } else if (p[0] == 0xEB) {  //ASM_JMP8REL, Visual Studio 7.1 implements new[] as an 8 bit jump to new
         p = follow_JMP8REL(p);
      } else if (p[0] == 0xFF && p[1] == 0x25) {  // ASM_JMP32ABS_0 ASM_JMP32ABS_1
         p = follow_JMP32ABS(p);
      } else if (sizeof(void*) == 8 && p[0] == 0x48 && p[1] == 0xFF && p[2] == 0x25) {  // in Visual Studio 2012 we're seeing jump like that: rex.W jmpq *0x11d019(%rip)
         p = follow_JMP32ABS(p + 1);
      } else {
         break;
      }
   }
   return (void*)p;
}

#elif defined __arm64__ || defined __aarch64__

static inline long long sign_extend(unsigned long long value, unsigned int bits)
{
   const unsigned int left = 64 - bits;
   const long long m1 = -1;
   const long long wide = (long long)(value << left);
   const long long sign = (wide < 0) ? (m1 << left) : 0;
   return value | sign;
}

static inline unsigned long fetch_opcode(void* fp, int i = 0)
{
   return *(unsigned long*)(((unsigned char*)fp) + i * 4);
}

h2_inline void* h2_cxa::follow_jmp(void* fp, int n)
{
   while (n--) {
      // (gdb) disassemble /r printf
      // Dump of assembler code for function printf@plt:
      //    0x0000000000c1f8a0 <+0>:     90 66 00 b0     adrp    x16, 0x18f0000 <div@got.plt>
      //    0x0000000000c1f8a4 <+4>:     11 ee 41 f9     ldr     x17, [x16, #984]
      //    0x0000000000c1f8a8 <+8>:     10 62 0f 91     add     x16, x16, #0x3d8
      //    0x0000000000c1f8ac <+12>:    20 02 1f d6     br      x17
      if ((fetch_opcode(fp, 0) & 0x9f00001f) == (0x90000000 | 16)) {                                                     // C6.2.10 ADRP
         if ((fetch_opcode(fp, 1) & 0xffe003ff) == (0xf9400000 | 16 << 5 | 17)) {                                        // C6.2.101 LDR (immediate)
            if ((fetch_opcode(fp, 2) & 0xfffffc1f) == 0xd61f0000 || (fetch_opcode(fp, 3) & 0xfffffc1f) == 0xd61f0000) {  // BR X17 0xd61f0000 | 17 << 5
               unsigned long long low2 = (fetch_opcode(fp, 0) >> 29) & 3;
               unsigned long long high19 = (fetch_opcode(fp, 0) >> 5) & ~(~0ULL << 19);
               long long page = sign_extend((high19 << 2) | low2, 21) << 12;
               unsigned long long offset = ((fetch_opcode(fp, 1) >> 10) & ~(~0ULL << 12)) << 3;
               unsigned char* target_p = (unsigned char*)((unsigned long long)fp & 0xfffffffffffff000ULL) + page + offset;
               fp = (void*)*(unsigned char**)target_p;
            }
         }
      } else if ((fetch_opcode(fp, 0) & 0xf8000000) == 0x58000000) {  // LDR X17,[PC+n]
         if ((fetch_opcode(fp, 1) & 0xfffffc1f) == 0xd61f0000) {      // BR X17
            unsigned long n = (fetch_opcode(fp, 0) & 0x07ffffe0) >> 3;
            fp = (void*)*(unsigned long long*)(((unsigned long long)fp) + n);
         }
      }
   }
   return fp;
}

#endif
