
struct h2__stdio {
   h2_singleton(h2__stdio);
   h2_string buffer;
   bool stdout_capturable, stderr_capturable;

   static ssize_t write(int fd, const void* buf, size_t count) {
      if (!((I().stdout_capturable && fd == fileno(stdout)) || (I().stderr_capturable && fd == fileno(stderr))))
         return h2_libc::write(fd, buf, count);
      I().buffer.append((char*)buf, count);
      return count;
   }

   static int vfprintf(FILE* stream, const char* format, va_list ap) {
      va_list bp;
      va_copy(bp, ap);
      int len = vsnprintf(nullptr, 0, format, bp);
      char* tmp = (char*)alloca(len + 1);
      len = vsnprintf(tmp, len + 1, format, ap);
      return write(fileno(stream), tmp, len);
   }

   static int fprintf(FILE* stream, const char* format, ...) {
      va_list a;
      va_start(a, format);
      int ret = vfprintf(stream, format, a);
      va_end(a);
      return ret;
   }

   static int fputc(int c, FILE* stream) {
      unsigned char t = c;
      int ret = write(fileno(stream), &t, 1);
      return ret == 1 ? c : EOF;
   }

   static int fputs(const char* s, FILE* stream) {
      return write(fileno(stream), s, strlen(s));
   }

   static size_t fwrite(const void* ptr, size_t size, size_t nitems, FILE* stream) {
      return write(fileno(stream), ptr, size * nitems);
   }

   static int printf(const char* format, ...) {
      va_list a;
      va_start(a, format);
      int ret = vfprintf(stdout, format, a);
      va_end(a);
      return ret;
   }

   static int vprintf(const char* format, va_list ap) {
      return vfprintf(stdout, format, ap);
   }

   static int putchar(int c) {
      unsigned char t = c;
      write(fileno(stdout), &t, 1);
      return c;
   }

   static int puts(const char* s) {
      write(fileno(stdout), s, strlen(s));
      write(fileno(stdout), "\n", 1);
      return 1;
   }

   static const char* start_capture(bool out, bool err) {
      h2_stub_g((void*)::write, (void*)write, "", "", "", 0);
#if defined __APPLE__
      h2_stub_g((void*)::printf, (void*)printf, "", "", "", 0);
      h2_stub_g((void*)::vprintf, (void*)vprintf, "", "", "", 0);
      h2_stub_g((void*)::putchar, (void*)putchar, "", "", "", 0);
      h2_stub_g((void*)::puts, (void*)puts, "", "", "", 0);
      h2_stub_g((void*)::fprintf, (void*)fprintf, "", "", "", 0);
      h2_stub_g((void*)::vfprintf, (void*)vfprintf, "", "", "", 0);
      h2_stub_g((void*)::fputc, (void*)fputc, "", "", "", 0);
      h2_stub_g((void*)::putc, (void*)fputc, "", "", "", 0);
      h2_stub_g((void*)::fputs, (void*)fputs, "", "", "", 0);
      h2_stub_g((void*)::fwrite, (void*)fwrite, "", "", "", 0);
#endif

      I().stdout_capturable = out;
      I().stderr_capturable = err;
      I().buffer.clear();
      return I().buffer.c_str();
   }

   static const char* stop_capture() {
      I().stdout_capturable = false;
      I().stderr_capturable = false;

      I().buffer.push_back('\0');
      return I().buffer.c_str();
   }
};

h2_inline void h2_stdio::init() {
   setbuf(stdout, 0);  // unbuffered. stderr is unbuffered default, stdout is line-buffered default
}
h2_inline const char* h2_stdio::capture_cout(char* type) {
   if (!type) return h2__stdio::I().stop_capture();
   return h2__stdio::I().start_capture(!strlen(type) || strcasestr(type, "out"), !strlen(type) || strcasestr(type, "err"));
}
