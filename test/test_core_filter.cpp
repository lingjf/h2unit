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

   Case(both empty)
   {
      OK(!h2::__filter(&s1, nullptr, empty_set, empty_set, false));
   }

   Case(both empty by tag)
   {
      OK(!h2::__filter(&s1, nullptr, empty_set, empty_set, true));
   }

   Case(included by file)
   {
      OK(!h2::__filter(&s1, nullptr, array_httpd, array_world, false));
   }

   Case(included by file line)
   {
      OK(!h2::__filter(&s1, nullptr, array_cpp123, array_world, false));
   }

   Case(included by name)
   {
      OK(!h2::__filter(&s1, nullptr, array_asuite, array_world, false));
   }

   Case(included by tag)
   {
      OK(!h2::__filter(&s1, nullptr, array_hello, empty_set, true));
   }

   Case(not both)
   {
      OK(h2::__filter(&s1, nullptr, array_hello, array_world, false));
   }

   Case(excluded by file)
   {
      OK(h2::__filter(&s1, nullptr, array_hello, array_httpd, false));
   }

   Case(excluded by file line)
   {
      OK(h2::__filter(&s1, nullptr, array_hello, array_cpp123, false));
   }

   Case(excluded by name)
   {
      OK(h2::__filter(&s1, nullptr, array_hello, array_asuite, false));
   }

   Case(excluded by tag)
   {
      OK(h2::__filter(&s1, nullptr, empty_set, array_hello, true));
   }

   Case(included by wildcard)
   {
      const char* array_HTTP_tcp[] = {"http*", "tcp", nullptr};
      const char* array_HELLO_world[] = {"he?lo", "world", nullptr};
      const char* array_SUITE_tcp[] = {"*suite*", "tcp", nullptr};
      const char* array_udp_tcp[] = {"udp", "tcp", nullptr};

      OK(!h2::__filter(&s1, nullptr, array_HTTP_tcp, array_HELLO_world, false));
      OK(!h2::__filter(&s1, nullptr, array_SUITE_tcp, array_HELLO_world, false));
      OK(!h2::__filter(&s1, nullptr, array_HELLO_world, array_udp_tcp, true));
   }
}

SUITE(filter case [yes])
{
   h2::h2_test s1("httpd.cpp:123", "httpd.cpp", 123, "this is asuite [hello server]");
   h2::h2_test c1("httpd.cpp:456", "httpd.cpp", 456, "this is acase [hello listen]");

   Case(both empty)
   {
      OK(!h2::__filter(&s1, &c1, empty_set, empty_set, false));
   }

   Case(both empty by tag)
   {
      OK(!h2::__filter(&s1, &c1, empty_set, empty_set, true));
   }

   Case(included by file)
   {
      OK(!h2::__filter(&s1, &c1, array_httpd, array_world, false));
   }

   Case(included by file line)
   {
      OK(h2::__filter(&s1, &c1, array_cpp123, array_world, false));
   }

   Case(included by case file line)
   {
      const char* array_cpp456[] = {"cpp:456", nullptr};

      OK(!h2::__filter(&s1, &c1, array_cpp456, array_world, false));
   }

   Case(included by suite name)
   {
      OK(!h2::__filter(&s1, &c1, array_asuite, array_world, false));
   }

   Case(included by case name)
   {
      OK(!h2::__filter(&s1, &c1, array_acase, array_world, false));
   }

   Case(included by suite tag)
   {
      const char* array_server[] = {"server", nullptr};
      OK(!h2::__filter(&s1, &c1, array_server, empty_set, true));
   }

   Case(included by case tag)
   {
      const char* array_listen[] = {"listen", nullptr};

      OK(!h2::__filter(&s1, &c1, array_listen, empty_set, true));
   }

   Case(not both)
   {
      OK(h2::__filter(&s1, &c1, array_hello, array_world, false));
   }

   Case(excluded by case file)
   {
      OK(h2::__filter(&s1, &c1, array_hello, array_httpd, false));
   }

   Case(excluded by suite tag)
   {
      const char* array_server[] = {"server", nullptr};

      OK(h2::__filter(&s1, &c1, array_httpd, array_server, true));
   }

   Case(excluded by case tag)
   {
      const char* array_listen[] = {"listen", nullptr};

      OK(h2::__filter(&s1, &c1, array_httpd, array_listen, true));
   }
}

CASE(filter [yes])
{
   h2::h2_test s1("httpd.cpp:123", "httpd.cpp", 123, "this is asuite [hello server]");
   h2::h2_test c1("httpd.cpp:456", "httpd.cpp", 456, "this is acase [hello listen]");

   OK(!h2::__filter(&s1, &c1, empty_includes, empty_excludes, false));
}
