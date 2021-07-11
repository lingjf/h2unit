
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
