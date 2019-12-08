
#ifndef ___H2_BT__H___
#define ___H2_BT__H___

#include <cxxabi.h>

class h2_backtrace
{
 private:
   int count;
   int offset;
   void* array[100];

 public:
   h2_backtrace()
     : count(0), offset(0) {}

   h2_backtrace& operator=(h2_backtrace& bt)
   {
      offset = bt.offset;
      count = bt.count;
      memcpy(array, bt.array, bt.count * sizeof(void*));

      return *this;
   }

   void backtrace(int offset = 0)
   {
      count = ::backtrace(array, H2_ARRAY_COUNTOF(array));
      this->offset = offset;
   }

   bool operator==(h2_backtrace& bt)
   {
      if (count != bt.count) return false;
      for (int i = 0; i < count; ++i) {
         if (array[i] != bt.array[i]) {
            return false;
         }
      }
      return true;
   }

   bool has(void* func, int size)
   {
      for (int i = 0; i < count; ++i) {
         if (func <= array[i] &&
             (unsigned char*)array[i] < ((unsigned char*)func) + size) {
            return true;
         }
      }
      return false;
   }

   void print()
   {
      char** s = backtrace_symbols(array, count);
      for (int i = offset; i < count; ++i) {
         const char* mangled_name = parse(s[i]);
         const char* demangled_name = demangle(mangled_name);

         int line = 0;
#if __linux__
         char func[1024];
         char file[1024];
         int ret = read_by_addr2line(array[i], h2_cfg::I().path, func, file, line);
         if (0 <= ret) {
            demangled_name = func;
         }
#endif
         ::printf("   %d. %s", i, demangled_name);
         if (0 < line) {
            ::printf(" : %d", line);
         }
         ::printf("\n");

         if (strcmp("main", demangled_name) == 0) {
            break;
         }
      }
      free(s);
   }

   int read_by_addr2line(void* addr, const char* path, char* func, char* file, int& line)
   {
      char buf[1024 * 2] = {0};
      FILE* fp;

      snprintf(buf, sizeof(buf), "addr2line -C -e %s -f -i %p", path, addr);
      // snprintf(buf, sizeof(buf), "atos -o %s %p", path, addr);

      fp = ::popen(buf, "r");
      if (fp == NULL) {
         printf("run addr2line error: %s\n", strerror(errno));
         return -1;
      }

      //1st line function name
      ::fgets(buf, sizeof(buf) - 1, fp);
      if (buf[strlen(buf) - 1] == '\n') {
         buf[strlen(buf) - 1] = '\0';
      }

      strcpy(func, buf);
      if (buf[0] == '?') {
         sprintf(func, "unknown %p", addr);
      }

      //2nd line file and line
      ::fgets(buf, sizeof(buf) - 1, fp);

      char* p = ::strchr(buf, ':');
      if (p) {
         *p = '\0';
         strcpy(file, buf);
         line = atoi(p + 1);
      }

      ::pclose(fp);
      return 0;
   }

   const char* parse(const char* symbol)
   {
      static char temp[128];

      if (1 == sscanf(symbol, "%*[^(]%*[^_]%127[^)+]", temp)) {
         return temp;
      }

      if (1 == sscanf(symbol, "%*s%*s%*s %127[^ )+]", temp)) {
         return temp;
      }

      if (1 == sscanf(symbol, "%127s", temp)) {
         return temp;
      }

      return symbol;
   }

   const char* demangle(const char* mangled_name)
   {
      static char temp[1024];
      int status = 0;
      const char* realname = abi::__cxa_demangle(mangled_name, 0, 0, &status);
      switch (status) {
      case 0:
         strcpy(temp, realname);
         break;
      case -1:
         // printf("FAIL: failed to allocate memory while demangling %s\n", mangled_name);
      case -2:
         // printf("FAIL: %s is not a valid name under the C++ ABI mangling rules\n", mangled_name);
      default:
         // printf("FAIL: some other unexpected error: %d\n", status);
         strcpy(temp, mangled_name);
         break;
      }
      if (realname) free((void*)realname);
      return temp;
   }
};

#endif
