#include "../source/h2_unit.h"

CASE(option) {}  // stay in line 3 to check TEST macro is works

SUITE(h2_option) 
{
   Case(default) 
   {
      const char* argv[] = {"./a.out"};
      h2::h2_option c(1, argv);
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
      h2::h2_option c(2, argv);
      OK(c.verbose);
   };

   Case(listing) 
   {
      const char* argv[] = {"./a.out", "-l"};
      h2::h2_option c(2, argv);
      OK('A', c.listing);
   };

   Case(listing suite) 
   {
      const char* argv[] = {"./a.out", "-l", "suite"};
      h2::h2_option c(3, argv);
      OK('s', c.listing);
   };

   Case(listing case) 
   {
      const char* argv[] = {"./a.out", "-l", "case"};
      h2::h2_option c(3, argv);
      OK('c', c.listing);
   };

   Case(colorable)
   {
      const char* argv[] = {"./a.out", "-c"};
      h2::h2_option c(2, argv);
      OK(!c.colorable);
   };

   Case(randomize memory_check breakable) 
   {
      const char* argv[] = {"./a.out", "-rmb"};
      h2::h2_option c(2, argv);
      OK(c.randomize);
      OK(!c.memory_check);
      OK(1, c.breakable);
   };

   Case(breakable) 
   {
      const char* argv[] = {"./a.out", "-b", "2"};
      h2::h2_option c(3, argv);
      OK(2, c.breakable);
   };

   Case(junit) 
   {
      const char* argv[] = {"./a.out", "-j", "jenkins.xml"};
      h2::h2_option c(3, argv);
      OK("jenkins.xml", c.junit);
   };

   Case(include substr) 
   {
      const char* argv[] = {"./a.out", "-i", "http"};
      h2::h2_option c(3, argv);
      OK(!c.filter("server", "connection", "httpd_connection.cpp"));
      OK(!c.filter("server", "my_http_connection", "tcp_connection.cpp"));
      OK(!c.filter("http", "connection", "tcp_connection.cpp"));
   };

   Case(include substr 2) 
   {
      const char* argv[] = {"./a.out", "-i", "http", "tcp*"};
      h2::h2_option c(4, argv);
      OK(!c.filter("server", "connection", "httpd_connection.cpp"));
      OK(!c.filter("server", "connection", "tcp_connection.cpp"));
      OK(!c.filter("http", "connection", "tcp_connection.cpp"));
   };

   Case(include exclude) 
   {
      const char* argv[] = {"./a.out", "-x", "http", "-x", "tcp"};
      h2::h2_option c(5, argv);
      OK(c.filter("server", "connection", "httpd_connection.cpp"));
      OK(c.filter("server", "connection", "tcp_connection.cpp"));
      OK(c.filter("http", "connection", "tcp_connection.cpp"));
   };
}
