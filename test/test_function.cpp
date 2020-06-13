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

static int int_foobar(int a, const char* b)
{
   return a + strlen(b);
}

static Foo foo_foobar(int a, std::string& d)
{
   Foo foo;
   foo.int_value = a;
   strcpy(foo.char_value, d.c_str());
   return foo;
}

static Foo& ref_foobar(int a, Foo& foo)
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

SUITE(function return void)
{
   char b[1024];
   std::string d = "hello";
   Foo foo;
   Bar bar;

   Case(empty)
   {
      h2::h2_function<std::false_type, void(int, char*)> f1;
      OK(f1.empty);

      h2::h2_function<Bar, void(int, char*)> f2;
      OK(f2.empty);

      h2::h2_function<std::false_type, int(int, const char*)> f3;
      OK(f3.empty);
      f3(nullptr, 1, "");

      h2::h2_function<Bar, int(int, const char*)> f4;
      OK(f4.empty);
      f4(&bar, 1, "");

      h2::h2_function<std::false_type, Foo(int, std::string&)> f5;
      OK(f5.empty);
      f5(nullptr, 1, d);

      h2::h2_function<Bar, Foo(int, std::string&)> f6;
      OK(f6.empty);
      f6(&bar, 1, d);

      // h2::h2_function<std::false_type, Foo&(int, Foo&)> f7;
      // OK(f7.empty);
      // f7(nullptr, 1, foo);
   }

   Case(return value)
   {
      h2::h2_function<std::false_type, int(int, const char*)> f3(1234);
      OK(!f3.empty);
      OK(1234, f3(nullptr, 1, ""));

      h2::h2_function<Bar, int(int, const char*)> f4(1234);
      OK(!f4.empty);
      OK(1234, f4(&bar, 1, ""));

      h2::h2_function<std::false_type, Foo(int, std::string&)> f5(foo);
      OK(!f5.empty);
      Foo a5 = f5(nullptr, 1, d);
      OK(42, a5.int_value);
      OK("42", a5.char_value);

      h2::h2_function<Bar, Foo(int, std::string&)> f6(foo);
      OK(!f6.empty);
      Foo a6 = f6(&bar, 1, d);
      OK(42, a6.int_value);
      OK("42", a6.char_value);
   }

   Case(normal function)
   {
      h2::h2_function<std::false_type, void(int, char*)> f1(void_foobar);
      OK(!f1.empty);
      f1(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<Bar, void(int, char*)> f2(void_bar_foobar);
      OK(!f2.empty);
      f2(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<std::false_type, int(int, const char*)> f3(int_foobar);
      OK(!f3.empty);
      OK(2, f3(nullptr, 1, "2"));

      h2::h2_function<Bar, int(int, const char*)> f4(int_bar_foobar);
      OK(!f4.empty);
      OK(2, f4(&bar, 1, "2"));

      h2::h2_function<std::false_type, Foo(int, std::string&)> f5(foo);
      OK(!f5.empty);
      Foo a5 = f5(nullptr, 1, d);
      OK(42, a5.int_value);
      OK("42", a5.char_value);

      h2::h2_function<Bar, Foo(int, std::string&)> f6(foo);
      OK(!f6.empty);
      Foo a6 = f6(&bar, 1, d);
      OK(42, a6.int_value);
      OK("42", a6.char_value);
   }

   Case(lambda function)
   {
      h2::h2_function<std::false_type, void(int, char*)> f1([](int a, char* b) {
         sprintf(b, "%d", a);
      });
      OK(!f1.empty);
      f1(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<Bar, void(int, char*)> f2([](int a, char* b) {
         sprintf(b, "%d", a);
      });
      OK(!f2.empty);
      f2(&bar, 1, b);
      OK("1", b);

      h2::h2_function<std::false_type, int(int, const char*)> f3([](int a, const char* b) {
         return a + strlen(b);
      });
      OK(!f3.empty);
      OK(2, f3(nullptr, 1, "2"));

      h2::h2_function<Bar, int(int, const char*)> f4([](int a, const char* b) {
         return a + strlen(b);
      });
      OK(!f4.empty);
      OK(2, f4(&bar, 1, "2"));

      h2::h2_function<std::false_type, Foo(int, std::string&)> f5([](int a, std::string& d) {
         Foo foo;
         foo.int_value = a;
         strcpy(foo.char_value, d.c_str());
         return foo;
      });
      OK(!f5.empty);
      Foo a5 = f5(nullptr, 1, d);
      OK(1, a5.int_value);
      OK("hello", a5.char_value);

      h2::h2_function<Bar, Foo(int, std::string&)> f6([](int a, std::string& d) {
         Foo foo;
         foo.int_value = a;
         strcpy(foo.char_value, d.c_str());
         return foo;
      });
      OK(!f6.empty);
      Foo a6 = f6(&bar, 1, d);
      OK(1, a6.int_value);
      OK("hello", a6.char_value);
   }

   Case(normal origin function)
   {
      h2::h2_function<std::false_type, void(int, char*)> f1;
      f1.origin_function = (void*)void_foobar;
      f1(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<Bar, void(int, char*)> f2;
      f2.origin_function = (void*)void_bar_foobar;
      f2(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<std::false_type, int(int, const char*)> f3;
      f3.origin_function = (void*)int_foobar;
      OK(2, f3(nullptr, 1, "2"));

      h2::h2_function<Bar, int(int, const char*)> f4;
      f4.origin_function = (void*)int_bar_foobar;
      OK(2, f4(&bar, 1, "2"));

      h2::h2_function<std::false_type, Foo(int, std::string&)> f5;
      f5.origin_function = (void*)foo_foobar;
      Foo a5 = f5(nullptr, 1, d);
      OK(1, a5.int_value);
      OK("hello", a5.char_value);

      h2::h2_function<Bar, Foo(int, std::string&)> f6;
      f6.origin_function = (void*)foo_bar_foobar;
      Foo a6 = f6(&bar, 1, d);
      OK(1, a6.int_value);
      OK("hello", a6.char_value);
   }

   Case(normal origin member function)
   {
      void* void_func = h2::h2_mfp<Bar, void(int, char*)>::A(&Bar::void_func,
                                                             "",
                                                             "",
                                                             "",
                                                             "",
                                                             "",
                                                             __FILE__,
                                                             __LINE__);

      void* int_func = h2::h2_mfp<Bar, int(int, const char*)>::A(&Bar::int_func,
                                                                 "",
                                                                 "",
                                                                 "",
                                                                 "",
                                                                 "",
                                                                 __FILE__,
                                                                 __LINE__);

      void* foo_func = h2::h2_mfp<Bar, Foo(int, std::string&)>::A(&Bar::foo_func,
                                                                  "",
                                                                  "",
                                                                  "",
                                                                  "",
                                                                  "",
                                                                  __FILE__,
                                                                  __LINE__);

      h2::h2_function<Bar, void(int, char*)> f2;
      f2.origin_function = (void*)void_func;
      f2(nullptr, 1, b);
      OK("1", b);

      h2::h2_function<Bar, int(int, const char*)> f4;
      f4.origin_function = (void*)int_func;
      OK(2, f4(&bar, 1, "2"));

      h2::h2_function<Bar, Foo(int, std::string&)> f6;
      f6.origin_function = (void*)foo_func;
      Foo a6 = f6(&bar, 1, d);
      OK(1, a6.int_value);
      OK("hello", a6.char_value);
   }
}
}  // namespace
