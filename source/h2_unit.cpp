#include "h2_unit.hpp"

#include <algorithm> /* shuffle */
#include <cassert>   /* assert */
#include <cctype>    /* tolower, isspace */
#include <cstdarg>   /* va_list */
#include <cstdint>   /* int32_t */
#include <errno.h>   /* strerror */
#include <iostream>  /* cout */
#include <map>       /* std::map */
#include <memory>    /* allocator */
#include <random>    /* shuffle */
#include <regex>     /* std::regex */
#include <signal.h>  /* sigaction */
#include <typeinfo>  /* typeid */

#if defined _WIN32
#   pragma comment(lib, "Winmm.lib")
#   pragma comment(lib, "Ws2_32.lib")
#   pragma comment(lib, "Shlwapi.lib")
#else
#   include <netdb.h>       /* getaddrinfo, gethostbyname */
#   include <libgen.h>      /* basename */
#   include <fcntl.h>       /* fcntl */
#   include <fnmatch.h>     /* fnmatch */
#   include <cxxabi.h>      /* demangle */
#   include <execinfo.h>    /* backtrace */
#   include <arpa/inet.h>   /* inet_addr */
#   include <syslog.h>      /* syslog, vsyslog */
#   include <unistd.h>      /* sysconf */
#   include <sys/ioctl.h>   /* ioctl */
#   include <sys/mman.h>    /* mprotect, mmap */
#   include <sys/socket.h>  /* sockaddr */
#   include <sys/syscall.h> /* syscall */
#   include <sys/time.h>    /* gettimeofday */
#   include <sys/types.h>   /* size_t */
#   if defined __GLIBC__
#      include <malloc.h> /* __malloc_hook */
#   elif defined __APPLE__
#      include <AvailabilityMacros.h>
#      include <malloc/malloc.h> /* malloc_zone_t */
#   endif
#endif

namespace h2 {
#include "h2_kit.cpp"  //unsort

#include "h2_backtrace.cpp"
#include "h2_callexp.cpp"
#include "h2_case.cpp"
#include "h2_debug.cpp"
#include "h2_directory.cpp"
#include "h2_dns.cpp"
#include "h2_expr.cpp"
#include "h2_failure.cpp"
#include "h2_heap.cpp"
#include "h2_json.cpp"
#include "h2_libc.cpp"
#include "h2_matcher.cpp"
#include "h2_mock.cpp"
#include "h2_option.cpp"
#include "h2_patch.cpp"
#include "h2_report.cpp"
#include "h2_socket.cpp"
#include "h2_stdio.cpp"
#include "h2_string.cpp"
#include "h2_stub.cpp"
#include "h2_suite.cpp"
#include "h2_task.cpp"
}  // namespace h2

#if defined _WIN32
#   define h2_weak_attribute
#else
#   define h2_weak_attribute __attribute__((weak))
#endif

h2_weak_attribute int main(int argc, const char** argv) {
   h2::h2_option::I().parse(argc, argv);
   h2::h2_task::I().prepare();
   h2::h2_task::I().execute();
   return h2::h2_task::I().finalize();
}
