
struct h2_stdio {
   h2_singleton(h2_stdio);

   char buffer[1024 * 1024], *p;
   int offset, size;

   // write, pwrite, writev
   static ssize_t write(int fd, const void* buf, size_t count) {
      if (I().p && I().offset + count < I().size) {
         memcpy(I().p + I().offset, buf, count);
         I().offset += count;
         I().p[I().offset] = '\0';
      }
      return count;
   }

   static int vprintf(const char* format, va_list ap) {
      int ret = 0;
      if (I().p) {
         ret = vsnprintf(I().p + I().offset, I().size - I().offset, format, ap);
         I().offset += ret;
      }
      return ret;
   }

   static int printf(const char* format, ...) {
      va_list a;
      va_start(a, format);
      int ret = vprintf(format, a);
      va_end(a);
      return ret;
   }

   static int putchar(int c) {
      if (I().p && I().offset + 1 < I().size) {
         I().p[I().offset++] = c;
         I().p[I().offset] = '\0';
      }
      return c;
   }

   static int puts(const char* s) {
      int len = strlen(s);
      if (I().p && I().offset + len < I().size) {
         strcpy(I().p + I().offset, s);
         I().offset += len;
      }
      return 1;
   }

   static int fprintf(FILE* stream, const char* format, ...) {
      if (stream != stdout && stream != stderr) {
         va_list a, b;
         va_start(a, format);
         int len = vsnprintf(nullptr, 0, format, a);
         va_end(a);
         char* t = (char*)alloca(len + 1);
         va_start(b, format);
         len = vsnprintf(t, len + 1, format, b);
         va_end(b);
         return h2_libc::write(stream, t, len);
      }

      va_list a;
      va_start(a, format);
      int ret = vprintf(format, a);
      va_end(a);
      return ret;
   }

   static int vfprintf(FILE* stream, const char* format, va_list ap) {
      if (stream != stdout && stream != stderr) {
         va_list a, b;
         va_copy(a, ap);
         va_copy(b, ap);

         int len = vsnprintf(nullptr, 0, format, a);
         char* t = (char*)alloca(len + 1);
         len = vsnprintf(t, len + 1, format, b);
         return h2_libc::write(stream, t, len);
      }

      return vprintf(format, ap);
   }

   static int fputc(int c, FILE* stream) {
      if (stream != stdout && stream != stderr) {
         unsigned char t = c;
         return 1 == h2_libc::write(stream, &t, 1) ? c : EOF;
      }

      return putchar(c);
   }

   static int fputs(const char* s, FILE* stream) {
      if (stream != stdout && stream != stderr) {
         return h2_libc::write(stream, s, strlen(s));
      }

      return puts(s);
   }

   static size_t fwrite(const void* ptr, size_t size, size_t nitems, FILE* stream) {
      if (stream != stdout && stream != stderr) {
         return h2_libc::write(stream, ptr, size * nitems);
      }

      size_t len = size * nitems;
      if (I().p && I().offset + len < I().size) {
         memcpy(I().p + I().offset, ptr, len);
         I().offset += len;
         I().p[I().offset] = '\0';
      }
      return len;
   }

   static const void start_capture(char* buffer, int size) {
      h2_stub_g((void*)::printf, (void*)printf, "", "", "", 0);
      h2_stub_g((void*)::putchar, (void*)putchar, "", "", "", 0);
      h2_stub_g((void*)::puts, (void*)puts, "", "", "", 0);
      h2_stub_g((void*)::vprintf, (void*)vprintf, "", "", "", 0);

      h2_stub_g((void*)::fprintf, (void*)fprintf, "", "", "", 0);
      h2_stub_g((void*)::fputc, (void*)fputc, "", "", "", 0);
      h2_stub_g((void*)::putc, (void*)fputc, "", "", "", 0);
      h2_stub_g((void*)::fputs, (void*)fputs, "", "", "", 0);
      h2_stub_g((void*)::fwrite, (void*)fwrite, "", "", "", 0);

#if defined __APPLE__
      h2_stub_g((void*)::vfprintf, (void*)vfprintf, "", "", "", 0);
#endif

      if (buffer) {
         I().size = size;
         I().p = buffer;
      } else {
         I().size = sizeof(I().buffer);
         I().p = I().buffer;
      }

      I().offset = 0;
      I().p[0] = '\0';
   }

   static const char* stop_capture() {
      fflush(stdout);
      fflush(stderr);
      std::cout << std::flush;

      char* buffer = I().p;
      I().offset = 0;
      I().size = 0;
      I().p = nullptr;

      h2_stub_g((void*)::printf, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::putchar, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::puts, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::vprintf, (void*)0, "", "", "", 0);

      h2_stub_g((void*)::fprintf, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::fputc, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::putc, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::fputs, (void*)0, "", "", "", 0);
      h2_stub_g((void*)::fwrite, (void*)0, "", "", "", 0);

#if defined __APPLE__
      h2_stub_g((void*)::vfprintf, (void*)0, "", "", "", 0);
#endif

      return buffer;
   }
};

h2_inline void h2_stdio_exporter::capture_cout(char* buffer, int size) {
   h2_stdio::I().start_capture(buffer, size);
}

h2_inline const char* h2_stdio_exporter::capture_cout() {
   return h2_stdio::I().stop_capture();
}
