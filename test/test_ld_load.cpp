#include "../source/h2_unit.cpp"

void say_something1()
{
   ::printf("hello world\n");
}
#if 0
SUITE(load)
{
   Case(symbol_to_addr)
   {
      h2::h2_symbol* res[16];
      int n = h2::h2_nm::get_by_name("say_something1", res, 16);
      OK(1, n);
      OK(Nq((unsigned long long)say_something1), (unsigned long long)res[0]->offset);
      OK((unsigned long long)say_something1, (unsigned long long)res[0]->offset);
      OK((unsigned long long)say_something1, (unsigned long long)h2::h2_load::symbol_to_addr(res[0]->offset));
   }
}
#endif
