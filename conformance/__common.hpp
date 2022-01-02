
#define H2PP_STR(...) _H2PP_STR(__VA_ARGS__)
#define _H2PP_STR(...) #__VA_ARGS__

const char* cc_version()
{
#ifdef _MSC_VER
   switch (_MSC_VER) {
      case 1200: return "Microsoft Visual Studio 6.0";
      case 1300: return "Microsoft Visual Studio .NET 2002 (7.0)";
      case 1310: return "Microsoft Visual Studio .NET 2003 (7.1)";
      case 1400: return "Microsoft Visual Studio 2005 (8.0)";
      case 1500: return "Microsoft Visual Studio 2008 (9.0)";
      case 1600: return "Microsoft Visual Studio 2010 (10.0)";
      case 1700: return "Microsoft Visual Studio 2012 (11.0)";
      case 1800: return "Microsoft Visual Studio 2013 (12.0)";
      case 1900: return "Microsoft Visual Studio 2015 (14.0)";
      case 1910: return "Microsoft Visual Studio 2017 RTW (15.0)";
      case 1911: return "Microsoft Visual Studio 2017 version 15.3";
      case 1912: return "Microsoft Visual Studio 2017 version 15.5";
      case 1913: return "Microsoft Visual Studio 2017 version 15.6";
      case 1914: return "Microsoft Visual Studio 2017 version 15.7";
      case 1915: return "Microsoft Visual Studio 2017 version 15.8";
      case 1916: return "Microsoft Visual Studio 2017 version 15.9";
      case 1920: return "Microsoft Visual Studio 2019 RTW (16.0)";
      case 1921: return "Microsoft Visual Studio 2019 version 16.1";
      case 1922: return "Microsoft Visual Studio 2019 version 16.2";
      case 1923: return "Microsoft Visual Studio 2019 version 16.3";
      case 1924: return "Microsoft Visual Studio 2019 version 16.4";
      case 1925: return "Microsoft Visual Studio 2019 version 16.5";
      case 1926: return "Microsoft Visual Studio 2019 version 16.6";
      case 1927: return "Microsoft Visual Studio 2019 version 16.7";
      case 1928: return "Microsoft Visual Studio 2019 version 16.8, 16.9";
      case 1929: return "Microsoft Visual Studio 2019 version 16.10, 16.11";
      case 1930: return "Microsoft Visual Studio 2022 RTW (17.0)";
      default: return "Microsoft Visual Studio ? version";
   }
#elif defined __clang__
   return "clang " H2PP_STR(__clang_major__) "." H2PP_STR(__clang_minor__) "." H2PP_STR(__clang_patchlevel__);
#else
   return "gcc " H2PP_STR(__GNUC__) "." H2PP_STR(__GNUC_MINOR__) "." H2PP_STR(__GNUC_PATCHLEVEL__);
#endif
}

const char* std_version()
{
   switch (
#ifdef _MSC_VER
     _MSVC_LANG
#else
     __cplusplus
#endif
   ) {
      case 201103L: return "c++11";
      case 201402L: return "c++14";
      case 201703L: return "c++17";
      case 202002L: return "c++20/2a";
      case 202102L: return "c++23/2b";
   }
}
