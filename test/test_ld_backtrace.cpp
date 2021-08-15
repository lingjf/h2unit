#include "../source/h2_unit.cpp"

SUITE(backtrace)
{
   Case(constructor 0)
   {
      h2::h2_backtrace bt;
      OK(0, bt.count);
      OK(0, bt.shift);
   }

   Case(copy constructor)
   {
      auto bt = h2::h2_backtrace::dump(2);
      h2::h2_backtrace b1(bt);
      OK(Gt(0), b1.count);
      OK(2, b1.shift);
      OK(b1 == bt);
   }

   Case(assign constructor)
   {
      auto bt = h2::h2_backtrace::dump(2);

      h2::h2_backtrace b1 = bt;
      OK(Gt(0), b1.count);
      OK(2, b1.shift);
      OK(b1 == bt);

      h2::h2_backtrace b2;
      b2 = bt;
      OK(Gt(0), b2.count);
      OK(2, b2.shift);
      OK(b2 == bt);
   }
}

SUITE(backtrace extract)
{
   char mangled[256] = "---";
   unsigned long long offset = 0xFFFFEEEE;

#if defined __APPLE__
   Case(MAC backtrace_symbols line)
   {
      const char* p = "3   a.out  0x000000010e777f3d _ZN2h24hook6mallocEm + 45";
      bool ret = h2::backtrace_extract(p, mangled, &offset);

      OK(ret);
      OK("_ZN2h24hook6mallocEm", mangled);
      OK(45, offset);
   }
#endif

#if defined __linux__

   Case("./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]")
   {
      const char* p = "./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]";
      bool ret = h2::backtrace_extract(p, mangled, &offset);

      OK(ret);
      OK("_ZN2h24task7executeEv", mangled);
      OK(0x131, offset);
   }

   Case("./a.out(+0xb1887)[0x560c5ed06887]")
   {
      const char* p = "./a.out(+0xb1887)[0x560c5ed06887]";
      bool ret = h2::backtrace_extract(p, mangled, &offset);

      OK(ret);
      OK("", mangled);
      OK(0xb1887, offset);
   }

   Case("./a.out() [0x40b960]")
   {
      const char* p = "./a.out() [0x40b960]";
      bool ret = h2::backtrace_extract(p, mangled, &offset);

      OK(!ret);
   }

   Case("./a.out[0x4060e7]")
   {
      const char* p = "./a.out[0x4060e7]";
      bool ret = h2::backtrace_extract(p, mangled, &offset);

      OK(!ret);
   }
#endif
}
