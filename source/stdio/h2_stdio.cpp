struct h2_stdio {
   h2_singleton(h2_stdio);
   h2_string* buffer;
   bool stdout_capturable = false, stderr_capturable = false, syslog_capturable = false;
   size_t capture_length = 0;

   static ssize_t write(int fd, const void* buf, size_t count)
   {
      if (O.verbose >= VerboseNormal || (fd != fileno(stdout) && fd != fileno(stderr))) {
#if defined _WIN32 || defined __CYGWIN__  // +MinGW
         h2::h2_stub_temporary_restore _((void*)H2_LIBC_WRITE);
#endif  // Linux/macOS low level IO is syscall
         if ((fd == fileno(stdout) || fd == fileno(stderr)) && h2_report::I().backable) {
            h2_libc::write(fd, "\n", 1);  // fall printf/cout into new line from report title
            h2_report::I().backable = false;
         }
         h2_libc::write(fd == -21371647 ? fileno(stdout) : fd, buf, count);
         if (fd == fileno(stdout) || fd == fileno(stderr)) I().capture_length += count;
      }
      if ((I().stdout_capturable && fd == fileno(stdout)) || (I().stderr_capturable && fd == fileno(stderr))) I().buffer->append((char*)buf, count);
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

   static void initialize()
   {
      h2_console::show_cursor(false);
      ::setbuf(stdout, 0);  // unbuffered
      I().buffer = new h2_string();
      static h2_list stubs;

      h2_stubs::add(stubs, (void*)::printf, (void*)printf, "printf", H2_FILINE);
      h2_stubs::add(stubs, (void*)::vprintf, (void*)vprintf, "vprintf", H2_FILINE);
      h2_stubs::add(stubs, (void*)::putchar, (void*)putchar, "putchar", H2_FILINE);
      h2_stubs::add(stubs, (void*)::puts, (void*)puts, "puts", H2_FILINE);
      h2_stubs::add(stubs, (void*)::fprintf, (void*)fprintf, "fprintf", H2_FILINE);
      // h2_stubs::add(stubs,(void*)::vfprintf, (void*)vfprintf,  "vfprintf",H2_FILINE);
      h2_stubs::add(stubs, (void*)::fputc, (void*)fputc, "fputc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::putc, (void*)fputc, "fputc", H2_FILINE);
      h2_stubs::add(stubs, (void*)::fputs, (void*)fputs, "fputs", H2_FILINE);
      h2_stubs::add(stubs, (void*)::fwrite, (void*)fwrite, "fwrite", H2_FILINE);
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
      h2_stubs::add(stubs, (void*)H2_LIBC_WRITE, (void*)write, "write", H2_FILINE);
#if !defined _WIN32
      h2_stubs::add(stubs, (void*)::syslog, (void*)syslog, "syslog", H2_FILINE);
      h2_stubs::add(stubs, (void*)::vsyslog, (void*)vsyslog, "vsyslog", H2_FILINE);
#endif
   }

   static void finalize()
   {
      h2_console::show_cursor(true);
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

h2_inline h2_cout::h2_cout(h2_matcher<const char*> m_, const char* e_, const char* type_, const char* filine_) : filine(filine_), m(m_), e(e_), type(type_)
{
   bool all = !strlen(type);
   h2_stdio::I().start_capture(all || get_keyvalue(type, "out"), all || get_keyvalue(type, "err"), all || get_keyvalue(type, "syslog"));
}

h2_inline h2_cout::~h2_cout()
{
   h2_runner::asserts();
   h2_fail* fail = m.matches(h2_stdio::I().stop_capture(), 0);
   if (fail) {
      fail->filine = filine;
      fail->assert_type = "OK2";
      fail->e_expression = e;
      fail->a_expression = "";
      fail->explain = "COUT";
      h2_runner::failing(fail);
   }
}
