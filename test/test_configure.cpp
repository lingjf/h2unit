#include "../source/h2_unit.h"

CASE(configure) {}  // stay in line 3 to check TEST macro is works

SUITE(h2_configure) 
{
   h2_configure c;
   Case(verbose) 
   {
      OK(!c.verbose);
      const char* argv[] = {"./a.out", "-v"};
      c.opt(2, argv);
      OK(c.verbose);
   };

   Case(listing) 
   {
      OK(0, c.listing);
      const char* argv[] = {"./a.out", "-l"};
      c.opt(2, argv);
      OK('A', c.listing);
   };

   Case(listing suite) 
   {
      OK(0, c.listing);
      const char* argv[] = {"./a.out", "-l", "suite"};
      c.opt(3, argv);
      OK('s', c.listing);
   };

   Case(listing case) 
   {
      OK(0, c.listing);
      const char* argv[] = {"./a.out", "-l", "case"};
      c.opt(3, argv);
      OK('c', c.listing);
   };

   Case(colorable)
   {
      OK(c.colorable);
      const char* argv[] = {"./a.out", "-c"};
      c.opt(2, argv);
      OK(!c.colorable);
   };

   Case(randomize memory_check breakable) 
   {
      OK(!c.randomize);
      OK(c.memory_check);
      OK(0, c.breakable);
      const char* argv[] = {"./a.out", "-rmb"};
      c.opt(2, argv);
      OK(c.randomize);
      OK(!c.memory_check);
      OK(1, c.breakable);
   };

   Case(breakable) 
   {
      OK(0, c.breakable);
      const char* argv[] = {"./a.out", "-b", "2"};
      c.opt(3, argv);
      OK(2, c.breakable);
   };

   Case(junit) 
   {
      const char* argv[] = {"./a.out", "-j", "jenkins.xml"};
      c.opt(3, argv);
      OK("jenkins.xml", c.junit);
   };

   Case(include substr) 
   {
      const char* argv[] = {"./a.out", "-i", "http"};
      c.opt(3, argv);
      OK(!c.filter("server", "connection", "httpd_connection.cpp"));
      OK(!c.filter("server", "my_http_connection", "tcp_connection.cpp"));
      OK(!c.filter("http", "connection", "tcp_connection.cpp"));
   };

   Case(include substr 2) 
   {
      const char* argv[] = {"./a.out", "-i", "http", "tcp*"};
      c.opt(4, argv);
      OK(!c.filter("server", "connection", "httpd_connection.cpp"));
      OK(!c.filter("server", "connection", "tcp_connection.cpp"));
      OK(!c.filter("http", "connection", "tcp_connection.cpp"));
   };

   Case(include exclude) 
   {
      const char* argv[] = {"./a.out", "-x", "http", "-x", "tcp"};
      c.opt(5, argv);
      OK(c.filter("server", "connection", "httpd_connection.cpp"));
      OK(c.filter("server", "connection", "tcp_connection.cpp"));
      OK(c.filter("http", "connection", "tcp_connection.cpp"));
   };
}
