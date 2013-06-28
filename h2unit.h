/* Jeff Ling , lingjf@gmail.com */

#ifndef ___H2UNIT_H___
#define ___H2UNIT_H___

#if defined(__cplusplus)


typedef struct h2unit_string
{
   const char* style;
   char* data;
   struct h2unit_string* next;
} h2unit_string;

class h2unit_case
{
public:
   static const int _INITED_ = 0;
   static const int _PASSED_ = 1;
   static const int _FAILED_ = 2;
   static const int _IGNORE_ = 3;
   static const int _FILTER_ = 4;

   int _status_;
   const char* _unitname_;
   const char* _casename_;
   const char* _casefile_;
   int _caseline_;
   h2unit_case* _chain_;
   h2unit_case* _next_;

   void _prev_setup_();
   void _post_teardown_();
   void _execute_();

public:
   long _start_, _endup_;
   const char* _checkfile_;
   int _checkline_;
   int _checkcount_;
   h2unit_string* _errormsg_;
   h2unit_string* _expected_;
   h2unit_string* _actually_;
   h2unit_string** _addition_;
public:
   h2unit_case();
   virtual ~h2unit_case();
   void _init_(const char* unitname, const char* casename, bool ignored, const char* file, int line);
   static h2unit_case* _current_;
   void _limit_(unsigned long bytes);
   void _stub_(void* native, void* fake, const char* native_name, const char* fake_name);
   void _stub_static_(const char* native, void* fake, const char* native_name, const char* fake_name);

   void _enter_check_(const char* file, int line);
   void _check_true_(const char* condition, bool result);
   void _check_equal_(int expected, int actual);
   void _check_equal_(double expected, double actual, double threshold = 0.000001);
   void _check_equal_(char* expected, char* actual);
   void _check_equal_(unsigned char* expected, unsigned char* actual, int length);
   void _check_equal_strcmp_nocase_(char* expected, char* actual);
   void _check_regex_(char* express, char* actual);
   void _check_catch_(const char* expected, const char* actual, const char* exceptype);

   virtual void setup();
   virtual void teardown();
   virtual void _testcase_() = 0;
};

#define __H2UNIT_UNIT_NAME(name) h2unit_##name##_unit
#define ____H2UNIT_CASE_NAME(name, line) h2unit_##name##line##_case
#define __H2UNIT_CASE_NAME(name, line) ____H2UNIT_CASE_NAME(name, line)
#define ____H2UNIT_CASE_INST(name, line) h2unit_##name##line##_inst
#define __H2UNIT_CASE_INST(name, line) ____H2UNIT_CASE_INST(name, line)

#define H2UNIT(_unit_)  \
   struct __H2UNIT_UNIT_NAME(_unit_): public h2unit_case

#define H2CASE(_unit_, _case_)                                    \
   class __H2UNIT_CASE_NAME(_unit_, __LINE__):                    \
      public __H2UNIT_UNIT_NAME(_unit_)                           \
   {  public:                                                     \
      __H2UNIT_CASE_NAME(_unit_, __LINE__)()                      \
      { _init_(#_unit_, _case_, false, __FILE__, __LINE__); }     \
      void _testcase_();                                          \
   } __H2UNIT_CASE_INST(_unit_, __LINE__);                        \
   void __H2UNIT_CASE_NAME(_unit_, __LINE__)::_testcase_()

#define H2CASE_IGNORE(_unit_, _case_)                             \
   class __H2UNIT_CASE_NAME(_unit_, __LINE__):                    \
      public __H2UNIT_UNIT_NAME(_unit_)                           \
   {  public:                                                     \
      __H2UNIT_CASE_NAME(_unit_, __LINE__)()                      \
      { _init_(#_unit_, _case_, true, __FILE__, __LINE__); }      \
      void _testcase_();                                          \
   } __H2UNIT_CASE_INST(_unit_, __LINE__);                        \
   void __H2UNIT_CASE_NAME(_unit_, __LINE__)::_testcase_()

#define H2CHECK(condition)                                                       \
   do {                                                                          \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                 \
      h2unit_case::_current_->_check_true_(#condition, (condition));             \
   } while(0)

#if defined(__GNUC__) || (defined(_WIN32) && _MSC_VER >= 1400)
#define H2EQUAL(...)                                                             \
   do {                                                                          \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                 \
      h2unit_case::_current_->_check_equal_(__VA_ARGS__);                        \
   } while(0)
#else
#define H2EQUAL(expected, actual)                                                \
   do {                                                                          \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                 \
      h2unit_case::_current_->_check_equal_(expected, actual);                   \
   } while(0)
#endif

#define H2EQUAL_INTEGER(expected, actual)                                        \
   do {                                                                          \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                 \
      h2unit_case::_current_->_check_equal_((int)(expected), (int)(actual));     \
   } while(0)

#define H2EQUAL_DOUBLE(expected, actual, threshold)                              \
   do {                                                                          \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                 \
      h2unit_case::_current_->_check_equal_(                                     \
         (double)(expected), (double)(actual), (double)(threshold) );            \
   } while(0)

#define H2EQUAL_STRCMP(expected, actual)                                         \
   do {                                                                          \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                 \
      h2unit_case::_current_->_check_equal_((char*)(expected), (char*)(actual)); \
   } while(0)

#define H2EQUAL_STRCMP_NOCASE(expected, actual)                                  \
   do {                                                                          \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                 \
      h2unit_case::_current_->_check_equal_strcmp_nocase_(                       \
         (char*)(expected), (char*)(actual) );                                   \
   } while(0)

#define H2EQUAL_MEMCMP(expected, actual, length)                                 \
   do {                                                                          \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                 \
      h2unit_case::_current_->_check_equal_(                                     \
         (unsigned char*)(expected), (unsigned char*)(actual), (int)(length) );  \
   } while(0)

#define H2EQUAL_WILDCARD(expected, actual)                                       \
   do {                                                                          \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                 \
      h2unit_case::_current_->_check_regex_((char*)(expected), (char*)(actual)); \
   } while(0)

#define H2EQUAL_REGEX H2EQUAL_WILDCARD

#define H2CATCH_NONE(expressions)                                                \
   do {                                                                          \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                 \
      try {                                                                      \
         expressions;                                                            \
      } catch(...) {                                                             \
         h2unit_case::_current_->_check_catch_("throw none", "throw some", "" ); \
      }                                                                          \
   } while(0)

#define H2CATCH_THROW(expressions, exceptype)                                    \
   do {                                                                          \
      bool catched = false;                                                      \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                 \
      try {                                                                      \
         expressions;                                                            \
      } catch(exceptype) {                                                       \
         catched = true;                                                         \
      } catch (...) {  }                                                         \
      if (!catched) {                                                            \
         h2unit_case::_current_->_check_catch_("throw", "lost", #exceptype);     \
      }                                                                          \
   } while(0)

#define H2STUB_FPOINT(native, fake)                                                    \
   do {                                                                                \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                       \
      h2unit_case::_current_->_stub_((void*)&native, (void*)&fake, #native, #fake);    \
   } while(0)

#define H2STUB_STATIC(native, fake)                                                    \
   do {                                                                                \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                       \
      h2unit_case::_current_->_stub_static_(native, (void*)&fake, #native, #fake);     \
   } while(0)

#define H2STUB(native, fake)                                                           \
   do {                                                                                \
      h2unit_case::_current_->_enter_check_(__FILE__, __LINE__);                       \
      if (__builtin_types_compatible_p (typeof(native), char*)) {                       \
         h2unit_case::_current_->_stub_((void*)&native, (void*)&fake, #native, #fake); \
      } else {                                                                         \
         h2unit_case::_current_->_stub_static_(native, (void*)&fake, #native, #fake);  \
      }                                                                                \
   } while(0)

/**
 * TODO: 提供二个动态打桩的接口，一个是根据函数指针，一个是根据函数名。 打算将二个接口合并为一个。
 *       第一个参数  orig 既可以是函数指针也可以是函数名，在H2STUB宏内类型强转为 void*。
 *       之后在_stub_中查询 它所在的内存的属性，如果可执行则为函数指针，如果可读写则为函数名。
 *       Windows系统可以通过VirtualQuery()接口查询内存属性，Linux没有类似的接口。
 *       mprotect()可以设置，但不可以查询。
 */

#define H2_FAULTY_INJECT_MEMORY(bytes)  h2unit_case::_current_->_limit_(bytes)

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

extern void* h2unit_malloc(size_t, const char*, int);
extern void* h2unit_calloc(size_t, size_t, const char*, int);
extern void* h2unit_realloc(void*, size_t, const char*, int);
extern void h2unit_free(void*, const char*, int);
extern char* h2unit_strdup(const char*, const char*, int);
extern char* h2unit_strndup(const char*, size_t, const char*, int);

#if defined(__cplusplus)
}
#endif

#define malloc(a) h2unit_malloc(a, __FILE__, __LINE__)
#define calloc(a, b) h2unit_calloc(a, b, __FILE__, __LINE__)
#define realloc(a, b) h2unit_realloc(a, b, __FILE__, __LINE__)
#define free(a) h2unit_free(a, __FILE__, __LINE__)
#define strdup(a) h2unit_strdup(a, __FILE__, __LINE__)
#define strndup(a, n) h2unit_strndup(a, n, __FILE__, __LINE__)

#if defined(__cplusplus)
#include <new>
void* operator new(size_t, const char*, int);
void* operator new[](size_t, const char*, int);
void operator delete(void*);
void operator delete[](void*);
#define new new(__FILE__, __LINE__)
#endif


#endif

