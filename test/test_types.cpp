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

int foobar3(char* a, ...)
{
   return 3;
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
