#include "../source/h2_unit.cpp"

SUITE(match_names [yes])
{
   Case([yes fine, nice] 0 pattern)
   {
      OK(!h2::match_names({}, "httpd.cpp:123"));
      OK(!h2::match_names({}, "tcp.cpp:123"));
      OK(!h2::match_names({}, "connection"));
      OK(!h2::match_names({}, ""));
   }

   Case(1 pattern [yes fine, nice])
   {
      OK(h2::match_names({"http"}, "httpd.cpp:123"));
      OK(!h2::match_names({"http"}, "tcp.cpp:123"));
      OK(!h2::match_names({"http"}, "connection"));
      OK(!h2::match_names({"http"}, ""));
   }

   Case(2 [yes fine, nice] pattern)
   {
      OK(h2::match_names({"http", "tcp"}, "httpd.cpp:123"));
      OK(h2::match_names({"http", "tcp"}, "tcp.cpp:123"));
      OK(!h2::match_names({"http", "tcp"}, "connection"));
      OK(!h2::match_names({"http", "tcp"}, ""));
   }

   Case(2 wildcard)
   {
      OK(h2::match_names({"http", "tcp*"}, "httpd.cpp:123"));
      OK(h2::match_names({"http", "tcp*"}, "tcp.cpp:123"));
      OK(!h2::match_names({"http", "tcp*"}, "connection"));
      OK(!h2::match_names({"http", "tcp*"}, ""));
   }
}

SUITE(match_tags [yes])
{
   Case(0 patterns)
   {
      h2::h2_test test("httpd.cpp:123", "httpd.cpp", 123, "this is a test [hello world]");
      OK(!h2::match_tags({}, &test));
   }

   Case(1 patterns)
   {
      h2::h2_test test1("httpd.cpp:123", "httpd.cpp", 123, "this is a test [hello world]");
      OK(h2::match_tags({"hello"}, &test1));

      h2::h2_test test2("httpd.cpp:123", "httpd.cpp", 123, "this is a test [world]");
      OK(!h2::match_tags({"hello"}, &test2));

      h2::h2_test test3("httpd.cpp:123", "httpd.cpp", 123, "this is a test []");
      OK(!h2::match_tags({"hello"}, &test3));

      h2::h2_test test4("httpd.cpp:123", "httpd.cpp", 123, "this is a test");
      OK(!h2::match_tags({"hello"}, &test4));
   }

   Case(2 patterns)
   {
      h2::h2_test test1("httpd.cpp:123", "httpd.cpp", 123, "this is a test [hello world]");
      OK(h2::match_tags({"hello", "world"}, &test1));

      h2::h2_test test2("httpd.cpp:123", "httpd.cpp", 123, "this is a test [world]");
      OK(h2::match_tags({"hello", "world"}, &test2));

      h2::h2_test test3("httpd.cpp:123", "httpd.cpp", 123, "this is a test []");
      OK(!h2::match_tags({"hello", "world"}, &test3));

      h2::h2_test test4("httpd.cpp:123", "httpd.cpp", 123, "this is a test");
      OK(!h2::match_tags({"hello", "world"}, &test4));
   }

   Case(2 patterns wildcard)
   {
      h2::h2_test test1("httpd.cpp:123", "httpd.cpp", 123, "this is a test [hello world]");
      OK(h2::match_tags({"he?lo", "wo*d"}, &test1));

      h2::h2_test test2("httpd.cpp:123", "httpd.cpp", 123, "this is a test [world]");
      OK(h2::match_tags({"he?lo", "wo*d"}, &test2));

      h2::h2_test test3("httpd.cpp:123", "httpd.cpp", 123, "this is a test []");
      OK(!h2::match_tags({"he?lo", "wo*d"}, &test3));

      h2::h2_test test4("httpd.cpp:123", "httpd.cpp", 123, "this is a test");
      OK(!h2::match_tags({"he?lo", "wo*d"}, &test4));
   }
}

SUITE(filter suite [yes])
{
   h2::h2_test s1("httpd.cpp:123", "httpd.cpp", 123, "this is asuite [hello world]");

   Case(both empty)
   {
      OK(!h2::__filter(&s1, nullptr, {}, {}, false));
   }

   Case(both empty by tag)
   {
      OK(!h2::__filter(&s1, nullptr, {}, {}, true));
   }

   Case(included by file)
   {
      OK(!h2::__filter(&s1, nullptr, {"httpd"}, {"world"}, false));
   }

   Case(included by file line)
   {
      OK(!h2::__filter(&s1, nullptr, {"cpp:123"}, {"world"}, false));
   }

   Case(included by name)
   {
      OK(!h2::__filter(&s1, nullptr, {"asuite"}, {"world"}, false));
   }

   Case(included by tag)
   {
      OK(!h2::__filter(&s1, nullptr, {"hello"}, {}, true));
   }

   Case(not both)
   {
      OK(h2::__filter(&s1, nullptr, {"hello"}, {"world"}, false));
   }

   Case(excluded by file)
   {
      OK(h2::__filter(&s1, nullptr, {"hello"}, {"httpd"}, false));
   }

   Case(excluded by file line)
   {
      OK(h2::__filter(&s1, nullptr, {"hello"}, {"cpp:123"}, false));
   }

   Case(excluded by name)
   {
      OK(h2::__filter(&s1, nullptr, {"hello"}, {"asuite"}, false));
   }

   Case(excluded by tag)
   {
      OK(h2::__filter(&s1, nullptr, {}, {"hello"}, true));
   }

   Case(included by wildcard)
   {
      OK(!h2::__filter(&s1, nullptr, {"http*", "tcp"}, {"he?lo", "world"}, false));
      OK(!h2::__filter(&s1, nullptr, {"*suite*", "tcp"}, {"he?lo", "world"}, false));
      OK(!h2::__filter(&s1, nullptr, {"he?lo", "world"}, {"udp", "tcp"}, true));
   }
}

SUITE(filter case [yes])
{
   h2::h2_test s1("httpd.cpp:123", "httpd.cpp", 123, "this is asuite [hello server]");
   h2::h2_test c1("httpd.cpp:456", "httpd.cpp", 456, "this is acase [hello listen]");

   Case(both empty)
   {
      OK(!h2::__filter(&s1, &c1, {}, {}, false));
   }

   Case(both empty by tag)
   {
      OK(!h2::__filter(&s1, &c1, {}, {}, true));
   }

   Case(included by file)
   {
      OK(!h2::__filter(&s1, &c1, {"httpd"}, {"world"}, false));
   }

   Case(included by file line)
   {
      OK(h2::__filter(&s1, &c1, {"cpp:123"}, {"world"}, false));
   }

   Case(included by case file line)
   {
      OK(!h2::__filter(&s1, &c1, {"cpp:456"}, {"world"}, false));
   }

   Case(included by suite name)
   {
      OK(!h2::__filter(&s1, &c1, {"asuite"}, {"world"}, false));
   }

   Case(included by case name)
   {
      OK(!h2::__filter(&s1, &c1, {"acase"}, {"world"}, false));
   }

   Case(included by suite tag)
   {
      OK(!h2::__filter(&s1, &c1, {"server"}, {}, true));
   }

   Case(included by case tag)
   {
      OK(!h2::__filter(&s1, &c1, {"listen"}, {}, true));
   }

   Case(not both)
   {
      OK(h2::__filter(&s1, &c1, {"hello"}, {"world"}, false));
   }

   Case(excluded by case file)
   {
      OK(h2::__filter(&s1, &c1, {"hello"}, {"httpd"}, false));
   }

   Case(excluded by suite tag)
   {
      OK(h2::__filter(&s1, &c1, {"httpd"}, {"server"}, true));
   }

   Case(excluded by case tag)
   {
      OK(h2::__filter(&s1, &c1, {"httpd"}, {"listen"}, true));
   }
}

CASE(filter [yes])
{
   h2::h2_test s1("httpd.cpp:123", "httpd.cpp", 123, "this is asuite [hello server]");
   h2::h2_test c1("httpd.cpp:456", "httpd.cpp", 456, "this is acase [hello listen]");

   OK(!h2::__filter(&s1, &c1, {}, {}, false));
}
