#include "../source/h2_unit.cpp"

SUITE(h2_option)
{
   h2::h2_option c;

   Case(default)
   {
      const char* argv[] = {"./a.out"};
      c.parse(1, argv);
      OK(!c.verbose);
      OK(!c.listing);
      OK(c.colorfull);
      OK(!c.shuffle);
      OK(c.memory_check);
      OK(!c.breakable);
      OK(1, c.rounds);
   }

   Case(verbose)
   {
      const char* argv[] = {"./a.out", "-v"};
      c.parse(2, argv);
      OK(c.verbose);
   }

   Case(listing)
   {
      const char* argv[] = {"./a.out", "-l"};
      c.parse(2, argv);
      OK(c.listing);
   }

   Case(colorfull)
   {
      const char* argv[] = {"./a.out", "-c"};
      c.parse(2, argv);
      OK(!c.colorfull);
   }

   Case(rounds)
   {
      const char* argv[] = {"./a.out", "-r"};
      c.parse(2, argv);
      OK(1, c.rounds);
   }

   Case(rounds - r1)
   {
      const char* argv[] = {"./a.out", "-r1"};
      c.parse(2, argv);
      OK(1, c.rounds);
   }

   Case(rounds - r 2)
   {
      const char* argv[] = {"./a.out", "-r", "2"};
      c.parse(3, argv);
      OK(2, c.rounds);
   }

   Case(shuffle memory_check breakable)
   {
      const char* argv[] = {"./a.out", "-smb"};
      c.parse(2, argv);
      OK(c.shuffle);
      OK(!c.memory_check);
      OK(1, c.breakable);
   }

   Case(breakable - b2)
   {
      const char* argv[] = {"./a.out", "-b2"};
      c.parse(2, argv);
      OK(2, c.breakable);
   }

   Case(breakable - b 2)
   {
      const char* argv[] = {"./a.out", "-b", "2"};
      c.parse(3, argv);
      OK(2, c.breakable);
   }

   Case(junit)
   {
      const char* argv[] = {"./a.out", "-j", "jenkins.xml"};
      c.parse(3, argv);
      OK("jenkins.xml", c.junit);
   }

   Case(include substr)
   {
      const char* argv[] = {"./a.out", "-i", "http"};
      c.parse(3, argv);
      OK(!c.filter("server", "connection", "httpd_connection.cpp"));
      OK(!c.filter("server", "my_http_connection", "tcp_connection.cpp"));
      OK(!c.filter("http", "connection", "tcp_connection.cpp"));
   }

   Case(include substr 2)
   {
      const char* argv[] = {"./a.out", "-i", "http", "tcp*"};
      c.parse(4, argv);
      OK(!c.filter("server", "connection", "httpd_connection.cpp"));
      OK(!c.filter("server", "connection", "tcp_connection.cpp"));
      OK(!c.filter("http", "connection", "tcp_connection.cpp"));
   }

   Case(include exclude)
   {
      const char* argv[] = {"./a.out", "-x", "http", "-x", "tcp"};
      c.parse(5, argv);
      OK(c.filter("server", "connection", "httpd_connection.cpp"));
      OK(c.filter("server", "connection", "tcp_connection.cpp"));
      OK(c.filter("http", "connection", "tcp_connection.cpp"));
   }
}
