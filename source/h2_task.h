
#ifndef ___H2_TASK__H___
#define ___H2_TASK__H___

class h2_task
{
 public:
   std::vector<h2_case*> case_list;

   h2_case* currcase;
   h2_leak_stack leak_stack;

   h2_logs logs;
   h2_log_console log_console;
   h2_log_xml log_xml;

   h2_task()
     : currcase(nullptr)
   {
   }

   ~h2_task()
   {
   }

   static h2_task& I()
   {
      static h2_task I;
      return I;
   }

   void configure(int argc, char** argv)
   {
      h2_cfg::I().configure(argc, argv);
      logs.add(&log_console);
   }

   h2_stub getaddrinfo_stub;
   h2_stub freeaddrinfo_stub;

   h2_stub free_stub;
   h2_stub malloc_stub;
   h2_stub calloc_stub;
   h2_stub realloc_stub;
   // #if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
   h2_stub posix_memalign_stub;
// #endif
#ifdef _ISOC11_SOURCE
   h2_stub aligned_alloc_stub;
#endif
   h2_stub strdup_stub;
   h2_stub strndup_stub;

   void prepare()
   {
      if (h2_cfg::I().junit) {
         if (!log_xml.set_file(h2_cfg::I().junit)) {
            printf("Can't open file %s\n", h2_cfg::I().junit);
            exit(1);
         }
         logs.add(&log_xml);
      }

      for (auto i = h2_suite::G()->begin(); i != h2_suite::G()->end(); i++) {
         for (auto j = (*i)->case_list.begin(); j != (*i)->case_list.end(); j++) {
            case_list.push_back(*j);
         }
      }

      if (h2_cfg::I().randomize) {
         random_shuffle(case_list.begin(), case_list.end());
      }

      getaddrinfo_stub.save((void*)::getaddrinfo);
      freeaddrinfo_stub.save((void*)::freeaddrinfo);

      getaddrinfo_stub.set((void*)h2extra::getaddrinfo);
      freeaddrinfo_stub.set((void*)h2extra::freeaddrinfo);

      h2_set_addrinfo(1, "127.0.0.1");

      leak_stack.push("", 0, "root");

      if (h2_cfg::I().memory_check) {
         free_stub.save((void*)::free);
         malloc_stub.save((void*)::malloc);
         calloc_stub.save((void*)::calloc);
         realloc_stub.save((void*)::realloc);
         // #if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
         posix_memalign_stub.save((void*)::posix_memalign);
// #endif
#ifdef _ISOC11_SOURCE
         aligned_alloc_stub.save((void*)::aligned_alloc);
#endif
         strdup_stub.save((void*)::strdup);
         strndup_stub.save((void*)::strndup);

         free_stub.set((void*)h2_leak_inspector::free);
         malloc_stub.set((void*)h2_leak_inspector::malloc);
         calloc_stub.set((void*)h2_leak_inspector::calloc);
         realloc_stub.set((void*)h2_leak_inspector::realloc);
         // #if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
         posix_memalign_stub.set((void*)h2_leak_inspector::posix_memalign);
// #endif
#ifdef _ISOC11_SOURCE
         aligned_alloc_stub.set((void*)h2_leak_inspector::aligned_alloc);
#endif
         strdup_stub.set((void*)h2_leak_inspector::strdup);
         strndup_stub.set((void*)h2_leak_inspector::strndup);
      }
   }

   void roundoff()
   {
      if (h2_cfg::I().memory_check) {
         free_stub.restore();
         malloc_stub.restore();
         calloc_stub.restore();
         realloc_stub.restore();
         // #if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
         posix_memalign_stub.restore();
// #endif
#ifdef _ISOC11_SOURCE
         aligned_alloc_stub.restore();
#endif
         strdup_stub.restore();
         strndup_stub.restore();
      }

      getaddrinfo_stub.restore();
      freeaddrinfo_stub.restore();
   }

   void run()
   {
      int status_count[10];
      memset(status_count, 0, sizeof(status_count));

      long start = h2_milliseconds();

      logs.on_task_start();

      for (auto it = case_list.begin(); it != case_list.end(); it++) {
         currcase = *it;
         logs.on_case_start(currcase);

         if (h2_cfg::I().filter(currcase->_suite_->name, currcase->_casename_)) {
            currcase->_status_ = h2_case::_FILTED_;
         }

         if (h2_case::_INITED_ == currcase->_status_) {
            currcase->execute(&leak_stack);
         }

         logs.on_case_endup(currcase);

         status_count[currcase->_status_] += 1;
         currcase->_suite_->status_count[currcase->_status_] += 1;
      }

      logs.on_task_endup(status_count[h2_case::_FAILED_],
                         status_count[h2_case::_PASSED_],
                         status_count[h2_case::_TODOED_],
                         status_count[h2_case::_FILTED_],
                         case_list.size(),
                         h2_milliseconds() - start);
   }
};

static inline void h2_stub_g(void* be_fp, void* to_fp, const char* be_fn, const char* to_fn, const char* file, int line)
{
   h2_task::I().currcase->_stub_(be_fp, to_fp, be_fn, to_fn, file, line);
}

static inline void h2_mock_g(h2_mock* mock)
{
   h2_task::I().currcase->_mock_(mock);
}

static inline void h2_fail_g(h2_fail* fail)
{
   if (fail) {
      h2_task::I().currcase->_status_ = h2_case::_FAILED_;
      h2_task::I().currcase->_fail_list_.push_back(fail);
      h2_task::I().currcase->stash_pop();
   }
}

static inline h2_mm* h2_newm_g(size_t size, size_t alignment, const char* fill, h2_backtrace &bt)
{
   return h2_task::I().leak_stack.new_mm(size, alignment, fill, bt);
}

static inline h2_mm* h2_getm_g(void* ptr)
{
   return h2_task::I().leak_stack.get_mm(ptr);
}

static inline void h2_relm_g(h2_mm* mm)
{
   auto fail = h2_task::I().leak_stack.rel_mm(mm);
   if (fail) {
      h2_fail_g(fail);
   }
}

#endif
