
static inline void h2_stub_g(void* befp, void* tofp, const char* befn, const char* tofn, const char* file, int line);

struct h2_stdio {
   static int sys_write(FILE* stream, const void* buf, size_t nbyte) {
      return write(fileno(stream), buf, nbyte);
   }

   char buffer[1024 * 1024];
   long offset;
   static int STUB_vprintf(const char* format, va_list ap) {
      int ret = vsnprintf(I().buffer + I().offset, sizeof(buffer) - I().offset, format, ap);
      I().offset += ret;
      return ret;
   }

   static int STUB_printf(const char* format, ...) {
      va_list a;
      va_start(a, format);
      int ret = STUB_vprintf(format, a);
      va_end(a);
      return ret;
   }

   static int STUB_putchar(int c) {
      I().buffer[I().offset++] = c;
      I().buffer[I().offset] = '\0';
      return c;
   }

   static int STUB_puts(const char* s) {
      int len = strlen(s);
      strcpy(I().buffer + I().offset, s);
      I().offset += len;
      return 1;
   }

   static int STUB_fprintf(FILE* stream, const char* format, ...) {
      if (stream != stdout && stream != stderr) {
         va_list a, b;
         va_start(a, format);
         int len = vsnprintf(nullptr, 0, format, a);
         va_end(a);
         char* t = (char*)alloca(len + 1);
         va_start(b, format);
         len = vsnprintf(t, len + 1, format, b);
         va_end(b);
         return sys_write(stream, t, len);
      }

      va_list a;
      va_start(a, format);
      int ret = STUB_vprintf(format, a);
      va_end(a);
      return ret;
   }

   static int STUB_vfprintf(FILE* stream, const char* format, va_list ap) {
      if (stream != stdout && stream != stderr) {
         va_list a, b;
         va_copy(a, ap);
         va_copy(b, ap);

         int len = vsnprintf(nullptr, 0, format, a);
         char* t = (char*)alloca(len + 1);
         len = vsnprintf(t, len + 1, format, b);
         return sys_write(stream, t, len);
      }

      return STUB_vprintf(format, ap);
   }

   static int STUB_fputc(int c, FILE* stream) {
      if (stream != stdout && stream != stderr) {
         unsigned char t = c;
         return 1 == sys_write(stream, &t, 1) ? c : EOF;
      }

      return STUB_putchar(c);
   }

   static int STUB_fputs(const char* s, FILE* stream) {
      if (stream != stdout && stream != stderr) {
         return sys_write(stream, s, strlen(s));
      }

      return STUB_puts(s);
   }

   static size_t STUB_fwrite(const void* ptr, size_t size, size_t nitems, FILE* stream) {
      if (stream != stdout && stream != stderr) {
         return sys_write(stream, ptr, size * nitems);
      }

      memcpy(I().buffer + I().offset, ptr, size * nitems);
      I().offset += size * nitems;
      I().buffer[I().offset] = '\0';
      return size * nitems;
   }

   /* clang-format off */
   static h2_stdio& I() { static h2_stdio __; return __; }
   /* clang-format on */

   static const char* capture_cout() {
      h2_stub_g((void*)::printf, (void*)STUB_printf, "", "", "", 0);
      h2_stub_g((void*)::putchar, (void*)STUB_putchar, "", "", "", 0);
      h2_stub_g((void*)::puts, (void*)STUB_puts, "", "", "", 0);
      h2_stub_g((void*)::vprintf, (void*)STUB_vprintf, "", "", "", 0);

      h2_stub_g((void*)::fprintf, (void*)STUB_fprintf, "", "", "", 0);
#ifdef __APPLE__
      h2_stub_g((void*)::vfprintf, (void*)STUB_vfprintf, "", "", "", 0);
#endif
      h2_stub_g((void*)::fputc, (void*)STUB_fputc, "", "", "", 0);
      h2_stub_g((void*)::putc, (void*)STUB_fputc, "", "", "", 0);
      h2_stub_g((void*)::fputs, (void*)STUB_fputs, "", "", "", 0);
      h2_stub_g((void*)::fwrite, (void*)STUB_fwrite, "", "", "", 0);

      I().offset = 0;
      I().buffer[0] = '\0';
      return I().buffer;
   }
};
