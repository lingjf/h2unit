
#include "../source/h2_unit.cpp"

SUITE(h2_option)
{
   h2::h2_option c;

   Case(default)
   {
      const char* argv[] = {"./a.out"};
      c.parse(1, argv);
      OK(c.colorful);
      OK(c.progressing);
      OK(!c.only_last_failed);
      OK(0, c.shuffle_cases);
      OK(c.memory_check);
      OK(!c.continue_assert);
      OK(!c.debugger_trap);
      OK(!c.quit_exit_code);
      OK(0, c.break_after_fails);
      OK(!c.exception_as_fail);
      OK(!c.tags_filter);
      OK(0, c.list_cases);
      OK(1, c.run_rounds);
      OK(5, c.fold_json);
      OK(3, c.verbose);
      OK("", c.json_source_quote);
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

   Case(colorful)
   {
      const char* argv[] = {"./a.out", "-w"};
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

   Case(break_after_fails -b2)
   {
      const char* argv[] = {"./a.out", "-b2"};
      c.parse(2, argv);
      OK(2, c.break_after_fails);
   }

   Case(break_after_fails -b2c)
   {
      OK(2, atoi("2c"));
      const char* argv[] = {"./a.out", "-b2c"};
      c.parse(2, argv);
      OK(2, c.break_after_fails);
   }

   Case(break_after_fails -b 2)
   {
      const char* argv[] = {"./a.out", "-b", "2"};
      c.parse(3, argv);
      OK(2, c.break_after_fails);
   }

   Case(break_after_fails -b)
   {
      const char* argv[] = {"./a.out", "-b", ""};
      c.parse(3, argv);
      OK(1, c.break_after_fails);
   }

   Case(junit)
   {
      const char* argv[] = {"./a.out", "-o", "jenkins.xml"};
      c.parse(3, argv);
      OK("jenkins.xml", c.junit_path);
   }

   Case(tags -t)
   {
      const char* argv[] = {"./a.out", "-t", ""};
      c.parse(3, argv);
      OK(c.tags_filter);
   }

   Case(include substr)
   {
      const char* argv[] = {"./a.out", "-i", "http"};
      c.parse(3, argv);
      OK(ListOf("http"), c.includes);
      OK(CountOf(0), c.excludes);
   }

   Case(include substr 2)
   {
      const char* argv[] = {"./a.out", "-i", "http", "tcp*"};
      c.parse(4, argv);
      OK(ListOf("http", "tcp*"), c.includes);
      OK(CountOf(0), c.excludes);
   }

   Case(include exclude)
   {
      const char* argv[] = {"./a.out", "-e", "http", "-e", "tcp"};
      c.parse(5, argv);
      OK(ListOf("http", "tcp"), c.excludes);
      OK(CountOf(0), c.includes);
   }

   Case(include verbose)
   {
      const char* argv[] = {"./a.out", "-i", "http", "-v"};
      c.parse(4, argv);
      OK(8, c.verbose);
   }
}

SUITE(option list)
{
   h2::h2_option c;

   Case(list_cases -l)
   {
      const char* argv[] = {"./a.out", "-l"};
      c.parse(2, argv);
      OK(0x1110, c.list_cases);
   }

   Case(list_cases -l suite)
   {
      const char* argv[] = {"./a.out", "-l", "suite"};
      c.parse(3, argv);
      OK(0x10, c.list_cases);
   }

   Case(list_cases -l case)
   {
      const char* argv[] = {"./a.out", "-l", "case"};
      c.parse(3, argv);
      OK(0x100, c.list_cases);
   }

   Case(list_cases -l todo)
   {
      const char* argv[] = {"./a.out", "-l", "todo"};
      c.parse(3, argv);
      OK(0x1000, c.list_cases);
   }

   Case(list_cases -l tag)
   {
      const char* argv[] = {"./a.out", "-l", "tag"};
      c.parse(3, argv);
      OK(0x10000, c.list_cases);
   }

   Case(list_cases -l s)
   {
      const char* argv[] = {"./a.out", "-l", "s"};
      c.parse(3, argv);
      OK(0x10, c.list_cases);
   }
}

SUITE(option shuffle)
{
   h2::h2_option c;

   Case(shuffle -s)
   {
      const char* argv[] = {"./a.out", "-s"};
      c.parse(2, argv);
      OK(0x10, c.shuffle_cases);
   }

   Case(shuffle -s random)
   {
      const char* argv[] = {"./a.out", "-s", "random"};
      c.parse(3, argv);
      OK(0x10, c.shuffle_cases);
   }

   Case(shuffle -s name)
   {
      const char* argv[] = {"./a.out", "-s", "name"};
      c.parse(3, argv);
      OK(0x100, c.shuffle_cases);
   }

   Case(shuffle -s reverse)
   {
      const char* argv[] = {"./a.out", "-s", "reverse"};
      c.parse(3, argv);
      OK(0x10000, c.shuffle_cases);
   }

   Case(shuffle -s name reverse)
   {
      const char* argv[] = {"./a.out", "-s", "name", "reverse"};
      c.parse(4, argv);
      OK(0x10100, c.shuffle_cases);
   }

   Case(shuffle -s n re)
   {
      const char* argv[] = {"./a.out", "-s", "n", "re"};
      c.parse(4, argv);
      OK(0x10100, c.shuffle_cases);
   }
}

SUITE(option json source quote)
{
   h2::h2_option c;

   Case(list_cases -S)
   {
      const char* argv[] = {"./a.out", "-S"};
      c.parse(2, argv);
      OK("\\\"", c.json_source_quote);
   }
}
