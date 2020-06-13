
struct h2__stdio {
   h2_singleton(h2__stdio);
   h2_stubs stubs;
   h2_string* buffer;
   bool stdout_capturable = false, stderr_capturable = false, syslog_capturable = false;
   size_t capture_length = 0;

   static ssize_t write(int fd, const void* buf, size_t count)
   {
      h2_libc::write(fd, buf, count);
      if (fd == fileno(stdout) || fd == fileno(stderr))
         I().capture_length += count;
      if ((I().stdout_capturable && fd == fileno(stdout)) || (I().stderr_capturable && fd == fileno(stderr)))
         I().buffer->append((char*)buf, count);
      return count;
   }

   static int vfprintf(FILE* stream, const char* format, va_list ap)
   {
      char* alloca_str;
      h2_sprintvf(alloca_str, format, ap);
      return write(fileno(stream), alloca_str, strlen(alloca_str));
   }

   static int fprintf(FILE* stream, const char* format, ...)
   {
      va_list a;
      va_start(a, format);
      int ret = vfprintf(stream, format, a);
      va_end(a);
      return ret;
   }

   static int fputc(int c, FILE* stream)
   {
      unsigned char t = c;
      int ret = write(fileno(stream), &t, 1);
      return ret == 1 ? c : EOF;
   }

   static int fputs(const char* s, FILE* stream)
   {
      return write(fileno(stream), s, strlen(s));
   }

   static size_t fwrite(const void* ptr, size_t size, size_t nitems, FILE* stream)
   {
      return write(fileno(stream), ptr, size * nitems);
   }

   static int printf(const char* format, ...)
   {
      va_list a;
      va_start(a, format);
      int ret = vfprintf(stdout, format, a);
      va_end(a);
      return ret;
   }

   static int vprintf(const char* format, va_list ap)
   {
      return vfprintf(stdout, format, ap);
   }

   static int putchar(int c)
   {
      unsigned char t = c;
      write(fileno(stdout), &t, 1);
      return c;
   }

   static int puts(const char* s)
   {
      write(fileno(stdout), s, strlen(s));
      write(fileno(stdout), "\n", 1);
      return 1;
   }

   static void vsyslog(int priority, const char* format, va_list ap)
   {
      if (!I().syslog_capturable) return;
      char* alloca_str;
      h2_sprintvf(alloca_str, format, ap);
      I().buffer->append(alloca_str, strlen(alloca_str));
   }

   static void syslog(int priority, const char* format, ...)
   {
      va_list a;
      va_start(a, format);
      vsyslog(priority, format, a);
      va_end(a);
   }

   h2__stdio()
   {
#ifndef _WIN32
      stubs.add((void*)::write, (void*)write);
#endif
#if defined __APPLE__ || defined _WIN32
      stubs.add((void*)::printf, (void*)printf);
      stubs.add((void*)::vprintf, (void*)vprintf);
      stubs.add((void*)::putchar, (void*)putchar);
      stubs.add((void*)::puts, (void*)puts);
      stubs.add((void*)::fprintf, (void*)fprintf);
      stubs.add((void*)::vfprintf, (void*)vfprintf);
      stubs.add((void*)::fputc, (void*)fputc);
      stubs.add((void*)::putc, (void*)fputc);
      stubs.add((void*)::fputs, (void*)fputs);
      stubs.add((void*)::fwrite, (void*)fwrite);
#endif
#ifndef _WIN32
      stubs.add((void*)::syslog, (void*)syslog);
      stubs.add((void*)::vsyslog, (void*)vsyslog);
#endif
   }

   const char* start_capture(bool _stdout, bool _stderr, bool _syslog)
   {
      capturing = true;
      stdout_capturable = _stdout;
      stderr_capturable = _stderr;
      syslog_capturable = _syslog;
      buffer->clear();
      return buffer->c_str();
   }

   const char* stop_capture()
   {
      capturing = false;
      stdout_capturable = stderr_capturable = syslog_capturable = false;
      buffer->push_back('\0');
      return buffer->c_str();
   }

   bool capturing = false;
   const char* toggle_capture()
   {
      if (capturing)
         return stop_capture();
      else
         return start_capture(true, true, true);
   }
};

h2_inline void h2_stdio::initialize()
{
   ::setbuf(stdout, 0);  // unbuffered
   h2__stdio::I().buffer = new h2_string();
}

h2_inline size_t h2_stdio::capture_length()
{
   return h2__stdio::I().capture_length;
}

h2_inline const char* h2_stdio::capture_cout(const char* type)
{
   if (!strlen(type)) return h2__stdio::I().toggle_capture();
   if (strcasestr(type, "stop")) return h2__stdio::I().stop_capture();
   return h2__stdio::I().start_capture(strcasestr(type, "out"), strcasestr(type, "err"), strcasestr(type, "syslog"));
}

h2_inline void h2_stdio::capture_cancel()
{
   h2__stdio::I().stop_capture();
}
