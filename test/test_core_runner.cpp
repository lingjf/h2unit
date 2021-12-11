#include "../source/h2_unit.cpp"

struct compare_host : h2::h2_test {
   compare_host(const char* name_) : h2_test("", "", 0, "") { name = name_; }
   compare_host(int seq_) : h2_test("", "", 0, "") { seq = seq_; }
   compare_host(const char* file_, int line_) : h2_test("", "", 0, "") { file = file_, line = line_; }
};

SUITE(shuffle_comparison)
{
   int ret;

   Case(same name)
   {
      compare_host a("abc");
      compare_host b("abc");
      ret = h2::shuffle_comparison::name(&a.x, &b.x);
      OK(ret == 0);
   }

   Case(name abc-bcd)
   {
      OK(strcmp("abc", "bcd") < 0);

      compare_host a("abc");
      compare_host b("bcd");
      ret = h2::shuffle_comparison::name(&a.x, &b.x);
      OK(ret < 0);
   }

   Case(name bcd-abc)
   {
      OK(strcmp("bcd", "abc") > 0);

      compare_host a("bcd");
      compare_host b("abc");
      ret = h2::shuffle_comparison::name(&a.x, &b.x);
      OK(ret > 0);
   }

   Case(same seq)
   {
      compare_host a(123);
      compare_host b(123);
      ret = h2::shuffle_comparison::seq(&a.x, &b.x);
      OK(ret == 0);
   }

   Case(seq 123-234)
   {
      compare_host a(123);
      compare_host b(234);
      ret = h2::shuffle_comparison::seq(&a.x, &b.x);
      OK(ret < 0);
   }

   Case(seq 234-123)
   {
      compare_host a(234);
      compare_host b(123);
      ret = h2::shuffle_comparison::seq(&a.x, &b.x);
      OK(ret > 0);
   }

   Case(same file line)
   {
      compare_host a("abc.cpp", 123);
      compare_host b("abc.cpp", 123);
      OK(h2::shuffle_comparison::file(&a.x, &b.x) == 0);
   }

   Case(file line abc.cpp:123-abc.cpp:234)
   {
      compare_host a("abc.cpp", 123);
      compare_host b("abc.cpp", 234);
      OK(h2::shuffle_comparison::file(&a.x, &b.x) < 0);
   }

   Case(file line abc.cpp:123-bcd.cpp:123)
   {
      compare_host a("abc.cpp", 123);
      compare_host b("bcd.cpp", 123);
      OK(h2::shuffle_comparison::file(&a.x, &b.x) < 0);
   }

   Case(file line abc.cpp:234-abc.cpp:123)
   {
      compare_host a("abc.cpp", 234);
      compare_host b("abc.cpp", 123);
      OK(h2::shuffle_comparison::file(&a.x, &b.x) > 0);
   }

   Case(file line bcd.cpp:123-abc.cpp:123)
   {
      compare_host a("bcd.cpp", 123);
      compare_host b("abc.cpp", 123);
      OK(h2::shuffle_comparison::file(&a.x, &b.x) > 0);
   }
}

SUITE(runner)
{
   Case()
   {
   }
}
