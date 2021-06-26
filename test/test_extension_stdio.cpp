#include "../source/h2_unit.cpp"

static int my_printf(const char* fmt, ...)
{
   va_list a;
   va_start(a, fmt);
   int ret = vprintf(fmt, a);
   va_end(a);
   return ret;
}

static int my_fprintf(FILE* stream, const char* fmt, ...)
{
   va_list a;
   va_start(a, fmt);
   int ret = vfprintf(stream, fmt, a);
   va_end(a);
   return ret;
}

SUITE(stdio)
{
   const char* path = ".stdio.txt";
   int ret = 0;

   Cleanup() { ::remove(path); }

   Case(printf)
   {
      const char* e = "printf! 42";
      COUT(e)
      {
         ret = printf("printf! %d", 42);
         OK(strlen(e), ret);
      }
   }

   Case(putchar)
   {
      COUT(CaseLess("C"))
      {
         ret = putchar('C');
         OK('C', ret);
      }
   }

   Case(puts)
   {
      COUT(Se("puts! 42\n"))
      {
         ret = puts("puts! 42");
         OK(Gt(0), ret);
      }
   }

   Case(vprintf)
   {
      const char* e = "vprintf! 42";
      COUT(e)
      {
         ret = my_printf("vprintf! %d", 42);
         OK(strlen(e), ret);
      }
   }

#ifndef _WIN32
   Case(std::cout)
   {
      COUT("std::cout! 42")
      {
         std::cout << "std::cout! " << 42;
      }
   }

   Case(std::cerr)
   {
      COUT("std::cerr! 42")
      {
         std::cerr << "std::cerr! " << 42;
      }
   }

   Case(std::clog)
   {
      COUT("std::clog! 42")
      {
         std::clog << "std::clog! " << 42;
      }
   }

   Case(fprintf)
   {
      const char* e1 = "fprintf stdout! 42";
      COUT(e1)
      {
         ret = fprintf(stdout, "fprintf stdout! %d", 42);
         OK(strlen(e1), ret);
      }

      const char* e2 = "fprintf stderr! 42";
      COUT(e2)
      {
         ret = fprintf(stderr, "fprintf stderr! %d", 42);
         OK(strlen(e2), ret);
      }
   }

   Case(vfprintf)
   {
      const char* e1 = "vfprintf stdout! 42";
      COUT(e1)
      {
         ret = my_fprintf(stdout, "vfprintf stdout! %d", 42);
         OK(strlen(e1), ret);
      }

      const char* e2 = "vfprintf stderr! 42";
      COUT(e2)
      {
         ret = my_fprintf(stderr, "vfprintf stderr! %d", 42);
         OK(strlen(e2), ret);
      }
   }

   Case(fputc)
   {
      COUT("C")
      {
         ret = fputc('C', stdout);
         OK('C', ret);
      }

      COUT("D")
      {
         ret = fputc('D', stderr);
         OK('D', ret);
      }
   }

   Case(putc)
   {
      COUT("C")
      {
         ret = putc('C', stdout);
         OK('C', ret);
      }

      COUT("D")
      {
         ret = putc('D', stderr);
         OK('D', ret);
      }
   }

   Case(fputs)
   {
      COUT("fputs stdout! 42")
      {
         ret = fputs("fputs stdout! 42", stdout);
         OK(Gt(0), ret);
      }

      COUT("fputs stderr! 42")
      {
         ret = fputs("fputs stderr! 42", stderr);
         OK(Gt(0), ret);
      }
   }

   Case(fputs only stderr)
   {
      COUT("fputs stderr! 42", stderr)
      {
         ret = fputs("fputs stdout! 42", stdout);
         OK(Gt(0), ret);
         ret = fputs("fputs stderr! 42", stderr);
         OK(Gt(0), ret);
      }
   }

   Case(fwrite)
   {
      const char* e1 = "fwrite stdout! 42";
      COUT(e1)
      {
         ret = fwrite(e1, 1, strlen(e1), stdout);
         OK(strlen(e1), ret);
      }

      const char* e2 = "fwrite stderr! 42";
      COUT(e2)
      {
         ret = fwrite(e2, 1, strlen(e2), stderr);
         OK(strlen(e2), ret);
      }
   }

   Case(syslog)
   {
#   if !defined _WIN32
      const char* e1 = "syslog! 42";
      COUT(e1)
      {
         syslog(LOG_DEBUG, "syslog! %d", 42);
      }
#   endif
   }

   Case(normal file write)
   {
      COUT("")
      {
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
         OK(e, t);
         OK(strlen(e) + 1, ret);
         fclose(fp2);
      }
   }

   Case(normal file vfprintf)
   {
      COUT("")
      {
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
      }
   }
#endif
}
