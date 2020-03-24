
struct h2_json_exporter {
   static bool match(const h2_string expect, const h2_string actual);
   static int diff(const h2_string expect, const h2_string actual, int terminal_width, h2_string& str);
};
