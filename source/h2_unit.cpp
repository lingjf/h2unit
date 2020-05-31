#include "h2_unit.hpp"

#include <cassert>  /* assert */
#include <cctype>   /* tolower, isspace */
#include <cstdarg>  /* va_list */
#include <cstdint>  /* int32_t */
#include <errno.h>  /* strerror */
#include <iostream> /* cout */
#include <memory>   /* allocator */
#include <regex>    /* std::regex */
#include <signal.h> /* sigaction */
#include <typeinfo> /* typeid */

#if defined _WIN32
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
#   include <cxxabi.h>      /* demangle */
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

namespace h2 {
#include "h2_kit.cpp"
#include "h2_color.cpp"
#include "h2_list.cpp"
#include "h2_string.cpp"
#include "h2_line.cpp"
#include "h2_expr.cpp"

#include "h2_backtrace.cpp"
#include "h2_callexp.cpp"
#include "h2_case.cpp"
#include "h2_debug.cpp"
#include "h2_layout.cpp"
#include "h2_directory.cpp"
#include "h2_dns.cpp"
#include "h2_failure.cpp"
#include "h2_heap.cpp"
#include "h2_json.cpp"
#include "h2_libc.cpp"
#include "h2_matchee.cpp"
#include "h2_matcher.cpp"
#include "h2_matches.cpp"
#include "h2_mock.cpp"
#include "h2_option.cpp"
#include "h2_patch.cpp"
#include "h2_report.cpp"
#include "h2_socket.cpp"
#include "h2_stdio.cpp"
#include "h2_stub.cpp"
#include "h2_suite.cpp"
#include "h2_task.cpp"
}

#if defined _WIN32
#   define h2_weak_attribute
#else
#   define h2_weak_attribute __attribute__((weak))
#endif

h2_weak_attribute int main(int argc, const char** argv)
{
   h2::h2_option::I().parse(argc, argv);
   return h2::h2_task::I().execute();
}
