
#include "h2_unit.hpp"

#include <cassert>   /* assert */
#include <cctype>    /* tolower, isspace */
#include <cstdarg>   /* va_list */
#include <errno.h>   /* strerror */
#include <exception> /* std::exception */
#include <iostream>  /* std::cout, std::streambuf */
#include <memory>    /* std::allocator */
#include <regex>     /* std::regex */
#include <signal.h>  /* sigaction */
#include <typeinfo>  /* std::typeid, std::type_info */

#if defined WIN32 || defined __WIN32__ || defined _WIN32 || defined _MSC_VER || defined __MINGW32__
#   include <winsock2.h> /* socket */
#   include <ws2tcpip.h> /* getaddrinfo */
#   include <io.h>       /* _wirte */
#   include <shlwapi.h>  /* PathRemoveFileSpecA, StrStrIA */
#   define fileno _fileno
#   define socklen_t int
#   define strcasestr StrStrIA
#   pragma comment(lib, "Ws2_32.lib")
#   pragma comment(lib, "Shlwapi.lib")
#else
#   include <arpa/inet.h>   /* inet_addr, inet_pton */
#   include <cxxabi.h>      /* abi::__cxa_demangle, abi::__cxa_throw */
#   include <execinfo.h>    /* backtrace */
#   include <fcntl.h>       /* fcntl */
#   include <fnmatch.h>     /* fnmatch */
#   include <libgen.h>      /* basename */
#   include <netdb.h>       /* getaddrinfo, gethostbyname */
#   include <sys/ioctl.h>   /* ioctl */
#   include <sys/mman.h>    /* mprotect, mmap */
#   include <sys/socket.h>  /* sockaddr */
#   include <sys/syscall.h> /* syscall */
#   include <sys/time.h>    /* gettimeofday */
#   include <sys/types.h>   /* size_t */
#   include <syslog.h>      /* syslog, vsyslog */
#   include <unistd.h>      /* sysconf */
#   if defined __GLIBC__
#      include <malloc.h> /* __malloc_hook */
#   elif defined __APPLE__
#      include <AvailabilityMacros.h>
#      include <malloc/malloc.h> /* malloc_zone_t */
#   endif
#endif

#if defined WIN32 || defined __WIN32__ || defined _WIN32 || defined _MSC_VER || defined __MINGW32__
#   define h2_weak_attribute
#else
#   define h2_weak_attribute __attribute__((weak))
#endif

h2_weak_attribute int main(int argc, const char** argv)
{
   h2::h2_option::I().parse(argc, argv);
   h2::h2_task::I().execute();
   return 0;
}

namespace h2 {

#include "utils/h2_list.cpp"
#include "utils/h2_misc.cpp"
#include "utils/h2_numeric.cpp"
#include "utils/h2_libc.cpp"
#include "utils/h2_string.cpp"
#include "utils/h2_line.cpp"
#include "utils/h2_color.cpp"
#include "utils/h2_nm.cpp"
#include "utils/h2_backtrace.cpp"

#include "json/h2_tinyexpr.cpp"
#include "json/h2_node.cpp"
#include "json/h2_lexical.cpp"
#include "json/h2_syntax.cpp"
#include "json/h2_select.cpp"
#include "json/h2_tree.cpp"
#include "json/h2_match.cpp"
#include "json/h2_dual.cpp"
#include "json/h2_json.cpp"

#include "matcher/h2_strcmp.cpp"
#include "matcher/h2_memcmp.cpp"

#include "memory/h2_piece.cpp"
#include "memory/h2_leaky.cpp"
#include "memory/h2_block.cpp"
#include "memory/h2_stack.cpp"
#include "memory/h2_override.cpp"
#if defined __GLIBC__
#   include "memory/h2_wrapper_linux.cpp"
#elif defined __APPLE__
// #   include "memory/h2_wrapper_macos1.cpp"
#   include "memory/h2_wrapper_macos2.cpp"
// #   include "memory/h2_wrapper_macos3.cpp"
#elif defined _WIN32
#   include "memory/h2_wrapper_windows.cpp"
#endif
#include "memory/h2_wrapper.cpp"
#include "memory/h2_crash.cpp"
#include "memory/h2_memory.cpp"
#include "memory/h2_exempt.cpp"

#include "exception/h2_exception.cpp"

#include "stub/h2_e9.cpp"
#include "stub/h2_native.cpp"
#include "stub/h2_stub.cpp"

#include "mock/h2_checkin.cpp"
#include "mock/h2_mock.cpp"
#include "mock/h2_mocks.cpp"

#include "extension/h2_dns.cpp"
#include "extension/h2_socket.cpp"
#include "extension/h2_stdio.cpp"

#include "core/h2_case.cpp"
#include "core/h2_suite.cpp"
#include "core/h2_task.cpp"

#include "assert/h2_assert.cpp"

#include "other/h2_debug.cpp"
#include "other/h2_failure.cpp"
#include "other/h2_report.cpp"

#include "other/h2_layout.cpp"
#include "other/h2_option.cpp"

}  // namespace h2
