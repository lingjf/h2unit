struct h2_stdio {
   h2_singleton(h2_stdio);
   h2_string* buffer;
   bool stdout_capturable = false, stderr_capturable = false, syslog_capturable = false;
   long long capture_length = 0;

   static ssize_t write(int fd, const void* buf, size_t count)
   {
      h2::h2_stub_temporary_restore t((void*)LIBC__write);
      if ((fd == fileno(stdout) || fd == fileno(stderr)) && h2_report::I().escape_length == I().capture_length && !h2_report::I().in)
         LIBC__write(fd, "\n", 1); // fall printf/cout into new line from report title
      LIBC__write(fd, buf, count);
      if (fd == fileno(stdout) || fd == fileno(stderr))
         I().capture_length += count;
      if ((I().stdout_capturable && fd == fileno(stdout)) || (I().stderr_capturable && fd == fileno(stderr)))
         I().buffer->append((char*)buf, count);
      return (ssize_t)count;
   }

   static int vfprintf(FILE* stream, const char* format, va_list ap)
   {
      char* alloca_str;
      h2_sprintvf(alloca_str, format, ap);
      return (int)write(fileno(stream), alloca_str, strlen(alloca_str));
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

   size_t test_count = 0;
   static ssize_t test_write(int fd, const void* buf, size_t count) { return I().test_count += count, (ssize_t)count; }

   static void initialize()
   {
      ::setbuf(stdout, 0);  // unbuffered
      I().buffer = new h2_string();
      static h2_stubs stubs;

#if !defined _WIN32
      stubs.add((void*)LIBC__write, (void*)test_write, {__FILE__, __LINE__, "write"});
      ::printf("\r"), ::fwrite("\r", 1, 1, stdout);
      stubs.clear();
#endif
      if (I().test_count != 2) {
         stubs.add((void*)::printf, (void*)printf, {__FILE__, __LINE__, "printf"});
         stubs.add((void*)::vprintf, (void*)vprintf, {__FILE__, __LINE__, "vprintf"});
         stubs.add((void*)::putchar, (void*)putchar, {__FILE__, __LINE__, "putchar"});
         stubs.add((void*)::puts, (void*)puts, {__FILE__, __LINE__, "puts"});
         stubs.add((void*)::fprintf, (void*)fprintf, {__FILE__, __LINE__, "fprintf"});
         // stubs.add((void*)::vfprintf, (void*)vfprintf, {__FILE__, __LINE__, "vfprintf"});
         stubs.add((void*)::fputc, (void*)fputc, {__FILE__, __LINE__, "fputc"});
         stubs.add((void*)::putc, (void*)fputc, {__FILE__, __LINE__, "fputc"});
         stubs.add((void*)::fputs, (void*)fputs, {__FILE__, __LINE__, "fputs"});
         stubs.add((void*)::fwrite, (void*)fwrite, {__FILE__, __LINE__, "fwrite"});
#if defined __GNUC__
         struct streambuf : public std::streambuf {
            FILE* f;
            int sync() override { return 0; }
            int overflow(int c) override { return (c != EOF) && h2_stdio::fputc(c, f), 0; }
            streambuf(FILE* _f) : f(_f) { setp(nullptr, 0); }
         };
         static streambuf sb_out(stdout);
         static streambuf sb_err(stderr);
         std::cout.rdbuf(&sb_out); /* internal fwrite() called, but */
         std::cerr.rdbuf(&sb_err);
         std::clog.rdbuf(&sb_err); /* print to stderr */
#endif
      }
      stubs.add((void*)LIBC__write, (void*)write, {__FILE__, __LINE__, "write"});
#if !defined _WIN32
      stubs.add((void*)::syslog, (void*)syslog, {__FILE__, __LINE__, "syslog"});
      stubs.add((void*)::vsyslog, (void*)vsyslog, {__FILE__, __LINE__, "vsyslog"});
#endif
   }

   void start_capture(bool stdout_capturable_, bool stderr_capturable_, bool syslog_capturable_)
   {
      stdout_capturable = stdout_capturable_;
      stderr_capturable = stderr_capturable_;
      syslog_capturable = syslog_capturable_;
      buffer->clear();
   }

   const char* stop_capture()
   {
      stdout_capturable = stderr_capturable = syslog_capturable = false;
      buffer->push_back('\0');
      return buffer->c_str();
   }
};

h2_inline h2_cout::h2_cout(h2_matcher<const char*> m_, const char* e_, const char* type_, const h2_fs& fs_) : fs(fs_), m(m_), e(e_), type(type_)
{
   bool all = !strlen(type);
   h2_stdio::I().start_capture(all || h2_extract::has(type, "out"), all || h2_extract::has(type, "err"), all || h2_extract::has(type, "syslog"));
}

h2_inline h2_cout::~h2_cout()
{
   h2_assert_g();
   h2_fail* fail = m.matches(h2_stdio::I().stop_capture(), 0);
   if (fail) {
      fail->fs = fs;
      fail->assert_type = "OK";
      fail->e_expression = e;
      fail->a_expression = "";
      fail->explain = "COUT";
      h2_fail_g(fail);
   }
}
