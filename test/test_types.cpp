
#ifdef _WIN32
#   define IMPORT_MAIN
#endif

#include "../source/h2_unit.cpp"

extern "C" {
int foobar0()
{
   return 0;
}
}

int foobar1(int a)
{
   return 1;
}

int foobar2(int a, const char* b)
{
   return 2;
}

int foobar3(int a)
{
   return 3;
}

const char* foobar3(const char* a)
{
   return "3";
}

int foobar6(char* a, ...)
{
   return 6;
}

namespace test_ns {
int foobar1(float a)
{
   return 1;
}
int foobar2(int a, const char* b)
{
   return 2;
}
}  // namespace test_ns

int my_printf(const char* fmt, ...)
{
   va_list a;
   va_start(a, fmt);
   int ret = vprintf(fmt, a);
   va_end(a);
   return ret;
}

int my_fprintf(FILE* stream, const char* fmt, ...)
{
   va_list a;
   va_start(a, fmt);
   int ret = vfprintf(stream, fmt, a);
   va_end(a);
   return ret;
}

int my_sprintf(char* t, const char* fmt, ...)
{
   va_list a;
   va_start(a, fmt);
   int ret = vsprintf(t, fmt, a);
   va_end(a);
   return ret;
}

int my_snprintf(char* t, int n, const char* fmt, ...)
{
   va_list a;
   va_start(a, fmt);
   int ret = vsnprintf(t, n, fmt, a);
   va_end(a);
   return ret;
}

void* my_pthread(void *arg)
{
   return NULL;
}
