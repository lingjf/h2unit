
struct h2_stdio {
   static void initialize();
   static size_t get_length();
   static const char* capture_cout(const char* type = nullptr);
};
