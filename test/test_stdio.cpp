#include "../source/h2_unit.h"

using namespace h2;

static int my_printf(const char* fmt, ...) {
   va_list a;
   va_start(a, fmt);
   int ret = vprintf(fmt, a);
   va_end(a);
   return ret;
}

static int my_fprintf(FILE* stream, const char* fmt, ...) {
   va_list a;
   va_start(a, fmt);
   int ret = vfprintf(stream, fmt, a);
   va_end(a);
   return ret;
}

SUITE(stdio) {
   const char* path = ".normal.txt";
   int ret = 0;
   const char* a;

   Teardown() {
      ::remove(path);
   };

   Case(printf) {
      a = COUT();
      const char* e = "printf! 42";
      ret = printf("printf! %d", 42);
      OK(strlen(e), ret);
      OK(e, a);
   };

   Case(putchar) {
      a = COUT();
      ret = putchar('C');
      OK('C', ret);
      OK("C", a);
   };

   Case(puts) {
      a = COUT();
      ret = puts("puts! 42");
      OK(Gt(0), ret);
      OK("puts! 42", a);
   };

   Case(vprintf) {
      a = COUT();
      const char* e = "vprintf! 42";
      ret = my_printf("vprintf! %d", 42);
      OK(strlen(e), ret);
      OK(e, a);
   };

   Case(std::cout) { /* internal fwrite() called */
      a = COUT();
      const char* e = "std::cout! 42";
      std::cout << "std::cout! " << 42;
      OK(e, a);
   };

   Case(fprintf_stdout) {
      a = COUT();
      const char* e = "fprintf stdout! 42";
      ret = fprintf(stdout, "fprintf stdout! %d", 42);
      OK(strlen(e), ret);
      OK(e, a);
   };

   Case(fprintf_stderr) {
      a = COUT();
      const char* e = "fprintf stderr! 42";
      ret = fprintf(stderr, "fprintf stderr! %d", 42);
      OK(strlen(e), ret);
      OK(e, a);
   };
#ifdef __APPLE__
   Case(vfprintf_stdout) {
      a = COUT();
      const char* e = "vfprintf stdout! 42";
      ret = my_fprintf(stdout, "vfprintf stdout! %d", 42);
      OK(strlen(e), ret);
      OK(e, a);
   };

   Case(vfprintf_stderr) {
      a = COUT();
      const char* e = "vfprintf stderr! 42";
      ret = my_fprintf(stderr, "vfprintf stderr! %d", 42);
      OK(strlen(e), ret);
      OK(e, a);
   };
#endif
   Case(fputc_stdout) {
      a = COUT();
      ret = fputc('C', stdout);
      OK('C', ret);
      OK("C", a);
   };

   Case(fputc_stderr) {
      a = COUT();
      ret = fputc('D', stderr);
      OK('D', ret);
      OK("D", a);
   };

   Case(putc_stdout) {
      a = COUT();
      ret = putc('C', stdout);
      OK('C', ret);
      OK("C", a);
   };

   Case(putc_stderr) {
      a = COUT();
      ret = putc('D', stderr);
      OK('D', ret);
      OK("D", a);
   };

   Case(fputs_stdout) {
      a = COUT();
      ret = fputs("fputs stdout! 42", stdout);
      OK(Gt(0), ret);
      OK("fputs stdout! 42", a);
   };

   Case(fputs_stderr) {
      a = COUT();
      ret = fputs("fputs stderr! 42", stderr);
      OK(Gt(0), ret);
      OK("fputs stderr! 42", a);
   };

   Case(fwrite_stdout) {
      a = COUT();
      const char* e = "fwrite stdout! 42";
      ret = fwrite(e, 1, strlen(e), stdout);
      OK(strlen(e), ret);
      OK(e, a);
   };

   Case(fwrite_stderr) {
      a = COUT();
      const char* e = "fwrite stderr! 42";
      ret = fwrite(e, 1, strlen(e), stderr);
      OK(strlen(e), ret);
      OK(e, a);
   };

   Case(normal file write) {
      a = COUT();

      FILE* fp1 = fopen(path, "w");
      ret = fprintf(fp1, "fprintf! %d", 42);
      OK(strlen("fprintf! 42"), ret);
      ret = fputc('A', fp1);
      OK('A', ret);
      ret = putc('B', fp1);
      OK('B', ret);
      ret = fputs("fputs! 42", fp1);
      OK(Gt(0), ret);
      ret = fwrite("fwrite! 42", 1, strlen("fwrite! 42") + 1, fp1);
      OK(strlen("fwrite! 42") + 1, ret);
      fclose(fp1);

      const char* e = "fprintf! 42ABfputs! 42fwrite! 42";
      FILE* fp2 = fopen(path, "r");
      char t[1024] = {'\0'};
      ret = fread(t, 1, sizeof(t), fp2);
      OK(t, e);
      OK(strlen(e) + 1, ret);
      fclose(fp2);

      ::remove(path);

      OK("", a);
   }

#ifdef __APPLE__
   Case(normal file vfprintf) {
      a = COUT();

      FILE* fp1 = fopen(path, "w");
      ret = my_fprintf(fp1, "vfprintf! %d", 42);
      OK(strlen("vfprintf! 42"), ret);
      fclose(fp1);

      const char* e = "vfprintf! 42";
      FILE* fp2 = fopen(path, "r");
      char t[1024] = {'\0'};
      ret = fread(t, 1, sizeof(t), fp2);
      OK(t, e);
      OK(strlen(e), ret);
      fclose(fp2);

      ::remove(path);

      OK("", a);
   }
#endif
}
