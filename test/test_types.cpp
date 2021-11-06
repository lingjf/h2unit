
#if defined _WIN32 || defined __CYGWIN__
#define H2UNIT_IMPORT_MAIN
#endif

#include "../source/h2_unit.cpp"

char buffer[1024];

extern "C" {
int foobar0()
{
   return 0;
}
}

int foobar1(int a)
{
   return 1;
}

int foobar2(int a, const char* b)
{
   return 2;
}

int foobar3(int a)
{
   return 3;
}

const char* foobar3(const char* a)
{
   return "3";
}

int foobar6(char* a, ...)
{
   return 6;
}

namespace test_ns {
int foobar1(float a)
{
   return 1;
}
int foobar2(int a, const char* b)
{
   return 2;
}
}

int my_printf(const char* fmt, ...)
{
   va_list a;
   va_start(a, fmt);
   int ret = vprintf(fmt, a);
   va_end(a);
   return ret;
}

int my_fprintf(FILE* stream, const char* fmt, ...)
{
   va_list a;
   va_start(a, fmt);
   int ret = vfprintf(stream, fmt, a);
   va_end(a);
   return ret;
}

int my_sprintf(char* t, const char* fmt, ...)
{
   va_list a;
   va_start(a, fmt);
   int ret = vsprintf(t, fmt, a);
   va_end(a);
   return ret;
}

int my_snprintf(char* t, int n, const char* fmt, ...)
{
   va_list a;
   va_start(a, fmt);
   int ret = vsnprintf(t, n, fmt, a);
   va_end(a);
   return ret;
}

void* my_pthread(void* arg)
{
   return NULL;
}

const char* node_type_tostring(const int type)
{
   switch (type) {
      case h2::h2_json_node::t_absent: return "absent";
      case h2::h2_json_node::t_null: return "null";
      case h2::h2_json_node::t_boolean: return "boolean";
      case h2::h2_json_node::t_number: return "number";
      case h2::h2_json_node::t_string: return "string";
      case h2::h2_json_node::t_pattern: return "pattern";
      case h2::h2_json_node::t_array: return "array";
      case h2::h2_json_node::t_object: return "object";
   }
   return "?";
}

h2::h2_string node_tojson(h2::h2_json_node* node)
{
   h2::h2_string out;
   out.sprintf("{");
   out.sprintf("'type': '%s',", node_type_tostring(node->type));
   out.sprintf("'index': %d,", node->index);
   out.sprintf("'key_string': '%s',", node->key_string.c_str());
   out.sprintf("'value_string': '%s',", node->value_string.c_str());
   out.sprintf("'value_double': %g,", node->value_double);
   out.sprintf("'value_boolean': %d,", node->value_boolean);
   out.sprintf("'children':[");
   h2_list_for_each_entry (p, i, node->children, h2::h2_json_node, x) {
      if (i) out.sprintf(",");
      out += node_tojson(p);
   }
   out.sprintf("]");
   out.sprintf("}");
   return out;
}

h2::h2_string node_dump(h2::h2_json_node* node)
{
   h2::h2_string out;
   if (node->key_string.size()) out.sprintf("\"%s\":", node->key_string.c_str());
   if (node->is_null()) out += "null";
   if (node->is_bool()) out += node->value_boolean ? "true" : "false";
   if (node->is_number()) out.sprintf("%.15g", node->value_double);
   if (node->is_string()) out.sprintf("\"%s\"", node->value_string.c_str());
   if (node->is_pattern()) out.sprintf("\"/%s/\"", node->value_string.c_str());

   if (node->is_array()) {
      out.sprintf("[");
      h2_list_for_each_entry (p, i, node->children, h2::h2_json_node, x) {
         if (i) out.sprintf(",");
         out += node_dump(p);
      }
      out.sprintf("]");
   }

   if (node->is_object()) {
      out.sprintf("{");
      h2_list_for_each_entry (p, i, node->children, h2::h2_json_node, x) {
         if (i) out.sprintf(",");
         out += node_dump(p);
      }
      out.sprintf("}");
   }
   return out;
}
