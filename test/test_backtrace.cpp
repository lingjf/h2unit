#include "../source/h2_unit.hpp"
#include "../source/h2_unit.cpp"

SUITE(backtrace)
{
   Case(constructor 0)
   {
      h2::h2_backtrace bt;
      OK(0, bt.count);
      OK(0, bt.shift);
   };

   Case(constructor 1)
   {
      h2::h2_backtrace bt(2);
      OK(Gt(0), bt.count);
      OK(2, bt.shift);
   };

   Case(copy constructor)
   {
      h2::h2_backtrace bt(2);

      h2::h2_backtrace b1(bt);
      OK(Gt(0), b1.count);
      OK(2, b1.shift);
      OK(b1 == bt);
   };

   Case(assign constructor)
   {
      h2::h2_backtrace bt(2);

      h2::h2_backtrace b1 = bt;
      OK(Gt(0), b1.count);
      OK(2, b1.shift);
      OK(b1 == bt);

      h2::h2_backtrace b2;
      b2 = bt;
      OK(Gt(0), b2.count);
      OK(2, b2.shift);
      OK(b2 == bt);
   };

#ifdef __APPLE__
   Case(demangle namespace class member function)
   {
      char mangled[] = "_ZN2h24task7executeEv";
      char demangled[1024] = "1";

      size_t len = sizeof(demangled);
      int status = 0;
      abi::__cxa_demangle(mangled, demangled, &len, &status);

      OK(0, status);
      OK("h2::task::execute()", demangled);
   };

   Case(demangle namespace class member function one parameter)
   {
      char mangled[] = "_ZN2h24hook6mallocEm";
      char demangled[1024] = "2";

      h2::h2_backtrace bt;
      bool ret = h2::demangle(mangled, demangled, sizeof(demangled));
      OK(ret);
      OK("h2::hook::malloc(unsigned long)", demangled);
   };
#endif

   Case(demangle c-function)
   {
      char mangled[] = "malloc";
      char demangled[1024] = "3";

      h2::h2_backtrace bt;
      bool ret = h2::demangle(mangled, demangled, sizeof(demangled));
      OK(!ret);
   };
}

SUITE(backtrace extract)
{
   h2::h2_backtrace bt;
   char module[256] = "---", mangled[256] = "---";
   unsigned long long offset = 0xFFFFEEEE;

   Case(MAC backtrace_symbols line)
   {
      const char* p = "3   a.out  0x000000010e777f3d _ZN2h24hook6mallocEm + 45";
      bool ret = h2::backtrace_extract(p, module, mangled, &offset);

      OK(ret);
      OK("a.out", module);
      OK("_ZN2h24hook6mallocEm", mangled);
      OK(45, offset);
   };

   Case(Linux with '-rdynamic' linker option)
   {
      const char* p = "./a.out(_ZN2h24task7executeEv+0x131)[0x55aa6bb840ef]";
      bool ret = h2::backtrace_extract(p, module, mangled, &offset);

      OK(ret);
      OK("./a.out", module);
      OK("_ZN2h24task7executeEv", mangled);
      OK(0x131, offset);
   };

   Case(Ubuntu without '-rdynamic' linker option)
   {
      const char* p = "./a.out(+0xb1887)[0x560c5ed06887]";
      bool ret = h2::backtrace_extract(p, module, mangled, &offset);

      OK(ret);
      OK("./a.out", module);
      OK("", mangled);
      OK(0xb1887, offset);
   };

   Case(Redhat/CentOS without '-rdynamic' linker option)
   {
      const char* p = "./a.out() [0x40b960]";
      bool ret = h2::backtrace_extract(p, module, mangled, &offset);

      OK(ret);
      OK("./a.out", module);
      OK("", mangled);
      OK(0x40b960, offset);
   };

   Case(Linux 4)
   {
      const char* p = "./a.out[0x4060e7]";
      bool ret = h2::backtrace_extract(p, module, mangled, &offset);

      OK(ret);
      OK("./a.out", module);
      OK("", mangled);
      OK(0x4060e7, offset);
   };
}
