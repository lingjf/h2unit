#if defined __GNUC__ || defined __clang__
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "../source/h2_unit.cpp"

static const char* empty_set[128] = {nullptr};
static const char* empty_includes[128] = {nullptr};
static const char* empty_excludes[128] = {nullptr};
static const char* array_http[] = {"http", nullptr};
static const char* array_http_tcp[] = {"http", "tcp", nullptr};
static const char* array_httpd[] = {"httpd", nullptr};
static const char* array_hello[] = {"hello", nullptr};
static const char* array_world[] = {"world", nullptr};
static const char* array_hello_world[] = {"hello", "world", nullptr};
static const char* array_asuite[] = {"asuite", nullptr};
static const char* array_acase[] = {"acase", nullptr};
static const char* array_cpp123[] = {"cpp:123", nullptr};

SUITE(match_names [yes])
{
   Case([yes fine, nice] 0 pattern)
   {
      OK(!h2::match_names(empty_set, "httpd.cpp:123"));
      OK(!h2::match_names(empty_set, "tcp.cpp:123"));
      OK(!h2::match_names(empty_set, "connection"));
      OK(!h2::match_names(empty_set, ""));
   }

   Case(1 pattern [yes fine, nice])
   {
      OK(h2::match_names(array_http, "httpd.cpp:123"));
      OK(!h2::match_names(array_http, "tcp.cpp:123"));
      OK(!h2::match_names(array_http, "connection"));
      OK(!h2::match_names(array_http, ""));
   }

   Case(2 [yes fine, nice] pattern)
   {
      OK(h2::match_names(array_http_tcp, "httpd.cpp:123"));
      OK(h2::match_names(array_http_tcp, "tcp.cpp:123"));
      OK(!h2::match_names(array_http_tcp, "connection"));
      OK(!h2::match_names(array_http_tcp, ""));
   }

   Case(2 wildcard)
   {
      static const char* array_http_TCP[] = {"http", "tcp*", nullptr};

      OK(h2::match_names(array_http_TCP, "httpd.cpp:123"));
      OK(h2::match_names(array_http_TCP, "tcp.cpp:123"));
      OK(!h2::match_names(array_http_TCP, "connection"));
      OK(!h2::match_names(array_http_TCP, ""));
   }
}

SUITE(match_tags [yes])
{
   Case(0 patterns)
   {
      h2::h2_test test("httpd.cpp:123", "httpd.cpp", 123, "this is a test [hello world]");
      OK(!h2::match_tags(empty_set, &test));
   }

   Case(1 patterns)
   {
      h2::h2_test test1("httpd.cpp:123", "httpd.cpp", 123, "this is a test [hello world]");
      OK(h2::match_tags(array_hello, &test1));

      h2::h2_test test2("httpd.cpp:123", "httpd.cpp", 123, "this is a test [world]");
      OK(!h2::match_tags(array_hello, &test2));

      h2::h2_test test3("httpd.cpp:123", "httpd.cpp", 123, "this is a test []");
      OK(!h2::match_tags(array_hello, &test3));

      h2::h2_test test4("httpd.cpp:123", "httpd.cpp", 123, "this is a test");
      OK(!h2::match_tags(array_hello, &test4));
   }

   Case(2 patterns)
   {
      h2::h2_test test1("httpd.cpp:123", "httpd.cpp", 123, "this is a test [hello world]");
      OK(h2::match_tags(array_hello_world, &test1));

      h2::h2_test test2("httpd.cpp:123", "httpd.cpp", 123, "this is a test [world]");
      OK(h2::match_tags(array_hello_world, &test2));

      h2::h2_test test3("httpd.cpp:123", "httpd.cpp", 123, "this is a test []");
      OK(!h2::match_tags(array_hello_world, &test3));

      h2::h2_test test4("httpd.cpp:123", "httpd.cpp", 123, "this is a test");
      OK(!h2::match_tags(array_hello_world, &test4));
   }

   Case(2 patterns wildcard)
   {
      static const char* array_hello_world_wildcard[] = {"he?lo", "wo*d", nullptr};

      h2::h2_test test1("httpd.cpp:123", "httpd.cpp", 123, "this is a test [hello world]");
      OK(h2::match_tags(array_hello_world_wildcard, &test1));

      h2::h2_test test2("httpd.cpp:123", "httpd.cpp", 123, "this is a test [world]");
      OK(h2::match_tags(array_hello_world_wildcard, &test2));

      h2::h2_test test3("httpd.cpp:123", "httpd.cpp", 123, "this is a test []");
      OK(!h2::match_tags(array_hello_world_wildcard, &test3));

      h2::h2_test test4("httpd.cpp:123", "httpd.cpp", 123, "this is a test");
      OK(!h2::match_tags(array_hello_world_wildcard, &test4));
   }
}

SUITE(filter suite [yes])
{
   h2::h2_test s1("httpd.cpp:123", "httpd.cpp", 123, "this is asuite [hello world]");

   static const char* array_not[] = {"not", nullptr};

   Case(all empty)
   {
      OK(!h2::__filter(&s1, nullptr, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
   }

   Case(all by file)
   {
      OK(!h2::__filter(&s1, nullptr, array_httpd, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
      OK(h2::__filter(&s1, nullptr, empty_set, array_httpd, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));

      OK(h2::__filter(&s1, nullptr, array_not, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
      OK(!h2::__filter(&s1, nullptr, empty_set, array_not, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
   }

   Case(all by file line)
   {
      OK(!h2::__filter(&s1, nullptr, array_cpp123, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
      OK(h2::__filter(&s1, nullptr, empty_set, array_cpp123, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
   }

   Case(all by name)
   {
      OK(!h2::__filter(&s1, nullptr, array_asuite, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
      OK(h2::__filter(&s1, nullptr, empty_set, array_asuite, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
   }

   Case(all by tag)
   {
      OK(!h2::__filter(&s1, nullptr, array_hello, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
      OK(h2::__filter(&s1, nullptr, empty_set, array_hello, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
   }

   Case(all by wildcard)
   {
      const char* array_HTTP_tcp[] = {"http*", "tcp", nullptr};
      const char* array_SUITE_tcp[] = {"*suite*", "tcp", nullptr};
      const char* array_HELLO_world[] = {"he?lo", "world", nullptr};

      OK(!h2::__filter(&s1, nullptr, array_HTTP_tcp, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
      OK(h2::__filter(&s1, nullptr, empty_set, array_HTTP_tcp, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));

      OK(!h2::__filter(&s1, nullptr, array_SUITE_tcp, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
      OK(h2::__filter(&s1, nullptr, empty_set, array_SUITE_tcp, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));

      OK(!h2::__filter(&s1, nullptr, array_HELLO_world, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
      OK(h2::__filter(&s1, nullptr, empty_set, array_HELLO_world, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set, empty_set));
   }

   Case(by file)
   {
      OK(!h2::__filter(&s1, nullptr,
                       empty_set, empty_set,    // all
                       array_httpd, empty_set,  // file
                       empty_set, empty_set,    // suite
                       empty_set, empty_set,    // case
                       empty_set, empty_set     // tags
                       ));

      OK(h2::__filter(&s1, nullptr,
                      empty_set, empty_set,    // all
                      empty_set, array_httpd,  // file
                      empty_set, empty_set,    // suite
                      empty_set, empty_set,    // case
                      empty_set, empty_set     // tags
                      ));

      OK(h2::__filter(&s1, nullptr,
                      empty_set, empty_set,  // all
                      array_not, empty_set,  // file
                      empty_set, empty_set,  // suite
                      empty_set, empty_set,  // case
                      empty_set, empty_set   // tags
                      ));

      OK(!h2::__filter(&s1, nullptr,
                       empty_set, empty_set,  // all
                       empty_set, array_not,  // file
                       empty_set, empty_set,  // suite
                       empty_set, empty_set,  // case
                       empty_set, empty_set   // tags
                       ));
   }

   Case(by suite)
   {
      OK(!h2::__filter(&s1, nullptr,
                       empty_set, empty_set,     // all
                       empty_set, empty_set,     // file
                       array_asuite, empty_set,  // suite
                       empty_set, empty_set,     // case
                       empty_set, empty_set      // tags
                       ));

      OK(h2::__filter(&s1, nullptr,
                      empty_set, empty_set,     // all
                      empty_set, empty_set,     // file
                      empty_set, array_asuite,  // suite
                      empty_set, empty_set,     // case
                      empty_set, empty_set      // tags
                      ));

      OK(h2::__filter(&s1, nullptr,
                      empty_set, empty_set,  // all
                      empty_set, empty_set,  // file
                      array_not, empty_set,  // suite
                      empty_set, empty_set,  // case
                      empty_set, empty_set   // tags
                      ));

      OK(!h2::__filter(&s1, nullptr,
                       empty_set, empty_set,  // all
                       empty_set, empty_set,  // file
                       empty_set, array_not,  // suite
                       empty_set, empty_set,  // case
                       empty_set, empty_set   // tags
                       ));
   }

   Case(by tags)
   {
      OK(!h2::__filter(&s1, nullptr,
                       empty_set, empty_set,   // all
                       empty_set, empty_set,   // file
                       empty_set, empty_set,   // suite
                       empty_set, empty_set,   // case
                       array_hello, empty_set  // tags
                       ));

      OK(h2::__filter(&s1, nullptr,
                      empty_set, empty_set,   // all
                      empty_set, empty_set,   // file
                      empty_set, empty_set,   // suite
                      empty_set, empty_set,   // case
                      empty_set, array_hello  // tags
                      ));

      OK(h2::__filter(&s1, nullptr,
                      empty_set, empty_set,  // all
                      empty_set, empty_set,  // file
                      empty_set, empty_set,  // suite
                      empty_set, empty_set,  // case
                      array_not, empty_set   // tags
                      ));

      OK(!h2::__filter(&s1, nullptr,
                       empty_set, empty_set,  // all
                       empty_set, empty_set,  // file
                       empty_set, empty_set,  // suite
                       empty_set, empty_set,  // case
                       empty_set, array_not   // tags
                       ));
   }
}

SUITE(filter case [yes])
{
   h2::h2_test s1("httpd.cpp:123", "httpd.cpp", 123, "this is asuite [hello server]");
   h2::h2_test c1("httpd.cpp:456", "httpd.cpp", 456, "this is acase [hello listen]");

   Case(all empty)
   {
      OK(!h2::__filter(&s1, &c1,
                       empty_set, empty_set,  // all
                       empty_set, empty_set,  // file
                       empty_set, empty_set,  // suite
                       empty_set, empty_set,  // case
                       empty_set, empty_set   // tags
                       ));
   }

   Case(all by file)
   {
      OK(!h2::__filter(&s1, &c1,
                       array_httpd, empty_set,  // all
                       empty_set, empty_set,    // file
                       empty_set, empty_set,    // suite
                       empty_set, empty_set,    // case
                       empty_set, empty_set     // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, array_httpd,  // all
                      empty_set, empty_set,    // file
                      empty_set, empty_set,    // suite
                      empty_set, empty_set,    // case
                      empty_set, empty_set     // tags
                      ));
   }

   Case(all by suite file line)
   {
      OK(!h2::__filter(&s1, &c1,
                       array_cpp123, empty_set,  // all
                       empty_set, empty_set,     // file
                       empty_set, empty_set,     // suite
                       empty_set, empty_set,     // case
                       empty_set, empty_set      // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, array_cpp123,  // all
                      empty_set, empty_set,     // file
                      empty_set, empty_set,     // suite
                      empty_set, empty_set,     // case
                      empty_set, empty_set      // tags
                      ));
   }

   Case(all by case file line)
   {
      const char* array_cpp456[] = {"cpp:456", nullptr};

      OK(!h2::__filter(&s1, &c1,
                       array_cpp456, empty_set,  // all
                       empty_set, empty_set,     // file
                       empty_set, empty_set,     // suite
                       empty_set, empty_set,     // case
                       empty_set, empty_set      // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, array_cpp456,  // all
                      empty_set, empty_set,     // file
                      empty_set, empty_set,     // suite
                      empty_set, empty_set,     // case
                      empty_set, empty_set      // tags
                      ));
   }

   Case(all by suite name)
   {
      OK(!h2::__filter(&s1, &c1,
                       array_asuite, empty_set,  // all
                       empty_set, empty_set,     // file
                       empty_set, empty_set,     // suite
                       empty_set, empty_set,     // case
                       empty_set, empty_set      // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, array_asuite,  // all
                      empty_set, empty_set,     // file
                      empty_set, empty_set,     // suite
                      empty_set, empty_set,     // case
                      empty_set, empty_set      // tags
                      ));
   }

   Case(all by case name)
   {
      OK(!h2::__filter(&s1, &c1,
                       array_acase, empty_set,  // all
                       empty_set, empty_set,    // file
                       empty_set, empty_set,    // suite
                       empty_set, empty_set,    // case
                       empty_set, empty_set     // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, array_acase,  // all
                      empty_set, empty_set,    // file
                      empty_set, empty_set,    // suite
                      empty_set, empty_set,    // case
                      empty_set, empty_set     // tags
                      ));
   }

   Case(all by suite tag)
   {
      const char* array_server[] = {"server", nullptr};

      OK(!h2::__filter(&s1, &c1,
                       array_server, empty_set,  // all
                       empty_set, empty_set,     // file
                       empty_set, empty_set,     // suite
                       empty_set, empty_set,     // case
                       empty_set, empty_set      // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, array_server,  // all
                      empty_set, empty_set,     // file
                      empty_set, empty_set,     // suite
                      empty_set, empty_set,     // case
                      empty_set, empty_set      // tags
                      ));
   }

   Case(all by case tag)
   {
      const char* array_listen[] = {"listen", nullptr};

      OK(!h2::__filter(&s1, &c1,
                       array_listen, empty_set,  // all
                       empty_set, empty_set,     // file
                       empty_set, empty_set,     // suite
                       empty_set, empty_set,     // case
                       empty_set, empty_set      // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, array_listen,  // all
                      empty_set, empty_set,     // file
                      empty_set, empty_set,     // suite
                      empty_set, empty_set,     // case
                      empty_set, empty_set      // tags
                      ));
   }

   Case(by case file line)
   {
      const char* array_cpp456[] = {"cpp:456", nullptr};

      OK(!h2::__filter(&s1, &c1,
                       empty_set, empty_set,     // all
                       array_cpp456, empty_set,  // file
                       empty_set, empty_set,     // suite
                       empty_set, empty_set,     // case
                       empty_set, empty_set      // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, empty_set,     // all
                      empty_set, array_cpp456,  // file
                      empty_set, empty_set,     // suite
                      empty_set, empty_set,     // case
                      empty_set, empty_set      // tags
                      ));
   }

   Case(by suite name)
   {
      OK(!h2::__filter(&s1, &c1,
                       empty_set, empty_set,     // all
                       empty_set, empty_set,     // file
                       array_asuite, empty_set,  // suite
                       empty_set, empty_set,     // case
                       empty_set, empty_set      // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, empty_set,     // all
                      empty_set, empty_set,     // file
                      empty_set, array_asuite,  // suite
                      empty_set, empty_set,     // case
                      empty_set, empty_set      // tags
                      ));
   }

   Case(by case name)
   {
      OK(!h2::__filter(&s1, &c1,
                       empty_set, empty_set,    // all
                       empty_set, empty_set,    // file
                       empty_set, empty_set,    // suite
                       array_acase, empty_set,  // case
                       empty_set, empty_set     // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, empty_set,    // all
                      empty_set, empty_set,    // file
                      empty_set, empty_set,    // suite
                      empty_set, array_acase,  // case
                      empty_set, empty_set     // tags
                      ));
   }

   Case(by suite tag)
   {
      const char* array_server[] = {"server", nullptr};

      OK(!h2::__filter(&s1, &c1,
                       empty_set, empty_set,    // all
                       empty_set, empty_set,    // file
                       empty_set, empty_set,    // suite
                       empty_set, empty_set,    // case
                       array_server, empty_set  // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, empty_set,    // all
                      empty_set, empty_set,    // file
                      empty_set, empty_set,    // suite
                      empty_set, empty_set,    // case
                      empty_set, array_server  // tags
                      ));
   }

   Case(by case tag)
   {
      const char* array_listen[] = {"listen", nullptr};

      OK(!h2::__filter(&s1, &c1,
                       empty_set, empty_set,    // all
                       empty_set, empty_set,    // file
                       empty_set, empty_set,    // suite
                       empty_set, empty_set,    // case
                       array_listen, empty_set  // tags
                       ));

      OK(h2::__filter(&s1, &c1,
                      empty_set, empty_set,    // all
                      empty_set, empty_set,    // file
                      empty_set, empty_set,    // suite
                      empty_set, empty_set,    // case
                      empty_set, array_listen  // tags
                      ));
   }
}
