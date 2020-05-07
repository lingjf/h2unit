#include "../source/h2_unit.cpp"

SUITE(allocate)
{
   Case(sys_malloc / sys_free)
   {
      char* p = (char*)h2::h2_libc::malloc(100);
      OK(p != NULL);
      strcpy(p, "Hello World brk mmap");
      h2::h2_libc::free(p);
   }
}
