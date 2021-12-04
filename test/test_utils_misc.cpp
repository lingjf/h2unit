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

CASE(once)
{
   h2::h2_once once;
   bool a1 = once;
   bool a2 = !once;
   bool a3 = !once;
   OK(a1);
   OK(a2);
   OK(a3);
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

SUITE(strip)
{
   Case(left no stripped)
   {
      const char* a1 = "hello world";
      OK(a1, h2::strip_left(a1));
      OK(a1, h2::strip_left(a1, a1 + 5));
   }

   Case(left empty)
   {
      const char* a1 = "";
      OK(a1, h2::strip_left(a1));

      const char* a2 = "  ";
      OK(a2 + 2, h2::strip_left(a2));

      const char* a3 = " \t \n ";
      OK(a3 + 5, h2::strip_left(a3));
   }

   Case(left spaces)
   {
      const char* a1 = "  hello world";
      OK(a1 + 2, h2::strip_left(a1));
      OK(a1 + 1, h2::strip_left(a1, a1 + 1));
      OK(a1 + 2, h2::strip_left(a1, a1 + 5));
   }

   Case(left tabs)
   {
      const char* a1 = "  \t\nhello world";
      OK("hello world", h2::strip_left(a1));
      OK(" \t\nhello world", h2::strip_left(a1, a1 + 1));
      OK("\t\nhello world", h2::strip_left(a1, a1 + 2));
      OK("\nhello world", h2::strip_left(a1, a1 + 3));
      OK("hello world", h2::strip_left(a1, a1 + 4));
      OK("hello world", h2::strip_left(a1, a1 + 5));
   }

   Case(right no stripped)
   {
      const char* a1 = "hello world";
      OK("", h2::strip_right(a1));
      OK("", h2::strip_right(a1, a1 + strlen(a1)));
      OK("d", h2::strip_right(a1, a1 + strlen(a1) - 1));
      OK("ld", h2::strip_right(a1, a1 + strlen(a1) - 2));
      OK("rld", h2::strip_right(a1, a1 + strlen(a1) - 3));
   }

   Case(right empty)
   {
      const char* a1 = "";
      OK(a1, h2::strip_right(a1));

      const char* a2 = "  ";
      OK(a2, h2::strip_right(a2));

      const char* a3 = " \r \n ";
      OK(a3, h2::strip_right(a3));
   }

   Case(right)
   {
      const char* a1 = "hello world  ";
      OK(a1 + 11, h2::strip_right(a1));
      OK(a1 + 11, h2::strip_right(a1, a1 + strlen(a1)));
      OK(a1 + 11, h2::strip_right(a1, a1 + strlen(a1) - 1));
      OK(a1 + 11, h2::strip_right(a1, a1 + strlen(a1) - 2));

      const char* a2 = "hello world \r \n ";
      OK(a2 + 11, h2::strip_right(a2));
      OK(a2 + 11, h2::strip_right(a2, a2 + strlen(a2)));
      OK(a2 + 11, h2::strip_right(a2, a2 + strlen(a2) - 1));
      OK(a2 + 11, h2::strip_right(a2, a2 + strlen(a2) - 2));
      OK(a2 + 11, h2::strip_right(a2, a2 + strlen(a2) - 3));
      OK(a2 + 11, h2::strip_right(a2, a2 + strlen(a2) - 4));
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

      OK(NULL, h2::h2_candidate("z", 3, "single", "double", "sample"));
   }

   Case(ambiguous)
   {
      OK("ambiguous argument: s, candidates: single | sample", h2::h2_candidate("s", 3, "single", "double", "sample"));
   }
}

CASE(H2Foreach)
{
   int s = 0;
#define FOO(x) s += x;
   H2Foreach(FOO, (1, 2, 3))
#undef FOO
     OK(6, s);
}

SUITE(H2Fullmesh){
  Case(direct){
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

SUITE(get key value)
{
   Case(unary)
   {
      const char* a0 = "ok";
      OK("", h2::get_keyvalue(a0, "ok"));
      OK(NULL, h2::get_keyvalue(a0, "ko"));
   }

   Case(key=value)
   {
      const char* a1 = "to=1.2.3.4";
      OK("1.2.3.4", h2::get_keyvalue(a1, "to"));
   }

   Case(strip space)
   {
      const char* a2 = "to=1.2.3.4, name = zhang3";
      OK(Not(NULL), h2::get_keyvalue(a2, "to"));
      OK("zhang3", h2::get_keyvalue(a2, "name"));
   }
}

SUITE(numeric)
{
   unsigned char z1[1024];

   Case(not2n)
   {
      OK(!h2::not2n(0));

      for (int i = 0; i < 32; i++)
         OK(!h2::not2n(1 << i));

      OK(h2::not2n(3));
      OK(h2::not2n(0x55));
   }

   Case(mask2n)
   {
      OK(0x0, h2::mask2n(0));
      OK(0x1, h2::mask2n(1));
      OK(0x3, h2::mask2n(2));
      OK(0x3, h2::mask2n(3));
      OK(0x7, h2::mask2n(4));
      OK(0x7, h2::mask2n(5));
      OK(0x7, h2::mask2n(6));
      OK(0x7, h2::mask2n(7));
      OK(0xF, h2::mask2n(8));
      OK(0xF, h2::mask2n(9));
      OK(0xF, h2::mask2n(10));
      OK(0xF, h2::mask2n(11));
      OK(0xF, h2::mask2n(12));
      OK(0xF, h2::mask2n(13));
      OK(0xF, h2::mask2n(14));
      OK(0xF, h2::mask2n(15));
      for (unsigned i = 16; i < 32; ++i) {
         OK(0x1F, h2::mask2n(i));
      }
      for (unsigned i = 32; i < 64; ++i) {
         OK(0x3F, h2::mask2n(i));
      }
      for (unsigned i = 64; i < 128; ++i) {
         OK(0x7F, h2::mask2n(i));
      }
      for (unsigned i = 128; i < 256; ++i) {
         OK(0xFF, h2::mask2n(i));
      }
      for (unsigned i = 0x7FFFFFF0U; i < 0x7FFFFFFFU; ++i) {
         OK(0x7FFFFFFFU, h2::mask2n(i));
      }
      for (unsigned i = 0xFFFFFFF0U; i < 0xFFFFFFFFU; ++i) {
         OK(0xFFFFFFFFU, h2::mask2n(i));
      }
   }

   Case(hex2byte)
   {
      OK(0, h2::hex2byte('0'));
      OK(1, h2::hex2byte('1'));
      OK(2, h2::hex2byte('2'));
      OK(3, h2::hex2byte('3'));
      OK(4, h2::hex2byte('4'));
      OK(5, h2::hex2byte('5'));
      OK(6, h2::hex2byte('6'));
      OK(7, h2::hex2byte('7'));
      OK(8, h2::hex2byte('8'));
      OK(9, h2::hex2byte('9'));
      OK(10, h2::hex2byte('a'));
      OK(11, h2::hex2byte('b'));
      OK(12, h2::hex2byte('c'));
      OK(13, h2::hex2byte('d'));
      OK(14, h2::hex2byte('e'));
      OK(15, h2::hex2byte('f'));
      OK(10, h2::hex2byte('A'));
      OK(11, h2::hex2byte('B'));
      OK(12, h2::hex2byte('C'));
      OK(13, h2::hex2byte('D'));
      OK(14, h2::hex2byte('E'));
      OK(15, h2::hex2byte('F'));
   }

   Case(hex2bytes C)
   {
      unsigned char e1[] = {0xC};
      OK(1, h2::hex2bytes("C", z1));
      OK(Me(e1, 1), z1);
   }

   Case(hex2bytes 4C)
   {
      unsigned char e1[] = {0x4C};
      OK(1, h2::hex2bytes("4C", z1));
      OK(memcmp(e1, z1, 1) == 0);
   }

   Case(hex2bytes Cd8)
   {
      unsigned char e1[] = {0xC, 0xD8};
      OK(2, h2::hex2bytes("Cd8", z1));
      OK(memcmp(e1, z1, 2) == 0);
   }

   Case(hex2bytes 4Cd8)
   {
      unsigned char e1[] = {0x4C, 0xD8};
      OK(2, h2::hex2bytes("4Cd8", z1));
      OK(memcmp(e1, z1, 2) == 0);
   }

   Case(hex2bytes 24Cd8)
   {
      unsigned char e1[] = {0x2, 0x4C, 0xD8};
      OK(3, h2::hex2bytes("24Cd8", z1));
      OK(memcmp(e1, z1, 3) == 0);
   }
}

SUITE(index_th)
{
   Cases(base0, (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18))
   {
      const char* e[] = {"0th", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th", "13th", "14th", "15th", "16th", "17th", "18th", "19th", "20th"};

      OK(e[x], h2::index_th(x, 0));
   }

   Cases(base1, (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18))
   {
      const char* e[] = {"1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th", "13th", "14th", "15th", "16th", "17th", "18th", "19th", "20th"};

      OK(e[x], h2::index_th(x));
   }
}

SUITE(numeric number width)
{
   Case(base 10 dec)
   {
      OK(1, h2::number_strlen(0, 10));
      OK(1, h2::number_strlen(9, 10));
      OK(2, h2::number_strlen(10, 10));
      OK(2, h2::number_strlen(99, 10));
      OK(3, h2::number_strlen(100, 10));
      OK(3, h2::number_strlen(999, 10));
      OK(4, h2::number_strlen(1000, 10));
      OK(4, h2::number_strlen(9999, 10));
      OK(5, h2::number_strlen(10000, 10));
      OK(5, h2::number_strlen(99999, 10));
      OK(6, h2::number_strlen(100000, 10));
      OK(6, h2::number_strlen(999999, 10));
      OK(7, h2::number_strlen(1000000, 10));
      OK(7, h2::number_strlen(9999999, 10));
      OK(8, h2::number_strlen(10000000, 10));
      OK(8, h2::number_strlen(99999999, 10));
      OK(9, h2::number_strlen(100000000, 10));
      OK(9, h2::number_strlen(999999999, 10));
   }
   Case(base 16 hex)
   {
      OK(1, h2::number_strlen(0x0, 16));
      OK(1, h2::number_strlen(0xF, 16));
      OK(2, h2::number_strlen(0x10, 16));
      OK(2, h2::number_strlen(0xFF, 16));
      OK(3, h2::number_strlen(0x100, 16));
      OK(3, h2::number_strlen(0xFF0, 16));
      OK(4, h2::number_strlen(0x1000, 16));
      OK(4, h2::number_strlen(0xFFFF, 16));
      OK(5, h2::number_strlen(0x10000, 16));
      OK(5, h2::number_strlen(0xFFFFF, 16));
      OK(6, h2::number_strlen(0x100000, 16));
      OK(6, h2::number_strlen(0xFFFFFF, 16));
      OK(7, h2::number_strlen(0x1000000, 16));
      OK(7, h2::number_strlen(0xFFFFFFF, 16));
      OK(8, h2::number_strlen(0x10000000, 16));
      OK(8, h2::number_strlen(0xFFFFFFFF, 16));
   }
}
