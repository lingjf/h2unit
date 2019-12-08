#include "../source/h2_unit.h"

static int __node_tojson(h2json::node* node, char* b)
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
      for (size_t i = 0; i < node->child_array_or_object.size(); i++) {
         if (i) l += sprintf(b + l, ",");
         l += __node_tojson(node->child_array_or_object[i], b + l);
      }
      l += sprintf(b + l, "]");
   }
   if (node->is_object()) {
      l += sprintf(b + l, "{");
      for (int i = 0; i < node->size(); i++) {
         if (i) l += sprintf(b + l, ",");
         l += __node_tojson(node->child_array_or_object[i], b + l);
      }
      l += sprintf(b + l, "}");
   }

   return l;
}

char* node_tojson(h2json::node* node, char* b)
{
   __node_tojson(node, b);
   return b;
}

H2UNIT (h2_json_node) {
   bool res;
   char t2[1024 * 8];

   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(h2_json_node, number)
{
   const char* n1 = "-123.456";
   h2json::node* c1 = h2json::parse(n1);
   H2EQ(c1->is_number());
   H2EQ(-123.456, c1->value_double);
   h2json::frees(c1);

   const char* n2 = "0";
   h2json::node* c2 = h2json::parse(n2);
   H2EQ(c2->is_number());
   H2EQ(0, c2->value_double);
   h2json::frees(c2);

   const char* n3 = "-1";
   h2json::node* c3 = h2json::parse(n3);
   H2EQ(c3->is_number());
   H2EQ(-1, c3->value_double);
   h2json::frees(c3);

   const char* n4 = "12345678";
   h2json::node* c4 = h2json::parse(n4, 4);
   H2EQ(c4->is_number());
   H2EQ(1234, c4->value_double);
   h2json::frees(c4);

   const char* n5 = "3+2*(4-sqrt(4))";
   h2json::node* c5 = h2json::parse(n5);
   H2EQ(c5->is_number());
   H2EQ(7, c5->value_double);
   h2json::frees(c5);
}

H2CASE(h2_json_node, number error)
{
   const char* n1 = "";
   h2json::node* c1 = h2json::parse(n1);
   H2EQ(NULL == c1);

   const char* n2 = "-";
   h2json::node* c2 = h2json::parse(n2);
   H2EQ(NULL == c2);
}

H2CASE(h2_json_node, string)
{
   const char* n1 = "\"\"";
   h2json::node* c1 = h2json::parse(n1);
   H2EQ(c1->is_string());
   H2EQ("", c1->value_string);
   h2json::frees(c1);

   const char* n2 = "\"12345678\"";
   h2json::node* c2 = h2json::parse(n2);
   H2EQ(c2->is_string());
   H2EQ("12345678", c2->value_string);
   h2json::frees(c2);

   const char* n3 = "'12345678'";
   h2json::node* c3 = h2json::parse(n3);
   H2EQ(c2->is_string());
   H2EQ("12345678", c3->value_string);
   h2json::frees(c3);
}

H2CASE(h2_json_node, string error)
{
   const char* n3 = "\"12345678\"";
   h2json::node* c3 = h2json::parse(n3, 4);
   H2EQ(NULL == c3);
}

H2CASE(h2_json_node, regexp)
{
   const char* n3 = "/123*567/";
   h2json::node* c3 = h2json::parse(n3);
   H2EQ(c3->is_regexp());
   H2EQ("123*567", c3->value_string);
   h2json::frees(c3);
}

H2CASE(h2_json_node, empty array)
{
   const char* n1 = " []";
   h2json::node* c1 = h2json::parse(n1);
   H2EQ(c1->is_array());
   H2EQ(0, c1->size());
   h2json::frees(c1);
}

H2CASE(h2_json_node, normal array)
{
   const char* week = "[\"Sunday\", 'Monday', \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";

   h2json::node* c = h2json::parse(week);

   H2EQ(c->is_array());
   H2EQ(0, c->key_string.size());
   H2EQ(7, c->size());

   h2json::node* d[7];
   d[0] = c->get(0);
   H2EQ("Sunday", d[0]->value_string);

   d[1] = c->get(1);
   H2EQ("Monday", d[1]->value_string);

   d[2] = c->get(2);
   H2EQ("Tuesday", d[2]->value_string);

   d[3] = c->get(3);
   H2EQ("Wednesday", d[3]->value_string);

   d[4] = c->get(4);
   H2EQ("Thursday", d[4]->value_string);

   d[5] = c->get(5);
   H2EQ("Friday", d[5]->value_string);

   d[6] = c->get(6);
   H2EQ("Saturday", d[6]->value_string);

   H2JE(week, node_tojson(c, t2));

   H2EQ(NULL == c->get("ling"));

   for (int i = 0; i < c->size(); i++) {
      H2EQ(d[i]->value_string, c->child_array_or_object[i]->value_string);
   }

   h2json::frees(c);
}

H2CASE(h2_json_node, empty object)
{
   const char* n1 = " {  }";
   h2json::node* c1 = h2json::parse(n1);
   H2EQ(c1->is_object());
   H2EQ(0, c1->size());
   h2json::frees(c1);
}

H2CASE(h2_json_node, normal object)
{
   const char* obj = "{                                                 \
        \"data\": 'Click Here',                                         \
        'size': 36,                                                     \
        \"alignment\": true,                                            \
        \"bold\": false,                                                \
        \"token\": null,                                                \
        \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"    \
    }";

   h2json::node* c = h2json::parse(obj);

   H2EQ(c->is_object());
   H2EQ(6, c->size());

   h2json::node* c0 = c->get(0);
   H2EQ(c0->is_string());
   H2EQ("data", c0->key_string);
   H2EQ("Click Here", c0->value_string);

   h2json::node* c1 = c->get(1);
   H2EQ(c1->is_number());
   H2EQ("size", c1->key_string);
   H2EQ(36, c1->value_double);

   h2json::node* c2 = c->get(2);
   H2EQ(c2->is_bool());
   H2EQ("alignment", c2->key_string);
   H2EQ(c2->value_boolean);

   h2json::node* c3 = c->get(3);
   H2EQ(c3->is_bool());
   H2EQ("bold", c3->key_string);
   H2EQ(!c3->value_boolean);

   h2json::node* c4 = c->get(4);
   H2EQ(c4->is_null());
   H2EQ("token", c4->key_string);

   h2json::node* c5 = c->get(5);
   H2EQ(c5->is_string());
   H2EQ("onMouseUp", c5->key_string);
   H2EQ("sun1.opacity = (sun1.opacity / 100) * 90;", c5->value_string);

   H2JE(obj, node_tojson(c, t2));

   h2json::frees(c);
}

H2CASE(h2_json_node, "http://www.json.org/example.html")
{
   const char* j1 =
     "{                                                                                                             \
    \"glossary\": {                                                                                                 \
        \"title\": \"example glossary\",                                                                            \
		\"GlossDiv\": {                                                                                             \
            \"title\": \"S\",                                                                                       \
			\"GlossList\": {                                                                                        \
                \"GlossEntry\": {                                                                                   \
                    \"ID\": \"SGML\",                                                                               \
					\"SortAs\": \"SGML\",                                                                           \
					\"GlossTerm\": \"Standard Generalized Markup Language\",                                        \
					\"Acronym\": \"SGML\",                                                                          \
					\"Abbrev\": \"ISO 8879:1986\",                                                                  \
					\"GlossDef\": {                                                                                 \
                        \"para\": \"A meta-markup language, used to create markup languages such as DocBook.\",     \
						\"GlossSeeAlso\": [\"GML\", \"XML\"]                                                        \
                    },                                                                                              \
					\"GlossSee\": \"markup\"                                                                        \
                }                                                                                                   \
            }                                                                                                       \
        }                                                                                                           \
    }                                                                                                               \
}";

   h2json::node* c1 = h2json::parse(j1);
   H2JE(j1, node_tojson(c1, t2));
   h2json::frees(c1);

   const char* j2 =
     "{\"menu\": {                                                  \
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

   h2json::node* c2 = h2json::parse(j2);
   H2JE(j2, node_tojson(c2, t2));
   h2json::frees(c2);

   const char* j3 =
     "{\"widget\": {                                                        \
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

   h2json::node* c3 = h2json::parse(j3);
   H2JE(j3, node_tojson(c3, t2));
   h2json::frees(c3);

   const char* j4 =
     "{\"web-app\": {                                                                       \
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

   h2json::node* c4 = h2json::parse(j4);
   H2JE(j4, node_tojson(c4, t2));
   h2json::frees(c4);

   const char* j5 =
     "{\"menu\": {                                                          \
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

   h2json::node* c5 = h2json::parse(j5);
   H2JE(j5, node_tojson(c5, t2));
   h2json::frees(c5);
}

H2CASE(h2_json_node, more 1)
{
   const char* j1 =
     "[                                             \
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

   h2json::node* c1 = h2json::parse(j1);
   auto s1 = node_tojson(c1, t2);
   H2JE(j1, s1);
   h2json::frees(c1);
}

H2CASE(h2_json_node, more 2)
{
   const char* j2 =
     "    [             \
    [0, -1, 0],         \
    [1, 0, 0],          \
    [0, 0, 1]           \
	]                   \
";

   h2json::node* c2 = h2json::parse(j2);
   H2JE(j2, node_tojson(c2, t2));
   h2json::frees(c2);
}

H2CASE(h2_json_node, more 3)
{
   const char* j3 =
     "{                                             \
\"name\": \"Jack (\\\"Bee\\\") Nimble\",            \
\"format\": {\"type\":       \"rect\",              \
\"width\":      1920,                               \
\"height\":     1080,                               \
\"interlace\":  false,\"frame rate\": 24            \
}                                                   \
}";

   h2json::node* c3 = h2json::parse(j3);
   H2EQ("Jack (\"Bee\") Nimble", c3->get("name")->value_string);
   h2json::frees(c3);
}

H2UNIT (h2_json_match) {
   bool res;
   char t2[1024 * 8];

   void setup()
   {
   }

   void teardown()
   {
   }
};

H2CASE(h2_json_match, match null)
{
   H2EQ(h2json::match("null", " null "));
}

H2CASE(h2_json_match, match bool)
{
   H2EQ(h2json::match("true", " true "));
   H2EQ(h2json::match("false", " false "));
   H2EQ(!h2json::match("true", " false "));
}

H2CASE(h2_json_match, match number)
{
   H2EQ(h2json::match("0", "1 - 1 * 1 / sqrt(1)"));
   H2EQ(h2json::match("-123.456", "-123 - 0.456"));
}

H2CASE(h2_json_match, match string)
{
   H2EQ(h2json::match("\"hello world\"", " 'hello world' "));
}

H2CASE(h2_json_match, match regexp)
{
   H2EQ(h2json::match("/.*/", " 'hello world' "));
}

H2CASE(h2_json_match, match array)
{
   H2EQ(h2json::match("[]", " [] "));
   H2EQ(h2json::match("[1]", " [1-0] "));
   H2EQ(h2json::match("[1, null, 'hello']", " [1, null, \"hello\"] "));
}

H2CASE(h2_json_match, match object)
{
   H2EQ(h2json::match("{}", " {} "));
   H2EQ(h2json::match("{'a': 1}", " {\"a\": 1} "));
   H2EQ(h2json::match("{'a': 1}", " {\"a\": 1, \"b\": false} "));
}

H2UNIT (h2_json_diff) {
   bool res;
   char t2[1024 * 8];

   void setup()
   {
   }

   void teardown()
   {
   }
};
