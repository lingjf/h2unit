struct h2_report_list : h2_report_interface {
   int suites = 0, cases = 0, todos = 0;
   int unfiltered_suites = 0, unfiltered_cases = 0, unfiltered_todos = 0;
   int suite_cases = 0, suite_todos = 0;

   struct tag {
      const char* name;
      int suites = 0, cases = 0;
   } tags[1024], unfiltered_tags[1024];
   int tagc = 0, unfiltered_tagc = 0;

   static tag* get_tag(tag tags[], int& tagc, const char* name)
   {
      for (int i = 0; i < tagc; ++i)
         if (!strcmp(name, tags[i].name)) return &tags[i];
      return nullptr;
   }
   static tag* add_tag(tag tags[], int& tagc, const char* name)
   {
      tag* t = get_tag(tags, tagc, name);
      if (t) return t;
      if (tagc >= 1024) return nullptr;
      t = &tags[tagc++];
      t->name = name;
      return t;
   }
   static void add_suite_tag(tag tags[], int& tagc, const char* name)
   {
      auto t = add_tag(tags, tagc, name);
      if (t) t->suites++;
   }
   static void add_case_tag(tag tags[], int& tagc, const char* name)
   {
      auto t = add_tag(tags, tagc, name);
      if (t) t->cases++;
   }

   void on_runner_start(h2_runner* r) override {}
   void on_runner_endup(h2_runner* r) override
   {
      if (O.lists & ListTag) {
         for (int i = 0; i < unfiltered_tagc; ++i) {
            h2_line line;
            line.printf("dark gray", "TAG-%d. ", i).printf("bold,light purple", "%s ", unfiltered_tags[i].name);
            if (unfiltered_tags[i].suites) line.printf("", " %d ", unfiltered_tags[i].suites).printf("dark gray", H2_UNITS(unfiltered_tags[i].suites, "suite"));
            if (unfiltered_tags[i].cases) line.printf("", " %d ", unfiltered_tags[i].cases).printf("dark gray", H2_UNITS(unfiltered_tags[i].cases, "case"));
            h2_console::printl(line);
         }
      }

      h2_line line;
      if (O.lists & ListSuite) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_suites), "green") + " " + gray(H2_UNITS(unfiltered_suites, "suite"));
      if (O.lists & ListCase) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_cases), "green") + " " + gray(H2_UNITS(unfiltered_cases, "case"));
      if (O.lists & ListTodo) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_todos), "green") + " " + gray(H2_UNITS(unfiltered_todos, "todo"));
      if (O.lists & ListTag) line += gray(comma_if(line.width())) + color(h2_stringify(unfiltered_tagc), "green") + " " + gray(H2_UNITS(unfiltered_tagc, "tag"));
      if (O.lists & ListSuite && suites > unfiltered_suites) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), suites - unfiltered_suites, H2_UNITS(suites - unfiltered_suites, "suite"));
      if (O.lists & ListCase && cases > unfiltered_cases) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), cases - unfiltered_cases, H2_UNITS(cases - unfiltered_cases, "case"));
      if (O.lists & ListTodo && todos > unfiltered_todos) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), todos - unfiltered_todos, H2_UNITS(todos - unfiltered_todos, "todo"));
      if (O.lists & ListTag && tagc > unfiltered_tagc) line.printf("dark gray", "%s%d filtered %s", comma_if(line.width()), tagc - unfiltered_tagc, H2_UNITS(tagc - unfiltered_tagc, "tag"));
      h2_console::printl("Listing " + line);
   }
   h2_line format_tags(const char* tags[])
   {
      h2_line line;
      if (O.lists & ListTag)
         for (int i = 0; tags[i]; ++i) line.printf("purple", " %s", tags[i]);
      return line;
   }
   void on_suite_start(h2_suite* s) override
   {
      suite_cases = 0;
      suite_todos = 0;
      if (s->absent()) return;  // CASE
      for (int i = 0; s->tags[i]; ++i) add_suite_tag(tags, tagc, s->tags[i]);
      if (!s->filtered)
         for (int i = 0; s->tags[i]; ++i) add_suite_tag(unfiltered_tags, unfiltered_tagc, s->tags[i]);
      ++suites;
      if (!s->filtered && O.lists & ListSuite) {
         ++unfiltered_suites;
         h2_line line;
         line.printf("dark gray", "SUITE-%d. ", unfiltered_suites);
         h2_console::printl(line + color(s->name, "bold,blue") + " " + gray(h2_basefile(s->filine)) + format_tags(s->tags));
      }
   }
   void on_suite_endup(h2_suite* s) override {}
   void on_case_start(h2_suite* s, h2_case* c) override
   {
      for (int i = 0; c->tags[i]; ++i) add_case_tag(tags, tagc, c->tags[i]);
      if (!c->filtered)
         for (int i = 0; c->tags[i]; ++i) add_case_tag(unfiltered_tags, unfiltered_tagc, c->tags[i]);

      const char* type = nullptr;
      if (c->todo) {
         ++todos;
         if (!c->filtered && O.lists & ListTodo) {
            type = s->absent() ? "TODO" : "Todo";
            ++unfiltered_todos, ++suite_todos;
         }
      } else {
         ++cases;
         if (!c->filtered && O.lists & ListCase) {
            type = s->absent() ? "CASE" : "Case";
            ++unfiltered_cases, ++suite_cases;
         }
      }

      if (type) {
         h2_line line;
         if (O.lists & ListSuite) line.printf("dark gray", " %s/%d-%d. ", type, suite_cases + suite_todos, unfiltered_cases + unfiltered_todos);
         else line.printf("dark gray", " %s-%d. ", type, unfiltered_cases + unfiltered_todos);

         h2_console::printl(line + color(c->name, "cyan") + " " + gray(h2_basefile(c->filine)) + format_tags(c->tags));
      }
   }
   void on_case_endup(h2_suite* s, h2_case* c) override {}
};
