#include "../source/h2_unit.cpp"

namespace {

struct Foo {
   int int_value = 42;
   char char_value[100];
   Foo()
   {
      strcpy(char_value, "42");
   }
};

static void void_foobar(int a, char* b)
{
   sprintf(b, "%d", a);
}

static const int int_foobar(int a, const char* b)
{
   return a + strlen(b);
}

static const Foo foo_foobar(int a, std::string& d)
{
   Foo foo;
   foo.int_value = a;
   strcpy(foo.char_value, d.c_str());
   return foo;
}

static const Foo& ref_foobar(int a, Foo& foo)
{
   foo.int_value = a;
   sprintf(foo.char_value, "%d", a);
   return foo;
}

struct Bar {
   void void_func(int a, char* b)
   {
      sprintf(b, "%d", a);
   }

   int int_func(int a, const char* b)
   {
      return a + strlen(b);
   }

   Foo foo_func(int a, std::string& d)
   {
      Foo foo;
      foo.int_value = a;
      strcpy(foo.char_value, d.c_str());
      return foo;
   }

   Foo& ref_func(int a, Foo& foo)
   {
      foo.int_value = a;
      sprintf(foo.char_value, "%d", a);
      return foo;
   }
};

static void void_bar_foobar(Bar*, int a, char* b)
{
   sprintf(b, "%d", a);
}

static int int_bar_foobar(Bar*, int a, const char* b)
{
   return a + strlen(b);
}

static Foo foo_bar_foobar(Bar*, int a, std::string& d)
{
   Foo foo;
   foo.int_value = a;
   strcpy(foo.char_value, d.c_str());
   return foo;
}

static Foo& ref_bar_foobar(Bar*, int a, Foo& foo)
{
   foo.int_value = a;
   sprintf(foo.char_value, "%d", a);
   return foo;
}

SUITE(function return void)
{
   char b[1024];
   std::string d = "hello";
   Foo foo;
   Bar bar;

   Case(return value)
   {
      h2::h2_function<std::false_type, int(int, const char*)> f3(1234);
      OK(1234, f3(nullptr, 1, ""));

      h2::h2_function<Bar, int(int, const char*)> f4(1234);
      OK(1234, f4(&bar, 1, ""));

      h2::h2_function<std::false_type, const Foo(int, std::string&)> f5(foo);
      auto a5 = f5(nullptr, 1, d);
      OK(42, a5.int_value);
      OK("42", a5.char_value);

      h2::h2_function<Bar, const Foo(int, std::string&)> f6(foo);
      auto a6 = f6(&bar, 1, d);
      OK(42, a6.int_value);
      OK("42", a6.char_value);

      h2::h2_function<std::false_type, const Foo&(int, Foo&)> f7(foo);
      auto a7 = f7(nullptr, 1, foo);
      OK(42, a7.int_value);
      OK("42", a7.char_value);

      h2::h2_function<Bar, Foo&(int, Foo&)> f8(foo);
      auto a8 = f8(&bar, 1, foo);
      OK(42, a8.int_value);
      OK("42", a8.char_value);
   }

   Case(normal function)
   {
      h2::h2_function<std::false_type, void(int, char*)> f1(void_foobar);
      f1(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<Bar, void(int, char*)> f2(void_bar_foobar);
      f2(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<std::false_type, const int(int, const char*)> f3(int_foobar);
      OK(2, f3(nullptr, 1, "2"));

      h2::h2_function<Bar, int(int, const char*)> f4(int_bar_foobar);
      OK(2, f4(&bar, 1, "2"));

      h2::h2_function<std::false_type, const Foo(int, std::string&)> f5(foo_foobar);
      auto a5 = f5(nullptr, 1, d);
      OK(1, a5.int_value);
      OK("hello", a5.char_value);

      h2::h2_function<Bar, Foo(int, std::string&)> f6(foo_bar_foobar);
      auto a6 = f6(&bar, 1, d);
      OK(1, a6.int_value);
      OK("hello", a6.char_value);

      h2::h2_function<std::false_type, const Foo&(int, Foo&)> f7(ref_foobar);
      auto a7 = f7(nullptr, 1, foo);
      OK(1, a7.int_value);
      OK("1", a7.char_value);

      h2::h2_function<Bar, Foo&(int, Foo&)> f8(ref_bar_foobar);
      auto a8 = f8(&bar, 1, foo);
      OK(1, a8.int_value);
      OK("1", a8.char_value);
   }

   Case(lambda function)
   {
      h2::h2_function<std::false_type, void(int, char*)> f1([](int a, char* b) {
         sprintf(b, "%d", a);
      });
      f1(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<Bar, void(int, char*)> f2([](int a, char* b) {
         sprintf(b, "%d", a);
      });
      f2(&bar, 1, b);
      OK("1", b);

      h2::h2_function<std::false_type, int(int, const char*)> f3([](int a, const char* b) {
         return a + strlen(b);
      });
      OK(2, f3(nullptr, 1, "2"));

      h2::h2_function<Bar, int(int, const char*)> f4([](int a, const char* b) {
         return a + strlen(b);
      });
      OK(2, f4(&bar, 1, "2"));

      h2::h2_function<std::false_type, const Foo(int, std::string&)> f5([](int a, std::string& d) {
         Foo foo;
         foo.int_value = a;
         strcpy(foo.char_value, d.c_str());
         return foo;
      });
      auto a5 = f5(nullptr, 1, d);
      OK(1, a5.int_value);
      OK("hello", a5.char_value);

      h2::h2_function<Bar, const Foo(int, std::string&)> f6([](int a, std::string& d) {
         Foo foo;
         foo.int_value = a;
         strcpy(foo.char_value, d.c_str());
         return foo;
      });
      auto a6 = f6(&bar, 1, d);
      OK(1, a6.int_value);
      OK("hello", a6.char_value);
   }

   Case(normal origin function)
   {
      h2::h2_function<std::false_type, void(int, char*)> f1(void_foobar);
      f1(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<Bar, void(int, char*)> f2(void_bar_foobar);
      f2(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<std::false_type, const int(int, const char*)> f3(int_foobar);
      OK(2, f3(nullptr, 1, "2"));

      h2::h2_function<Bar, int(int, const char*)> f4(int_bar_foobar);
      OK(2, f4(&bar, 1, "2"));

      h2::h2_function<std::false_type, const Foo(int, std::string&)> f5(foo_foobar);
      auto a5 = f5(nullptr, 1, d);
      OK(1, a5.int_value);
      OK("hello", a5.char_value);

      h2::h2_function<Bar, Foo(int, std::string&)> f6(foo_bar_foobar);
      auto a6 = f6(&bar, 1, d);
      OK(1, a6.int_value);
      OK("hello", a6.char_value);
   }

   Case(normal origin member function)
   {
      void* void_func = h2::h2_mfp<Bar, void(int, char*)>::A(&Bar::void_func);

      void* int_func = h2::h2_mfp<Bar, int(int, const char*)>::A(&Bar::int_func);

      void* foo_func = h2::h2_mfp<Bar, Foo(int, std::string&)>::A(&Bar::foo_func);

      void* ref_func = h2::h2_mfp<Bar, Foo&(int, Foo&)>::A(&Bar::ref_func);

      h2::h2_function<Bar, void(int, char*)> f2((void (*)(Bar*, int, char*))void_func);
      f2(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<Bar, int(int, const char*)> f4((int (*)(Bar*, int, const char*))int_func);
      OK(2, f4(&bar, 1, "2"));

      h2::h2_function<Bar, const Foo(int, std::string&)> f6((const Foo (*)(Bar*, int, std::string&))foo_func);
      auto a6 = f6(&bar, 1, d);
      OK(1, a6.int_value);
      OK("hello", a6.char_value);

      h2::h2_function<Bar, Foo&(int, Foo&)> f8((Foo & (*)(Bar*, int, Foo&)) ref_func);
      auto a8 = f8(&bar, 1, foo);
      OK(1, a8.int_value);
      OK("1", a8.char_value);
   }
}
}  // namespace
