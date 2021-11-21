#include "../source/h2_unit.cpp"

/* clang-format off */
CASE(file line)
{
   OK(EndsWith("test_utils_misc.cpp:6"), H2_FILINE);
}
/* clang-format on */

CASE(type of __LINE__)
{
   auto line = __LINE__;
   // MSVC is long, gcc/clang is int
   OK(AnyOf("int", "long"), h2::h2_cxa::demangle(typeid(decltype(line)).name()));
}

SUITE(blank)
{
   Case(nullptr)
   {
      OK(h2::h2_blank(nullptr));
   }

   Case(empty)
   {
      OK(h2::h2_blank(""));
   }

   Case(space)
   {
      OK(h2::h2_blank(" "));
   }

   Case(space tab)
   {
      OK(h2::h2_blank(" \t \r \n "));
   }

   Case(space mix char)
   {
      OK(!h2::h2_blank(" \t abc \n "));
   }

   Case(abc)
   {
      OK(!h2::h2_blank("abc"));
   }
}

SUITE(basename)
{
   Case(basename "abc.cpp")
   {
      OK("abc.cpp", h2::h2_basefile("abc.cpp"));
   }

   Case(basename "test/abc.cpp:123")
   {
      OK("abc.cpp:123", h2::h2_basefile("test/abc.cpp:123"));
   }

   Case(basename "/proj/test/abc.cpp")
   {
      OK("abc.cpp", h2::h2_basefile("/proj/test/abc.cpp"));
   }

   Case(basename windows)
   {
      OK("abc.cpp", h2::h2_basefile("c:\\proj\\test\\abc.cpp"));
      OK("abc.cpp", h2::h2_basefile("c:\\abc.cpp"));
   }

   Case(basename "proj/test\\abc.cpp")
   {
      OK("abc.cpp", h2::h2_basefile("proj/test\\abc.cpp"));
   }
}

CASE(comma_if)
{
   OK("", h2::comma_if(false));
   OK(", ", h2::comma_if(true));

   OK("", h2::comma_if(false, ",,"));
   OK(",,", h2::comma_if(true, ",,"));

   OK("  ", h2::comma_if(false, ",,", "  "));
   OK(",,", h2::comma_if(true, ",,", "  "));
}

SUITE(h2_in)
{
   Case(3 items)
   {
      OK(h2::h2_in("a", 3, "a", "ab", "abc"));
      OK(h2::h2_in("ab", 3, "a", "ab", "abc"));
      OK(h2::h2_in("abc", 3, "a", "ab", "abc"));

      OK(!h2::h2_in("", 3, "a", "ab", "abc"));
      OK(!h2::h2_in("abcd", 3, "a", "ab", "abc"));
      OK(!h2::h2_in("s", 3, "a", "ab", "abc"));
   }

   Case(0 items)
   {
      OK(!h2::h2_in("a", 0));
   }
}

SUITE(h2_candidate)
{
   Case(3 candidates)
   {
      OK("single", h2::h2_candidate("si", 3, "single", "double", "sample"));
      OK("single", h2::h2_candidate("sin", 3, "single", "double", "sample"));
      OK("single", h2::h2_candidate("sing", 3, "single", "double", "sample"));
      OK("single", h2::h2_candidate("singl", 3, "single", "double", "sample"));
      OK("single", h2::h2_candidate("single", 3, "single", "double", "sample"));

      OK(IsNull, h2::h2_candidate("z", 3, "single", "double", "sample"));
   }

   Case(ambiguous)
   {
      OK("ambiguous argument: s, candidates: single | sample", h2::h2_candidate("s", 3, "single", "double", "sample"));
   }
}

CASE(ss)
{
   OK("h2unit", h2::ss("h2unit"));
   OK("", h2::ss(""));
   OK("", h2::ss(nullptr));
}

CASE(sn)
{
   OK(42, h2::sn(42));
   OK(0, h2::sn());
}

SUITE(h2_extract)
{
   Case(has)
   {
      const char* a0 = "ok";
      OK(NotNull, h2::h2_extract::has(a0, "ok"));

      const char* a1 = "to=1.2.3.4";
      OK(NotNull, h2::h2_extract::has(a1, "to"));

      const char* a2 = "to=1.2.3.4, name = zhang3";
      OK(NotNull, h2::h2_extract::has(a2, "to"));
      OK(NotNull, h2::h2_extract::has(a2, "name"));
   }

   Case(numeric)
   {
      double ret;

      const char* a1 = "len=1234";
      OK(true, h2::h2_extract::numeric(a1, "len", ret));
      OK(1234, ret);

      const char* a2 = "len =1234.56";
      OK(true, h2::h2_extract::numeric(a2, "len", ret));
      OK(1234.56, ret);

      const char* a3 = "len =  1234.56 ";
      OK(true, h2::h2_extract::numeric(a3, "len", ret));
      OK(1234.56, ret);
   }

   Case(iport)
   {
      char ret[256];

      const char* a1 = "to=1.2.3.4:5678";
      OK(h2::h2_extract::iport(a1, "to", ret));
      OK("1.2.3.4:5678", ret);

      const char* a2 = "to = 1.2.3.4:*";
      OK(h2::h2_extract::iport(a2, "to", ret));
      OK("1.2.3.4:*", ret);

      const char* a3 = "to=  * :5678";
      OK(h2::h2_extract::iport(a3, "to", ret));
      OK("*:5678", ret);

      const char* a4 = "to=*:  *";
      OK(h2::h2_extract::iport(a4, "to", ret));
      OK("*:*", ret);
   }

   Case(fill)
   {
      unsigned char ret[256];

      const char* a1 = "bin=1";
      OK(1, h2::h2_extract::fill(a1, "bin", ret));
      OK(Me("\x01", 1), ret);

      const char* a2 = "bin =0x55";
      OK(1, h2::h2_extract::fill(a2, "bin", ret));
      OK(Me("\x55", 1), ret);

      const char* a3 = "bin= 60000";
      OK(2, h2::h2_extract::fill(a3, "bin", ret));
      OK(Me("\x60\xEA", 2), ret);

      const char* a4 = "bin=  0x5566";
      OK(2, h2::h2_extract::fill(a4, "bin", ret));
      OK(Me("\x55\x66", 2), ret);

      const char* a5 = "bin=0x1122334455667788";
      OK(8, h2::h2_extract::fill(a5, "bin", ret));
      OK(Me("\x11\x22\x33\x44\x55\x66\x77\x88", 8), ret);
   }
}

CASE(once)
{
   h2::h2_once once;
   OK(once);
   OK(!once);
   OK(!once);
}

CASE(H2Foreach)
{
   int s = 0;
#define FOO(x) s += x;
   H2Foreach(FOO, (1, 2, 3))
#undef FOO
     OK(6, s);
}

SUITE(H2Fullmesh)
{
   Case(direct)
   {
      int s = 0;
#define FOO(x, y) s += x * y;
      H2Fullmesh(FOO, (1, 2, 3));
#undef FOO
      OK(1 * 1 + 1 * 2 + 1 * 3 + 2 * 1 + 2 * 2 + 2 * 3 + 3 * 1 + 3 * 2 + 3 * 3, s);
   }

   Case(macro)
   {
#define M123 1, 2, 3
      int s = 0;
#define FOO(x, y) s += x * y;
      H2Fullmesh(FOO, (M123), (M123));
#undef FOO
      OK(1 * 1 + 1 * 2 + 1 * 3 + 2 * 1 + 2 * 2 + 2 * 3 + 3 * 1 + 3 * 2 + 3 * 3, s);
   }

#define BAR(x, y) \
   Case(generator x y) {}

   H2Fullmesh(BAR, (A, B, C), (1, 2, 3))
#undef BAR
}
