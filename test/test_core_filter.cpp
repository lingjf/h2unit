#include "../source/h2_unit.cpp"

SUITE(match_names [yes])
{
   bool ret;

   Case([yes fine, nice] 0 pattern)
   {
      std::vector<const char*> patterns = {};
      ret = h2::match_names(patterns, "httpd_connection.cpp:123");
      OK(!ret);
      ret = h2::match_names(patterns, "tcp_connection.cpp:123");
      OK(!ret);
      ret = h2::match_names(patterns, "connection");
      OK(!ret);
      ret = h2::match_names(patterns, "");
      OK(!ret);
   }

   Case(1 pattern [yes fine, nice])
   {
      std::vector<const char*> patterns = {"http"};
      ret = h2::match_names(patterns, "httpd_connection.cpp:123");
      OK(ret);
      ret = h2::match_names(patterns, "tcp_connection.cpp:123");
      OK(!ret);
      ret = h2::match_names(patterns, "connection");
      OK(!ret);
      ret = h2::match_names(patterns, "");
      OK(!ret);
   }

   Case(2 [yes fine, nice] pattern)
   {
      std::vector<const char*> patterns = {"http", "tcp"};
      ret = h2::match_names(patterns, "httpd_connection.cpp:123");
      OK(ret);
      ret = h2::match_names(patterns, "tcp_connection.cpp:123");
      OK(ret);
      ret = h2::match_names(patterns, "connection");
      OK(!ret);
      ret = h2::match_names(patterns, "");
      OK(!ret);
   }

   Case(2 wildcard)
   {
      std::vector<const char*> patterns = {"http", "tcp*"};
      ret = h2::match_names(patterns, "httpd_connection.cpp:123");
      OK(ret);
      ret = h2::match_names(patterns, "tcp_connection.cpp:123");
      OK(ret);
      ret = h2::match_names(patterns, "connection");
      OK(!ret);
      ret = h2::match_names(patterns, "");
      OK(!ret);
   }
}
