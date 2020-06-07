#include "../source/h2_unit.cpp"

SUITE(Matchee utils)
{
   bool ret;
   unsigned char z1[1024];
   int nbits;

   Case(is bin string)
   {
      OK(h2::__is_bin_string("110011100"));
      OK(h2::__is_bin_string("1100 11100"));
      OK(!h2::__is_bin_string("110211100"));
   }

   Case(bits_equal tobytes 10001110)
   {
      unsigned char a1[] = {0x8E};
      nbits = h2::__bin_to_bytes("10001110", z1);
      OK(8, nbits);

      ret = h2::__bits_equal(z1, (const unsigned char*)a1, nbits);
      OK(ret);
      OK(memcmp(z1, a1, 1) == 0);
   }

   Case(bits_equal tobytes 1000 1110)
   {
      unsigned char a1[] = {0x8E};
      nbits = h2::__bin_to_bytes("1000 1110", z1);
      OK(8, nbits);

      ret = h2::__bits_equal(z1, (const unsigned char*)a1, nbits);
      OK(ret);
      OK(memcmp(z1, a1, 1) == 0);
   }

   Case(bits_equal tobytes 1000 1110 1100)
   {
      unsigned char a1[] = {0x8E, 0xC0};
      nbits = h2::__bin_to_bytes("1000 1110 1100", z1);
      OK(12, nbits);

      ret = h2::__bits_equal(z1, (const unsigned char*)a1, nbits);
      OK(ret);
      OK(memcmp(z1, a1, 2) == 0);
   }

   Case(bits_equal tobytes 1000 1110 1100 1)
   {
      unsigned char a1[] = {0x8E, 0xC8};
      nbits = h2::__bin_to_bytes("1000 1110 1100 1", z1);
      OK(13, nbits);

      ret = h2::__bits_equal(z1, (const unsigned char*)a1, nbits);
      OK(ret);
      OK(memcmp(z1, a1, 2) == 0);
   }

   Case(is hex string)
   {
      OK(h2::__is_hex_string("10Ac1EE0Fd"));
      OK(h2::__is_hex_string("10Ac1 E0Fd"));
      OK(h2::__is_hex_string("0xDeadC0fe"));
      OK(!h2::__is_hex_string("10Ac1PE0Fd"));
   }

   Case(hex to bytes)
   {
      unsigned char e1[] = {0xF1, 0xD8};
      OK(16, h2::__hex_to_bytes("F1d8", z1));
      OK(16, h2::__hex_to_bytes("0xF1d8", z1));
      OK(memcmp(e1, z1, 2) == 0);
   }
}

SUITE(Matchee)
{
   Case(Equal)
   {
      h2::h2_equals<int> a1(123);
      OK(nullptr == a1.matches(123));
      h2::h2_equals<const int> a2(123);
      OK(nullptr == a2.matches(123));
      h2::h2_equals<unsigned long long> a3(123);
      OK(nullptr == a3.matches(123));

      h2::h2_equals<bool> b1(false);
      OK(nullptr == b1.matches(false));
      h2::h2_equals<const bool> b2(false);
      OK(nullptr == b2.matches(false));

      h2::h2_equals<float> c1(65.000000001);
      OK(nullptr == c1.matches(65));
      h2::h2_equals<double> c2(65.000000001);
      OK(nullptr == c2.matches(65));
      h2::h2_equals<double> c3(65.000000001, 0.001);
      OK(nullptr == c3.matches(65));
      h2::h2_equals<double> c4(65.001, 0.0001);
      OK(nullptr != c4.matches(65));

      h2::h2_equals<char*> d1("abc");
      OK(nullptr == d1.matches("abc"));
      h2::h2_equals<h2::h2_string> d2("abc");
      OK(nullptr == d2.matches("abc"));
      h2::h2_equals<std::string> d3("abc");
      OK(nullptr == d3.matches("abc"));

      h2::h2_equals<char*> d4("*bc");
      OK(nullptr == d4.matches("abc"));

      h2::h2_equals<char*> d5(".*bc");
      OK(nullptr == d5.matches("abc"));
   }

   Case(Any)
   {
      h2::h2_matchee_any a;
      OK(nullptr == a.matches(65));
      OK(nullptr == a.matches(65.000000001));
      OK(nullptr == a.matches(true));
      OK(nullptr == a.matches("abc"));
      OK(nullptr == a.matches(NULL));
      OK(nullptr == a.matches(nullptr));
   }

   Case(Null)
   {
      int int65 = 65;

      h2::h2_matchee_null ee1(false);
      OK(nullptr == ee1.matches(NULL));
      OK(nullptr == ee1.matches(nullptr));
      OK(nullptr != ee1.matches(&int65));
      OK("IsNull", ee1.expects(nullptr));
      OK("NotNull", ee1.expects(nullptr, false, true));

      h2::h2_matchee_null ee2(true);
      OK(nullptr == ee2.matches(&int65));
      OK(nullptr != ee2.matches(nullptr));
      OK("NotNull", ee2.expects(nullptr));
      OK("IsNull", ee2.expects(nullptr, false, true));
   }

   Case(Boolean)
   {
      h2::h2_matchee_boolean<true> istrue;
      OK(nullptr == istrue.matches(true));
      OK(nullptr != istrue.matches(false));
      OK("true", istrue.expects(false));
      OK("false", istrue.expects(false, false, true));

      h2::h2_matchee_boolean<false> isfalse;
      OK(nullptr == isfalse.matches(false));
      OK(nullptr != isfalse.matches(true));
      OK("false", isfalse.expects(false));
      OK("true", isfalse.expects(false, false, true));
   }

   Case(ge, gt, le, lt)
   {
      h2::h2_matchee_ge<int> ge1(0);
      OK(nullptr == ge1.matches(1));
      OK(nullptr == ge1.matches(0));
      OK(nullptr != ge1.matches(-1));

      h2::h2_matchee_gt<int> gt1(0);
      OK(nullptr == gt1.matches(1));
      OK(nullptr != gt1.matches(0));
      OK(nullptr != gt1.matches(-1));

      h2::h2_matchee_le<int> le1(0);
      OK(nullptr != le1.matches(1));
      OK(nullptr == le1.matches(0));
      OK(nullptr == le1.matches(-1));

      h2::h2_matchee_lt<int> lt1(0);
      OK(nullptr != lt1.matches(1));
      OK(nullptr != lt1.matches(0));
      OK(nullptr == lt1.matches(-1));
   }

   Case(memcmp bytes)
   {
      unsigned char e[] = {1, 2, 3, 4, 5, 6, 7, 8};
      unsigned char a1[] = {1, 2, 3, 4, 5, 6, 7, 8};
      unsigned char a2[] = {5, 5, 5, 5, 5, 5, 5, 5};

      h2::h2_matchee_memcmp m(8, e, 8 * 8);
      OK(nullptr == m.matches(a1));
      OK(nullptr != m.matches(a2));
   }

   Case(memcmp bits)
   {
      h2::h2_matchee_memcmp m(1, "100011100010", -1);
      unsigned char a[] = {0x8E, 0x20};
      OK(nullptr == m.matches(a));
   }

   Case(memcmp shorts)
   {
      short e[] = {1, 2, 3, 4, 5, 6, 7, 8};

      h2::h2_matchee_memcmp m(32, e, 8 * 2 * 8);
      short a1[] = {1, 2, 3, 4, 5, 6, 7, 8};
      short a2[] = {1, 2, 3, 44, 5, 6, 7, 8};
      OK(nullptr == m.matches(a1));
      OK(nullptr != m.matches(a2));
   }

   Case(memcmp ints)
   {
      int e[] = {1, 2, 3, 4, 5, 6, 7, 8};

      h2::h2_matchee_memcmp m(32, e, 8 * 4 * 8);
      int a1[] = {1, 2, 3, 4, 5, 6, 7, 8};
      int a2[] = {1, 2, 3, 44, 5, 6, 7, 8};
      OK(nullptr == m.matches(a1));
      OK(nullptr != m.matches(a2));
   }

   Case(memcmp uint64)
   {
      long long e[] = {1, 2, 3, 4, 5, 6, 7, 8};

      h2::h2_matchee_memcmp m(32, e, 8 * 8 * 8);
      long long a1[] = {1, 2, 3, 4, 5, 6, 7, 8};
      long long a2[] = {1, 2, 3, 44, 5, 6, 7, 8};
      OK(nullptr == m.matches(a1));
      OK(nullptr != m.matches(a2));
   }

   Case(Regex)
   {
      h2::h2_matchee_regex a("A.*");
      OK(nullptr == a.matches("ABC"));
      OK(nullptr != a.matches("BBC"));
   }

   Case(Wildcard)
   {
      h2::h2_matchee_wildcard a("A*");
      OK(nullptr == a.matches("ABC"));
      OK(nullptr != a.matches("BBC"));
   }

   Case(Substr)
   {
      h2::h2_matchee_substr a("A");
      OK(nullptr == a.matches("ABC"));
      OK(nullptr != a.matches("BBC"));
   }

   Case(StartsWith)
   {
      h2::h2_matchee_startswith a("A");
      OK(nullptr == a.matches("ABC"));
      OK(nullptr != a.matches("BBC"));
   }

   Case(EndsWith)
   {
      h2::h2_matchee_endswith a("A");
      OK(nullptr == a.matches("CBA"));
      OK(nullptr != a.matches("ABC"));
   }

   Case(Json)
   {
      h2::h2_matchee_json a("[65]");
      OK(nullptr == a.matches("[65]"));
      OK(nullptr != a.matches("[66]"));
   }
}
