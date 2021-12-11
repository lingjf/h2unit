#include "../source/h2_unit.cpp"

static int counts(const char* array[])
{
   int count = 0;
   while (array[count]) ++count;
   return count;
}

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
      OK(0, c.shuffles);
      OK(c.memory_check);
      OK(!c.continue_assert);
      OK(!c.debugger_trap);
      OK(!c.quit_exit_code);
      OK(0, c.break_after_fails);
      OK(!c.exception_as_fail);
      OK(!c.tags_filter);
      OK(0, c.lists);
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

   Case(rounds -r1)
   {
      const char* argv[] = {"./a.out", "-r1"};
      c.parse(2, argv);
      OK(1, c.run_rounds);
   }

   Case(rounds -r 4)
   {
      const char* argv[] = {"./a.out", "-r", "4"};
      c.parse(3, argv);
      OK(4, c.run_rounds);
   }

   Case(shuffle memory_check break_after_fails)
   {
      const char* argv[] = {"./a.out", "-smb"};
      c.parse(2, argv);
      OK(c.shuffles);
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
}

SUITE(option filter)
{
   h2::h2_option c;

   Case(include)
   {
      const char* argv[] = {"./a.out", "-i", "http"};
      c.parse(3, argv);
      OK(ListOf("http"), c.includes, 1);
      OK(0, counts(c.excludes));
   }

   Case(2 include)
   {
      const char* argv[] = {"./a.out", "-i", "http", "-i", "tcp"};
      c.parse(5, argv);
      OK(ListOf("http", "tcp"), c.includes, 2);
      OK(0, counts(c.excludes));
   }

   Case(include 2)
   {
      const char* argv[] = {"./a.out", "-i", "http", "tcp*"};
      c.parse(4, argv);
      OK(ListOf("http", "tcp*"), c.includes, 2);
      OK(0, counts(c.excludes));
   }

   Case(2 exclude)
   {
      const char* argv[] = {"./a.out", "-e", "http", "-e", "tcp"};
      c.parse(5, argv);
      OK(ListOf("http", "tcp"), c.excludes, 2);
      OK(0, counts(c.includes));
   }
}

SUITE(option list)
{
   h2::h2_option c;

   Case(list -l)
   {
      const char* argv[] = {"./a.out", "-l"};
      c.parse(2, argv);
      OK(0x1110, c.lists);
   }

   Case(list -l suite)
   {
      const char* argv[] = {"./a.out", "-l", "suite"};
      c.parse(3, argv);
      OK(0x10, c.lists);
   }

   Case(list -l case)
   {
      const char* argv[] = {"./a.out", "-l", "case"};
      c.parse(3, argv);
      OK(0x100, c.lists);
   }

   Case(list -l todo)
   {
      const char* argv[] = {"./a.out", "-l", "todo"};
      c.parse(3, argv);
      OK(0x1000, c.lists);
   }

   Case(list -l tag)
   {
      const char* argv[] = {"./a.out", "-l", "tag"};
      c.parse(3, argv);
      OK(0x10000, c.lists);
   }

   Case(list -l prefix)
   {
      const char* argv_s[] = {"./a.out", "-l", "s"};
      const char* argv_su[] = {"./a.out", "-l", "su"};
      const char* argv_sui[] = {"./a.out", "-l", "sui"};
      const char* argv_suit[] = {"./a.out", "-l", "suit"};
      const char* argv_suite[] = {"./a.out", "-l", "suite"};
      h2::h2_option option_s;
      h2::h2_option option_su;
      h2::h2_option option_sui;
      h2::h2_option option_suit;
      h2::h2_option option_suite;
      option_s.parse(3, argv_s);
      option_su.parse(3, argv_su);
      option_sui.parse(3, argv_sui);
      option_suit.parse(3, argv_suit);
      option_suite.parse(3, argv_suite);
      OK(0x10, option_s.lists);
      OK(0x10, option_su.lists);
      OK(0x10, option_sui.lists);
      OK(0x10, option_suit.lists);
      OK(0x10, option_suite.lists);
   }
}

SUITE(option shuffle)
{
   h2::h2_option c;

   Case(shuffle -s)
   {
      const char* argv[] = {"./a.out", "-s"};
      c.parse(2, argv);
      OK(0x10, c.shuffles);
   }

   Case(shuffle -s random)
   {
      const char* argv[] = {"./a.out", "-s", "random"};
      c.parse(3, argv);
      OK(0x10, c.shuffles);
   }

   Case(shuffle -s name)
   {
      const char* argv[] = {"./a.out", "-s", "name"};
      c.parse(3, argv);
      OK(0x100, c.shuffles);
   }

   Case(shuffle -s file)
   {
      const char* argv[] = {"./a.out", "-s", "file"};
      c.parse(3, argv);
      OK(0x1000, c.shuffles);
   }

   Case(shuffle -s reverse)
   {
      const char* argv[] = {"./a.out", "-s", "reverse"};
      c.parse(3, argv);
      OK(0x10000, c.shuffles);
   }

   Case(shuffle -s name reverse)
   {
      const char* argv[] = {"./a.out", "-s", "name", "reverse"};
      c.parse(4, argv);
      OK(0x10100, c.shuffles);
   }

   Case(shuffle -s n re)
   {
      const char* argv[] = {"./a.out", "-s", "n", "re"};
      c.parse(4, argv);
      OK(0x10100, c.shuffles);
   }
}

SUITE(option json source quote)
{
   h2::h2_option c;

   Case(quote -S default)
   {
      const char* argv[] = {"./a.out", "-S"};
      c.parse(2, argv);
      OK("\\\"", c.json_source_quote);
   }

   Case(quote -S source code quote)
   {
      const char* argv[] = {"./a.out", "-S", "\\\""};
      c.parse(3, argv);
      OK("\\\"", c.json_source_quote);
   }

   Case(quote -S source code prefix)
   {
      const char* argv[] = {"./a.out", "-S", "\\"};
      c.parse(3, argv);
      OK("\\\"", c.json_source_quote);
   }

   Case(quote -S single)
   {
      const char* argv[] = {"./a.out", "-S", "single"};
      c.parse(3, argv);
      OK("\'", c.json_source_quote);
   }

   Case(quote -S single prefix)
   {
      h2::h2_option option_s;
      h2::h2_option option_si;
      h2::h2_option option_sin;
      h2::h2_option option_sing;
      h2::h2_option option_singl;
      h2::h2_option option_single;
      const char* argv_s[] = {"./a.out", "-S", "s"};
      const char* argv_si[] = {"./a.out", "-S", "si"};
      const char* argv_sin[] = {"./a.out", "-S", "sin"};
      const char* argv_sing[] = {"./a.out", "-S", "sing"};
      const char* argv_singl[] = {"./a.out", "-S", "singl"};
      const char* argv_single[] = {"./a.out", "-S", "single"};
      option_s.parse(3, argv_s);
      option_si.parse(3, argv_si);
      option_sin.parse(3, argv_sin);
      option_sing.parse(3, argv_sing);
      option_singl.parse(3, argv_singl);
      option_single.parse(3, argv_single);
      OK("\'", option_s.json_source_quote);
      OK("\'", option_si.json_source_quote);
      OK("\'", option_sin.json_source_quote);
      OK("\'", option_sing.json_source_quote);
      OK("\'", option_singl.json_source_quote);
      OK("\'", option_single.json_source_quote);
   }

   Case(quote -S single quote)
   {
      const char* argv[] = {"./a.out", "-S", "\'"};
      c.parse(3, argv);
      OK("\'", c.json_source_quote);
   }

   Case(quote -S double)
   {
      const char* argv[] = {"./a.out", "-S", "double"};
      c.parse(3, argv);
      OK("\"", c.json_source_quote);
   }

   Case(quote -S double quote)
   {
      const char* argv[] = {"./a.out", "-S", "\""};
      c.parse(3, argv);
      OK("\"", c.json_source_quote);
   }
}
