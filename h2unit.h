/* Jeff Ling , lingjf@gmail.com */

#ifndef ___H2UNIT_H___
#define ___H2UNIT_H___

#if defined(__cplusplus)

typedef struct h2unit_list {
   struct h2unit_list *next;
   struct h2unit_list *prev;
} h2unit_list;

class h2unit_auto {
public :
   bool done;
   h2unit_auto(const char* file, int line);
   ~h2unit_auto();
};

class h2unit_case
{
public:
   static const int _INITED_ = 0;
   static const int _PASSED_ = 1;
   static const int _FAILED_ = 2;
   static const int _IGNORE_ = 3;
   static const int _FILTED_ = 4;

   int _status_;
   const char* _unitname_;
   const char* _casename_;
   const char* _casefile_;
   int _caseline_;
   h2unit_case* _chain_;
   h2unit_case* _next_;

   void _prev_setup_();
   virtual void setup();
   void _post_setup_();
   void _prev_teardown_();
   virtual void teardown();
   void _post_teardown_();
   void _execute_();

public:
   long _start_, _endup_;
   const char* _checkfile_;
   int _checkline_;
   int _checkcount_;
   h2unit_list _errormsg_;
   h2unit_list _expected_;
   h2unit_list _actually_;
   h2unit_list* _addition_;
   void _vmsg_(h2unit_list* typed, const char *style, const char* format, ...);
public:
   h2unit_list _leak_stack_;
   void _leak_push_(const char* file, int line);
   bool _leak_pop_();
   void _blob_add_(h2unit_list* blob);
   void _limit_(unsigned long bytes);
public:
   h2unit_list _stub_list_;
   void* _addr_(const char* native, const char* native_name, const char* fake_name);
   void _stub_(void* native, void* fake, const char* native_name, const char* fake_name);

public:
   static h2unit_case* _current_;
   h2unit_case();
   virtual ~h2unit_case();
   void _init_(const char* unitname, const char* casename, bool ignored, const char* file, int line);

   void _enter_check_(const char* file, int line);
   void _check_equal_(bool result);
   void _check_equal_(int expected, int actually);
   void _check_equal_(unsigned long expected, unsigned long actually);
   void _check_equal_(unsigned long long expected, unsigned long long actually);
   void _check_equal_(double expected, double actually);
   void _check_equal_(char* expected, char* actually);
   void _check_equal_(unsigned char* expected, unsigned char* actually, int length);
   void _check_range_(double from, double to, double actually);
   void _check_inset_(double *inset, int count, double actually);
   void _check_equal_strcmp_nocase_(char* expected, char* actually);
   void _check_regex_(char* express, char* actually);
   void _check_catch_(const char* expected, const char* actually, const char* exceptype);

   virtual void _testcase_() = 0;
};

#define __H2UNIT_UNIT_NAME(name) h2unit_##name##_unit

#define ____H2UNIT_CASE_NAME(name, line) h2unit_##name##line##_case
#define ____H2UNIT_CASE_INST(name, line) h2unit_##name##line##_inst
#define ___H2UNIT_CASE_NAME(name, line) ____H2UNIT_CASE_NAME(name, line)
#define ___H2UNIT_CASE_INST(name, line) ____H2UNIT_CASE_INST(name, line)
#define __H2UNIT_CASE_NAME(name) ___H2UNIT_CASE_NAME(name, __LINE__)
#define __H2UNIT_CASE_INST(name) ___H2UNIT_CASE_INST(name, __LINE__)

#define H2UNIT(_unit_)  \
   struct __H2UNIT_UNIT_NAME(_unit_): public h2unit_case

#define H2CASE(_unit_, _case_)                                                                           \
   class __H2UNIT_CASE_NAME(_unit_): public __H2UNIT_UNIT_NAME(_unit_)                                   \
   {  public:                                                                                            \
      __H2UNIT_CASE_NAME(_unit_)() { _init_(#_unit_, _case_, false, __FILE__, __LINE__); }               \
      void _testcase_();                                                                                 \
   } __H2UNIT_CASE_INST(_unit_);                                                                         \
   void __H2UNIT_CASE_NAME(_unit_)::_testcase_()

#define H2TODO(_unit_, _case_)                                                                           \
   class __H2UNIT_CASE_NAME(_unit_): public __H2UNIT_UNIT_NAME(_unit_)                                   \
   {  public:                                                                                            \
      __H2UNIT_CASE_NAME(_unit_)() { _init_(#_unit_, _case_, true, __FILE__, __LINE__); }                \
      void _testcase_();                                                                                 \
   } __H2UNIT_CASE_INST(_unit_);                                                                         \
   void __H2UNIT_CASE_NAME(_unit_)::_testcase_()


/** Visual C++ 8.0 2005 (_MSC_VER = 1400) and later version support 'variadic macros' */

#if defined(__GNUC__) || (defined(_MSC_VER) && _MSC_VER >= 1400)
#define H2EQUAL(...)                                                                                     \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      h2unit_case::_current_->_check_equal_(__VA_ARGS__);                                                \
   } while(0)
#else
#define H2EQUAL(expected, actually)                                                                      \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      h2unit_case::_current_->_check_equal_(expected, actually);                                         \
   } while(0)
#endif

#define H2EQUAL_TRUE(condition)                                                                          \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      h2unit_case::_current_->_check_equal_(condition);                                                  \
   } while(0)

#define H2EQUAL_INT(expected, actually)                                                                  \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      h2unit_case::_current_->_check_equal_((unsigned long)(expected), (unsigned long)(actually));       \
   } while(0)

#define H2EQUAL_FLOAT(expected, actually)                                                                \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      h2unit_case::_current_->_check_equal_((double)(expected), (double)(actually));                     \
   } while(0)

#define H2EQUAL_RANGE(from, to, actually)                                                                \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      h2unit_case::_current_->_check_range_((double)(from), (double)(to), (double)(actually));           \
   } while(0)

#define H2EQUAL_INSET(inset, count, actually)                                                            \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      h2unit_case::_current_->_check_inset_((double*)(inset), (int)(count), (double)(actually));         \
   } while(0)

#define H2EQUAL_STRCMP(expected, actually)                                                               \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      h2unit_case::_current_->_check_equal_((char*)(expected), (char*)(actually));                       \
   } while(0)

#define H2EQUAL_STRCMP_NOCASE(expected, actually)                                                        \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      h2unit_case::_current_->_check_equal_strcmp_nocase_((char*)(expected), (char*)(actually));         \
   } while(0)

#define H2EQUAL_WILDCARD(expected, actually)                                                             \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      h2unit_case::_current_->_check_regex_((char*)(expected), (char*)(actually));                       \
   } while(0)

#define H2EQUAL_REGEX H2EQUAL_WILDCARD

#define H2EQUAL_MEMCMP(expected, actually, length)                                                       \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      h2unit_case::_current_->_check_equal_((unsigned char*)(expected), (unsigned char*)(actually), (int)(length));  \
   } while(0)

#define H2CATCH_NONE(expressions)                                                                        \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      try {                                                                                              \
         expressions;                                                                                    \
      } catch(...) {                                                                                     \
         h2unit_case::_current_->_check_catch_("throw none", "throw some", "" );                         \
      }                                                                                                  \
   } while(0)

#define H2CATCH_THROW(expressions, exceptype)                                                            \
   do {                                                                                                  \
      bool catched = false;                                                                              \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      try {                                                                                              \
         expressions;                                                                                    \
      } catch(exceptype) {                                                                               \
         catched = true;                                                                                 \
      } catch (...) {  }                                                                                 \
      if (!catched) {                                                                                    \
         h2unit_case::_current_->_check_catch_("throw", "lost", #exceptype);                             \
      }                                                                                                  \
   } while(0)


#include <typeinfo>

#define H2STUB(native, fake)                                                                             \
   do {                                                                                                  \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                                         \
      const char* ti = typeid(native).name();    /* ISO/IEC 14882 C++ Standard Section 5.2.8 */          \
      if (ti[0] == 'F' || (ti[0] == 'P' && ti[1] == 'F') || strstr(ti, "__cdecl")) {                     \
         h2unit_case::_current_->_stub_((void*)native, (void*)fake, #native, #fake);                     \
      } else {                                                                                           \
         void* addr = h2unit_case::_current_->_addr_((const char*)native, #native, #fake);               \
         h2unit_case::_current_->_stub_(addr, (void*)fake, #native, #fake);                              \
      }                                                                                                  \
   } while(0)


#define H2_FAULTY_INJECT_MEMORY(bytes)  h2unit_case::_current_->_limit_(bytes)

#define H2LEAK_BLOCK() \
   for (class h2unit_auto _h2unit_auto_(__FILE__, __LINE__); !_h2unit_auto_.done; _h2unit_auto_.done = true)

#endif

#if defined(__cplusplus)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

extern void* h2unit_alloc(void* ptr, size_t size, size_t alignment, unsigned char c, const char* file, int line);
extern void h2unit_free(void*, const char*, int);
extern int h2unit_posix_memalign(void**, size_t, size_t, const char*, int);
extern char* h2unit_strdup(const char*, const char*, int);
extern char* h2unit_strndup(const char*, size_t, const char*, int);

#if defined(__cplusplus)
}
#endif

#define malloc(size) h2unit_alloc(NULL, size, 1, 0xED, __FILE__, __LINE__)
#define calloc(nmemb, size) h2unit_alloc(NULL, (nmemb) * (size), 1, 0x00, __FILE__, __LINE__)
#define realloc(ptr, size) h2unit_alloc(ptr, size, 1, 0xED, __FILE__, __LINE__)
#define free(ptr) h2unit_free(ptr, __FILE__, __LINE__)
#define posix_memalign(memptr, alignment, size) h2unit_posix_memalign(memptr, alignment, size, __FILE__, __LINE__)
#define memalign(boundary, size) h2unit_alloc(NULL, size, boundary, 0xED, __FILE__, __LINE__)
#define strdup(s) h2unit_strdup(s, __FILE__, __LINE__)
#define strndup(s, n) h2unit_strndup(s, n, __FILE__, __LINE__)

#if defined(_WIN32)
/* Windows specific allocator */
#include <windows.h>
#define _aligned_malloc(size, alignment) h2unit_alloc(NULL, size, alignment, 0xED, __FILE__, __LINE__)
#define _aligned_free(memblock) h2unit_free(memblock, __FILE__, __LINE__)
#define _aligned_realloc(memblock, size, alignment) h2unit_alloc(memblock, size, 1, 0xED, __FILE__, __LINE__)
#define _aligned_recalloc(memblock, num, size, alignment) h2unit_alloc(memblock, (num) * (size), 1, 0x00, __FILE__, __LINE__)
#define _aligned_offset_malloc(size, alignment, offset) h2unit_alloc(NULL, size, alignment, 0xED, __FILE__, __LINE__)
#define _aligned_offset_realloc(memblock, size, alignment, offset) h2unit_alloc(memblock, size, alignment, 0xED, __FILE__, __LINE__)
#define _aligned_offset_recalloc(memblock, num, size, alignment, offset) h2unit_alloc(memblock, (num) * (size), alignment, 0x00, __FILE__, __LINE__)

#define CoTaskMemAlloc(size) h2unit_alloc(NULL, size, 1, 0xED, __FILE__, __LINE__)
#define CoTaskMemRealloc(ptr, size) h2unit_alloc(ptr, size, 1, 0xED, __FILE__, __LINE__)
#define CoTaskMemFree(ptr) h2unit_free(ptr, __FILE__, __LINE__)
#define GlobalAlloc(flag, size) h2unit_alloc(NULL, size, 1, 0x00, __FILE__, __LINE__)
#define GlobalFree(ptr) h2unit_free(ptr, __FILE__, __LINE__)
#define HeapAlloc(hand, flag, size) h2unit_alloc(NULL, size, 1, 0x00, __FILE__, __LINE__)
#define HeapReAlloc(hand, flag, ptr, size) h2unit_alloc(ptr, size, 1, 0x00, __FILE__, __LINE__)
#define HeapFree(hand, flag, ptr) h2unit_free(ptr, __FILE__, __LINE__)
#define LocalAlloc(flag, size) h2unit_alloc(NULL, size, 1, 0x00, __FILE__, __LINE__)
#define LocalReAlloc(flag, ptr, size) h2unit_alloc(ptr, size, 1, 0x00, __FILE__, __LINE__)
#define LocalFree(flag, ptr) h2unit_free(ptr, __FILE__, __LINE__)
#endif

#if defined(__cplusplus)
#include <new>
void* operator new(size_t, const char*, int);
void* operator new[](size_t, const char*, int);
void operator delete(void*);
void operator delete[](void*);
#define new new(__FILE__, __LINE__)
#endif


#endif

