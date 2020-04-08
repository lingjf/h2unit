
struct h2_stdio {
   static void capture_cout(char* buffer, int size = 1024 * 1024);
   static const char* capture_cout();
};
