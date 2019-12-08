
#ifndef ___H2_CASE__H___
#define ___H2_CASE__H___

#include <csetjmp>

class h2_case
{
 public:
   enum {
      _INITED_ = 0,
      _PASSED_ = 1,
      _FAILED_ = 2,
      _TODOED_ = 3,
      _FILTED_ = 4,
   };

   const char* status()
   {
      switch (_status_) {
      case _TODOED_: return "TODO";
      case _FILTED_: return "Filtered";
      case _PASSED_: return "Passed";
      case _FAILED_: return "Failed";
      }
      return "";
   }

   jmp_buf _jb;
   int _jc;
   int stash()
   {
      _jc = 0;
      return ::setjmp(_jb);
   }
   void stash_pop()
   {
      if (_jc++ == 0) ::longjmp(_jb, 1);
   }

   int _status_;
   const char* _casename_;
   const char* _casefile_;
   int _caseline_;

   h2_suite* _suite_;

   void _prev_setup_()
   {
      _leak_push_(_casefile_, _caseline_, "case");

      // _stub_((void *)free, (void *)h2_leak_inspector::free, "", "", "", 0);
      // _stub_((void *)malloc, (void *)h2_leak_inspector::malloc, "", "", "", 0);
      // _stub_((void *)calloc, (void *)h2_leak_inspector::calloc, "", "", "", 0);
      // _stub_((void *)realloc, (void *)h2_leak_inspector::realloc, "", "", "", 0);
      // _stub_((void *)strdup, (void *)h2_leak_inspector::strdup, "", "", "", 0);
      // _stub_((void *)strndup, (void *)h2_leak_inspector::strndup, "", "", "", 0);
   }
   virtual void setup() {}
   void _post_setup_()
   {
   }
   void _prev_teardown_()
   {
   }
   virtual void teardown() {}
   void _post_teardown_()
   {
      _mock_double_check_();
      _stub_restore_();
      _leak_pop_();
   }
   void execute(h2_leak_stack* leak_stack)
   {
      _leak_stack_ = leak_stack;

      _start_ = h2_milliseconds();

      _status_ = _PASSED_;
      _prev_setup_();
      setup();
      _post_setup_();
      if (!stash()) {
         testcase();
      }
      else {
         _status_ = _FAILED_;
      }

      _prev_teardown_();
      teardown();
      _post_teardown_();

      _endup_ = h2_milliseconds();
   }

 public:
   long _start_, _endup_;
   std::vector<h2_fail*, h2_allocator<h2_fail*>> _fail_list_;

 public:
   h2_leak_stack* _leak_stack_;
   bool _leak_push_(const char* file, int line, const char* where, long long limited = 0x7fffffffffffLL, const char* fill = NULL)
   {
      _leak_stack_->push(file, line, where, limited, fill);
      return true;
   }
   void _leak_pop_()
   {
      if (_status_ == _FAILED_) { /* other failure already happen, ignore memory leak failure */
         return;
      }

      auto fail = _leak_stack_->pop();
      if (fail) {
         _status_ = _FAILED_;
         _fail_list_.push_back(fail);
         stash_pop();
      }
   }

 public:
   std::vector<h2_stub*, h2_allocator<h2_stub*>> _stub_list_;
   void _stub_(void* be_fp, void* to_fp, const char* be_fn, const char* to_fn, const char* file, int line)
   {
      h2_stub* stub = NULL;
      for (auto it = _stub_list_.begin(); it != _stub_list_.end(); it++) {
         if ((*it)->be_fp == be_fp) {
            stub = NULL;
         }
      }

      if (!stub) {
         stub = h2_stub::acq();
         _stub_list_.push_back(stub);
         stub->save(be_fp);
      }
      stub->set(to_fp);
   }

   void _stub_restore_()
   {
      auto it = _stub_list_.begin();
      while (it != _stub_list_.end()) {
         h2_stub* stub = *it;
         it = _stub_list_.erase(it);
         stub->restore();
         h2_stub::rel(stub);
      }
   }

   std::vector<h2_mock*, h2_allocator<h2_mock*>> _mock_list_;
   void _mock_(h2_mock* mock)
   {
      _stub_(mock->_be_fp, mock->_to_fp, mock->_be_fn, "", mock->_file, mock->_line);
      _mock_list_.push_back(mock);
   }

   void _mock_double_check_()
   {
      if (_status_ == _FAILED_) { /* other failure already happen, ignore mock call count failure */
         return;
      }

      h2_fail* fail = nullptr;
      auto it = _mock_list_.begin();
      while (it != _mock_list_.end()) {
         h2_mock* mock = *it;
         it = _mock_list_.erase(it);
         auto f = mock->double_check();
         if (!fail) {
            fail = f;
         }
         else {
            fail->appendx(f);
         }
      }

      if (fail) {
         _status_ = _FAILED_;
         _fail_list_.push_back(fail);
         return;
      }
   }

 public:
   h2_case() {}
   virtual ~h2_case() {}
   void _init_(h2_suite* suite, const char* casename, bool todo, const char* file, int line)
   {
      _status_ = _INITED_;
      _suite_ = suite;
      _casename_ = casename;
      _casefile_ = file;
      _caseline_ = line;

      if (todo) { _status_ = _TODOED_; }

      _suite_->install(this);
   }

   virtual void testcase() {}
};

#endif
