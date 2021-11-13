#include "../source/h2_unit.cpp"

struct compare_host {
   h2::h2_list x;
   h2::h2_describe describe;
   int seq = 0;

   compare_host(const char* describe_) : describe(describe_) {}
   compare_host(int seq_) : describe(""), seq(seq_) {}
};

SUITE(shuffle_comparison)
{
   Case(same name)
   {
      compare_host a("abc");
      compare_host b("abc");
      int ret1 = h2::shuffle_comparison<compare_host, 1>::name(&a.x, &b.x);
      CP(ret1 == 0);

      int ret2 = h2::shuffle_comparison<compare_host, -1>::name(&a.x, &b.x);
      CP(ret2 == 0);
   }

   Case(name abc-bcd)
   {
      CP(strcmp("abc", "bcd") < 0);

      compare_host a("abc");
      compare_host b("bcd");
      int ret1 = h2::shuffle_comparison<compare_host, 1>::name(&a.x, &b.x);
      CP(ret1 < 0);

      int ret2 = h2::shuffle_comparison<compare_host, -1>::name(&a.x, &b.x);
      CP(ret2 > 0);
   }

   Case(name bcd-abc)
   {
      CP(strcmp("bcd", "abc") > 0);

      compare_host a("bcd");
      compare_host b("abc");
      int ret1 = h2::shuffle_comparison<compare_host, 1>::name(&a.x, &b.x);
      CP(ret1 > 0);

      int ret2 = h2::shuffle_comparison<compare_host, -1>::name(&a.x, &b.x);
      CP(ret2 < 0);
   }

   Case(same seq)
   {
      compare_host a(123);
      compare_host b(123);
      int ret1 = h2::shuffle_comparison<compare_host, 1>::seq(&a.x, &b.x);
      CP(ret1 == 0);

      int ret2 = h2::shuffle_comparison<compare_host, -1>::seq(&a.x, &b.x);
      CP(ret2 == 0);
   }

   Case(seq 123-234)
   {
      compare_host a(123);
      compare_host b(234);
      int ret1 = h2::shuffle_comparison<compare_host, 1>::seq(&a.x, &b.x);
      CP(ret1 < 0);

      int ret2 = h2::shuffle_comparison<compare_host, -1>::seq(&a.x, &b.x);
      CP(ret2 > 0);
   }

   Case(seq 234-123)
   {
      compare_host a(234);
      compare_host b(123);
      int ret1 = h2::shuffle_comparison<compare_host, 1>::seq(&a.x, &b.x);
      CP(ret1 > 0);

      int ret2 = h2::shuffle_comparison<compare_host, -1>::seq(&a.x, &b.x);
      CP(ret2 < 0);
   }
}

SUITE(runner) {}
