#include "../source/h2_unit.cpp"

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

   Case(hex_to_byte)
   {
      OK(0, h2::hex_to_byte('0'));
      OK(1, h2::hex_to_byte('1'));
      OK(2, h2::hex_to_byte('2'));
      OK(3, h2::hex_to_byte('3'));
      OK(4, h2::hex_to_byte('4'));
      OK(5, h2::hex_to_byte('5'));
      OK(6, h2::hex_to_byte('6'));
      OK(7, h2::hex_to_byte('7'));
      OK(8, h2::hex_to_byte('8'));
      OK(9, h2::hex_to_byte('9'));
      OK(10, h2::hex_to_byte('a'));
      OK(11, h2::hex_to_byte('b'));
      OK(12, h2::hex_to_byte('c'));
      OK(13, h2::hex_to_byte('d'));
      OK(14, h2::hex_to_byte('e'));
      OK(15, h2::hex_to_byte('f'));
      OK(10, h2::hex_to_byte('A'));
      OK(11, h2::hex_to_byte('B'));
      OK(12, h2::hex_to_byte('C'));
      OK(13, h2::hex_to_byte('D'));
      OK(14, h2::hex_to_byte('E'));
      OK(15, h2::hex_to_byte('F'));
   }

   Case(is bin string)
   {
      OK(h2::h2_numeric::is_bin_string("110011100"));
      OK(h2::h2_numeric::is_bin_string("1100 11100"));
      OK(!h2::h2_numeric::is_bin_string("110211100"));
   }

   Case(is hex string)
   {
      OK(h2::h2_numeric::is_hex_string("10Ac1EE0Fd"));
      OK(h2::h2_numeric::is_hex_string("10Ac1 E0Fd"));
      OK(h2::h2_numeric::is_hex_string("0xDeadC0fe"));
      OK(!h2::h2_numeric::is_hex_string("10Ac1PE0Fd"));
   }

   Case(hex_to_bytes C)
   {
      unsigned char e1[] = {0xC};
      OK(1, h2::h2_numeric::hex_to_bytes("C", z1));
      OK(Me(e1, 1), z1);
   }

   Case(hex_to_bytes 4C)
   {
      unsigned char e1[] = {0x4C};
      OK(1, h2::h2_numeric::hex_to_bytes("4C", z1));
      OK(memcmp(e1, z1, 1) == 0);
   }

   Case(hex_to_bytes Cd8)
   {
      unsigned char e1[] = {0xC, 0xD8};
      OK(2, h2::h2_numeric::hex_to_bytes("Cd8", z1));
      OK(memcmp(e1, z1, 2) == 0);
   }

   Case(hex_to_bytes 4Cd8)
   {
      unsigned char e1[] = {0x4C, 0xD8};
      OK(2, h2::h2_numeric::hex_to_bytes("4Cd8", z1));
      OK(memcmp(e1, z1, 2) == 0);
   }

   Case(hex_to_bytes 24Cd8)
   {
      unsigned char e1[] = {0x2, 0x4C, 0xD8};
      OK(3, h2::h2_numeric::hex_to_bytes("24Cd8", z1));
      OK(memcmp(e1, z1, 3) == 0);
   }
}

SUITE(numeric bits_equal)
{
   bool ret;
   size_t nbits;
   unsigned char z1[1024];

   Case(bits_equal tobytes 10001110)
   {
      unsigned char a1[] = {0x8E};
      nbits = h2::h2_numeric::bin_to_bits("10001110", z1);
      OK(8, nbits);

      ret = h2::h2_numeric::bits_equal(z1, (const unsigned char*)a1, nbits);
      OK(ret);
      OK(memcmp(z1, a1, 1) == 0);
   }

   Case(bits_equal tobytes 1000 1110)
   {
      unsigned char a1[] = {0x8E};
      nbits = h2::h2_numeric::bin_to_bits("1000 1110", z1);
      OK(8, nbits);

      ret = h2::h2_numeric::bits_equal(z1, (const unsigned char*)a1, nbits);
      OK(ret);
      OK(memcmp(z1, a1, 1) == 0);
   }

   Case(bits_equal tobytes 1000 1110 1100)
   {
      unsigned char a1[] = {0x8E, 0xC0};
      nbits = h2::h2_numeric::bin_to_bits("1000 1110 1100", z1);
      OK(12, nbits);

      ret = h2::h2_numeric::bits_equal(z1, (const unsigned char*)a1, nbits);
      OK(ret);
      OK(memcmp(z1, a1, 2) == 0);
   }

   Case(bits_equal tobytes 1000 1110 1100 1)
   {
      unsigned char a1[] = {0x8E, 0xC8};
      nbits = h2::h2_numeric::bin_to_bits("1000 1110 1100 1", z1);
      OK(13, nbits);

      ret = h2::h2_numeric::bits_equal(z1, (const unsigned char*)a1, nbits);
      OK(ret);
      OK(memcmp(z1, a1, 2) == 0);
   }
}

SUITE(numeric sequence_number)
{
   Cases(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18)
   {
      const char* e[] = {"0th", "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th", "13th", "14th", "15th", "16th", "17th", "18th", "19th", "20th"};

      OK(e[x], h2::h2_numeric::sequence_number(x, 0));
   }

   Cases(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18)
   {
      const char* e[] = {"1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th", "13th", "14th", "15th", "16th", "17th", "18th", "19th", "20th"};

      OK(e[x], h2::h2_numeric::sequence_number(x));
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

CASE(format_duration)
{
   OK("99 milliseconds", h2::format_duration(99));
   OK("0.8 seconds", h2::format_duration(799));
   OK("1 second", h2::format_duration(1000));
   OK("8.2 seconds", h2::format_duration(1000 * 8 + 200));
   OK("1 minute", h2::format_duration(1000 * 60));
   OK("1.5 minutes", h2::format_duration(1000 * 60 + 1000 * 30));
   OK("1 hour", h2::format_duration(1000 * 60 * 60));
   OK("1.5 hours", h2::format_duration(1000 * 60 * 60 + 1000 * 60 * 30));
}

CASE(format_volume)
{
   OK("101", h2::format_volume(101));
   OK("1KB", h2::format_volume(1024));
   OK("1.5KB", h2::format_volume(1024 + 512));
   OK("1MB", h2::format_volume(1024 * 1024));
   OK("1.5MB", h2::format_volume(1024 * 1024 + 1024 * 512));
   OK("1GB", h2::format_volume(1024 * 1024 * 1024));
   OK("1.5GB", h2::format_volume(1024 * 1024 * 1024 + 1024 * 1024 * 512));
}
