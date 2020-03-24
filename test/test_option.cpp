#include "../source/h2_unit.hpp"
#include "../source/h2_unit.cpp"

SUITE(h2_option) 
{
   h2::h2_option c;
   
   Case(default)
   {
      const char* argv[] = {"./a.out"};
      c.parse(1, argv);
      OK(!c.verbose);
      OK(0, c.listing);
      OK(c.colorable);
      OK(!c.randomize);
      OK(c.memory_check);
      OK(0, c.breakable);
   };

   Case(verbose) 
   {
      const char* argv[] = {"./a.out", "-v"};
      c.parse(2, argv);
      OK(c.verbose);
   };

   Case(listing) 
   {
      const char* argv[] = {"./a.out", "-l"};
      c.parse(2, argv);
      OK('A', c.listing);
   };

   Case(listing suite) 
   {
      const char* argv[] = {"./a.out", "-l", "suite"};
      c.parse(3, argv);
      OK('s', c.listing);
   };

   Case(listing case) 
   {
      const char* argv[] = {"./a.out", "-l", "case"};
      c.parse(3, argv);
      OK('c', c.listing);
   };

   Case(colorable)
   {
      const char* argv[] = {"./a.out", "-c"};
      c.parse(2, argv);
      OK(!c.colorable);
   };

   Case(randomize memory_check breakable)
   {
      const char* argv[] = {"./a.out", "-rmb"};
      c.parse(2, argv);
      OK(c.randomize);
      OK(!c.memory_check);
      OK(1, c.breakable);
   };

   Case(breakable -b2)
   {
      const char* argv[] = {"./a.out", "-b2"};
      c.parse(2, argv);
      OK(2, c.breakable);
   };

   Case(breakable -b 2)
   {
      const char* argv[] = {"./a.out", "-b", "2"};
      c.parse(3, argv);
      OK(2, c.breakable);
   };

   Case(junit) 
   {
      const char* argv[] = {"./a.out", "-j", "jenkins.xml"};
      c.parse(3, argv);
      OK("jenkins.xml", c.junit);
   };

   Case(include substr)
   {
      const char* argv[] = {"./a.out", "-i", "http"};
      c.parse(3, argv);
      OK(!c.filter("server", "connection", "httpd_connection.cpp"));
      OK(!c.filter("server", "my_http_connection", "tcp_connection.cpp"));
      OK(!c.filter("http", "connection", "tcp_connection.cpp"));
   };

   Case(include substr 2)
   {
      const char* argv[] = {"./a.out", "-i", "http", "tcp*"};
      c.parse(4, argv);
      OK(!c.filter("server", "connection", "httpd_connection.cpp"));
      OK(!c.filter("server", "connection", "tcp_connection.cpp"));
      OK(!c.filter("http", "connection", "tcp_connection.cpp"));
   };

   Case(include exclude)
   {
      const char* argv[] = {"./a.out", "-x", "http", "-x", "tcp"};
      c.parse(5, argv);
      OK(c.filter("server", "connection", "httpd_connection.cpp"));
      OK(c.filter("server", "connection", "tcp_connection.cpp"));
      OK(c.filter("http", "connection", "tcp_connection.cpp"));
   };
}
