
#include "../source/h2_unit.cpp"

SUITE(h2_option)
{
   h2::h2_option c;

   Case(default)
   {
      const char* argv[] = {"./a.out"};
      c.parse(1, argv);
      OK(3, c.verbose);
      OK(!c.list_cases);
      OK(c.colorful);
      OK(!c.shuffle_cases);
      OK(c.memory_check);
      OK(!c.break_after_fails);
      OK(1, c.run_rounds);
   }

   Case(verbose -v)
   {
      const char* argv[] = {"./a.out", "-v"};
      c.parse(2, argv);
      OK(8, c.verbose);
   }

   Case(verbose -v2)
   {
      const char* argv[] = {"./a.out", "-v2"};
      c.parse(2, argv);
      OK(2, c.verbose);
   }

   Case(verbose -v 3)
   {
      const char* argv[] = {"./a.out", "-v", "3"};
      c.parse(3, argv);
      OK(3, c.verbose);
   }

   Case(list_cases)
   {
      const char* argv[] = {"./a.out", "-l"};
      c.parse(2, argv);
      OK(c.list_cases);
   }

   Case(colorful)
   {
      const char* argv[] = {"./a.out", "-c"};
      c.parse(2, argv);
      OK(!c.colorful);
   }

   Case(rounds)
   {
      const char* argv[] = {"./a.out", "-r"};
      c.parse(2, argv);
      OK(2, c.run_rounds);
   }

   Case(rounds - r1)
   {
      const char* argv[] = {"./a.out", "-r1"};
      c.parse(2, argv);
      OK(1, c.run_rounds);
   }

   Case(rounds - r 4)
   {
      const char* argv[] = {"./a.out", "-r", "4"};
      c.parse(3, argv);
      OK(4, c.run_rounds);
   }

   Case(shuffle memory_check break_after_fails)
   {
      const char* argv[] = {"./a.out", "-smb"};
      c.parse(2, argv);
      OK(c.shuffle_cases);
      OK(!c.memory_check);
      OK(1, c.break_after_fails);
   }

   Case(break_after_fails - b2)
   {
      const char* argv[] = {"./a.out", "-b2"};
      c.parse(2, argv);
      OK(2, c.break_after_fails);
   }

   Case(break_after_fails - b2c)
   {
      OK(2, atoi("2c"));
      const char* argv[] = {"./a.out", "-b2c"};
      c.parse(2, argv);
      OK(2, c.break_after_fails);
   }

   Case(break_after_fails - b 2)
   {
      const char* argv[] = {"./a.out", "-b", "2"};
      c.parse(3, argv);
      OK(2, c.break_after_fails);
   }

   Case(break_after_fails - b)
   {
      const char* argv[] = {"./a.out", "-b", ""};
      c.parse(3, argv);
      OK(1, c.break_after_fails);
   }

   Case(junit)
   {
      const char* argv[] = {"./a.out", "-j", "jenkins.xml"};
      c.parse(3, argv);
      OK("jenkins.xml", c.junit_path);
   }

   Case(include substr)
   {
      const char* argv[] = {"./a.out", "-i", "http"};
      c.parse(3, argv);
      OK(!c.filter("server", "connection", "httpd_connection.cpp:123"));
      OK(!c.filter("server", "my_http_connection", "tcp_connection.cpp:123"));
      OK(!c.filter("http", "connection", "tcp_connection.cpp:123"));
   }

   Case(include substr 2)
   {
      const char* argv[] = {"./a.out", "-i", "http", "tcp*"};
      c.parse(4, argv);
      OK(!c.filter("server", "connection", "httpd_connection.cpp:123"));
      OK(!c.filter("server", "connection", "tcp_connection.cpp:123"));
      OK(!c.filter("http", "connection", "tcp_connection.cpp:123"));
   }

   Case(include exclude)
   {
      const char* argv[] = {"./a.out", "-e", "http", "-e", "tcp"};
      c.parse(5, argv);
      OK(c.filter("server", "connection", "httpd_connection.cpp:123"));
      OK(c.filter("server", "connection", "tcp_connection.cpp:123"));
      OK(c.filter("http", "connection", "tcp_connection.cpp:123"));
   }

   Case(include verbose)
   {
      const char* argv[] = {"./a.out", "-i", "http", "-v"};
      c.parse(4, argv);
      OK(8, c.verbose);
   }
}
