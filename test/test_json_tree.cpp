#include "../source/h2_unit.cpp"

extern char* node_tojson(h2::h2_json_node* node, char* b);

const char* week_json = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";

SUITE(json parse)
{
   char t2[1024 * 32];

   Case(number)
   {
      const char* n1 = "-123.456";
      h2::h2_json_tree c1(n1);
      OK(c1.is_number());
      OK(-123.456, c1.value_double);

      const char* n2 = "0";
      h2::h2_json_tree c2(n2);
      OK(c2.is_number());
      OK(0, c2.value_double);

      const char* n3 = "-1";
      h2::h2_json_tree c3(n3);
      OK(c3.is_number());
      OK(-1, c3.value_double);

      const char* n4 = "12345678";
      h2::h2_json_tree c4(n4, 4);
      OK(c4.is_number());
      OK(1234, c4.value_double);

      const char* n5 = "3+2*(4-sqrt(4))";
      h2::h2_json_tree c5(n5);
      OK(c5.is_number());
      OK(7, c5.value_double);
   }

   Case(number error)
   {
      const char* n1 = "";
      h2::h2_json_tree c1(n1);
      OK(h2::h2_json_node::t_absent, c1.type);

      const char* n2 = "-";
      h2::h2_json_tree c2(n2);
      OK(h2::h2_json_node::t_string, c2.type);
   }

   Case(string)
   {
      const char* n1 = "\"\"";
      h2::h2_json_tree c1(n1);
      OK(c1.is_string());
      OK("", c1.value_string);

      const char* n2 = "\"12345678\"";
      h2::h2_json_tree c2(n2);
      OK(c2.is_string());
      OK("12345678", c2.value_string);

      const char* n3 = "'12345678'";
      h2::h2_json_tree c3(n3);
      OK(c2.is_string());
      OK("12345678", c3.value_string);
   }

   Case(string error)
   {
      h2::h2_json_tree c1("\"12345678\"", 4);
      OK(h2::h2_json_node::t_absent, c1.type);
   }

   Case(pattern)
   {
      h2::h2_json_tree c1("/123*567/");
      OK(c1.is_pattern());
      OK("123*567", c1.value_string);
   }

   Case(empty array)
   {
      h2::h2_json_tree c1(" []");
      OK(c1.is_array());
      OK(0, c1.size());
   }

   Case(normal array)
   {
      const char* week = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";

      h2::h2_json_tree c1(week);
      JE(week_json, node_tojson(&c1, t2));
   }

   Case(extend array)
   {
      const char* week = "['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday']";

      h2::h2_json_tree c1(week);
      JE(week_json, node_tojson(&c1, t2));
   }

   Case(javascript array)
   {
      const char* week = "[Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday]";

      h2::h2_json_tree c1(week);
      JE(week_json, node_tojson(&c1, t2));
   }

   Case(empty object)
   {
      h2::h2_json_tree c1(" {  }");
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

      h2::h2_json_tree c(obj);

      JE(obj, node_tojson(&c, t2));
   }

   Case(javascript object)
   {
      const char* obj = "{                                     \
        data: Click Here,                                      \
        size: 36,                                              \
        alignment: true,                                       \
        bold: false,                                           \
        token: null,                                           \
        onMouseUp: sun1.opacity = (sun1.opacity / 100) * 90;   \
      }";

      h2::h2_json_tree c(obj);
      JE("{                                                                 \
            \"data\": \"Click Here\",                                       \
            \"size\": 36,                                                   \
            \"alignment\": true,                                            \
            \"bold\": false,                                                \
            \"token\": null,                                                \
            \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"    \
         }",
         node_tojson(&c, t2));
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

      h2::h2_json_tree c1(j1);
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

      h2::h2_json_tree c2(j2);
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

      h2::h2_json_tree c3(j3);
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

      h2::h2_json_tree c4(j4);
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

      h2::h2_json_tree c5(j5);
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

      h2::h2_json_tree c1(j1);
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

      h2::h2_json_tree c2(j2);
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

      h2::h2_json_tree c3(j3);
      OK("Jack (\"Bee\") Nimble", c3.get("name", false)->value_string);
   }
}
