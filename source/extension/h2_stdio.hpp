
struct h2_stdio {
   static void initialize();
   static size_t capture_length();
   static const char* capture_cout(const char* type);
   static void capture_cancel();
};

#define H2COUT(...) h2::h2_stdio::capture_cout(#__VA_ARGS__)
