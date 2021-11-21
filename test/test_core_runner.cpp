#include "../source/h2_unit.cpp"

struct compare_host {
   h2::h2_list x;
   const char* file;
   int line;
   const char* name = "";
   int seq = 0;

   compare_host(const char* name_) : name(name_) {}
   compare_host(int seq_) : seq(seq_) {}
   compare_host(const char* file_, int line_) : file(file_), line(line_) {}
};

SUITE(shuffle_comparison)
{
   int ret;

   Case(same name)
   {
      compare_host a("abc");
      compare_host b("abc");
      ret = h2::shuffle_comparison<compare_host>::name(&a.x, &b.x);
      CP(ret == 0);
   }

   Case(name abc-bcd)
   {
      CP(strcmp("abc", "bcd") < 0);

      compare_host a("abc");
      compare_host b("bcd");
      ret = h2::shuffle_comparison<compare_host>::name(&a.x, &b.x);
      CP(ret < 0);
   }

   Case(name bcd-abc)
   {
      CP(strcmp("bcd", "abc") > 0);

      compare_host a("bcd");
      compare_host b("abc");
      ret = h2::shuffle_comparison<compare_host>::name(&a.x, &b.x);
      CP(ret > 0);
   }

   Case(same seq)
   {
      compare_host a(123);
      compare_host b(123);
      ret = h2::shuffle_comparison<compare_host>::seq(&a.x, &b.x);
      CP(ret == 0);
   }

   Case(seq 123-234)
   {
      compare_host a(123);
      compare_host b(234);
      ret = h2::shuffle_comparison<compare_host>::seq(&a.x, &b.x);
      CP(ret < 0);
   }

   Case(seq 234-123)
   {
      compare_host a(234);
      compare_host b(123);
      ret = h2::shuffle_comparison<compare_host>::seq(&a.x, &b.x);
      CP(ret > 0);
   }

   Case(same file line)
   {
      compare_host a("abc.cpp", 123);
      compare_host b("abc.cpp", 123);
      CP(h2::shuffle_comparison<compare_host>::file(&a.x, &b.x) == 0);
   }

   Case(file line abc.cpp:123-abc.cpp:234)
   {
      compare_host a("abc.cpp", 123);
      compare_host b("abc.cpp", 234);
      CP(h2::shuffle_comparison<compare_host>::file(&a.x, &b.x) < 0);
   }

   Case(file line abc.cpp:123-bcd.cpp:123)
   {
      compare_host a("abc.cpp", 123);
      compare_host b("bcd.cpp", 123);
      CP(h2::shuffle_comparison<compare_host>::file(&a.x, &b.x) < 0);
   }

   Case(file line abc.cpp:234-abc.cpp:123)
   {
      compare_host a("abc.cpp", 234);
      compare_host b("abc.cpp", 123);
      CP(h2::shuffle_comparison<compare_host>::file(&a.x, &b.x) > 0);
   }

   Case(file line bcd.cpp:123-abc.cpp:123)
   {
      compare_host a("bcd.cpp", 123);
      compare_host b("abc.cpp", 123);
      CP(h2::shuffle_comparison<compare_host>::file(&a.x, &b.x) > 0);
   }
}

SUITE(runner)
{
   Case(C++ array elements with missing values will be initialized to 0)
   {
      int a[10] = {1, 2};  // initialize to 1,2,0,0,0...
      OK(1, a[0]);
      OK(2, a[1]);
      for (int i = 2; i < 10; i++)
         OK(0, a[i]);
   }

   Case(initialize all elements to 0)
   {
      int a[10] = {0};  // all elements 0
      for (int i = 0; i < 10; i++)
         OK(0, a[i]);
   }

   Case(C++ an empty initialization list will initialize every element to 0)
   {
      int a[10] = {};  // all elements 0 in C++, is not allowed with C
      for (int i = 0; i < 10; i++)
         OK(0, a[i]);
   }
}
