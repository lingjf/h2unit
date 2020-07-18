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

#if defined _WIN32
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

#include "h2_kit.cpp"      //
#include "h2_numeric.cpp"  //
#include "h2_list.cpp"     //

#include "json/h2_tinyexpr.cpp"  //
#include "json/h2_node.cpp"      // ..
#include "json/h2_lexical.cpp"   // ..
#include "json/h2_syntax.cpp"    // ..
#include "json/h2_tree.cpp"      // ..
#include "json/h2_match.cpp"     // ..
#include "json/h2_dual.cpp"      // ..
#include "json/h2_json.cpp"      // ..

#include "matcher/h2_matcher.cpp"    //
#include "matcher/h2_strcmp.cpp"     //
#include "matcher/h2_memcmp.cpp"     //

#include "memory/h2_piece.cpp"     // kit
#include "memory/h2_block.cpp"     // ..
#include "memory/h2_stack.cpp"     // ..
#include "memory/h2_override.cpp"  // ..
#include "memory/h2_crash.cpp"     // ..
#include "memory/h2_memory.cpp"    // ..

#include "stub/h2_e9.cpp"      //
#include "stub/h2_native.cpp"  // ..
#include "stub/h2_stub.cpp"    // .., kit

#include "mock/h2_checkin.cpp"  //
#include "mock/h2_mock.cpp"     //

#include "extension/h2_dns.cpp"     // kit
#include "extension/h2_socket.cpp"  // kit
#include "extension/h2_stdio.cpp"   // kit

#include "h2_color.cpp"    // kit
#include "h2_debug.cpp"    // kit
#include "h2_failure.cpp"  // kit
#include "h2_libc.cpp"     // kit
#include "h2_line.cpp"     // kit
#include "h2_report.cpp"   // kit
#include "h2_string.cpp"   // kit

#include "h2_nm.cpp"         //
#include "h2_backtrace.cpp"  //
#include "h2_check.cpp"      //
#include "h2_layout.cpp"     //
#include "h2_option.cpp"     //
#include "h2_patch.cpp"      //

#include "core/h2_case.cpp"   //
#include "core/h2_suite.cpp"  //
#include "core/h2_task.cpp"   //

}  // namespace h2
