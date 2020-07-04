#include "../source/h2_unit.cpp"

int foobar(int a, const char* b)
{
   return 0;
}

SUITE(MOCK fails)
{
   Case(greed true)
   {
      MOCK(foobar, int, (int, const char*))
        .greed(true)
        .between(1, 3)
        .with(1, "A")
        .returns(11)
        .once(1, _)
        .returns(22);

      OK(11, foobar(1, "A"));
      OK(22, foobar(1, "A"));
   }
}
