#include "h2_unit.hpp"

#include <cctype>    /* tolower, isspace */
#include <cstdarg>   /* va_list */
#include <errno.h>   /* strerror */
#include <exception> /* std::exception */
#include <iostream>  /* std::cout, std::streambuf */
#include <regex>     /* std::regex */
#include <signal.h>  /* sigaction */
#include <time.h>    /* clock */
#include <typeinfo>  /* std::typeid, std::type_info */

#if !defined _MSC_VER
#include <cxxabi.h> /* abi::__cxa_demangle, abi::__cxa_throw */
#endif

#if defined __linux || defined __APPLE__  // -MSVC -Cygwin -MinGW
#include <execinfo.h>                     /* backtrace, backtrace_symbols */
#endif

#if defined __GLIBC__
#include <malloc.h> /* __malloc_hook */
#endif

#if defined _WIN32  // +MinGW
#ifndef NOMINMAX    // MinGW already defined
#define NOMINMAX    // fix std::min/max conflict with windows::min/max
#endif
#include <winsock2.h> /* socket */
#include <windows.h>
#include <ws2tcpip.h> /* getaddrinfo */
#include <io.h>       /* _write */
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define fileno _fileno
#define socklen_t int
#define ssize_t int
#endif

#if defined __CYGWIN__
#include <windows.h>
#endif

#if defined _WIN32 || defined __CYGWIN__  // +MinGW
#include <dbghelp.h>                      /* CaptureStackBackTrace, SymFromAddr */
#include <shlwapi.h>                      /* StrStrIA */
#define strcasestr StrStrIA
#endif

#if defined _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Dbghelp.lib")
#endif

#if defined __linux || defined __APPLE__ || defined __CYGWIN__
#include <arpa/inet.h>  /* inet_addr, inet_pton */
#include <fcntl.h>      /* fcntl */
#include <fnmatch.h>    /* fnmatch */
#include <netdb.h>      /* getaddrinfo, gethostbyname */
#include <sys/time.h>   /* gettimeofday */
#include <sys/ioctl.h>  /* ioctl */
#include <sys/mman.h>   /* mprotect, mmap */
#include <sys/socket.h> /* sockaddr */
#include <sys/types.h>  /* size_t */
#include <syslog.h>     /* syslog, vsyslog */
#include <unistd.h>     /* sysconf */
#endif

#if defined __APPLE__
#include <AvailabilityMacros.h>
#include <malloc/malloc.h> /* malloc_zone_t */
#endif

#if defined _WIN32  // +MinGW
#define LIBC__write ::_write
#else
#define LIBC__write ::write
#endif

#if defined _MSC_VER
#define h2_stdcall __stdcall
#else
#define h2_stdcall
#endif

#if defined _WIN32 || defined __CYGWIN__  // +MinGW
#if defined __H2UNIT_HPP__ || defined H2UNIT_IMPORT_MAIN
int main(int argc, const char** argv) { return h2::h2_runner::I().main(argc, argv); }
#else
int main(int argc, const char** argv);
#endif
#else
__attribute__((weak)) int main(int argc, const char** argv) { return h2::h2_runner::I().main(argc, argv); }
#endif

namespace h2 {
#include "utils/h2_list.cpp"
#include "utils/h2_pattern.cpp"
#include "utils/h2_diff.cpp"
#include "utils/h2_misc.cpp"
#include "utils/h2_libc.cpp"
#include "utils/h2_string.cpp"
#include "utils/h2_color.cpp"
#include "utils/h2_line.cpp"
#include "render/h2_console.cpp"
#include "symbol/h2_nm.cpp"
#include "symbol/h2_load.cpp"
#include "symbol/h2_backtrace.cpp"
#include "symbol/h2_cxa.cpp"
#include "json/h2_tinyexpr.cpp"
#include "json/h2_node.cpp"
#include "json/h2_lexical.cpp"
#include "json/h2_syntax.cpp"
#include "json/h2_select.cpp"
#include "json/h2_tree.cpp"
#include "json/h2_match.cpp"
#include "json/h2_dual.cpp"
#include "json/h2_json.cpp"
#include "matcher/h2_matches_memcmp.cpp"
#include "matcher/h2_matches_strcmp.cpp"
#include "memory/h2_piece.cpp"
#include "memory/h2_leaky.cpp"
#include "memory/h2_block.cpp"
#include "memory/h2_stack.cpp"
#include "memory/h2_override.cpp"
#include "memory/h2_override_stdlib.cpp"
#if defined __linux
#include "memory/h2_override_linux.cpp"
#elif defined __APPLE__
#include "memory/h2_override_macos.cpp"
#elif defined _MSC_VER
#include "memory/h2_override_windows.cpp"
#else  // +MinGW
#include "memory/h2_override_cygwin.cpp"
#endif
#include "memory/h2_memory.cpp"
#include "memory/h2_exempt.cpp"
#include "except/h2_debug.cpp"
#include "except/h2_crash.cpp"
#include "except/h2_exception.cpp"
#include "stub/h2_e9.cpp"
#include "stub/h2_source.cpp"
#include "stub/h2_stubs.cpp"
#include "stub/h2_temporary.cpp"
#include "mock/h2_checkin.cpp"
#include "mock/h2_mocker.cpp"
#include "mock/h2_mocks.cpp"
#include "stdio/h2_stdio.cpp"
#include "net/h2_dns.cpp"
#include "net/h2_socket.cpp"
#include "core/h2_test.cpp"
#include "core/h2_case.cpp"
#include "core/h2_suite.cpp"
#include "core/h2_filter.cpp"
#include "core/h2_runner.cpp"
#include "assert/h2_assert.cpp"
#include "assert/h2_timer.cpp"
#include "render/h2_layout.cpp"
#include "render/h2_failure.cpp"
#include "render/h2_report_console.cpp"
#include "render/h2_report_list.cpp"
#include "render/h2_report_junit.cpp"
#include "render/h2_report.cpp"
#include "render/h2_option.cpp"
}
