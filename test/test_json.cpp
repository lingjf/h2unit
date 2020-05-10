#include "../source/h2_unit.cpp"

static int __node_tojson(h2::h2_json_node* node, char* b)
{
   int l = 0;

   if (node->key_string.size()) {
      l += sprintf(b + l, "\"%s\":", node->key_string.c_str());
   }
   if (node->is_null()) {
      l += sprintf(b + l, "null");
   }
   if (node->is_bool()) {
      l += sprintf(b + l, "%s", node->value_boolean ? "true" : "false");
   }
   if (node->is_number()) {
      l += sprintf(b + l, "%.15g", node->value_double);
   }
   if (node->is_string()) {
      l += sprintf(b + l, "\"%s\"", node->value_string.c_str());
   }
   if (node->is_regexp()) {
      l += sprintf(b + l, "\"/%s/\"", node->value_string.c_str());
   }
   if (node->is_array()) {
      l += sprintf(b + l, "[");
      for (size_t i = 0; i < node->children.size(); i++) {
         if (i)
            l += sprintf(b + l, ",");
         l += __node_tojson(node->children[i], b + l);
      }
      l += sprintf(b + l, "]");
   }
   if (node->is_object()) {
      l += sprintf(b + l, "{");
      for (int i = 0; i < node->size(); i++) {
         if (i)
            l += sprintf(b + l, ",");
         l += __node_tojson(node->children[i], b + l);
      }
      l += sprintf(b + l, "}");
   }

   return l;
}

char* node_tojson(h2::h2_json_node* node, char* b)
{
   __node_tojson(node, b);
   return b;
}

static const char* __type_tostr(const int type)
{
   switch (type) {
   case h2::h2_json_node::t_absent: return "absent";
   case h2::h2_json_node::t_null: return "null";
   case h2::h2_json_node::t_boolean: return "boolean";
   case h2::h2_json_node::t_number: return "number";
   case h2::h2_json_node::t_string: return "string";
   case h2::h2_json_node::t_regexp: return "regexp";
   case h2::h2_json_node::t_array: return "array";
   case h2::h2_json_node::t_object: return "object";
   }
   return "?";
}

static int __dual_tostr(h2::h2_json_dual* dual, char* b)
{
   int l = 0;

   l += sprintf(b + l, "(%d;%s-%s;%s-%s;%s-%s)", dual->depth,
                __type_tostr(dual->e_type), __type_tostr(dual->e_type),
                dual->e_key.c_str(), dual->a_key.c_str(),
                dual->e_value.c_str(), dual->a_value.c_str());

   if (dual->children.size()) {
      l += sprintf(b + l, "[");
      for (size_t i = 0; i < dual->children.size(); i++) {
         l += __dual_tostr(dual->children[i], b + l);
      }
      l += sprintf(b + l, "]");
   }
   return l;
}

char* dual_tostr(h2::h2_json_dual* dual, char* b)
{
   __dual_tostr(dual, b);
   return b;
}

SUITE(json utils)
{
   Case(h2_line_pad_tail)
   {
      h2::h2_line line = {"hello", " ", "world"};
      h2::h2_line_pad_tail(line, 15);
      OK(ListOf("hello", " ", "world", "    "), line);
   }

   Case(samesizify string)
   {
      h2::h2_string e = "123456";
      h2::h2_string a = "123";
      h2::samesizify(e, a);
      OK("123456", e);
      OK("123   ", a);
   }

   Todo(samesizify lines)
   {
      h2::h2_vector<h2::h2_line> e = {{"123", "456"}, {"1234"}};
      h2::h2_vector<h2::h2_line> a = {{"12345"}};
      samesizify(e, a);

      OK(ListOf(
           ListOf("123", "456"),
           ListOf("1234", "  ")),
         e);

      OK(ListOf(
           ListOf("12345", " "),
           ListOf("      ")),
         a);
   }
}

SUITE(json node)
{
   char t2[1024 * 32];

   Case(number)
   {
      const char* n1 = "-123.456";
      h2::h2_json_node c1(n1);
      OK(c1.is_number());
      OK(-123.456, c1.value_double);

      const char* n2 = "0";
      h2::h2_json_node c2(n2);
      OK(c2.is_number());
      OK(0, c2.value_double);

      const char* n3 = "-1";
      h2::h2_json_node c3(n3);
      OK(c3.is_number());
      OK(-1, c3.value_double);

      const char* n4 = "12345678";
      h2::h2_json_node c4(n4, 4);
      OK(c4.is_number());
      OK(1234, c4.value_double);

      const char* n5 = "3+2*(4-sqrt(4))";
      h2::h2_json_node c5(n5);
      OK(c5.is_number());
      OK(7, c5.value_double);
   }

   Case(number error)
   {
      const char* n1 = "";
      h2::h2_json_node c1(n1);
      OK(c1.type == h2::h2_json_node::t_absent);

      const char* n2 = "-";
      h2::h2_json_node c2(n2);
      OK(c2.type == h2::h2_json_node::t_absent);
   }

   Case(string)
   {
      const char* n1 = "\"\"";
      h2::h2_json_node c1(n1);
      OK(c1.is_string());
      OK("", c1.value_string);

      const char* n2 = "\"12345678\"";
      h2::h2_json_node c2(n2);
      OK(c2.is_string());
      OK("12345678", c2.value_string);

      const char* n3 = "'12345678'";
      h2::h2_json_node c3(n3);
      OK(c2.is_string());
      OK("12345678", c3.value_string);
   }

   Case(string error)
   {
      h2::h2_json_node c1("\"12345678\"", 4);
      OK(c1.type == h2::h2_json_node::t_absent);
   }

   Case(regexp)
   {
      h2::h2_json_node c1("/123*567/");
      OK(c1.is_regexp());
      OK("123*567", c1.value_string);
   }

   Case(empty array)
   {
      h2::h2_json_node c1(" []");
      OK(c1.is_array());
      OK(0, c1.size());
   }

   Case(normal array)
   {
      const char* week = "[\"Sunday\", 'Monday', \"Tuesday\", \"Wednesday\", "
                         "\"Thursday\", \"Friday\", \"Saturday\"]";

      h2::h2_json_node c1(week);

      OK(c1.is_array());
      OK(0, c1.key_string.size());
      OK(7, c1.size());

      h2::h2_json_node* d[7];
      d[0] = c1.get(0);
      OK("Sunday", d[0]->value_string);

      d[1] = c1.get(1);
      OK("Monday", d[1]->value_string);

      d[2] = c1.get(2);
      OK("Tuesday", d[2]->value_string);

      d[3] = c1.get(3);
      OK("Wednesday", d[3]->value_string);

      d[4] = c1.get(4);
      OK("Thursday", d[4]->value_string);

      d[5] = c1.get(5);
      OK("Friday", d[5]->value_string);

      d[6] = c1.get(6);
      OK("Saturday", d[6]->value_string);

      JE(week, node_tojson(&c1, t2));

      OK(NULL == c1.get("ling"));

      for (int i = 0; i < c1.size(); i++) {
         OK(d[i]->value_string, c1.children[i]->value_string);
      }
   }

   Case(empty object)
   {
      h2::h2_json_node c1(" {  }");
      OK(c1.is_object());
      OK(0, c1.size());
   }

   Case(normal object)
   {
      const char* obj = "{                                              \
        \"data\": 'Click Here',                                         \
        'size': 36,                                                     \
        \"alignment\": true,                                            \
        \"bold\": false,                                                \
        \"token\": null,                                                \
        \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"    \
      }";

      h2::h2_json_node c(obj);

      OK(c.is_object());
      OK(6, c.size());

      h2::h2_json_node* c0 = c.get(0);
      OK(c0->is_string());
      OK("data", c0->key_string);
      OK("Click Here", c0->value_string);

      h2::h2_json_node* c1 = c.get(1);
      OK(c1->is_number());
      OK("size", c1->key_string);
      OK(36, c1->value_double);

      h2::h2_json_node* c2 = c.get(2);
      OK(c2->is_bool());
      OK("alignment", c2->key_string);
      OK(c2->value_boolean);

      h2::h2_json_node* c3 = c.get(3);
      OK(c3->is_bool());
      OK("bold", c3->key_string);
      OK(!c3->value_boolean);

      h2::h2_json_node* c4 = c.get(4);
      OK(c4->is_null());
      OK("token", c4->key_string);

      h2::h2_json_node* c5 = c.get(5);
      OK(c5->is_string());
      OK("onMouseUp", c5->key_string);
      OK("sun1.opacity = (sun1.opacity / 100) * 90;", c5->value_string);

      JE(obj, node_tojson(&c, t2));
   }

   Case("http://www.json.org/example.html")
   {
      const char* j1 =
        "{                                                                                                        \
    \"glossary\": {                                                                                               \
        \"title\": \"example glossary\",                                                                          \
		\"GlossDiv\": {                                                                                             \
            \"title\": \"S\",                                                                                     \
			\"GlossList\": {                                                                                         \
                \"GlossEntry\": {                                                                                 \
                    \"ID\": \"SGML\",                                                                             \
					\"SortAs\": \"SGML\",                                                                              \
					\"GlossTerm\": \"Standard Generalized Markup Language\",                                           \
					\"Acronym\": \"SGML\",                                                                             \
					\"Abbrev\": \"ISO 8879:1986\",                                                                     \
					\"GlossDef\": {                                                                                    \
                        \"para\": \"A meta-markup language, used to create markup languages such as DocBook.\",   \
						\"GlossSeeAlso\": [\"GML\", \"XML\"]                                                            \
                    },                                                                                            \
					\"GlossSee\": \"markup\"                                                                           \
                }                                                                                                 \
            }                                                                                                     \
        }                                                                                                         \
    }                                                                                                             \
}";

      h2::h2_json_node c1(j1);
      JE(j1, node_tojson(&c1, t2));

      const char* j2 =
        "{\"menu\": {                                               \
  \"id\": \"file\",                                                 \
  \"value\": \"File\",                                              \
  \"popup\": {                                                      \
    \"menuitem\": [                                                 \
      {\"value\": \"New\", \"onclick\": \"CreateNewDoc()\"},        \
      {\"value\": \"Open\", \"onclick\": \"OpenDoc()\"},            \
      {\"value\": \"Close\", \"onclick\": \"CloseDoc()\"}           \
    ]                                                               \
  }                                                                 \
}}";

      h2::h2_json_node c2(j2);
      JE(j2, node_tojson(&c2, t2));

      const char* j3 =
        "{\"widget\": {                                                     \
    \"debug\": \"on\",                                                      \
    \"window\": {                                                           \
        \"title\": \"Sample Konfabulator Widget\",                          \
        \"name\": \"main_window\",                                          \
        \"width\": 500,                                                     \
        \"height\": 500                                                     \
    },                                                                      \
    \"image\": {                                                            \
        \"src\": \"Images/Sun.png\",                                        \
        \"name\": \"sun1\",                                                 \
        \"hOffset\": 250,                                                   \
        \"vOffset\": 250,                                                   \
        \"alignment\": \"center\"                                           \
    },                                                                      \
    \"text\": {                                                             \
        \"data\": \"Click Here\",                                           \
        \"size\": 36,                                                       \
        \"style\": \"bold\",                                                \
        \"name\": \"text1\",                                                \
        \"hOffset\": 250,                                                   \
        \"vOffset\": 100,                                                   \
        \"alignment\": \"center\",                                          \
        \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"        \
    }                                                                       \
}}";

      h2::h2_json_node c3(j3);
      JE(j3, node_tojson(&c3, t2));

      const char* j4 =
        "{\"web-app\": {                                                                    \
  \"servlet\": [                                                                            \
    {                                                                                       \
      \"servlet-name\": \"cofaxCDS\",                                                       \
      \"servlet-class\": \"org.cofax.cds.CDSServlet\",                                      \
      \"init-param\": {                                                                     \
        \"configGlossary:installationAt\": \"Philadelphia, PA\",                            \
        \"configGlossary:adminEmail\": \"ksm@pobox.com\",                                   \
        \"configGlossary:poweredBy\": \"Cofax\",                                            \
        \"configGlossary:poweredByIcon\": \"/images/cofax.gif\",                            \
        \"configGlossary:staticPath\": \"/content/static\",                                 \
        \"templateProcessorClass\": \"org.cofax.WysiwygTemplate\",                          \
        \"templateLoaderClass\": \"org.cofax.FilesTemplateLoader\",                         \
        \"templatePath\": \"templates\",                                                    \
        \"templateOverridePath\": \"\",                                                     \
        \"defaultListTemplate\": \"listTemplate.htm\",                                      \
        \"defaultFileTemplate\": \"articleTemplate.htm\",                                   \
        \"useJSP\": false,                                                                  \
        \"jspListTemplate\": \"listTemplate.jsp\",                                          \
        \"jspFileTemplate\": \"articleTemplate.jsp\",                                       \
        \"cachePackageTagsTrack\": 200,                                                     \
        \"cachePackageTagsStore\": 200,                                                     \
        \"cachePackageTagsRefresh\": 60,                                                    \
        \"cacheTemplatesTrack\": 100,                                                       \
        \"cacheTemplatesStore\": 50,                                                        \
        \"cacheTemplatesRefresh\": 15,                                                      \
        \"cachePagesTrack\": 200,                                                           \
        \"cachePagesStore\": 100,                                                           \
        \"cachePagesRefresh\": 10,                                                          \
        \"cachePagesDirtyRead\": 10,                                                        \
        \"searchEngineListTemplate\": \"forSearchEnginesList.htm\",                         \
        \"searchEngineFileTemplate\": \"forSearchEngines.htm\",                             \
        \"searchEngineRobotsDb\": \"WEB-INF/robots.db\",                                    \
        \"useDataStore\": true,                                                             \
        \"dataStoreClass\": \"org.cofax.SqlDataStore\",                                     \
        \"redirectionClass\": \"org.cofax.SqlRedirection\",                                 \
        \"dataStoreName\": \"cofax\",                                                       \
        \"dataStoreDriver\": \"com.microsoft.jdbc.sqlserver.SQLServerDriver\",              \
        \"dataStoreUrl\": \"jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon\",  \
        \"dataStoreUser\": \"sa\",                                                          \
        \"dataStorePassword\": \"dataStoreTestQuery\",                                      \
        \"dataStoreTestQuery\": \"SET NOCOUNT ON;select test='test';\",                     \
        \"dataStoreLogFile\": \"/usr/local/tomcat/logs/datastore.log\",                     \
        \"dataStoreInitConns\": 10,                                                         \
        \"dataStoreMaxConns\": 100,                                                         \
        \"dataStoreConnUsageLimit\": 100,                                                   \
        \"dataStoreLogLevel\": \"debug\",                                                   \
        \"maxUrlLength\": 500}},                                                            \
    {                                                                                       \
      \"servlet-name\": \"cofaxEmail\",                                                     \
      \"servlet-class\": \"org.cofax.cds.EmailServlet\",                                    \
      \"init-param\": {                                                                     \
      \"mailHost\": \"mail1\",                                                              \
      \"mailHostOverride\": \"mail2\"}},                                                    \
    {                                                                                       \
      \"servlet-name\": \"cofaxAdmin\",                                                     \
      \"servlet-class\": \"org.cofax.cds.AdminServlet\"},                                   \
                                                                                            \
    {                                                                                       \
      \"servlet-name\": \"fileServlet\",                                                    \
      \"servlet-class\": \"org.cofax.cds.FileServlet\"},                                    \
    {                                                                                       \
      \"servlet-name\": \"cofaxTools\",                                                     \
      \"servlet-class\": \"org.cofax.cms.CofaxToolsServlet\",                               \
      \"init-param\": {                                                                     \
        \"templatePath\": \"toolstemplates/\",                                              \
        \"log\": 1,                                                                         \
        \"logLocation\": \"/usr/local/tomcat/logs/CofaxTools.log\",                         \
        \"logMaxSize\": \"\",                                                               \
        \"dataLog\": 1,                                                                     \
        \"dataLogLocation\": \"/usr/local/tomcat/logs/dataLog.log\",                        \
        \"dataLogMaxSize\": \"\",                                                           \
        \"removePageCache\": \"/content/admin/remove?cache=pages&id=\",                     \
        \"removeTemplateCache\": \"/content/admin/remove?cache=templates&id=\",             \
        \"fileTransferFolder\": \"/usr/local/tomcat/webapps/content/fileTransferFolder\",   \
        \"lookInContext\": 1,                                                               \
        \"adminGroupID\": 4,                                                                \
        \"betaServer\": true}}],                                                            \
  \"servlet-mapping\": {                                                                    \
    \"cofaxCDS\": \"/\",                                                                    \
    \"cofaxEmail\": \"/cofaxutil/aemail/*\",                                                \
    \"cofaxAdmin\": \"/admin/*\",                                                           \
    \"fileServlet\": \"/static/*\",                                                         \
    \"cofaxTools\": \"/tools/*\"},                                                          \
                                                                                            \
  \"taglib\": {                                                                             \
    \"taglib-uri\": \"cofax.tld\",                                                          \
    \"taglib-location\": \"/WEB-INF/tlds/cofax.tld\"}}}";

      h2::h2_json_node c4(j4);
      JE(j4, node_tojson(&c4, t2));

      const char* j5 =
        "{\"menu\": {                                                       \
    \"header\": \"SVG Viewer\",                                             \
    \"items\": [                                                            \
        {\"id\": \"Open\"},                                                 \
        {\"id\": \"OpenNew\", \"label\": \"Open New\"},                     \
        null,                                                               \
        {\"id\": \"ZoomIn\", \"label\": \"Zoom In\"},                       \
        {\"id\": \"ZoomOut\", \"label\": \"Zoom Out\"},                     \
        {\"id\": \"OriginalView\", \"label\": \"Original View\"},           \
        null,                                                               \
        {\"id\": \"Quality\"},                                              \
        {\"id\": \"Pause\"},                                                \
        {\"id\": \"Mute\"},                                                 \
        null,                                                               \
        {\"id\": \"Find\", \"label\": \"Find...\"},                         \
        {\"id\": \"FindAgain\", \"label\": \"Find Again\"},                 \
        {\"id\": \"Copy\"},                                                 \
        {\"id\": \"CopyAgain\", \"label\": \"Copy Again\"},                 \
        {\"id\": \"CopySVG\", \"label\": \"Copy SVG\"},                     \
        {\"id\": \"ViewSVG\", \"label\": \"View SVG\"},                     \
        {\"id\": \"ViewSource\", \"label\": \"View Source\"},               \
        {\"id\": \"SaveAs\", \"label\": \"Save As\"},                       \
        null,                                                               \
        {\"id\": \"Help\"},                                                 \
        {\"id\": \"About\", \"label\": \"About Adobe CVG Viewer...\"}       \
    ]                                                                       \
}}";

      h2::h2_json_node c5(j5);
      JE(j5, node_tojson(&c5, t2));
   }

   Case(more 1)
   {
      const char* j1 = "[                          \
	 {                                              \
	 \"precision\": \"zip\",                        \
	 \"Latitude\":  37.7668,                        \
	 \"Longitude\": -122.3959,                      \
	 \"Address\":   \"\",                           \
	 \"City\":      \"SAN FRANCISCO\",              \
	 \"State\":     \"CA\",                         \
	 \"Zip\":       \"94107\",                      \
	 \"Country\":   \"US\"                          \
	 },                                             \
	 {                                              \
	 \"precision\": \"zip\",                        \
	 \"Latitude\":  37.371991,                      \
	 \"Longitude\": -122.026020,                    \
	 \"Address\":   \"\",                           \
	 \"City\":      \"SUNNYVALE\",                  \
	 \"State\":     \"CA\",                         \
	 \"Zip\":       \"94085\",                      \
	 \"Country\":   \"US\"                          \
	 }                                              \
	 ]";

      h2::h2_json_node c1(j1);
      JE(j1, node_tojson(&c1, t2));
   }

   Case(more 2)
   {
      const char* j2 = "    [    \
    [0, -1, 0],                  \
    [1, 0, 0],                   \
    [0, 0, 1]                    \
	]                             \
";

      h2::h2_json_node c2(j2);
      JE(j2, node_tojson(&c2, t2));
   }

   Case(more 3)
   {
      const char* j3 = "{                           \
\"name\": \"Jack (\\\"Bee\\\") Nimble\",            \
\"format\": {\"type\":       \"rect\",              \
\"width\":      1920,                               \
\"height\":     1080,                               \
\"interlace\":  false,\"frame rate\": 24            \
}                                                   \
}";

      h2::h2_json_node c3(j3);
      OK("Jack (\"Bee\") Nimble", c3.get("name")->value_string);
   }
}

SUITE(json node dual)
{
   Case(simple plain)
   {
      const char* json = "{\"abc\": 123}";
      h2::h2_json_node node(json);

      int type;
      const char* cls;
      h2::h2_string key;
      h2::h2_string value;

      node.dual(type, cls, key, value);
      OK(h2::h2_json_node::t_object, type);
      OK("", key);
      OK("object", cls);
      OK("", value);

      node.get(0)->dual(type, cls, key, value);
      OK(h2::h2_json_node::t_string, type);
      OK("\"abc\"", key);
      OK("atomic", cls);
      OK("123", value);
   }
}

SUITE(json node print)
{
   Case(print simple plain)
   {
      const char* json = "{\"abc\": 123}";
      h2::h2_json_node node(json);

      h2::h2_vector<h2::h2_line> lines;
      node.print(lines, 0, 0, false);

      OK(3, lines.size());
      OK(ListOf(
           ListOf("", "{ "),
           ListOf("  ", "\"abc\": ", "123"),
           ListOf("", "}")),
         lines);
   }

   Case(print simple fold)
   {
      const char* json = "{\"abc\": 123}";
      h2::h2_json_node node(json);

      h2::h2_vector<h2::h2_line> lines;
      node.print(lines, 0, 0, true);

      OK(1, lines.size());
      OK(ListOf(ListOf("", "{ ", "\"abc\": ", "123", " }")), lines);
   }
}

SUITE(json match)
{
   Case(match null)
   {
      OK(h2::h2_json::match("null", " null "));
   }

   Case(match bool)
   {
      OK(h2::h2_json::match("true", " true "));
      OK(h2::h2_json::match("false", " false "));
      OK(!h2::h2_json::match("true", " false "));
   }

   Case(match number)
   {
      OK(h2::h2_json::match("0", "1 - 1 * 1 / sqrt(1)"));
      OK(h2::h2_json::match("-123.456", "-123 - 0.456"));
   }

   Case(match string)
   {
      OK(h2::h2_json::match("\"hello world\"", " 'hello world' "));
   }

   Case(match regexp) { OK(h2::h2_json::match("/.*/", " 'hello world' ")); };

   Case(match array)
   {
      OK(h2::h2_json::match("[]", " [] "));
      OK(h2::h2_json::match("[1]", " [1-0] "));
      OK(h2::h2_json::match("[1, null, 'hello']", " [1, null, \"hello\"] "));
   }

   Case(match object)
   {
      OK(h2::h2_json::match("{}", " {} "));
      OK(h2::h2_json::match("{'a': 1}", " {\"a\": 1} "));
      OK(h2::h2_json::match("{'a': 1}", " {\"a\": 1, \"b\": false} "));
   }

   Case(search)
   {
   }
}

SUITE(json dual)
{
   char t[1024 * 8];

   Case(object only)
   {
      const char* e = "{\"e\": 123}";
      const char* a = "{\"a\": 456}";
      h2::h2_json_node e_node(e);
      h2::h2_json_node a_node(a);

      h2::h2_json_dual dual(&e_node, &a_node);

      OK("(0;object-object;-;-)[(1;string-string;\"e\"-\"a\";123-456)]", dual_tostr(&dual, t));

      h2::h2_vector<h2::h2_line> e_lines, a_lines;
      dual.align(e_lines, a_lines);

      int width = std::max(h2::h2_lines_max_length(e_lines), h2::h2_lines_max_length(a_lines)) + 3;

      h2::h2_string result;
      h2::h2_json_diff::print(e_lines, a_lines, width, result);

      OK("{          \033[reset] \033[dark gray] │\033[reset] {          \033[reset] \033[dark gray] \033[reset]\n"
         "  \033[green]\"e\"\033[reset]: \033[green]123\033[reset] \033[reset] \033[dark gray] │\033[reset]   \033[red,bold]\"a\"\033[reset]: \033[red,bold]456\033[reset] \033[reset] \033[dark gray] \033[reset]\n"
         "\033[reset]}          \033[reset] \033[dark gray] │\033[reset] \033[reset]}          \033[reset] \033[dark gray] \033[reset]\n",
         result);
   }
}
