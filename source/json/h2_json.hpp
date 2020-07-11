
struct h2_json {
   static bool match(const h2_string& expect, const h2_string& actual, bool caseless);
   static void diff(const h2_string& expect, const h2_string& actual, h2_lines& e_lines, h2_lines& a_lines, bool caseless);
};
